%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "semantic.h"


extern int yylineno;
extern char *yytext;
extern int yylex(void);
extern FILE *yyin;


void yyerror(
    const char *s
);


/*
 * Raiz da AST.
 */

ASTNode *ast_root = NULL;

%}


/*
 * Ativa informações de localização.
 */

%locations


/*
 * União de valores semânticos.
 *
 * struct ASTNode * pode ser usado aqui
 * sem conhecer a definição completa da struct
 * no arquivo .tab.h gerado pelo Bison.
 */

%union {

    char *str;

    struct ASTNode *node;

    int type;

}


/* =========================================================
   TOKENS
   ========================================================= */

%token PRINCIPAL

%token INT
%token CAR

%token LEIA
%token ESCREVA
%token NOVALINHA

%token SE
%token ENTAO
%token SENAO
%token FIMSE
%token ENQUANTO

%token OU
%token E
%token IGUAL
%token DIFERENTE
%token MAIORIGUAL
%token MENORIGUAL


/*
 * Tokens que possuem lexema.
 */

%token <str> IDENTIFICADOR

%token <str> CADEIACARACTERES

%token <str> CARCONST

%token <str> INTCONST


/* =========================================================
   TIPOS DOS NÃO-TERMINAIS
   ========================================================= */

%type <node> Programa

%type <node> DeclPrograma

%type <node> Bloco

%type <node> VarSection

%type <node> ListaDeclVar

%type <node> DeclVar

%type <type> Tipo

%type <node> ListaComando

%type <node> Comando

%type <node> Expr

%type <node> OrExpr

%type <node> AndExpr

%type <node> EqExpr

%type <node> DesigExpr

%type <node> AddExpr

%type <node> MulExpr

%type <node> UnExpr

%type <node> PrimExpr


%%


/* =========================================================
   PROGRAMA
   ========================================================= */

Programa
    : DeclPrograma
      {
          /*
           * Criamos o nó raiz.
           */

          $$ =
              ast_new(
                  AST_PROGRAM,
                  @1.first_line,
                  NULL
              );


          ast_add_child(
              $$,
              $1
          );


          ast_root =
              $$;
      }
    ;


DeclPrograma
    : PRINCIPAL Bloco
      {
          /*
           * O único elemento do programa
           * é o bloco principal.
           */

          $$ =
              $2;
      }
    ;


/* =========================================================
   BLOCO
   ========================================================= */

Bloco
    : '{' ListaComando '}'
      {
          $$ =
              ast_new(
                  AST_BLOCK,
                  @1.first_line,
                  NULL
              );


          ASTNode *lista =
              ast_new(
                  AST_COMMAND_LIST,
                  @2.first_line,
                  NULL
              );


          ast_add_child_list(
              lista,
              $2
          );


          ast_add_child(
              $$,
              lista
          );
      }


    | VarSection '{' ListaComando '}'
      {
          $$ =
              ast_new(
                  AST_BLOCK,
                  @1.first_line,
                  NULL
              );


          /*
           * Primeiro ficam as declarações.
           */

          ast_add_child(
              $$,
              $1
          );


          /*
           * Depois fica a lista de comandos.
           */

          ASTNode *lista =
              ast_new(
                  AST_COMMAND_LIST,
                  @3.first_line,
                  NULL
              );


          ast_add_child_list(
              lista,
              $3
          );


          ast_add_child(
              $$,
              lista
          );
      }
    ;


/* =========================================================
   DECLARAÇÕES
   ========================================================= */

VarSection
    : '{' ListaDeclVar '}'
      {
          $$ =
              ast_new(
                  AST_VAR_SECTION,
                  @1.first_line,
                  NULL
              );


          ast_add_child_list(
              $$,
              $2
          );
      }
    ;


