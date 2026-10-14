#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"


/*
 * Pilha de escopos usada durante a análise.
 */

static SymbolTableStack symbol_stack;


/*
 * Número de erros.
 */

static int errors = 0;


/* =========================================================
   ERROS
   ========================================================= */

static void semantic_error(
    int line,
    const char *message
)
{
    fprintf(
        stderr,
        "ERRO: %s linha %d\n",
        message,
        line
    );

    errors++;
}


/* =========================================================
   TIPOS
   ========================================================= */

static const char *type_name(
    ValueType type
)
{
    switch (type) {

        case TYPE_INT:
            return "int";

        case TYPE_CAR:
            return "car";

        default:
            return "unknown";
    }
}


/* =========================================================
   DECLARAÇÃO
   ========================================================= */

static void analyze_var_section(
    ASTNode *node
);


/* =========================================================
   EXPRESSÕES
   ========================================================= */

static ValueType analyze_expression(
    ASTNode *node
);


/*
 * Analisa identificador.
 */

static ValueType analyze_identifier(
    ASTNode *node
)
{
    Symbol *symbol =
        symbol_table_lookup(
            &symbol_stack,
            node->lexeme
        );


    if (symbol == NULL) {

        char message[256];


        snprintf(
            message,
            sizeof(message),
            "IDENTIFICADOR NAO DECLARADO: %s",
            node->lexeme
        );


        semantic_error(
            node->line,
            message
        );


        return TYPE_UNKNOWN;
    }


    return symbol->type;
}


/*
 * Analisa atribuição:
 *
 * x = expressão
 */

static ValueType analyze_assignment(
    ASTNode *node
)
{
    ASTNode *identifier =
        node->child;


    ASTNode *expression =
        identifier->next;


    Symbol *symbol =
        symbol_table_lookup(
            &symbol_stack,
            identifier->lexeme
        );


    if (symbol == NULL) {

        char message[256];


        snprintf(
            message,
            sizeof(message),
            "IDENTIFICADOR NAO DECLARADO: %s",
            identifier->lexeme
        );


        semantic_error(
            identifier->line,
            message
        );


        analyze_expression(
            expression
        );


        return TYPE_UNKNOWN;
    }


    ValueType left_type =
        symbol->type;


    ValueType right_type =
        analyze_expression(
            expression
        );


    if (
        right_type != TYPE_UNKNOWN &&
        left_type != right_type
    ) {

        char message[256];


        snprintf(
            message,
            sizeof(message),
            "TIPOS INCOMPATIVEIS NA ATRIBUICAO: %s = %s",
            type_name(left_type),
            type_name(right_type)
        );


        semantic_error(
            node->line,
            message
        );


        return TYPE_UNKNOWN;
    }


    return left_type;
}


/*
 * Analisa operadores binários.
 */

static ValueType analyze_binary(
    ASTNode *node
)
{
    ASTNode *left =
        node->child;


    ASTNode *right =
        left->next;


    ValueType left_type =
        analyze_expression(
            left
        );


    ValueType right_type =
        analyze_expression(
            right
        );


    /*
     * Evita gerar vários erros derivados
     * do mesmo identificador inválido.
     */

    if (
        left_type == TYPE_UNKNOWN ||
        right_type == TYPE_UNKNOWN
    ) {

        return TYPE_UNKNOWN;
    }


    switch (node->kind) {

        /*
         * ------------------------------------------
         * Operações aritméticas
         * ------------------------------------------
         */

        case AST_ADD:

        case AST_SUB:

        case AST_MUL:

        case AST_DIV:

            if (
                left_type != TYPE_INT ||
                right_type != TYPE_INT
            ) {

                semantic_error(
                    node->line,
                    "OPERACAO ARITMETICA EXIGE EXPRESSOES INT"
                );

                return TYPE_UNKNOWN;
            }


            return TYPE_INT;


        /*
         * ------------------------------------------
         * Operadores relacionais
         * ------------------------------------------
         */

        case AST_LT:

        case AST_GT:

        case AST_LE:

        case AST_GE:

            if (
                left_type != right_type
            ) {

                semantic_error(
                    node->line,
                    "OPERADOR RELACIONAL EXIGE OPERANDOS DO MESMO TIPO"
                );

                return TYPE_UNKNOWN;
            }


            /*
             * Resultado de comparação:
             * int.
             */

            return TYPE_INT;


        /*
         * ------------------------------------------
         * Igualdade
         * ------------------------------------------
         */

        case AST_EQ:

        case AST_NEQ:

            if (
                left_type != right_type
            ) {

                semantic_error(
                    node->line,
                    "OPERADOR DE IGUALDADE EXIGE OPERANDOS DO MESMO TIPO"
                );

                return TYPE_UNKNOWN;
            }


            return TYPE_INT;


        /*
         * ------------------------------------------
         * Operadores lógicos
         * ------------------------------------------
         */

        case AST_OR:

        case AST_AND:

            if (
                left_type != TYPE_INT ||
                right_type != TYPE_INT
            ) {

                semantic_error(
                    node->line,
                    "OPERADOR LOGICO EXIGE EXPRESSOES INT"
                );

                return TYPE_UNKNOWN;
            }


            return TYPE_INT;


        default:

            return TYPE_UNKNOWN;
    }
}


