#ifndef _tipos_h
#define _tipos_h

typedef enum{
    /* Estrutura do Programa e Declarações */
    noProg,
    noBloco,
    noVarSec,
    noLstDecl,
    noDeclVar,
    noLstId,
    noTipoInt,
    noTipoCar,

    /* Comandos */
    noLstCmd,
    noCmdVazio,
    noAtrib,
    noLeia,
    noEscreva,
    noEscrevaCadeia,
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
    noIntConst,
    noCarConst,
    noCadConst
} TespecieOperador;

typedef struct operador {
    TespecieOperador tipoOperador;
    int linha;
    char * lexema;
    struct operador * filho1;
    struct operador * filho2;
    struct operador * filho3; /* Terceiro filho (necessário para if-then-else: cond, then, else) */
} Toperador;

#endif