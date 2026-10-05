#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "symble_table.h"


/*
 * Convenções do código gerado:
 *
 *   $t0  resultado da última expressão avaliada
 *   $t1  segundo operando de operações binárias
 *   $fp  base do quadro que guarda as variáveis
 *   $sp  pilha usada para guardar operandos temporários
 *
 * Todas as variáveis do programa ficam em um único
 * quadro de pilha, reservado no início do programa.
 * Cada símbolo guarda o seu deslocamento (offset) em
 * relação a $fp. Blocos aninhados usam a região logo
 * após a do bloco externo e liberam essa região
 * quando terminam.
 */


/*
 * Tamanho em bytes de cada tipo.
 */

#define SIZE_INT 4

#define SIZE_CAR 1


/*
 * Arquivo de saída.
 */

static FILE *out;


/*
 * Pilha de escopos usada durante a geração.
 */

static SymbolTableStack symbol_stack;


/*
 * Próximo deslocamento livre dentro do quadro.
 */

static int current_offset = 0;


/*
 * Contador para criar rótulos únicos.
 */

static int label_count = 0;


/*
 * Cadeias de caracteres encontradas, emitidas
 * na seção .data no final.
 */

typedef struct StringEntry {

    int id;

    const char *text;

    struct StringEntry *next;

} StringEntry;


static StringEntry *strings = NULL;

static StringEntry *strings_tail = NULL;

static int string_count = 0;


/* =========================================================
   UTILIDADES
   ========================================================= */

static int new_label(void)
{
    return label_count++;
}


static int type_size(
    ValueType type
)
{
    if (type == TYPE_CAR) {
        return SIZE_CAR;
    }

    return SIZE_INT;
}


static int align_up(
    int value,
    int alignment
)
{
    return (value + alignment - 1) / alignment * alignment;
}


/*
 * Registra uma cadeia e devolve o número do rótulo.
 */

static int add_string(
    const char *text
)
{
    StringEntry *entry =
        malloc(sizeof(StringEntry));

    if (entry == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        exit(1);
    }

    entry->id = string_count++;

    entry->text = text;

    entry->next = NULL;

    if (strings == NULL) {
        strings = entry;
    }
    else {
        strings_tail->next = entry;
    }

    strings_tail = entry;

    return entry->id;
}


static void free_strings(void)
{
    StringEntry *entry =
        strings;

    while (entry != NULL) {

        StringEntry *next =
            entry->next;

        free(entry);

        entry = next;
    }

    strings = NULL;

    strings_tail = NULL;

    string_count = 0;
}


/*
 * Código do caractere que começa em text.
 * Se text começa com \, trata a sequência de escape
 * (\n, \t, ...) e consumed recebe 2; senão, 1.
 */

static int decode_char(
    const char *text,
    int *consumed
)
{
    if (text[0] != '\\') {

        *consumed = 1;

        return (unsigned char) text[0];
    }

    *consumed = 2;

    switch (text[1]) {

        case 'n':
            return '\n';

        case 't':
            return '\t';

        case 'r':
            return '\r';

        case 'a':
            return '\a';

        case 'b':
            return '\b';

        case 'f':
            return '\f';

        case 'v':
            return '\v';

        case '0':
            return 0;

        default:
            return (unsigned char) text[1];
    }
}


/*
 * Converte o lexema de uma constante de caractere
 * ('a', '\n', '\t', ...) para o seu código.
 */

static int char_value(
    const char *lexeme
)
{
    int consumed;

    return decode_char(
        lexeme + 1,
        &consumed
    );
}


/* =========================================================
   TIPO DE UMA EXPRESSÃO
   ========================================================= */

/*
 * Necessário para escolher entre imprimir um inteiro
 * ou um caractere. A análise semântica já garantiu
 * que os tipos estão corretos.
 */

