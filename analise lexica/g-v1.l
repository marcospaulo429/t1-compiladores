%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap
%option yylineno

%x COMENTARIO

%%

"principal" {
    printf("PRINCIPAL -> %s\n", yytext);
}

"int" {
    printf("INT -> %s\n", yytext);
}

"car" {
    printf("CAR -> %s\n", yytext);
}

"leia" {
    printf("LEIA -> %s\n", yytext);
}

"escreva" {
    printf("ESCREVA -> %s\n", yytext);
}

"novalinha" {
    printf("NOVALINHA -> %s\n", yytext);
}

"se" {
    printf("SE -> %s\n", yytext);
}

"entao" {
    printf("ENTAO -> %s\n", yytext);
}

"senao" {
    printf("SENAO -> %s\n", yytext);
}

"fimse" {
    printf("FIMSE -> %s\n", yytext);
}

"enquanto" {
    printf("ENQUANTO -> %s\n", yytext);
}

"||" {
    printf("OU -> %s\n", yytext);
}

"==" {
    printf("IGUAL -> %s\n", yytext);
}

"!=" {
    printf("DIFERENTE -> %s\n", yytext);
}

">=" {
    printf("MAIORIGUAL -> %s\n", yytext);
}

"<=" {
    printf("MENORIGUAL -> %s\n", yytext);
}

"&" {
    printf("E -> %s\n", yytext);
}

[+-]?[0-9]+ {
    printf("INTCONST -> %s\n", yytext);
}

\'([^\\\n]|\\.)\' {
    printf("CARCONST -> %s\n", yytext);
}

\"([^\\\n\"]|\\.)*\" {
    printf("CADEIACARACTERES -> %s\n", yytext);
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
    printf("ERRO: COMENTARIO NAO TERMINA linha %d\n", yylineno);
    exit(1);
}

[A-Za-z_][A-Za-z0-9_]* {
    printf("IDENTIFICADOR -> %s\n", yytext);
}

[ \t\r\n]+ {
}

"{" {
    printf("'{' -> %s\n", yytext);
}

"}" {
    printf("'}' -> %s\n", yytext);
}

"(" {
    printf("'(' -> %s\n", yytext);
}

")" {
    printf("')' -> %s\n", yytext);
}

"," {
    printf("',' -> %s\n", yytext);
}

":" {
    printf("':' -> %s\n", yytext);
}

";" {
    printf("';' -> %s\n", yytext);
}

"+" {
    printf("'+' -> %s\n", yytext);
}

"-" {
    printf("'-' -> %s\n", yytext);
}

"*" {
    printf("'*' -> %s\n", yytext);
}

"/" {
    printf("'/' -> %s\n", yytext);
}

"<" {
    printf("'<' -> %s\n", yytext);
}

">" {
    printf("'>' -> %s\n", yytext);
}

"=" {
    printf("'=' -> %s\n", yytext);
}

"!" {
    printf("'!' -> %s\n", yytext);
}

\"[^\"\n]* {
    printf("ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA linha %d\n",
           yylineno);
    exit(1);
}

. {
    printf("ERRO: CARACTERE INVALIDO %s linha %d\n",
           yytext, yylineno);
    exit(1);
}

%%

int main(void)
{
    yylex();
    return 0;
}