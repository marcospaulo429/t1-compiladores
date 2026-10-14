%{
/* Declarações em linguagem C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"
#include "tabela_simbolos.h"
#include "semantico.h"
#include "gerador.h"

extern int yylineno;
extern char *yytext;
extern int yylex(void);
extern FILE *yyin;

void yyerror(const char *s);

Toperador * criaOperador(TespecieOperador tipoOperador, int linha, Toperador * filho1, Toperador * filho2, Toperador * filho3, char * lexema);
void percorreArvore(Toperador * raiz);
void obtemEspecieNoEnumLin(Toperador* no, char* nomeOperador);

void testaTabelaSimbolosAST(Toperador *no, PilhaTabelas *pilha);

Toperador * raiz = NULL;
char nomeOperador[200];
%}

/* Definições */
%union {
    int nlinha;
    Toperador * Tpont;
    char * numlexema;
}

/* Tokens com atributos semânticos */
%token <numlexema> IDENTIFICADOR
%token <numlexema> INTCONST
%token <numlexema> CARCONST
%token <numlexema> CADEIACARACTERES

/* Tokens simples */
%token PRINCIPAL
%token INT CAR
%token LEIA ESCREVA NOVALINHA
%token SE ENTAO SENAO FIMSE ENQUANTO
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL

/* Não-terminais */
%type <Tpont> Programa DeclPrograma Bloco VarSection ListaDeclVar DeclVar Tipo
%type <Tpont> ListaComando Comando Expr OrExpr AndExpr EqExpr DesigExpr AddExpr MulExpr UnExpr PrimExpr
%%
/* Produções da gramática */
Programa    : DeclPrograma  { raiz = $1; $$ = $1; }
    ;

DeclPrograma: PRINCIPAL Bloco   { $$ = criaOperador(noProg, yylineno, $2, NULL, NULL, NULL); }
    ;

Bloco       : '{' ListaComando '}'  { $$ = criaOperador(noBloco, yylineno, NULL, $2, NULL, NULL); }
                | VarSection '{' ListaComando '}'   {$$ = criaOperador(noBloco, yylineno, $1, $3, NULL, NULL); }
    ;

VarSection  : '{' ListaDeclVar '}' { $$ = criaOperador(noVarSec, yylineno, $2, NULL, NULL, NULL); }
    ;

ListaDeclVar: IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar
                {
                    Toperador *id = criaOperador(noId, yylineno, NULL, NULL, NULL, $1);
                    Toperador *lstIds = criaOperador(noLstId, yylineno, id, $2, NULL, NULL);
                    Toperador *decl = criaOperador(noDeclVar, yylineno, lstIds, $4, NULL, NULL);
                    $$ = criaOperador(noLstDecl, yylineno, decl, $6, NULL, NULL);
                }
                | IDENTIFICADOR DeclVar ':' Tipo ';'
                    { 
                        Toperador *id = criaOperador(noId, yylineno, NULL, NULL, NULL, $1);
                        Toperador *lstIds = criaOperador(noLstId, yylineno, id, $2, NULL, NULL);
                        Toperador *decl = criaOperador(noDeclVar, yylineno, lstIds, $4, NULL, NULL);
                        $$ = criaOperador(noLstDecl, yylineno, decl, NULL, NULL, NULL);
                    }
    ;

DeclVar     : /* Cadeia Vazia */    { $$ = NULL; }
                | ',' IDENTIFICADOR DeclVar
                    { 
                        Toperador *id = criaOperador(noId, yylineno, NULL, NULL, NULL, $2);
                        $$ = criaOperador(noLstId, yylineno, id, $3, NULL, NULL);
                    }
    ;

Tipo        : INT   { $$ = criaOperador(noTipoInt, yylineno, NULL, NULL, NULL, "int"); }
                | CAR   { $$ = criaOperador(noTipoCar, yylineno, NULL, NULL, NULL, "car"); }
    ;

ListaComando: Comando   { $$ = criaOperador(noLstCmd, yylineno, $1, NULL, NULL, NULL); }
                | Comando ListaComando  { $$ = criaOperador(noLstCmd, yylineno, $1, $2, NULL, NULL); }
    ;

Comando     : ';'   { $$ = criaOperador(noCmdVazio, yylineno, NULL, NULL, NULL, NULL); }
                | Expr ';'  { $$ = $1; }
                | LEIA IDENTIFICADOR ';'
                    { 
                        Toperador *id = criaOperador(noId, yylineno, NULL, NULL, NULL, $2);
                        $$ = criaOperador(noLeia, yylineno, id, NULL, NULL, NULL);
                    }
                | ESCREVA Expr ';'  { $$ = criaOperador(noEscreva, yylineno, $2, NULL, NULL, NULL); }
                | ESCREVA CADEIACARACTERES ';'
                    { 
                        Toperador *str = criaOperador(noCadConst, yylineno, NULL, NULL, NULL, $2);
                        $$ = criaOperador(noEscrevaCadeia, yylineno, str, NULL, NULL, NULL);
                    }
                | NOVALINHA ';' { $$ = criaOperador(noNovalinha, yylineno, NULL, NULL, NULL, NULL); }
                | SE '(' Expr ')' ENTAO Comando FIMSE   { $$ = criaOperador(noSe, yylineno, $3, $6, NULL, NULL); }
                | SE '(' Expr ')' ENTAO Comando SENAO Comando FIMSE    { $$ = criaOperador(noSeSenao, yylineno, $3, $6, $8, NULL); }
                | ENQUANTO '(' Expr ')' Comando    { $$ = criaOperador(noEnquanto, yylineno, $3, $5, NULL, NULL); }
                | Bloco   { $$ = $1; }
    ;

Expr        : OrExpr    { $$ = $1; }
                | IDENTIFICADOR '=' Expr
                    { 
                        Toperador *id = criaOperador(noId, yylineno, NULL, NULL, NULL, $1);
                        $$ = criaOperador(noAtrib, yylineno, id, $3, NULL, NULL);
                    }
    ;

OrExpr      : OrExpr OU AndExpr   { $$ = criaOperador(noOU, yylineno, $1, $3, NULL, NULL); }
                | AndExpr   { $$ = $1; }
    ;

AndExpr     : AndExpr E EqExpr   { $$ = criaOperador(noE, yylineno, $1, $3, NULL, NULL); }
                | EqExpr    { $$ = $1; }
    ;

EqExpr      : EqExpr IGUAL DesigExpr    { $$ = criaOperador(noIgual, yylineno, $1, $3, NULL, NULL); }
                | EqExpr DIFERENTE DesigExpr    { $$ = criaOperador(noDiferente, yylineno, $1, $3, NULL, NULL); }
                | DesigExpr   { $$ = $1; }
    ;

DesigExpr   : DesigExpr '<' AddExpr   { $$ = criaOperador(noMenor, yylineno, $1, $3, NULL, NULL); }
                | DesigExpr '>' AddExpr   { $$ = criaOperador(noMaior, yylineno, $1, $3, NULL, NULL); }
                | DesigExpr MAIORIGUAL AddExpr  { $$ = criaOperador(noMaiorIgual, yylineno, $1, $3, NULL, NULL); }
                | DesigExpr MENORIGUAL AddExpr  { $$ = criaOperador(noMenorIgual, yylineno, $1, $3, NULL, NULL); }
                | AddExpr   { $$ = $1; }
    ;

AddExpr     : AddExpr '+' MulExpr   { $$ = criaOperador(noMais, yylineno, $1, $3, NULL, NULL); }
                | AddExpr '-' MulExpr { $$ = criaOperador(noMenos, yylineno, $1, $3, NULL, NULL); }   
                | MulExpr   { $$ = $1; }
    ;

MulExpr     : MulExpr '*' UnExpr    { $$ = criaOperador(noMult, yylineno, $1, $3, NULL, NULL); }
                | MulExpr '/' UnExpr    { $$ = criaOperador(noDiv, yylineno, $1, $3, NULL, NULL); }
                | UnExpr    { $$ = $1; }
    ;

UnExpr      : '-' PrimExpr  { $$ = criaOperador(noMenosUnario, yylineno, $2, NULL, NULL, NULL); }
                | '!' PrimExpr  { $$ = criaOperador(noNao, yylineno, $2, NULL, NULL, NULL); }
                | PrimExpr  { $$ = $1; }
    ;

PrimExpr    : IDENTIFICADOR    { $$ = criaOperador(noId, yylineno, NULL, NULL, NULL, $1); }
                | CARCONST     { $$ = criaOperador(noCarConst, yylineno, NULL, NULL, NULL, $1); }
                | INTCONST     { $$ = criaOperador(noIntConst, yylineno, NULL, NULL, NULL, $1); }
                | '(' Expr ')'  { $$ = $2; }
    ;
%%
/* Funções do usuário escritas em C */
void yyerror(const char *s)
{
    fprintf(stderr, "ERRO: %s linha %d\n", s, yylineno);
}


int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Uso: %s arquivo.g [saida.asm]\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    int resultado = yyparse();
    fclose(yyin);
    if (resultado != 0) {
        return resultado;
    }
    
    analiseSemantica(raiz);

    char nomeSaida[256];
    if (argc == 3) {
        strncpy(nomeSaida, argv[2], sizeof(nomeSaida) - 1);
        nomeSaida[sizeof(nomeSaida) - 1] = '\0';
    } else {
        strncpy(nomeSaida, argv[1], sizeof(nomeSaida) - 5);
        nomeSaida[sizeof(nomeSaida) - 5] = '\0';
        char *ponto = strrchr(nomeSaida, '.');
        if (ponto) *ponto = '\0';
        strcat(nomeSaida, ".asm");
    }

    FILE *fout = fopen(nomeSaida, "w");
    if (!fout) {
        perror("Erro ao criar arquivo MIPS de saida");
        return 1;
    }

    geraCodigoMIPS(raiz, fout);
    fclose(fout);

    printf("Codigo MIPS gerado com sucesso: %s\n", nomeSaida);

    return 0;
}

