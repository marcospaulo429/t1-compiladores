#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"


/*
 * Imprime indentação.
 */

static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}


/*
 * Cria novo nó da AST.
 */

ASTNode *ast_new(
    ASTKind kind,
    int line,
    const char *lexeme
)
{
    ASTNode *node =
        malloc(sizeof(ASTNode));

    if (node == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        exit(1);
    }

    node->kind = kind;
    node->line = line;

    node->value_type =
        TYPE_UNKNOWN;

    node->child = NULL;
    node->next = NULL;

    node->lexeme = NULL;


    if (lexeme != NULL) {

        node->lexeme =
            malloc(strlen(lexeme) + 1);

        if (node->lexeme == NULL) {

            fprintf(
                stderr,
                "ERRO: memoria insuficiente\n"
            );

            free(node);

            exit(1);
        }

        strcpy(
            node->lexeme,
            lexeme
        );
    }


    return node;
}


/*
 * Cria nó assumindo posse do lexeme.
 */

ASTNode *ast_new_take(
    ASTKind kind,
    int line,
    char *lexeme
)
{
    ASTNode *node =
        malloc(sizeof(ASTNode));

    if (node == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        free(lexeme);

        exit(1);
    }

    node->kind = kind;
    node->line = line;

    node->value_type =
        TYPE_UNKNOWN;

    node->lexeme = lexeme;

    node->child = NULL;
    node->next = NULL;

    return node;
}


/*
 * Adiciona um filho ao final da lista.
 */

void ast_add_child(
    ASTNode *parent,
    ASTNode *child
)
{
    if (
        parent == NULL ||
        child == NULL
    ) {
        return;
    }


    child->next = NULL;


    if (parent->child == NULL) {

        parent->child = child;

        return;
    }


    ASTNode *current =
        parent->child;


    while (current->next != NULL) {

        current =
            current->next;
    }


    current->next = child;
}


/*
 * Acrescenta uma lista de filhos.
 */

void ast_add_child_list(
    ASTNode *parent,
    ASTNode *list
)
{
    if (
        parent == NULL ||
        list == NULL
    ) {
        return;
    }


    if (parent->child == NULL) {

        parent->child = list;

        return;
    }


    ASTNode *current =
        parent->child;


    while (current->next != NULL) {

        current =
            current->next;
    }


    current->next = list;
}


/*
 * Adiciona node ao final de uma lista.
 */

ASTNode *ast_append(
    ASTNode *list,
    ASTNode *node
)
{
    if (node == NULL) {
        return list;
    }


    if (list == NULL) {
        return node;
    }


    ASTNode *current =
        list;


    while (current->next != NULL) {

        current =
            current->next;
    }


    current->next = node;

    return list;
}


/*
 * Cria nó binário.
 */

ASTNode *ast_binary(
    ASTKind kind,
    int line,
    ASTNode *left,
    ASTNode *right
)
{
    ASTNode *node =
        ast_new(
            kind,
            line,
            NULL
        );


    ast_add_child(
        node,
        left
    );


    ast_add_child(
        node,
        right
    );


    return node;
}


/*
 * Cria nó unário.
 */

ASTNode *ast_unary(
    ASTKind kind,
    int line,
    ASTNode *operand
)
{
    ASTNode *node =
        ast_new(
            kind,
            line,
            NULL
        );


    ast_add_child(
        node,
        operand
    );


    return node;
}


/*
 * Nome textual dos nós da AST.
 */

const char *ast_kind_name(
    ASTKind kind
)
{
    switch (kind) {

        case AST_PROGRAM:
            return "PROGRAM";

        case AST_BLOCK:
            return "BLOCK";

        case AST_VAR_SECTION:
            return "VAR_SECTION";

        case AST_VAR_DECL:
            return "VAR_DECL";

        case AST_COMMAND_LIST:
            return "COMMAND_LIST";

        case AST_EMPTY:
            return "EMPTY";

        case AST_ASSIGN:
            return "ASSIGN";

        case AST_READ:
            return "READ";

        case AST_WRITE:
            return "WRITE";

        case AST_WRITE_STRING:
            return "WRITE_STRING";

        case AST_NEWLINE:
            return "NEWLINE";

        case AST_IF:
            return "IF";

        case AST_IF_ELSE:
            return "IF_ELSE";

        case AST_WHILE:
            return "WHILE";

        case AST_OR:
            return "OR";

        case AST_AND:
            return "AND";

        case AST_EQ:
            return "EQ";

        case AST_NEQ:
            return "NEQ";

        case AST_LT:
            return "LT";

        case AST_GT:
            return "GT";

        case AST_GE:
            return "GE";

        case AST_LE:
            return "LE";

        case AST_ADD:
            return "ADD";

        case AST_SUB:
            return "SUB";

        case AST_MUL:
            return "MUL";

        case AST_DIV:
            return "DIV";

        case AST_NEG:
            return "NEG";

        case AST_NOT:
            return "NOT";

        case AST_IDENTIFIER:
            return "IDENTIFIER";

        case AST_INTCONST:
            return "INTCONST";

        case AST_CARCONST:
            return "CARCONST";

        case AST_STRING:
            return "STRING";

        default:
            return "UNKNOWN";
    }
}


/*
 * Nome textual dos tipos da linguagem.
 */

const char *value_type_name(
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


/*
 * Imprime a AST recursivamente.
 */

void ast_print(
    ASTNode *node,
    int indent
)
{
    if (node == NULL) {
        return;
    }


    print_indent(indent);


    printf(
        "%s",
        ast_kind_name(node->kind)
    );


    printf(
        " [linha %d]",
        node->line
    );


    if (node->lexeme != NULL) {

        printf(
            " lexema=\"%s\"",
            node->lexeme
        );
    }


    if (
        node->value_type !=
        TYPE_UNKNOWN
    ) {

        printf(
            " tipo=%s",
            value_type_name(
                node->value_type
            )
        );
    }


    printf("\n");


    ASTNode *child =
        node->child;


    while (child != NULL) {

        ast_print(
            child,
            indent + 1
        );

        child =
            child->next;
    }
}


/*
 * Libera toda a AST.
 */

void ast_free(
    ASTNode *node
)
{
    while (node != NULL) {

        ASTNode *next =
            node->next;


        ast_free(
            node->child
        );


        free(
            node->lexeme
        );


        free(node);


        node = next;
    }
}