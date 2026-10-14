    /* Seção de definições */
%{
#include <stdio.h>
#include <stdlib.h>
#include "g-v1.tab.h"
%}

%option noyywrap
%option yylineno

%x COMENTARIO

%%
 /* Seção de regras */
[ \t\r\n]+ {
    /* descarta */
}

"/*" {
    BEGIN(COMENTARIO);
}

<COMENTARIO>"*/" {
    BEGIN(INITIAL);
}

<COMENTARIO>\n {
}

<COMENTARIO>. {
}

<COMENTARIO><<EOF>> {
    printf("ERRO: COMENTÁRIO NAO TERMINA %d\n", yylineno);
    exit(1);
}


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


"||" {
    return OU;
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

"&" {
    return E;
}

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


[0-9]+ {
    return INTCONST;
}

\'([^\\\n']|\\.)\' {
    return CARCONST;
}

\"([^\\\n\"]|\\.)*\" {
    return CADEIACARACTERES;
}


[A-Za-z_][A-Za-z0-9_]* {
    return IDENTIFICADOR;
}


\"[^\"\n]* {
    printf("ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n", yylineno);
    exit(1);
}

. {
    printf("ERRO: CARACTERE INVÁLIDO %d\n", yylineno);
    exit(1);
}

%%
    /* seção de código */