/*
 * Analisa operadores unários.
 */

static ValueType analyze_unary(
    ASTNode *node
)
{
    ASTNode *operand =
        node->child;


    ValueType operand_type =
        analyze_expression(
            operand
        );


    if (
        operand_type ==
        TYPE_UNKNOWN
    ) {

        return TYPE_UNKNOWN;
    }


    /*
     * Negação aritmética.
     *
     * -x exige int.
     */

    if (
        node->kind ==
        AST_NEG
    ) {

        if (
            operand_type !=
            TYPE_INT
        ) {

            semantic_error(
                node->line,
                "OPERADOR UNARIO - EXIGE EXPRESSAO INT"
            );

            return TYPE_UNKNOWN;
        }


        return TYPE_INT;
    }


    /*
     * Negação lógica.
     *
     * !x exige int.
     */

    if (
        node->kind ==
        AST_NOT
    ) {

        if (
            operand_type !=
            TYPE_INT
        ) {

            semantic_error(
                node->line,
                "OPERADOR ! EXIGE EXPRESSAO INT"
            );

            return TYPE_UNKNOWN;
        }


        return TYPE_INT;
    }


    return TYPE_UNKNOWN;
}


/*
 * Descobre o tipo de uma expressão.
 */

static ValueType analyze_expression(
    ASTNode *node
)
{
    if (node == NULL) {

        return TYPE_UNKNOWN;
    }


    switch (node->kind) {

        /*
         * Identificador.
         */

        case AST_IDENTIFIER:

            return analyze_identifier(
                node
            );


        /*
         * Constante inteira.
         */

        case AST_INTCONST:

            return TYPE_INT;


        /*
         * Constante de caractere.
         */

        case AST_CARCONST:

            return TYPE_CAR;


        /*
         * Atribuição.
         */

        case AST_ASSIGN:

            return analyze_assignment(
                node
            );


        /*
         * Operadores binários.
         */

        case AST_ADD:

        case AST_SUB:

        case AST_MUL:

        case AST_DIV:

        case AST_LT:

        case AST_GT:

        case AST_LE:

        case AST_GE:

        case AST_EQ:

        case AST_NEQ:

        case AST_OR:

        case AST_AND:

            return analyze_binary(
                node
            );


        /*
         * Operadores unários.
         */

        case AST_NEG:

        case AST_NOT:

            return analyze_unary(
                node
            );


        /*
         * String não é expressão da linguagem.
         */

        case AST_STRING:

            return TYPE_UNKNOWN;


        default:

            return TYPE_UNKNOWN;
    }
}


/* =========================================================
   DECLARAÇÕES
   ========================================================= */

static void analyze_var_section(
    ASTNode *node
)
{
    if (node == NULL) {

        return;
    }


    ASTNode *decl =
        node->child;


    while (decl != NULL) {

        ValueType type =
            decl->value_type;


        ASTNode *identifier =
            decl->child;


        while (identifier != NULL) {

            int inserted =
                symbol_table_insert(
                    &symbol_stack,
                    identifier->lexeme,
                    type,
                    identifier->line
                );


            if (!inserted) {

                char message[256];


                snprintf(
                    message,
                    sizeof(message),
                    "IDENTIFICADOR JA DECLARADO NO ESCOPO: %s",
                    identifier->lexeme
                );


                semantic_error(
                    identifier->line,
                    message
                );
            }


            identifier =
                identifier->next;
        }


        decl =
            decl->next;
    }
}


