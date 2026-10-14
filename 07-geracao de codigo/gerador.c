#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerador.h"
#include "tabela_simbolos.h"

static FILE *arqSaida = NULL;
static PilhaTabelas *pilhaCodegen = NULL;

static int offsetAtual = 0;

static int contadorRotulos = 0;
static int novoRotulo(void) {
    return ++contadorRotulos;
}

/* Tabela estática de strings literais para a seção .data */
typedef struct NodoString {
    int id;
    char *texto;
    struct NodoString *prox;
} NodoString;

static NodoString *listaStrings = NULL;
static int contadorStrings = 0;

static int registraString(const char *texto) {
    NodoString *nova = (NodoString*) malloc(sizeof(NodoString));
    nova->id = ++contadorStrings;
    nova->texto = strdup(texto);
    nova->prox = listaStrings;
    listaStrings = nova;
    return nova->id;
}

static TipoDado extraiTipoAST(Toperador *noTipo) {
    if (!noTipo) return TIPO_INDEFINIDO;
    if (noTipo->tipoOperador == noTipoInt) return TIPO_INT;
    if (noTipo->tipoOperador == noTipoCar) return TIPO_CAR;
    return TIPO_INDEFINIDO;
}

static int contaVariaveisLista(Toperador *noLstId) {
    if (!noLstId) return 0;
    int qtd = 0;
    if (noLstId->filho1) qtd = 1;
    return qtd + contaVariaveisLista(noLstId->filho2);
}

static void alocaVariaveis(Toperador *noLstId, TipoDado tipo, int linha) {
    if (!noLstId) return;

    if (noLstId->filho1 && noLstId->filho1->lexema) {
        offsetAtual += 4; /* Cada variável inteira ou caractere ocupa 4 bytes (1 palavra) */
        inserirSimbolo(pilhaCodegen, noLstId->filho1->lexema, tipo, linha, offsetAtual);
    }
    alocaVariaveis(noLstId->filho2, tipo, linha);
}

static void processaDeclaracoesCodegen(Toperador *noLstDecl) {
    if (!noLstDecl) return;

    Toperador *decl = noLstDecl->filho1;
    if (decl && decl->tipoOperador == noDeclVar) {
        TipoDado tipo = extraiTipoAST(decl->filho2);
        alocaVariaveis(decl->filho1, tipo, decl->linha);
    }
    processaDeclaracoesCodegen(noLstDecl->filho2);
}

/* Coleta antecipadamente todas as cadeias de caracteres do programa para a seção .data */
static void coletaStrings(Toperador *no) {
    if (!no) return;
    if (no->tipoOperador == noCadConst && no->lexema) {
        registraString(no->lexema);
    }
    coletaStrings(no->filho1);
    coletaStrings(no->filho2);
    coletaStrings(no->filho3);
}

static void cgenExpr(Toperador *expr);
static void cgenCmd(Toperador *cmd);
static void cgenBloco(Toperador *bloco);

