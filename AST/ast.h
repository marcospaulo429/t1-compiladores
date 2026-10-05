#ifndef AST_H
#define AST_H

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


typedef enum {
    TYPE_UNKNOWN,
    TYPE_INT,
    TYPE_CAR
} ValueType;


typedef struct ASTNode ASTNode;


struct ASTNode {
    ASTKind kind;

    int line;

    /*
     * Texto original do token.
     * Exemplos:
     *   x
     *   10
     *   'a'
     *   "hello"
     */
    char *lexeme;

    /*
     * Tipo associado ao nó.
     *
     * Em um primeiro momento será principalmente
     * utilizado nos nós de declaração.
     */
    ValueType value_type;

    /*
     * Primeiro filho do nó.
     */
    ASTNode *child;

    /*
     * Próximo irmão na lista.
     */
    ASTNode *next;
};


ASTNode *ast_new(ASTKind kind, int line, const char *lexeme);

ASTNode *ast_new_take(ASTKind kind, int line, char *lexeme);

void ast_add_child(ASTNode *parent, ASTNode *child);

void ast_add_child_list(ASTNode *parent, ASTNode *list);

ASTNode *ast_append(ASTNode *list, ASTNode *node);

ASTNode *ast_binary(ASTKind kind,
                    int line,
                    ASTNode *left,
                    ASTNode *right);

ASTNode *ast_unary(ASTKind kind,
                   int line,
                   ASTNode *operand);

const char *ast_kind_name(ASTKind kind);

const char *value_type_name(ValueType type);

void ast_print(ASTNode *node, int indent);

void ast_free(ASTNode *node);

#endif