static ValueType expression_type(
    ASTNode *node
)
{
    switch (node->kind) {

        case AST_CARCONST:

            return TYPE_CAR;


        case AST_IDENTIFIER:
        {
            Symbol *symbol =
                symbol_table_lookup(
                    &symbol_stack,
                    node->lexeme
                );

            if (symbol == NULL) {
                return TYPE_UNKNOWN;
            }

            return symbol->type;
        }


        case AST_ASSIGN:

            return expression_type(
                node->child
            );


        default:

            return TYPE_INT;
    }
}


/* =========================================================
   TAMANHO DO QUADRO
   ========================================================= */

static int command_frame(
    ASTNode *node
);


/*
 * Bytes ocupados pelas variáveis declaradas
 * diretamente em um bloco.
 */

static int declarations_size(
    ASTNode *var_section
)
{
    int size = 0;

    for (
        ASTNode *decl = var_section->child;
        decl != NULL;
        decl = decl->next
    ) {

        for (
            ASTNode *id = decl->child;
            id != NULL;
            id = id->next
        ) {

            int bytes =
                type_size(decl->value_type);

            size =
                align_up(size, bytes);

            size += bytes;
        }
    }

    /*
     * Cada bloco ocupa um múltiplo de 4 bytes,
     * para que o bloco interno comece alinhado.
     */

    return align_up(size, 4);
}


/*
 * Bytes necessários para um bloco e para o maior
 * dos blocos aninhados dentro dele.
 */

static int block_frame(
    ASTNode *block
)
{
    int own = 0;

    int nested = 0;

    for (
        ASTNode *node = block->child;
        node != NULL;
        node = node->next
    ) {

        if (node->kind == AST_VAR_SECTION) {

            own =
                declarations_size(node);
        }
        else if (node->kind == AST_COMMAND_LIST) {

            for (
                ASTNode *cmd = node->child;
                cmd != NULL;
                cmd = cmd->next
            ) {

                int size =
                    command_frame(cmd);

                if (size > nested) {
                    nested = size;
                }
            }
        }
    }

    return own + nested;
}


static int command_frame(
    ASTNode *node
)
{
    int size = 0;

    switch (node->kind) {

        case AST_BLOCK:

            return block_frame(node);


        case AST_IF:

            return command_frame(
                node->child->next
            );


        case AST_IF_ELSE:
        {
            int then_size =
                command_frame(node->child->next);

            int else_size =
                command_frame(node->child->next->next);

            size =
                then_size > else_size ? then_size : else_size;

            return size;
        }


        case AST_WHILE:

            return command_frame(
                node->child->next
            );


        default:

            return 0;
    }
}


/* =========================================================
   EXPRESSÕES
   ========================================================= */

static void gen_expression(
    ASTNode *node
);


static void gen_push(void)
{
    fprintf(out, "\taddiu $sp, $sp, -4\n");

    fprintf(out, "\tsw $t0, 0($sp)\n");
}


static void gen_pop(
    const char *reg
)
{
    fprintf(out, "\tlw %s, 0($sp)\n", reg);

    fprintf(out, "\taddiu $sp, $sp, 4\n");
}


static void gen_load(
    Symbol *symbol
)
{
    if (symbol->type == TYPE_CAR) {

        fprintf(
            out,
            "\tlb $t0, %d($fp)\t# %s\n",
            symbol->offset,
            symbol->name
        );
    }
    else {

        fprintf(
            out,
            "\tlw $t0, %d($fp)\t# %s\n",
            symbol->offset,
            symbol->name
        );
    }
}


static void gen_store(
    Symbol *symbol
)
{
    if (symbol->type == TYPE_CAR) {

        fprintf(
            out,
            "\tsb $t0, %d($fp)\t# %s\n",
            symbol->offset,
            symbol->name
        );
    }
    else {

        fprintf(
            out,
            "\tsw $t0, %d($fp)\t# %s\n",
            symbol->offset,
            symbol->name
        );
    }
}


