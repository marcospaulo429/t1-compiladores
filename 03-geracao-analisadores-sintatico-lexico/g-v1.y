%{
/* Declarações em linguagem C */
#include <stdio.h>

extern int yylineno;
extern char *yytext;
extern int yylex(void);
extern FILE *yyin;

void yyerror(const char *s);
%}
/* Definições */
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

%token OU
%token E
%token IGUAL
%token DIFERENTE
%token MAIORIGUAL
%token MENORIGUAL

%token CARCONST
%token INTCONST
%%
/* Produções da gramática */
Programa    : DeclPrograma
    ;

DeclPrograma: PRINCIPAL Bloco
    ;

Bloco       : '{' ListaComando '}'
                | VarSection '{' ListaComando '}'
    ;

VarSection  : '{' ListaDeclVar '}'
    ;

ListaDeclVar: IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar
                | IDENTIFICADOR DeclVar ':' Tipo ';'
    ;

DeclVar     : /* Cadeia Vazia */
                | ',' IDENTIFICADOR DeclVar
    ;

Tipo        : INT
                | CAR
    ;

ListaComando: Comando
                | Comando ListaComando
    ;

Comando     : ';'
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

Expr        : OrExpr
                | IDENTIFICADOR '=' Expr
    ;

OrExpr      : OrExpr OU AndExpr
                | AndExpr
    ;

AndExpr     : AndExpr E EqExpr
                | EqExpr
    ;

EqExpr      : EqExpr IGUAL DesigExpr
                | EqExpr DIFERENTE DesigExpr
                | DesigExpr
    ;

DesigExpr   : DesigExpr '<' AddExpr
                | DesigExpr '>' AddExpr
                | DesigExpr MAIORIGUAL AddExpr
                | DesigExpr MENORIGUAL AddExpr
                | AddExpr
    ;

AddExpr     : AddExpr '+' MulExpr
                | AddExpr '-' MulExpr
                | MulExpr
    ;

MulExpr     : MulExpr '*' UnExpr
                | MulExpr '/' UnExpr
                | UnExpr
    ;

UnExpr      : '-' PrimExpr
                | '!' PrimExpr
                | PrimExpr
    ;

PrimExpr    : IDENTIFICADOR
                | CARCONST
                | INTCONST
                | '(' Expr ')'
    ;
%%
/* Funções do usuário escritas em C */
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