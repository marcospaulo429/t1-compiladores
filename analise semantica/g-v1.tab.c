/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



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




/* Copy the first part of user declarations.  */
#line 1 "g-v1.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 44 "g-v1.y"
{

    char *str;

    struct ASTNode *node;

    int type;

}
/* Line 193 of yacc.c.  */
#line 179 "g-v1.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 204 "g-v1.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   121

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNRULES -- Number of states.  */
#define YYNSTATES  103

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,     2,     2,     2,
      29,    30,    36,    34,    28,    35,     2,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,    27,
      32,    31,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    12,    17,    21,    28,    34,
      35,    39,    41,    43,    45,    48,    50,    53,    57,    61,
      65,    68,    76,    86,    92,    94,    96,   100,   104,   106,
     110,   112,   116,   120,   122,   126,   130,   134,   138,   140,
     144,   148,   150,   154,   158,   160,   163,   166,   168,   170,
     172,   174
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    41,    -1,     3,    42,    -1,    24,    47,
      25,    -1,    43,    24,    47,    25,    -1,    24,    44,    25,
      -1,    20,    45,    26,    46,    27,    44,    -1,    20,    45,
      26,    46,    27,    -1,    -1,    28,    20,    45,    -1,     4,
      -1,     5,    -1,    48,    -1,    48,    47,    -1,    27,    -1,
      49,    27,    -1,     6,    20,    27,    -1,     7,    49,    27,
      -1,     7,    21,    27,    -1,     8,    27,    -1,     9,    29,
      49,    30,    10,    48,    12,    -1,     9,    29,    49,    30,
      10,    48,    11,    48,    12,    -1,    13,    29,    49,    30,
      48,    -1,    42,    -1,    50,    -1,    20,    31,    49,    -1,
      50,    14,    51,    -1,    51,    -1,    51,    15,    52,    -1,
      52,    -1,    52,    16,    53,    -1,    52,    17,    53,    -1,
      53,    -1,    53,    32,    54,    -1,    53,    33,    54,    -1,
      53,    18,    54,    -1,    53,    19,    54,    -1,    54,    -1,
      54,    34,    55,    -1,    54,    35,    55,    -1,    55,    -1,
      55,    36,    56,    -1,    55,    37,    56,    -1,    56,    -1,
      35,    57,    -1,    38,    57,    -1,    57,    -1,    20,    -1,
      22,    -1,    23,    -1,    29,    49,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   144,   144,   171,   189,   220,   271,   290,   340,   382,
     388,   409,   416,   429,   436,   452,   463,   470,   495,   512,
     537,   548,   576,   611,   639,   652,   659,   681,   693,   702,
     714,   723,   735,   747,   756,   768,   780,   792,   804,   813,
     825,   837,   846,   858,   870,   879,   890,   901,   910,   921,
     932,   943
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PRINCIPAL", "INT", "CAR", "LEIA",
  "ESCREVA", "NOVALINHA", "SE", "ENTAO", "SENAO", "FIMSE", "ENQUANTO",
  "OU", "E", "IGUAL", "DIFERENTE", "MAIORIGUAL", "MENORIGUAL",
  "IDENTIFICADOR", "CADEIACARACTERES", "CARCONST", "INTCONST", "'{'",
  "'}'", "':'", "';'", "','", "'('", "')'", "'='", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'!'", "$accept", "Programa", "DeclPrograma",
  "Bloco", "VarSection", "ListaDeclVar", "DeclVar", "Tipo", "ListaComando",
  "Comando", "Expr", "OrExpr", "AndExpr", "EqExpr", "DesigExpr", "AddExpr",
  "MulExpr", "UnExpr", "PrimExpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   123,   125,    58,    59,    44,    40,
      41,    61,    60,    62,    43,    45,    42,    47,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    42,    42,    43,    44,    44,    45,
      45,    46,    46,    47,    47,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    52,    52,    53,    53,    53,    53,    53,    54,
      54,    54,    55,    55,    55,    56,    56,    56,    57,    57,
      57,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     4,     3,     6,     5,     0,
       3,     1,     1,     1,     2,     1,     2,     3,     3,     3,
       2,     7,     9,     5,     1,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     2,     2,     1,     1,     1,
       1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     0,     3,     0,     1,     0,     0,
       0,     0,     0,    48,    49,    50,    15,     0,     0,     0,
      24,     0,     0,    13,     0,    25,    28,    30,    33,    38,
      41,    44,    47,     0,     0,    48,     0,     0,    20,     0,
       0,     0,     0,     0,     0,    48,    45,    46,     6,     4,
      14,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,    19,    18,     0,     0,
       9,    26,     0,    51,    27,    29,    31,    32,    36,    37,
      34,    35,    39,    40,    42,    43,     5,     0,     0,    10,
      11,    12,     0,     0,    23,     8,     0,     9,     7,     0,
      21,     0,    22
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,    20,     6,    21,    43,    92,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -82
static const yytype_int8 yypact[] =
{
       6,   -13,    22,   -82,    -3,   -82,    17,   -82,    -6,    26,
       2,    15,    27,    14,   -82,   -82,   -82,    40,     5,     5,
     -82,    33,    46,    30,    47,    37,    69,    60,   -17,    32,
      53,   -82,   -82,    30,    70,    72,    73,    77,   -82,    40,
      40,    85,    40,    81,    76,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    84,   -82,   -82,   -82,    78,    86,
      87,   -82,    82,   -82,    69,    60,   -17,   -17,    32,    32,
      32,    32,    53,    53,   -82,   -82,   -82,   100,    30,   -82,
     -82,   -82,    90,    30,   -82,    91,    80,    87,   -82,    30,
     -82,   101,   -82
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -82,   -82,   -82,   111,   -82,    19,    48,   -82,   -10,   -81,
      -9,   -82,    67,    68,    39,    24,    35,    36,    83
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -10
static const yytype_int8 yytable[] =
{
      37,    56,    57,     8,     9,    10,    11,    94,    44,     1,
      12,     4,    96,    50,    34,    58,    59,    13,   101,    14,
      15,     4,     7,    64,    16,    45,    17,    14,    15,    38,
      68,    69,    18,    71,    17,    19,     8,     9,    10,    11,
      -9,    33,    41,    12,    39,    42,    35,    36,    14,    15,
      35,    52,    14,    15,     4,    17,    40,    16,    48,    17,
      35,    18,    14,    15,    19,    18,    60,    61,    19,    17,
      45,    49,    14,    15,    51,    18,    54,    55,    19,    17,
      78,    79,    80,    81,    53,    18,    90,    91,    19,    62,
      63,    99,   100,    76,    77,    82,    83,    65,    84,    85,
      66,    46,    47,    42,    67,    70,    73,    72,    87,    86,
      93,    97,     5,   102,    98,    41,    88,    95,    89,    74,
       0,    75
};

static const yytype_int8 yycheck[] =
{
       9,    18,    19,     6,     7,     8,     9,    88,    17,     3,
      13,    24,    93,    23,    20,    32,    33,    20,    99,    22,
      23,    24,     0,    33,    27,    20,    29,    22,    23,    27,
      39,    40,    35,    42,    29,    38,     6,     7,     8,     9,
      26,    24,    28,    13,    29,    31,    20,    21,    22,    23,
      20,    14,    22,    23,    24,    29,    29,    27,    25,    29,
      20,    35,    22,    23,    38,    35,    34,    35,    38,    29,
      20,    25,    22,    23,    27,    35,    16,    17,    38,    29,
      56,    57,    58,    59,    15,    35,     4,     5,    38,    36,
      37,    11,    12,    54,    55,    60,    61,    27,    62,    63,
      27,    18,    19,    31,    27,    20,    30,    26,    30,    25,
      10,    20,     1,    12,    95,    28,    30,    27,    70,    52,
      -1,    53
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    40,    41,    24,    42,    43,     0,     6,     7,
       8,     9,    13,    20,    22,    23,    27,    29,    35,    38,
      42,    44,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    24,    20,    20,    21,    49,    27,    29,
      29,    28,    31,    45,    49,    20,    57,    57,    25,    25,
      47,    27,    14,    15,    16,    17,    18,    19,    32,    33,
      34,    35,    36,    37,    47,    27,    27,    27,    49,    49,
      20,    49,    26,    30,    51,    52,    53,    53,    54,    54,
      54,    54,    55,    55,    56,    56,    25,    30,    30,    45,
       4,     5,    46,    10,    48,    27,    48,    20,    44,    11,
      12,    48,    12
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 145 "g-v1.y"
    {
          /*
           * Criamos o nó raiz.
           */

          (yyval.node) =
              ast_new(
                  AST_PROGRAM,
                  (yylsp[(1) - (1)]).first_line,
                  NULL
              );


          ast_add_child(
              (yyval.node),
              (yyvsp[(1) - (1)].node)
          );


          ast_root =
              (yyval.node);
      ;}
    break;

  case 3:
#line 172 "g-v1.y"
    {
          /*
           * O único elemento do programa
           * é o bloco principal.
           */

          (yyval.node) =
              (yyvsp[(2) - (2)].node);
      ;}
    break;

  case 4:
#line 190 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_BLOCK,
                  (yylsp[(1) - (3)]).first_line,
                  NULL
              );


          ASTNode *lista =
              ast_new(
                  AST_COMMAND_LIST,
                  (yylsp[(2) - (3)]).first_line,
                  NULL
              );


          ast_add_child_list(
              lista,
              (yyvsp[(2) - (3)].node)
          );


          ast_add_child(
              (yyval.node),
              lista
          );
      ;}
    break;

  case 5:
