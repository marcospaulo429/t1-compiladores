%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "g-v1.tab.h"

/*
 * O Bison define yylloc quando usamos %locations.
 * O Flex precisa atualizar essa estrutura para que
 * as ações do Bison saibam em qual linha o token apareceu.
 */
extern YYLTYPE yylloc;
extern YYSTYPE yylval;

/*
 * Executado antes de cada regra reconhecida pelo Flex.
 */
#define YY_USER_ACTION                              \
    {                                               \
        yylloc.first_line = yylineno;              \
        yylloc.last_line = yylineno;               \
        yylloc.first_column = 0;                   \
        yylloc.last_column = 0;                    \
    }

/*
 * Último token retornado para o parser. Serve para saber se um
 * '+' ou '-' antes de um número é sinal ou operador (veja INTCONST).
 */
static int ultimo_token = 0;

#define RETORNA(t)          \
    {                       \
        ultimo_token = (t); \
        return (t);         \
    }

static int anterior_eh_operando(void)
{
    return ultimo_token == IDENTIFICADOR
        || ultimo_token == INTCONST
        || ultimo_token == CARCONST
        || ultimo_token == ')';
}
%}

%option noyywrap
%option yylineno

%x COMENTARIO

%%

"principal" {
    RETORNA(PRINCIPAL);
}

"int" {
    RETORNA(INT);
}

"car" {
    RETORNA(CAR);
}

"leia" {
    RETORNA(LEIA);
}

"escreva" {
    RETORNA(ESCREVA);
}

"novalinha" {
    RETORNA(NOVALINHA);
}

"se" {
    RETORNA(SE);
}

"entao" {
    RETORNA(ENTAO);
}

"senao" {
    RETORNA(SENAO);
}

"fimse" {
    RETORNA(FIMSE);
}

"enquanto" {
    RETORNA(ENQUANTO);
}


  /*
   * Operadores formados por mais de um caractere.
   */

"||" {
    RETORNA(OU);
}

"&" {
    RETORNA(E);
}

"==" {
    RETORNA(IGUAL);
}

"!=" {
    RETORNA(DIFERENTE);
}

">=" {
    RETORNA(MAIORIGUAL);
}

"<=" {
    RETORNA(MENORIGUAL);
}


  /*
   * Constante inteira, com ou sem sinal (+ ou -).
   *
   * Cuidado: em "n-1" o Flex pegaria "-1" como número (é o casamento
   * mais longo) e o parser veria IDENTIFICADOR INTCONST, que é erro de
   * sintaxe. Por isso, se o token anterior for um operando, o sinal é
   * operador: devolvemos o resto para a entrada com yyless(1) e
   * retornamos só o '+' ou '-'.
   */

[+-]?[0-9]+ {
    if ((yytext[0] == '+' || yytext[0] == '-') && anterior_eh_operando()) {
        yyless(1);
        RETORNA(yytext[0]);
    }

    yylval.str = strdup(yytext);
    RETORNA(INTCONST);
}


  /*
   * Constante de caractere.
   *
   * Exemplos:
   *
   * 'a'
   * '\n'
   * '\t'
   */

\'([^\\\n']|\\.)\' {
    yylval.str = strdup(yytext);
    RETORNA(CARCONST);
}


  /*
   * Cadeia de caracteres.
   *
   * Começa e termina com ".
   * Não pode conter quebra de linha.
   */

\"([^\\\n\"]|\\.)*\" {
    yylval.str = strdup(yytext);
    RETORNA(CADEIACARACTERES);
}


  /*
   * Início de comentário.
   */

"/*" {
    BEGIN(COMENTARIO);
}


  /*
   * Fim de comentário.
   */

<COMENTARIO>"*/" {
    BEGIN(INITIAL);
}


  /*
   * Nova linha dentro do comentário.
   */

<COMENTARIO>\n {
}


  /*
   * Qualquer outro caractere dentro do comentário
   * é descartado.
   */

<COMENTARIO>. {
}


  /*
   * Fim de arquivo dentro de comentário significa
   * que o comentário não terminou.
   */

<COMENTARIO><<EOF>> {
    printf(
        "ERRO: COMENTÁRIO NAO TERMINA %d\n",
        yylineno
    );

    exit(1);
}


  /*
   * Identificadores.
   *
   * Primeiro caractere:
   * letra ou _
   *
   * Demais:
   * letra, número ou _
   */

[A-Za-z_][A-Za-z0-9_]* {
    yylval.str = strdup(yytext);
    RETORNA(IDENTIFICADOR);
}


  /*
   * Espaços, tabs, retorno de carro e newline
   * não produzem tokens.
   */

[ \t\r\n]+ {
}


  /*
   * Símbolos de um caractere.
   */

"{" {
    RETORNA('{');
}

"}" {
    RETORNA('}');
}

"(" {
    RETORNA('(');
}

")" {
    RETORNA(')');
}

"," {
    RETORNA(',');
}

":" {
    RETORNA(':');
}

";" {
    RETORNA(';');
}

"+" {
    RETORNA('+');
}

"-" {
    RETORNA('-');
}

"*" {
    RETORNA('*');
}

"/" {
    RETORNA('/');
}

"<" {
    RETORNA('<');
}

">" {
    RETORNA('>');
}

"=" {
    RETORNA('=');
}

"!" {
    RETORNA('!');
}


  /*
   * Uma cadeia começou com " mas chegou
   * à quebra de linha sem terminar.
   */

\"[^\"\n]* {
    printf(
        "ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n",
        yylineno
    );

    exit(1);
}


  /*
   * Qualquer outro caractere é inválido.
   */

. {
    printf(
        "ERRO: CARACTERE INVÁLIDO %d\n",
        yylineno
    );

    exit(1);
}

%%