ListaDeclVar
    : IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar
      {
          ASTNode *decl =
              ast_new(
                  AST_VAR_DECL,
                  @1.first_line,
                  NULL
              );


          decl->value_type =
              $4;


          ASTNode *first_id =
              ast_new_take(
                  AST_IDENTIFIER,
                  @1.first_line,
                  $1
              );


          ast_add_child(
              decl,
              first_id
          );


          /*
           * Adiciona os demais identificadores.
           */

          ast_add_child_list(
              decl,
              $2
          );


          /*
           * Acrescenta a próxima declaração.
           */

          $$ =
              ast_append(
                  decl,
                  $6
              );
      }


    | IDENTIFICADOR DeclVar ':' Tipo ';'
      {
          ASTNode *decl =
              ast_new(
                  AST_VAR_DECL,
                  @1.first_line,
                  NULL
              );


          decl->value_type =
              $4;


          ASTNode *first_id =
              ast_new_take(
                  AST_IDENTIFIER,
                  @1.first_line,
                  $1
              );


          ast_add_child(
              decl,
              first_id
          );


          ast_add_child_list(
              decl,
              $2
          );


          $$ =
              decl;
      }
    ;


DeclVar
    : /* vazio */
      {
          $$ =
              NULL;
      }


    | ',' IDENTIFICADOR DeclVar
      {
          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  @2.first_line,
                  $2
              );


          id->next =
              $3;


          $$ =
              id;
      }
    ;


Tipo
    : INT
      {
          $$ =
              TYPE_INT;
      }


    | CAR
      {
          $$ =
              TYPE_CAR;
      }
    ;


/* =========================================================
   LISTA DE COMANDOS
   ========================================================= */

ListaComando
    : Comando
      {
          $$ =
              $1;
      }


    | Comando ListaComando
      {
          $$ =
              ast_append(
                  $1,
                  $2
              );
      }
    ;


/* =========================================================
   COMANDOS
   ========================================================= */

