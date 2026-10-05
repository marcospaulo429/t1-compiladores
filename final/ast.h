#ifndef AST_H
#define AST_H


/*
 * Tipos de nós da AST.
 */

typedef enum {

    AST_PROGRAM,
    AST_BLOCK,

    AST_VAR_SECTION,
    AST_VAR_DECL,
    AST_COMMAND_LIST,

    AST_EMPTY,
    AST_ASSIGN,
    AST_READ,
    AST_WRITE,
    AST_WRITE_STRING,
    AST_NEWLINE,

    AST_IF,
    AST_IF_ELSE,
    AST_WHILE,

    AST_OR,
    AST_AND,

    AST_EQ,
    AST_NEQ,

    AST_LT,
    AST_GT,
    AST_GE,
    AST_LE,

    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,

    AST_NEG,
    AST_NOT,

    AST_IDENTIFIER,
    AST_INTCONST,
    AST_CARCONST,
    AST_STRING

} ASTKind;


/*
 * Tipos da linguagem G-V1.
 */

typedef enum {

    TYPE_UNKNOWN,
    TYPE_INT,
    TYPE_CAR

} ValueType;


typedef struct ASTNode ASTNode;


/*
 * Estrutura de um nó da AST.
 */

struct ASTNode {

    ASTKind kind;

    /*
     * Linha no programa fonte.
     */
    int line;

    /*
     * Lexema original.
     *
     * Exemplos:
     * x
     * 10
     * 'a'
     * "hello"
     */
    char *lexeme;

    /*
     * Tipo semântico.
     */
    ValueType value_type;

    /*
     * Primeiro filho.
     */
    ASTNode *child;

    /*
     * Próximo irmão.
     */
    ASTNode *next;
};


/*
 * Cria nó copiando o lexema.
 */
ASTNode *ast_new(
    ASTKind kind,
    int line,
    const char *lexeme
);


/*
 * Cria nó assumindo posse da string recebida.
 */
ASTNode *ast_new_take(
    ASTKind kind,
    int line,
    char *lexeme
);


/*
 * Adiciona filho.
 */
void ast_add_child(
    ASTNode *parent,
    ASTNode *child
);


/*
 * Adiciona uma lista de filhos.
 */
void ast_add_child_list(
    ASTNode *parent,
    ASTNode *list
);


/*
 * Acrescenta node ao final de uma lista.
 */
ASTNode *ast_append(
    ASTNode *list,
    ASTNode *node
);


/*
 * Cria nó binário.
 */
ASTNode *ast_binary(
    ASTKind kind,
    int line,
    ASTNode *left,
    ASTNode *right
);


/*
 * Cria nó unário.
 */
ASTNode *ast_unary(
    ASTKind kind,
    int line,
    ASTNode *operand
);


/*
 * Nome textual do tipo da AST.
 */
const char *ast_kind_name(
    ASTKind kind
);


/*
 * Nome textual do tipo da linguagem.
 */
const char *value_type_name(
    ValueType type
);


/*
 * Imprime a AST.
 */
void ast_print(
    ASTNode *node,
    int indent
);


/*
 * Libera a AST.
 */
void ast_free(
    ASTNode *node
);

#endif