/*
 * || e & avaliam o operando da direita somente
 * se necessário. O resultado é sempre 0 ou 1.
 */

static void gen_logical(
    ASTNode *node
)
{
    int label =
        new_label();

    int is_or =
        node->kind == AST_OR;

    gen_expression(node->child);

    fprintf(
        out,
        "\t%s $t0, L_end_%d\n",
        is_or ? "bnez" : "beqz",
        label
    );

    gen_expression(node->child->next);

    fprintf(out, "L_end_%d:\n", label);

    fprintf(out, "\tsne $t0, $t0, $zero\n");
}


static void gen_binary(
    ASTNode *node
)
{
    if (node->kind == AST_OR || node->kind == AST_AND) {

        gen_logical(node);

        return;
    }

    /*
     * Esquerda -> pilha, direita -> $t0,
     * depois $t1 = direita e $t0 = esquerda.
     */

    gen_expression(node->child);

    gen_push();

    gen_expression(node->child->next);

    fprintf(out, "\tmove $t1, $t0\n");

    gen_pop("$t0");

    switch (node->kind) {

        case AST_ADD:

            fprintf(out, "\taddu $t0, $t0, $t1\n");

            break;


        case AST_SUB:

            fprintf(out, "\tsubu $t0, $t0, $t1\n");

            break;


        case AST_MUL:

            fprintf(out, "\tmul $t0, $t0, $t1\n");

            break;


        case AST_DIV:

            fprintf(out, "\tdiv $t0, $t1\n");

            fprintf(out, "\tmflo $t0\n");

            break;


        case AST_EQ:

            fprintf(out, "\tseq $t0, $t0, $t1\n");

            break;


        case AST_NEQ:

            fprintf(out, "\tsne $t0, $t0, $t1\n");

            break;


        case AST_LT:

            fprintf(out, "\tslt $t0, $t0, $t1\n");

            break;


        case AST_GT:

            fprintf(out, "\tsgt $t0, $t0, $t1\n");

            break;


        case AST_LE:

            fprintf(out, "\tsle $t0, $t0, $t1\n");

            break;


        case AST_GE:

            fprintf(out, "\tsge $t0, $t0, $t1\n");

            break;


        default:

            break;
    }
}


/*
 * Gera o código de uma expressão.
 * O valor fica em $t0.
 */

static void gen_expression(
    ASTNode *node
)
{
    switch (node->kind) {

        case AST_INTCONST:

            fprintf(
                out,
                "\tli $t0, %ld\n",
                strtol(node->lexeme, NULL, 10)
            );

            break;


        case AST_CARCONST:

            fprintf(
                out,
                "\tli $t0, %d\t# %s\n",
                char_value(node->lexeme),
                node->lexeme
            );

            break;


        case AST_IDENTIFIER:
        {
            Symbol *symbol =
                symbol_table_lookup(
                    &symbol_stack,
                    node->lexeme
                );

            gen_load(symbol);

            break;
        }


        case AST_ASSIGN:
        {
            Symbol *symbol =
                symbol_table_lookup(
                    &symbol_stack,
                    node->child->lexeme
                );

            gen_expression(node->child->next);

            gen_store(symbol);

            break;
        }


        case AST_NEG:

            gen_expression(node->child);

            fprintf(out, "\tsubu $t0, $zero, $t0\n");

            break;


        case AST_NOT:

            gen_expression(node->child);

            fprintf(out, "\tseq $t0, $t0, $zero\n");

            break;


        default:

            gen_binary(node);

            break;
    }
}


/* =========================================================
   COMANDOS
   ========================================================= */

static void gen_command(
    ASTNode *node
);


static void gen_syscall(
    int service
)
{
    fprintf(out, "\tli $v0, %d\n", service);

    fprintf(out, "\tsyscall\n");
}


/*
 * Declara as variáveis de um bloco: reserva
 * deslocamentos no quadro e zera o seu conteúdo.
 */