/* Geração de código para expressões (resultado sempre em $s0) */
static void cgenExpr(Toperador *expr) {
    if (!expr) return;

    switch (expr->tipoOperador) {
        case noIntConst:
            fprintf(arqSaida, "    li $s0, %s\n", expr->lexema);
            break;

        case noCarConst:
            if (strcmp(expr->lexema, "'\\n'") == 0) {
                fprintf(arqSaida, "    li $s0, 10\n");
            } else if (strcmp(expr->lexema, "'\\t'") == 0) {
                fprintf(arqSaida, "    li $s0, 9\n");
            } else {
                fprintf(arqSaida, "    li $s0, %d\n", (int)expr->lexema[1]);
            }
            break;

        case noId: {
            Simbolo *s = pesquisarNome(pilhaCodegen, expr->lexema);
            fprintf(arqSaida, "    lw $s0, -%d($fp)    # carrega %s\n", s->offset, s->nome);
            break;
        }

        case noAtrib: {
            cgenExpr(expr->filho2);
            Simbolo *s = pesquisarNome(pilhaCodegen, expr->filho1->lexema);
            fprintf(arqSaida, "    sw $s0, -%d($fp)    # %s = $s0\n", s->offset, s->nome);
            break;
        }

        /* Operadores Binários Aritméticos, Lógicos e Relacionais */
        case noMais:
        case noMenos:
        case noMult:
        case noDiv:
        case noOU:
        case noE:
        case noIgual:
        case noDiferente:
        case noMenor:
        case noMaior:
        case noMenorIgual:
        case noMaiorIgual: {
            /* 1. Computa filho1 (e1) */
            cgenExpr(expr->filho1);
            /* 2. Empilha $s0 */
            fprintf(arqSaida, "    sw $s0, 0($sp)\n");
            fprintf(arqSaida, "    addiu $sp, $sp, -4\n");
            /* 3. Computa filho2 (e2) */
            cgenExpr(expr->filho2);
            /* 4. Desempilha e1 em $t1 */
            fprintf(arqSaida, "    lw $t1, 4($sp)\n");
            fprintf(arqSaida, "    addiu $sp, $sp, 4\n");

            /* 5. Opera $t1 (esq) com $s0 (dir), guardando o resultado em $s0 */
            switch (expr->tipoOperador) {
                case noMais:       fprintf(arqSaida, "    add $s0, $t1, $s0\n"); break;
                case noMenos:      fprintf(arqSaida, "    sub $s0, $t1, $s0\n"); break;
                case noMult:       fprintf(arqSaida, "    mul $s0, $t1, $s0\n"); break;
                case noDiv:        fprintf(arqSaida, "    div $s0, $t1, $s0\n"); break;
                case noOU:         fprintf(arqSaida, "    or $s0, $t1, $s0\n"); break;
                case noE:          fprintf(arqSaida, "    and $s0, $t1, $s0\n"); break;
                case noIgual:      fprintf(arqSaida, "    seq $s0, $t1, $s0\n"); break;
                case noDiferente:  fprintf(arqSaida, "    sne $s0, $t1, $s0\n"); break;
                case noMenor:      fprintf(arqSaida, "    slt $s0, $t1, $s0\n"); break;
                case noMaior:      fprintf(arqSaida, "    sgt $s0, $t1, $s0\n"); break;
                case noMenorIgual: fprintf(arqSaida, "    sle $s0, $t1, $s0\n"); break;
                case noMaiorIgual: fprintf(arqSaida, "    sge $s0, $t1, $s0\n"); break;
                default: break;
            }
            break;
        }

        /* Operadores Unários */
        case noMenosUnario:
            cgenExpr(expr->filho1);
            fprintf(arqSaida, "    neg $s0, $s0\n");
            break;

        case noNao:
            cgenExpr(expr->filho1);
            fprintf(arqSaida, "    seq $s0, $s0, $zero\n");
            break;

        default:
            break;
    }
}

static int buscaIdString(const char *texto) {
    NodoString *atual = listaStrings;
    while (atual) {
        if (strcmp(atual->texto, texto) == 0) return atual->id;
        atual = atual->prox;
    }
    return 1;
}