#line 221 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_BLOCK,
                  (yylsp[(1) - (4)]).first_line,
                  NULL
              );


          /*
           * Primeiro ficam as declarações.
           */

          ast_add_child(
              (yyval.node),
              (yyvsp[(1) - (4)].node)
          );


          /*
           * Depois fica a lista de comandos.
           */

          ASTNode *lista =
              ast_new(
                  AST_COMMAND_LIST,
                  (yylsp[(3) - (4)]).first_line,
                  NULL
              );


          ast_add_child_list(
              lista,
              (yyvsp[(3) - (4)].node)
          );


          ast_add_child(
              (yyval.node),
              lista
          );
      ;}
    break;

  case 6:
#line 272 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_VAR_SECTION,
                  (yylsp[(1) - (3)]).first_line,
                  NULL
              );


          ast_add_child_list(
              (yyval.node),
              (yyvsp[(2) - (3)].node)
          );
      ;}
    break;

  case 7:
#line 291 "g-v1.y"
    {
          ASTNode *decl =
              ast_new(
                  AST_VAR_DECL,
                  (yylsp[(1) - (6)]).first_line,
                  NULL
              );


          decl->value_type =
              (yyvsp[(4) - (6)].type);


          ASTNode *first_id =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(1) - (6)]).first_line,
                  (yyvsp[(1) - (6)].str)
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
              (yyvsp[(2) - (6)].node)
          );


          /*
           * Acrescenta a próxima declaração.
           */

          (yyval.node) =
              ast_append(
                  decl,
                  (yyvsp[(6) - (6)].node)
              );
      ;}
    break;

  case 8:
#line 341 "g-v1.y"
    {
          ASTNode *decl =
              ast_new(
                  AST_VAR_DECL,
                  (yylsp[(1) - (5)]).first_line,
                  NULL
              );


          decl->value_type =
              (yyvsp[(4) - (5)].type);


          ASTNode *first_id =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(1) - (5)]).first_line,
                  (yyvsp[(1) - (5)].str)
              );


          ast_add_child(
              decl,
              first_id
          );


          ast_add_child_list(
              decl,
              (yyvsp[(2) - (5)].node)
          );


          (yyval.node) =
              decl;
      ;}
    break;

  case 9:
#line 382 "g-v1.y"
    {
          (yyval.node) =
              NULL;
      ;}
    break;

  case 10:
#line 389 "g-v1.y"
    {
          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(2) - (3)].str)
              );


          id->next =
              (yyvsp[(3) - (3)].node);


          (yyval.node) =
              id;
      ;}
    break;

  case 11:
#line 410 "g-v1.y"
    {
          (yyval.type) =
              TYPE_INT;
      ;}
    break;

  case 12:
#line 417 "g-v1.y"
    {
          (yyval.type) =
              TYPE_CAR;
      ;}
    break;

  case 13:
#line 430 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 14:
#line 437 "g-v1.y"
    {
          (yyval.node) =
              ast_append(
                  (yyvsp[(1) - (2)].node),
                  (yyvsp[(2) - (2)].node)
              );
      ;}
    break;

  case 15:
#line 453 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_EMPTY,
                  (yylsp[(1) - (1)]).first_line,
                  NULL
              );
      ;}
    break;

  case 16:
#line 464 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (2)].node);
      ;}
    break;

  case 17:
#line 471 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_READ,
                  (yylsp[(1) - (3)]).first_line,
                  NULL
              );


          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(2) - (3)].str)
              );


          ast_add_child(
              (yyval.node),
              id
          );
      ;}
    break;

  case 18:
#line 496 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_WRITE,
                  (yylsp[(1) - (3)]).first_line,
                  NULL
              );


          ast_add_child(
              (yyval.node),
              (yyvsp[(2) - (3)].node)
          );
      ;}
    break;

  case 19:
#line 513 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_WRITE_STRING,
                  (yylsp[(1) - (3)]).first_line,
                  NULL
              );


          ASTNode *string =
              ast_new_take(
                  AST_STRING,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(2) - (3)].str)
              );


          ast_add_child(
              (yyval.node),
              string
          );
      ;}
    break;

  case 20:
