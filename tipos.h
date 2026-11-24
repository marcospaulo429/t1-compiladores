#ifndef _tipos_h
#define _tipos_h

typedef enum{
    /* Estrutura do Programa e Declarações */
    noProg,
    noLstFunc,
    noFuncao,
    noLstParam,
    noParam,
    noBloco,
    noVarSec,
    noLstDecl,
    noDeclVar,
    noLstId,
    noIdVet,
    noTipoInt,
    noTipoCar,

    /* Comandos */
    noLstCmd,
    noCmdVazio,
    noAtrib,
    noLeia,
    noEscreva,
    noEscrevaCadeia,
    noRetorne,
    noNovalinha,
    noSe,
    noSeSenao,
    noEnquanto,

    /* Operadores Lógicos e Relacionais */
    noOU,
    noE,
    noIgual,
    noDiferente,
    noMenor,
    noMaior,
    noMenorIgual,
    noMaiorIgual,

    /* Operadores Aritméticos */
    noMais,
    noMenos,
    noMult,
    noDiv,

    /* Operadores Unários */
    noMenosUnario,
    noNao,

    /* Folhas / Terminais */
    noId,
    noIndice,
    noChamada,
    noLstExpr,
    noIntConst,
    noCarConst,
    noCadConst
} TespecieOperador;

typedef struct operador {
    TespecieOperador tipoOperador;
    int linha;
    char * lexema;
    int numero; /* noIdVet: tamanho do vetor; noParam: 1 se o parametro e vetor */
    struct operador * filho1;
    struct operador * filho2;
    struct operador * filho3; /* Terceiro filho (necessário para if-then-else: cond, then, else) */
} Toperador;

#endif