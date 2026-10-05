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
%}

%option noyywrap
%option yylineno

%x COMENTARIO

%%

"principal" {
    return PRINCIPAL;
}

"int" {
    return INT;
}

"car" {
    return CAR;
}

"leia" {
    return LEIA;
}

"escreva" {
    return ESCREVA;
}

"novalinha" {
    return NOVALINHA;
}

"se" {
    return SE;
}

"entao" {
    return ENTAO;
}

"senao" {
    return SENAO;
}

"fimse" {
    return FIMSE;
}

"enquanto" {
    return ENQUANTO;
}


  /*
   * Operadores formados por mais de um caractere.
   */

"||" {
    return OU;
}

"&" {
    return E;
}

"==" {
    return IGUAL;
}

"!=" {
    return DIFERENTE;
}

">=" {
    return MAIORIGUAL;
}

"<=" {
    return MENORIGUAL;
}


  /*
   * Constante inteira.
   *
   * A especificação da G-V1 permite sinal opcional.
   */

[+-]?[0-9]+ {
    yylval.str = strdup(yytext);
    return INTCONST;
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
    return CARCONST;
}


  /*
   * Cadeia de caracteres.
   *
   * Começa e termina com ".
   * Não pode conter quebra de linha.
   */

\"([^\\\n\"]|\\.)*\" {
    yylval.str = strdup(yytext);
    return CADEIACARACTERES;
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
    return IDENTIFICADOR;
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
    return '{';
}

"}" {
    return '}';
}

"(" {
    return '(';
}

")" {
    return ')';
}

"," {
    return ',';
}

":" {
    return ':';
}

";" {
    return ';';
}

"+" {
    return '+';
}

"-" {
    return '-';
}

"*" {
    return '*';
}

"/" {
    return '/';
}

"<" {
    return '<';
}

">" {
    return '>';
}

"=" {
    return '=';
}

"!" {
    return '!';
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