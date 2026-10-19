#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantico.h"

static PilhaTabelas * pilhaSemantica = NULL;

static void erroSemantico(const char *mensagem, int linha) {
    fprintf(stderr, "ERRO: %s linha %d\n", mensagem, linha);
    destruirPilha(pilhaSemantica);
    exit(1);
}

static TipoDado extraiTipoAST(Toperador * noTipo) {
    if (!noTipo) return TIPO_INDEFINIDO;
    if (noTipo->tipoOperador == noTipoInt) return TIPO_INT;
    if (noTipo->tipoOperador == noTipoCar) return TIPO_CAR;
    return TIPO_INDEFINIDO;
}

static void declaraIdentificadores(Toperador * noLstId, TipoDado tipo, int linha) {
    if (!noLstId) return;

    if (noLstId->filho1 && noLstId->filho1->lexema) {
        char * nome = noLstId->filho1->lexema;
        if (!inserirSimbolo(pilhaSemantica, nome, tipo, linha, 0)) {
            char msg[200];
            snprintf(msg, sizeof(msg), "variável '%s' redeclarada", nome);
            erroSemantico(msg, linha);
        }
    }

    declaraIdentificadores(noLstId->filho2, tipo, linha);
}

static void processaDeclaracoes(Toperador * noLstDecl) {
    if (!noLstDecl) return;

    Toperador * decl = noLstDecl->filho1;
    if (decl && decl->tipoOperador == noDeclVar) {
        TipoDado tipo = extraiTipoAST(decl->filho2);
        declaraIdentificadores(decl->filho1, tipo, decl->linha);
    }

    processaDeclaracoes(noLstDecl->filho2);
}

static TipoDado checarExpressao(Toperador * expr);
static void checarComando(Toperador * cmd);

static TipoDado checarExpressao(Toperador * expr) {
    if (!expr) return TIPO_INDEFINIDO;

    switch (expr->tipoOperador) {
        case noIntConst:
            return TIPO_INT;

        case noCarConst:
            return TIPO_CAR;

        case noId: {
            Simbolo * s = pesquisarNome(pilhaSemantica, expr->lexema);
            if (!s) {
                char msg[200];
                snprintf(msg, sizeof(msg), "variável '%s' não declarada", expr->lexema);
                erroSemantico(msg, expr->linha);
            }
            return s->tipo;
        }

        case noAtrib: {
            Toperador * noIdEsq = expr->filho1;
            Toperador * noExprDir = expr->filho2;

            Simbolo * s = pesquisarNome(pilhaSemantica, noIdEsq->lexema);
            if (!s) {
                char msg[200];
                snprintf(msg, sizeof(msg), "variável '%s' nao declarada", noIdEsq->lexema);
                erroSemantico(msg, noIdEsq->linha);
                exit(1);
            }

            TipoDado tipoDir = checarExpressao(noExprDir);
            if (s->tipo != tipoDir) {
                erroSemantico("tipos incompatíveis na atribuição", expr->linha);
            }
            return s->tipo;
        }

        case noMais:
        case noMenos:
        case noMult:
        case noDiv: {
            TipoDado t1 = checarExpressao(expr->filho1);
            TipoDado t2 = checarExpressao(expr->filho2);
            if (t1 != TIPO_INT || t2 != TIPO_INT) {
                erroSemantico("operador aritmético exige operandos do tipo int", expr->linha);
            }

            return TIPO_INT;
        }

        case noMenosUnario: {
            TipoDado t = checarExpressao(expr->filho1);
            if (t != TIPO_INT) {
                erroSemantico("operador menos unario exige operando do tipo int", expr->linha);
            }
            return TIPO_INT;
        }

        case noNao: {
            TipoDado t = checarExpressao(expr->filho1);
            if (t != TIPO_INT) {
                erroSemantico("operador negação exige operando do tipo int", expr->linha);
            }
            return TIPO_INT;
        }

        case noOU:
        case noE: {
            TipoDado t1 = checarExpressao(expr->filho1);
            TipoDado t2 = checarExpressao(expr->filho2);
            if (t1 != TIPO_INT || t2 != TIPO_INT) {
                erroSemantico("operador logico exige operandos do tipo int", expr->linha);
            }
            return TIPO_INT;
        }

        case noIgual:
        case noDiferente:
        case noMaior:
        case noMenor:
        case noMaiorIgual:
        case noMenorIgual: {
            TipoDado t1 = checarExpressao(expr->filho1);
            TipoDado t2 = checarExpressao(expr->filho2);
            if (t1 != t2) {
                erroSemantico("operador relacional exige operandos de mesmo tipo", expr->linha);
            }
            return TIPO_INT;
        }

        default:
            return TIPO_INDEFINIDO;
    }
}

static void checarBloco(Toperador * bloco) {
    if (!bloco) return;

    int empilhou = 0;
    if (bloco->filho1 != NULL) {
        empilharTabela(pilhaSemantica);
        empilhou = 1;

        if (bloco->filho1->filho1) {
            processaDeclaracoes(bloco->filho1->filho1);
        }
    }

    checarComando(bloco->filho2);

    if (empilhou) {
        desempilharTabela(pilhaSemantica);
    }
}

static void checarComando(Toperador * cmd) {
    if (!cmd) return;

    switch (cmd->tipoOperador) {
        case noLstCmd: {
            checarComando(cmd->filho1);
            checarComando(cmd->filho2);
            break;
        }

        case noCmdVazio:
        case noNovalinha:
        case noEscrevaCadeia:
            break;

        case noLeia: {
            Simbolo * s = pesquisarNome(pilhaSemantica, cmd->filho1->lexema);
            if (!s) {
                char msg[200];
                snprintf(msg, sizeof(msg), "variavel '%s' nao declarada", cmd->filho1->lexema);
                erroSemantico(msg, cmd->linha);
            }
            break;
        }

        case noEscreva:
            checarExpressao(cmd->filho1);
            break;

        case noSe: {
            TipoDado tCond = checarExpressao(cmd->filho1);
            if (tCond != TIPO_INT) {
                erroSemantico("condicao do 'se' deve ser do tipo int", cmd->linha);
            }
            checarComando(cmd->filho2);
            break;
        }

        case noSeSenao: {
            TipoDado tCond = checarExpressao(cmd->filho1);
            if (tCond != TIPO_INT) {
                erroSemantico("condicao do 'se' deve ser do tipo int", cmd->linha);
            }
            checarComando(cmd->filho2);
            checarComando(cmd->filho3);
            break;
        }

        case noEnquanto: {
            TipoDado tCond = checarExpressao(cmd->filho1);
            if (tCond != TIPO_INT) {
                erroSemantico("condicao do 'enquanto' deve ser do tipo int", cmd->linha);
            }
            checarComando(cmd->filho2);
            break;
        }

        case noBloco:
            checarBloco(cmd);
            break;
        
        default:
            checarExpressao(cmd);
            break;
    }
}

void analiseSemantica(Toperador * raiz) {
    if (!raiz) return;

    pilhaSemantica = iniciarPilha();

    if (raiz->tipoOperador == noProg) {
        checarBloco(raiz->filho1);
    } else if (raiz->tipoOperador == noBloco) {
        checarBloco(raiz);
    }

    destruirPilha(pilhaSemantica);
}