/* =========================================================
   COMANDOS
   ========================================================= */

static void analyze_command(
    ASTNode *node
);


static void analyze_command_list(
    ASTNode *node
)
{
    ASTNode *current =
        node->child;


    while (current != NULL) {

        analyze_command(
            current
        );


        current =
            current->next;
    }
}


/*
 * Verifica uma condição de se/enquanto.
 */

static void analyze_condition(
    ASTNode *condition
)
{
    ValueType type =
        analyze_expression(
            condition
        );


    if (
        type != TYPE_UNKNOWN &&
        type != TYPE_INT
    ) {

        semantic_error(
            condition->line,
            "CONDICAO EXIGE EXPRESSAO INT"
        );
    }
}


/*
 * Analisa um bloco.
 */

static void analyze_block(
    ASTNode *node
)
{
    if (node == NULL) {

        return;
    }


    ASTNode *current =
        node->child;


    int has_scope = 0;


    /*
     * Se o primeiro filho for VAR_SECTION,
     * o bloco possui declarações e portanto
     * cria um novo escopo.
     */

    if (
        current != NULL &&
        current->kind ==
            AST_VAR_SECTION
    ) {

        symbol_table_push(
            &symbol_stack
        );


        has_scope = 1;


        analyze_var_section(
            current
        );


        current =
            current->next;
    }


    /*
     * Analisa a lista de comandos.
     */

    while (current != NULL) {

        if (
            current->kind ==
            AST_COMMAND_LIST
        ) {

            analyze_command_list(
                current
            );
        }


        current =
            current->next;
    }


    /*
     * Sai do escopo.
     */

    if (has_scope) {

        symbol_table_pop(
            &symbol_stack
        );
    }
}


/*
 * Analisa um comando.
 */

static void analyze_command(
    ASTNode *node
)
{
    if (node == NULL) {

        return;
    }


    switch (node->kind) {

        case AST_EMPTY:

            break;


        case AST_ASSIGN:

            analyze_expression(
                node
            );

            break;


        case AST_READ:
        {
            ASTNode *identifier =
                node->child;


            Symbol *symbol =
                symbol_table_lookup(
                    &symbol_stack,
                    identifier->lexeme
                );


            if (symbol == NULL) {

                char message[256];


                snprintf(
                    message,
                    sizeof(message),
                    "IDENTIFICADOR NAO DECLARADO: %s",
                    identifier->lexeme
                );


                semantic_error(
                    identifier->line,
                    message
                );
            }


            break;
        }


        case AST_WRITE:

            analyze_expression(
                node->child
            );

            break;


        case AST_WRITE_STRING:

            /*
             * Não é necessária checagem
             * de tipo adicional.
             */

            break;


        case AST_NEWLINE:

            break;


        case AST_IF:
        {
            ASTNode *condition =
                node->child;


            ASTNode *command =
                condition->next;


            analyze_condition(
                condition
            );


            analyze_command(
                command
            );


            break;
        }


        case AST_IF_ELSE:
        {
            ASTNode *condition =
                node->child;


            ASTNode *then_command =
                condition->next;


            ASTNode *else_command =
                then_command->next;


            analyze_condition(
                condition
            );


            analyze_command(
                then_command
            );


            analyze_command(
                else_command
            );


            break;
        }


        case AST_WHILE:
        {
            ASTNode *condition =
                node->child;


            ASTNode *command =
                condition->next;


            analyze_condition(
                condition
            );


            analyze_command(
                command
            );


            break;
        }


        case AST_BLOCK:

            analyze_block(
                node
            );

            break;


        default:

            analyze_expression(
                node
            );

            break;
    }
}


/* =========================================================
   ANÁLISE SEMÂNTICA COMPLETA
   ========================================================= */

int semantic_analyze(
    ASTNode *root
)
{
    errors = 0;


    symbol_table_init(
        &symbol_stack
    );


    /*
     * Nossa AST é:
     *
     * PROGRAM
     *   |
     * BLOCK
     */

    if (root != NULL) {

        ASTNode *block =
            root->child;


        if (
            block != NULL &&
            block->kind ==
                AST_BLOCK
        ) {

            analyze_block(
                block
            );
        }
    }


    symbol_table_destroy(
        &symbol_stack
    );


    return errors == 0;
}


/*
 * Número de erros.
 */

int semantic_error_count(void)
{
    return errors;
}