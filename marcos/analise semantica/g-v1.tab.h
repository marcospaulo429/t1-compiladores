/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PRINCIPAL = 258,
     INT = 259,
     CAR = 260,
     LEIA = 261,
     ESCREVA = 262,
     NOVALINHA = 263,
     SE = 264,
     ENTAO = 265,
     SENAO = 266,
     FIMSE = 267,
     ENQUANTO = 268,
     OU = 269,
     E = 270,
     IGUAL = 271,
     DIFERENTE = 272,
     MAIORIGUAL = 273,
     MENORIGUAL = 274,
     IDENTIFICADOR = 275,
     CADEIACARACTERES = 276,
     CARCONST = 277,
     INTCONST = 278
   };
#endif
/* Tokens.  */
#define PRINCIPAL 258
#define INT 259
#define CAR 260
#define LEIA 261
#define ESCREVA 262
#define NOVALINHA 263
#define SE 264
#define ENTAO 265
#define SENAO 266
#define FIMSE 267
#define ENQUANTO 268
#define OU 269
#define E 270
#define IGUAL 271
#define DIFERENTE 272
#define MAIORIGUAL 273
#define MENORIGUAL 274
#define IDENTIFICADOR 275
#define CADEIACARACTERES 276
#define CARCONST 277
#define INTCONST 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 44 "g-v1.y"
{

    char *str;

    struct ASTNode *node;

    int type;

}
/* Line 1529 of yacc.c.  */
#line 105 "g-v1.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