static void cgenCmd(Toperador *cmd) {
    if (!cmd) return;

    switch (cmd->tipoOperador) {
        case noLstCmd:
            cgenCmd(cmd->filho1);
            cgenCmd(cmd->filho2);
            break;

        case noCmdVazio:
            break;

        case noNovalinha:
            fprintf(arqSaida, "    li $a0, 10\n");
            fprintf(arqSaida, "    li $v0, 11\n");
            fprintf(arqSaida, "    syscall\n");
            break;

        case noEscrevaCadeia: {
            int strId = buscaIdString(cmd->filho1->lexema);
            fprintf(arqSaida, "    la $a0, str_%d\n", strId);
            fprintf(arqSaida, "    li $v0, 4\n");
            fprintf(arqSaida, "    syscall\n");
            break;
        }

        case noEscreva: {
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    move $a0, $s0\n");
            if (cmd->filho1->tipoOperador == noCarConst) {
                fprintf(arqSaida, "    li $v0, 11\n");
            } else if (cmd->filho1->tipoOperador == noId) {
                Simbolo *s = pesquisarNome(pilhaCodegen, cmd->filho1->lexema);
                if (s && s->tipo == TIPO_CAR) {
                    fprintf(arqSaida, "    li $v0, 11\n");
                } else {
                    fprintf(arqSaida, "    li $v0, 1\n");
                }
            } else {
                fprintf(arqSaida, "    li $v0, 1\n");
            }
            fprintf(arqSaida, "    syscall\n");
            break;
        }

        case noLeia: {
            Simbolo *s = pesquisarNome(pilhaCodegen, cmd->filho1->lexema);
            if (s->tipo == TIPO_CAR) {
                fprintf(arqSaida, "    li $v0, 12\n");
                fprintf(arqSaida, "    syscall\n");
                fprintf(arqSaida, "    sw $v0, -%d($fp)\n", s->offset);
            } else {
                fprintf(arqSaida, "    li $v0, 5\n");
                fprintf(arqSaida, "    syscall\n");
                fprintf(arqSaida, "    sw $v0, -%d($fp)\n", s->offset);
            }
            break;
        }

        case noSe: {
            int lFim = novoRotulo();
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_fim_%d\n", lFim);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "L_fim_%d:\n", lFim);
            break;
        }

        case noSeSenao: {
            int lElse = novoRotulo();
            int lFim = novoRotulo();
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_else_%d\n", lElse);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "    j L_fim_%d\n", lFim);
            fprintf(arqSaida, "L_else_%d:\n", lElse);
            cgenCmd(cmd->filho3);
            fprintf(arqSaida, "L_fim_%d:\n", lFim);
            break;
        }

        case noEnquanto: {
            int lInicio = novoRotulo();
            int lFim = novoRotulo();
            fprintf(arqSaida, "L_while_%d:\n", lInicio);
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_fim_while_%d\n", lFim);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "    j L_while_%d\n", lInicio);
            fprintf(arqSaida, "L_fim_while_%d:\n", lFim);
            break;
        }

        case noBloco:
            cgenBloco(cmd);
            break;

        default:
            cgenExpr(cmd);
            break;
    }
}

static void cgenBloco(Toperador *bloco) {
    if (!bloco) return;

    int empilhou = 0;
    int bytesAlocados = 0;
    int offsetAnterior = offsetAtual;

    if (bloco->filho1 != NULL) {
        empilharTabela(pilhaCodegen);
        empilhou = 1;
        if (bloco->filho1->filho1) {
            processaDeclaracoesCodegen(bloco->filho1->filho1);
            bytesAlocados = offsetAtual - offsetAnterior;
            fprintf(arqSaida, "    addiu $sp, $sp, -%d   # aloca variaveis do bloco\n", bytesAlocados);
        }
    }

    cgenCmd(bloco->filho2);

    if (empilhou) {
        if (bytesAlocados > 0) {
            fprintf(arqSaida, "    addiu $sp, $sp, %d    # desaloca variaveis do bloco\n", bytesAlocados);
            offsetAtual = offsetAnterior;
        }
        desempilharTabela(pilhaCodegen);
    }
}

void geraCodigoMIPS(Toperador *raiz, FILE *saida) {
    if (!raiz || !saida) return;

    arqSaida = saida;
    pilhaCodegen = iniciarPilha();
    offsetAtual = 0;
    contadorRotulos = 0;
    listaStrings = NULL;
    contadorStrings = 0;

    /* Coleta e declara todas as strings no segmento .data */
    coletaStrings(raiz);

    fprintf(arqSaida, "    .data\n");
    NodoString *s = listaStrings;
    while (s) {
        fprintf(arqSaida, "str_%d: .asciiz %s\n", s->id, s->texto);
        s = s->prox;
    }

    /* Inicia o segmento .text */
    fprintf(arqSaida, "\n    .text\n");
    fprintf(arqSaida, "    .globl main\n");
    fprintf(arqSaida, "main:\n");
    fprintf(arqSaida, "    move $fp, $sp         # Frame Pointer inicializado\n");

    /* Gera código do bloco principal */
    if (raiz->tipoOperador == noProg) {
        cgenBloco(raiz->filho1);
    } else if (raiz->tipoOperador == noBloco) {
        cgenBloco(raiz);
    }

    /* Encerra o programa com syscall 10 (exit) */
    fprintf(arqSaida, "\n    # Encerra execucao\n");
    fprintf(arqSaida, "    li $v0, 10\n");
    fprintf(arqSaida, "    syscall\n");

    destruirPilha(pilhaCodegen);
}