#line 538 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_NEWLINE,
                  (yylsp[(1) - (2)]).first_line,
                  NULL
              );
      ;}
    break;

  case 21:
#line 549 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_IF,
                  (yylsp[(1) - (7)]).first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: comando
           */

          ast_add_child(
              (yyval.node),
              (yyvsp[(3) - (7)].node)
          );


          ast_add_child(
              (yyval.node),
              (yyvsp[(6) - (7)].node)
          );
      ;}
    break;

  case 22:
#line 577 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_IF_ELSE,
                  (yylsp[(1) - (9)]).first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: then
           * Filho 3: else
           */

          ast_add_child(
              (yyval.node),
              (yyvsp[(3) - (9)].node)
          );


          ast_add_child(
              (yyval.node),
              (yyvsp[(6) - (9)].node)
          );


          ast_add_child(
              (yyval.node),
              (yyvsp[(8) - (9)].node)
          );
      ;}
    break;

  case 23:
#line 612 "g-v1.y"
    {
          (yyval.node) =
              ast_new(
                  AST_WHILE,
                  (yylsp[(1) - (5)]).first_line,
                  NULL
              );


          /*
           * Filho 1: condição
           * Filho 2: comando
           */

          ast_add_child(
              (yyval.node),
              (yyvsp[(3) - (5)].node)
          );


          ast_add_child(
              (yyval.node),
              (yyvsp[(5) - (5)].node)
          );
      ;}
    break;

  case 24:
#line 640 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 25:
#line 653 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 26:
#line 660 "g-v1.y"
    {
          ASTNode *id =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(1) - (3)]).first_line,
                  (yyvsp[(1) - (3)].str)
              );


          (yyval.node) =
              ast_binary(
                  AST_ASSIGN,
                  (yylsp[(2) - (3)]).first_line,
                  id,
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 27:
#line 682 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_OR,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 28:
#line 694 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 29:
#line 703 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_AND,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 30:
#line 715 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 31:
#line 724 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_EQ,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 32:
#line 736 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_NEQ,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 33:
#line 748 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 34:
#line 757 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_LT,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 35:
#line 769 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_GT,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 36:
#line 781 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_GE,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 37:
#line 793 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_LE,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 38:
#line 805 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 39:
#line 814 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_ADD,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 40:
#line 826 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_SUB,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 41:
#line 838 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 42:
#line 847 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_MUL,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 43:
#line 859 "g-v1.y"
    {
          (yyval.node) =
              ast_binary(
                  AST_DIV,
                  (yylsp[(2) - (3)]).first_line,
                  (yyvsp[(1) - (3)].node),
                  (yyvsp[(3) - (3)].node)
              );
      ;}
    break;

  case 44:
#line 871 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 45:
#line 880 "g-v1.y"
    {
          (yyval.node) =
              ast_unary(
                  AST_NEG,
                  (yylsp[(1) - (2)]).first_line,
                  (yyvsp[(2) - (2)].node)
              );
      ;}
    break;

  case 46:
#line 891 "g-v1.y"
    {
          (yyval.node) =
              ast_unary(
                  AST_NOT,
                  (yylsp[(1) - (2)]).first_line,
                  (yyvsp[(2) - (2)].node)
              );
      ;}
    break;

  case 47:
#line 902 "g-v1.y"
    {
          (yyval.node) =
              (yyvsp[(1) - (1)].node);
      ;}
    break;

  case 48:
#line 911 "g-v1.y"
    {
          (yyval.node) =
              ast_new_take(
                  AST_IDENTIFIER,
                  (yylsp[(1) - (1)]).first_line,
                  (yyvsp[(1) - (1)].str)
              );
      ;}
    break;

  case 49:
#line 922 "g-v1.y"
    {
          (yyval.node) =
              ast_new_take(
                  AST_CARCONST,
                  (yylsp[(1) - (1)]).first_line,
                  (yyvsp[(1) - (1)].str)
              );
      ;}
    break;

  case 50:
#line 933 "g-v1.y"
    {
          (yyval.node) =
              ast_new_take(
                  AST_INTCONST,
                  (yylsp[(1) - (1)]).first_line,
                  (yyvsp[(1) - (1)].str)
              );
      ;}
    break;

  case 51:
#line 944 "g-v1.y"
    {
          /*
           * Não criamos um nó para parênteses.
           */

          (yyval.node) =
              (yyvsp[(2) - (3)].node);
      ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2318 "g-v1.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 955 "g-v1.y"



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
