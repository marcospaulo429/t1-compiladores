%{
#include <stdio.h>
#include <stdlib.h>
#include "g-v1.tab.h"
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

[+-]?[0-9]+ {
    return INTCONST;
}

\'([^\\\n']|\\.)\' {
    return CARCONST;
}

\"([^\\\n\"]|\\.)*\" {
    return CADEIACARACTERES;
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
    printf("ERRO: COMENTARIO NAO TERMINA %d\n", yylineno);
    exit(1);
}

[A-Za-z_][A-Za-z0-9_]* {
    return IDENTIFICADOR;
}

[ \t\r\n]+ {
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

\"[^\"\n]* {
    printf("ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA %d\n",
           yylineno);
    exit(1);
}

. {
    printf("ERRO: CARACTERE INVALIDO %s %d\n",
           yytext, yylineno);
    exit(1);
}

%%