Comando
    : ';'
      {
          $$ =
              ast_new(
                  AST_EMPTY,
                  @1.first_line,
                  NULL
              );
      }


    | Expr ';'
      {
          $$ =
              $1;
      }


    | LEIA IDENTIFICADOR ';'
      {
          $$ =
              ast_new(
                  AST_READ,
                  @1.first_line,
                  NULL
              );


          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  @2.first_line,
                  $2
              );


          ast_add_child(
              $$,
              id
          );
      }


    | ESCREVA Expr ';'
      {
          $$ =
              ast_new(
                  AST_WRITE,
                  @1.first_line,
                  NULL
              );


          ast_add_child(
              $$,
              $2
          );
      }


    | ESCREVA CADEIACARACTERES ';'
      {
          $$ =
              ast_new(
                  AST_WRITE_STRING,
                  @1.first_line,
                  NULL
              );


          ASTNode *string =
              ast_new_take(
                  AST_STRING,
                  @2.first_line,
                  $2
              );


          ast_add_child(
              $$,
              string
          );
      }


    | NOVALINHA ';'
      {
          $$ =
              ast_new(
                  AST_NEWLINE,
                  @1.first_line,
                  NULL
              );
      }


    | SE '(' Expr ')' ENTAO Comando FIMSE
      {
          $$ =
              ast_new(
                  AST_IF,
                  @1.first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: comando
           */

          ast_add_child(
              $$,
              $3
          );


          ast_add_child(
              $$,
              $6
          );
      }


    | SE '(' Expr ')' ENTAO Comando SENAO Comando FIMSE
      {
          $$ =
              ast_new(
                  AST_IF_ELSE,
                  @1.first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: then
           * Filho 3: else
           */

          ast_add_child(
              $$,
              $3
          );


          ast_add_child(
              $$,
              $6
          );


          ast_add_child(
              $$,
              $8
          );
      }


    | ENQUANTO '(' Expr ')' Comando
      {
          $$ =
              ast_new(
                  AST_WHILE,
                  @1.first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: comando
           */

          ast_add_child(
              $$,
              $3
          );


          ast_add_child(
              $$,
              $5
          );
      }


    | Bloco
      {
          $$ =
              $1;
      }
    ;


/* =========================================================
   EXPRESSÕES
   ========================================================= */

Expr
    : OrExpr
      {
          $$ =
              $1;
      }


    | IDENTIFICADOR '=' Expr
      {
          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  @1.first_line,
                  $1
              );


          $$ =
              ast_binary(
                  AST_ASSIGN,
                  @2.first_line,
                  id,
                  $3
              );
      }
    ;


OrExpr
    : OrExpr OU AndExpr
      {
          $$ =
              ast_binary(
                  AST_OR,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | AndExpr
      {
          $$ =
              $1;
      }
    ;


AndExpr
    : AndExpr E EqExpr
      {
          $$ =
              ast_binary(
                  AST_AND,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | EqExpr
      {
          $$ =
              $1;
      }
    ;


EqExpr
    : EqExpr IGUAL DesigExpr
      {
          $$ =
              ast_binary(
                  AST_EQ,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | EqExpr DIFERENTE DesigExpr
      {
          $$ =
              ast_binary(
                  AST_NEQ,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | DesigExpr
      {
          $$ =
              $1;
      }
    ;


DesigExpr
    : DesigExpr '<' AddExpr
      {
          $$ =
              ast_binary(
                  AST_LT,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | DesigExpr '>' AddExpr
      {
          $$ =
              ast_binary(
                  AST_GT,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | DesigExpr MAIORIGUAL AddExpr
      {
          $$ =
              ast_binary(
                  AST_GE,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | DesigExpr MENORIGUAL AddExpr
      {
          $$ =
              ast_binary(
                  AST_LE,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | AddExpr
      {
          $$ =
              $1;
      }
    ;


AddExpr
    : AddExpr '+' MulExpr
      {
          $$ =
              ast_binary(
                  AST_ADD,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | AddExpr '-' MulExpr
      {
          $$ =
              ast_binary(
                  AST_SUB,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | MulExpr
      {
          $$ =
              $1;
      }
    ;


MulExpr
    : MulExpr '*' UnExpr
      {
          $$ =
              ast_binary(
                  AST_MUL,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | MulExpr '/' UnExpr
      {
          $$ =
              ast_binary(
                  AST_DIV,
                  @2.first_line,
                  $1,
                  $3
              );
      }


    | UnExpr
      {
          $$ =
              $1;
      }
    ;


UnExpr
    : '-' PrimExpr
      {
          $$ =
              ast_unary(
                  AST_NEG,
                  @1.first_line,
                  $2
              );
      }


    | '!' PrimExpr
      {
          $$ =
              ast_unary(
                  AST_NOT,
                  @1.first_line,
                  $2
              );
      }


    | PrimExpr
      {
          $$ =
              $1;
      }
    ;


PrimExpr
    : IDENTIFICADOR
      {
          $$ =
              ast_new_take(
                  AST_IDENTIFIER,
                  @1.first_line,
                  $1
              );
      }


    | CARCONST
      {
          $$ =
              ast_new_take(
                  AST_CARCONST,
                  @1.first_line,
                  $1
              );
      }


    | INTCONST
      {
          $$ =
              ast_new_take(
                  AST_INTCONST,
                  @1.first_line,
                  $1
              );
      }


    | '(' Expr ')'
      {
          /*
           * Não criamos um nó para parênteses.
           */

          $$ =
              $2;
      }
    ;


%%


/* =========================================================
   ERRO SINTÁTICO
   ========================================================= */

void yyerror(
    const char *s
)
{
    fprintf(
        stderr,
        "ERRO: %s linha %d\n",
        s,
        yylineno
    );
}


/* =========================================================
   MAIN
   ========================================================= */

int main(
    int argc,
    char **argv
)
{
    if (argc != 2) {

        fprintf(
            stderr,
            "Uso: %s arquivo.g\n",
            argv[0]
        );

        return 1;
    }


    yyin =
        fopen(
            argv[1],
            "r"
        );


    if (yyin == NULL) {

        perror(
            "Erro ao abrir arquivo"
        );

        return 1;
    }


    int resultado =
        yyparse();


    fclose(yyin);


    /*
     * Se houve erro sintático,
     * não fazemos análise semântica.
     */

    if (resultado != 0) {

        ast_free(
            ast_root
        );

        return resultado;
    }


    printf(
        "\n=== AST ===\n"
    );


    ast_print(
        ast_root,
        0
    );


    printf(
        "\n=== ANALISE SEMANTICA ===\n"
    );


    int semantic_ok =
        semantic_analyze(
            ast_root
        );


    if (!semantic_ok) {

        ast_free(
            ast_root
        );

        return 1;
    }


    printf(
        "Programa semanticamente correto.\n"
    );


    ast_free(
        ast_root
    );


    return 0;
}