Toperador * criaOperador(TespecieOperador tipoOperador, int linha, Toperador * filho1, Toperador * filho2, Toperador * filho3, char * lexema) {
    Toperador * no = (Toperador*) malloc(sizeof(Toperador));
    if (!no) {
        fprintf(stderr, "Erro de alocacao de memoria para o no da AST\n");
        exit(1);
    }
    no->tipoOperador = tipoOperador;
    no->linha = linha;
    no->filho1 = filho1;
    no->filho2 = filho2;
    no->filho3 = filho3;
    no->lexema = lexema;
    return no;
}

void percorreArvore(Toperador* raiz)
{
    if (raiz) {
        obtemEspecieNoEnumLin(raiz, nomeOperador);
        printf("%s", nomeOperador);
        percorreArvore(raiz->filho1);
        percorreArvore(raiz->filho2);
        percorreArvore(raiz->filho3);
    }
}

void obtemEspecieNoEnumLin(Toperador* no, char* nomeOperador)
{
    switch(no->tipoOperador) {
        case noProg:
            sprintf(nomeOperador, "principal | Lin: %d\n", no->linha);
            break;
        case noBloco:
            sprintf(nomeOperador, "bloco | Lin: %d\n", no->linha);
            break;
        case noVarSec:
            sprintf(nomeOperador, "varSection | Lin: %d\n", no->linha);
            break;
        case noLstDecl:
            sprintf(nomeOperador, "lstDecl\n");
            break;
        case noDeclVar:
            sprintf(nomeOperador, "declVar | Lin: %d\n", no->linha);
            break;
        case noLstId:
            sprintf(nomeOperador, "lstId\n");
            break;
        case noTipoInt:
            sprintf(nomeOperador, "tipo: int\n");
            break;
        case noTipoCar:
            sprintf(nomeOperador, "tipo: car\n");
            break;

        case noLstCmd:
            sprintf(nomeOperador, "lstCmd\n");
            break;
        case noCmdVazio:
            sprintf(nomeOperador, "; | Lin: %d\n", no->linha);
            break;
        case noAtrib:
            sprintf(nomeOperador, "= | Lin: %d\n", no->linha);
            break;
        case noLeia:
            sprintf(nomeOperador, "leia | Lin: %d\n", no->linha);
            break;
        case noEscreva:
            sprintf(nomeOperador, "escreva | Lin: %d\n", no->linha);
            break;
        case noEscrevaCadeia:
            sprintf(nomeOperador, "escreva string | Lin: %d\n", no->linha);
            break;
        case noNovalinha:
            sprintf(nomeOperador, "novalinha | Lin: %d\n", no->linha);
            break;
        case noSe:
            sprintf(nomeOperador, "se | Lin: %d\n", no->linha);
            break;
        case noSeSenao:
            sprintf(nomeOperador, "se-senao | Lin: %d\n", no->linha);
            break;
        case noEnquanto:
            sprintf(nomeOperador, "enquanto | Lin: %d\n", no->linha);
            break;

        case noOU:
            sprintf(nomeOperador, "|| | Lin: %d\n", no->linha);
            break;
        case noE:
            sprintf(nomeOperador, "& | Lin: %d\n", no->linha);
            break;
        case noIgual:
            sprintf(nomeOperador, "== | Lin: %d\n", no->linha);
            break;
        case noDiferente:
            sprintf(nomeOperador, "!= | Lin: %d\n", no->linha);
            break;
        case noMenor:
            sprintf(nomeOperador, "< | Lin: %d\n", no->linha);
            break;
        case noMaior:
            sprintf(nomeOperador, "> | Lin: %d\n", no->linha);
            break;
        case noMenorIgual:
            sprintf(nomeOperador, "<= | Lin: %d\n", no->linha);
            break;
        case noMaiorIgual:
            sprintf(nomeOperador, ">= | Lin: %d\n", no->linha);
            break;

        case noMais:
            sprintf(nomeOperador, "+ | Lin: %d\n", no->linha);
            break;
        case noMenos:
            sprintf(nomeOperador, "- | Lin: %d\n", no->linha);
            break;
        case noMult:
            sprintf(nomeOperador, "* | Lin: %d\n", no->linha);
            break;
        case noDiv:
            sprintf(nomeOperador, "/ | Lin: %d\n", no->linha);
            break;

        case noMenosUnario:
            sprintf(nomeOperador, "- (unario) | Lin: %d\n", no->linha);
            break;
        case noNao:
            sprintf(nomeOperador, "! | Lin: %d\n", no->linha);
            break;

        case noId:
            sprintf(nomeOperador, "%s | Lin: %d\n", no->lexema, no->linha);
            break;
        case noIntConst:
            sprintf(nomeOperador, "%s | Lin: %d\n", no->lexema, no->linha);
            break;
        case noCarConst:
            sprintf(nomeOperador, "%s | Lin: %d\n", no->lexema, no->linha);
            break;
        case noCadConst:
            sprintf(nomeOperador, "%s | Lin: %d\n", no->lexema, no->linha);
            break;
    }
}