static void gen_declarations(
    ASTNode *var_section
)
{
    for (
        ASTNode *decl = var_section->child;
        decl != NULL;
        decl = decl->next
    ) {

        for (
            ASTNode *id = decl->child;
            id != NULL;
            id = id->next
        ) {

            int bytes =
                type_size(decl->value_type);

            current_offset =
                align_up(current_offset, bytes);

            symbol_table_insert(
                &symbol_stack,
                id->lexeme,
                decl->value_type,
                id->line
            );

            Symbol *symbol =
                symbol_table_lookup_current(
                    &symbol_stack,
                    id->lexeme
                );

            symbol->offset = current_offset;

            symbol->size = bytes;

            fprintf(
                out,
                "\t%s $zero, %d($fp)\t# %s: %s\n",
                bytes == SIZE_CAR ? "sb" : "sw",
                symbol->offset,
                symbol->name,
                decl->value_type == TYPE_CAR ? "car" : "int"
            );

            current_offset += bytes;
        }
    }

    current_offset =
        align_up(current_offset, 4);
}


static void gen_block(
    ASTNode *block
)
{
    int saved_offset =
        current_offset;

    int has_scope = 0;

    for (
        ASTNode *node = block->child;
        node != NULL;
        node = node->next
    ) {

        if (node->kind == AST_VAR_SECTION) {

            symbol_table_push(
                &symbol_stack
            );

            has_scope = 1;

            gen_declarations(node);
        }
        else if (node->kind == AST_COMMAND_LIST) {

            for (
                ASTNode *cmd = node->child;
                cmd != NULL;
                cmd = cmd->next
            ) {

                gen_command(cmd);
            }
        }
    }

    if (has_scope) {

        symbol_table_pop(
            &symbol_stack
        );
    }

    current_offset =
        saved_offset;
}


static void gen_command(
    ASTNode *node
)
{
    switch (node->kind) {

        case AST_EMPTY:

            break;


        case AST_READ:
        {
            Symbol *symbol =
                symbol_table_lookup(
                    &symbol_stack,
                    node->child->lexeme
                );

            fprintf(
                out,
                "\t# leia %s\n",
                symbol->name
            );

            gen_syscall(
                symbol->type == TYPE_CAR ? 12 : 5
            );

            fprintf(out, "\tmove $t0, $v0\n");

            gen_store(symbol);

            break;
        }


        case AST_WRITE:
        {
            ValueType type =
                expression_type(node->child);

            fprintf(
                out,
                "\t# escreva (linha %d)\n",
                node->line
            );

            gen_expression(node->child);

            fprintf(out, "\tmove $a0, $t0\n");

            gen_syscall(
                type == TYPE_CAR ? 11 : 1
            );

            break;
        }


        case AST_WRITE_STRING:
        {
            int id =
                add_string(node->child->lexeme);

            fprintf(
                out,
                "\t# escreva cadeia (linha %d)\n",
                node->line
            );

            fprintf(out, "\tla $a0, str_%d\n", id);

            gen_syscall(4);

            break;
        }


        case AST_NEWLINE:

            fprintf(out, "\t# novalinha\n");

            fprintf(out, "\tli $a0, 10\n");

            gen_syscall(11);

            break;


        case AST_IF:
        {
            int label =
                new_label();

            fprintf(
                out,
                "\t# se (linha %d)\n",
                node->line
            );

            gen_expression(node->child);

            fprintf(out, "\tbeqz $t0, L_fimse_%d\n", label);

            gen_command(node->child->next);

            fprintf(out, "L_fimse_%d:\n", label);

            break;
        }


        case AST_IF_ELSE:
        {
            int label =
                new_label();

            fprintf(
                out,
                "\t# se/senao (linha %d)\n",
                node->line
            );

            gen_expression(node->child);

            fprintf(out, "\tbeqz $t0, L_senao_%d\n", label);

            gen_command(node->child->next);

            fprintf(out, "\tj L_fimse_%d\n", label);

            fprintf(out, "L_senao_%d:\n", label);

            gen_command(node->child->next->next);

            fprintf(out, "L_fimse_%d:\n", label);

            break;
        }


        case AST_WHILE:
        {
            int label =
                new_label();

            fprintf(
                out,
                "\t# enquanto (linha %d)\n",
                node->line
            );

            fprintf(out, "L_enquanto_%d:\n", label);

            gen_expression(node->child);

            fprintf(out, "\tbeqz $t0, L_fimenq_%d\n", label);

            gen_command(node->child->next);

            fprintf(out, "\tj L_enquanto_%d\n", label);

            fprintf(out, "L_fimenq_%d:\n", label);

            break;
        }


        case AST_BLOCK:

            gen_block(node);

            break;


        default:

            /*
             * Expressão usada como comando
             * (por exemplo, uma atribuição).
             */

            gen_expression(node);

            break;
    }
}


