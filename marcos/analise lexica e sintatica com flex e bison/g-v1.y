%{
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;
extern char *yytext;
extern int yylex(void);
extern FILE *yyin;

void yyerror(const char *s);
%}

/* Tokens da linguagem G-V1 */

%token PRINCIPAL
%token IDENTIFICADOR

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

%token CADEIACARACTERES
%token CARCONST
%token INTCONST

%token OU
%token E
%token IGUAL
%token DIFERENTE
%token MAIORIGUAL
%token MENORIGUAL

%%

/* ---------------------------------------------------------
   Programa
   --------------------------------------------------------- */

Programa
    : DeclPrograma
    ;

DeclPrograma
    : PRINCIPAL Bloco
    ;


/* ---------------------------------------------------------
   Bloco
   --------------------------------------------------------- */

Bloco
    : '{' ListaComando '}'
    | VarSection '{' ListaComando '}'
    ;


/* ---------------------------------------------------------
   Declaração de variáveis
   --------------------------------------------------------- */

VarSection
    : '{' ListaDeclVar '}'
    ;

ListaDeclVar
    : IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar
    | IDENTIFICADOR DeclVar ':' Tipo ';'
    ;

DeclVar
    : /* vazio */
    | ',' IDENTIFICADOR DeclVar
    ;

Tipo
    : INT
    | CAR
    ;


/* ---------------------------------------------------------
   Lista de comandos
   --------------------------------------------------------- */

ListaComando
    : Comando
    | Comando ListaComando
    ;


/* ---------------------------------------------------------
   Comandos
   --------------------------------------------------------- */

Comando
    : ';'
    | Expr ';'
    | LEIA IDENTIFICADOR ';'
    | ESCREVA Expr ';'
    | ESCREVA CADEIACARACTERES ';'
    | NOVALINHA ';'
    | SE '(' Expr ')' ENTAO Comando FIMSE
    | SE '(' Expr ')' ENTAO Comando SENAO Comando FIMSE
    | ENQUANTO '(' Expr ')' Comando
    | Bloco
    ;


/* ---------------------------------------------------------
   Expressões
   --------------------------------------------------------- */

Expr
    : OrExpr
    | IDENTIFICADOR '=' Expr
    ;

OrExpr
    : OrExpr OU AndExpr
    | AndExpr
    ;

AndExpr
    : AndExpr E EqExpr
    | EqExpr
    ;

EqExpr
    : EqExpr IGUAL DesigExpr
    | EqExpr DIFERENTE DesigExpr
    | DesigExpr
    ;

DesigExpr
    : DesigExpr '<' AddExpr
    | DesigExpr '>' AddExpr
    | DesigExpr MAIORIGUAL AddExpr
    | DesigExpr MENORIGUAL AddExpr
    | AddExpr
    ;

AddExpr
    : AddExpr '+' MulExpr
    | AddExpr '-' MulExpr
    | MulExpr
    ;

MulExpr
    : MulExpr '*' UnExpr
    | MulExpr '/' UnExpr
    | UnExpr
    ;

UnExpr
    : '-' PrimExpr
    | '!' PrimExpr
    | PrimExpr
    ;

PrimExpr
    : IDENTIFICADOR
    | CARCONST
    | INTCONST
    | '(' Expr ')'
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "ERRO: %s linha %d\n", s, yylineno);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s arquivo.g\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if (yyin == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    int resultado = yyparse();

    fclose(yyin);

    return resultado;
}