/* =========================================================
   PROGRAMA
   ========================================================= */

/*
 * Emite uma cadeia sem depender de como cada simulador
 * trata as sequências de escape: texto comum vai em
 * .ascii e os demais caracteres em .byte.
 */

static void gen_string(
    const StringEntry *entry
)
{
    const char *text =
        entry->text + 1;

    int length =
        (int) strlen(entry->text) - 2;

    char run[512];

    int run_length = 0;

    fprintf(
        out,
        "str_%d:\t# %s\n",
        entry->id,
        entry->text
    );

    for (int i = 0; i < length; ) {

        int consumed;

        int code =
            decode_char(text + i, &consumed);

        int plain =
            consumed == 1 &&
            code >= 32 &&
            code < 127 &&
            code != '"' &&
            code != '\\';

        i += consumed;

        if (plain) {

            run[run_length++] = (char) code;

            if (run_length < (int) sizeof(run) - 1) {
                continue;
            }
        }

        if (run_length > 0) {

            run[run_length] = '\0';

            fprintf(out, "\t.ascii \"%s\"\n", run);

            run_length = 0;
        }

        if (!plain) {

            fprintf(out, "\t.byte %d\n", code);
        }
    }

    if (run_length > 0) {

        run[run_length] = '\0';

        fprintf(out, "\t.ascii \"%s\"\n", run);
    }

    fprintf(out, "\t.byte 0\n");
}


static void gen_data(void)
{
    if (strings == NULL) {
        return;
    }

    fprintf(out, "\n\t.data\n");

    for (
        StringEntry *entry = strings;
        entry != NULL;
        entry = entry->next
    ) {

        gen_string(entry);
    }
}


int codegen_generate(
    ASTNode *root,
    FILE *file
)
{
    if (
        root == NULL ||
        root->child == NULL ||
        root->child->kind != AST_BLOCK
    ) {
        return 0;
    }

    ASTNode *block =
        root->child;

    out = file;

    current_offset = 0;

    label_count = 0;

    symbol_table_init(
        &symbol_stack
    );

    int frame =
        block_frame(block);

    fprintf(out, "# Codigo gerado pelo compilador G-V1\n");

    fprintf(out, "\t.text\n");

    fprintf(out, "\t.globl main\n");

    fprintf(out, "main:\n");

    /*
     * Reserva o quadro com todas as variáveis.
     * $fp aponta para o início do quadro.
     */

    fprintf(out, "\t# quadro de %d bytes\n", frame);

    fprintf(out, "\taddiu $fp, $sp, -%d\n", frame);

    fprintf(out, "\tmove $sp, $fp\n");

    gen_block(block);

    fprintf(out, "\t# fim do programa\n");

    gen_syscall(10);

    gen_data();

    symbol_table_destroy(
        &symbol_stack
    );

    free_strings();

    return 1;
}
