#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantico.h"

static PilhaTabelas * pilhaSemantica = NULL;

/* Funcao em analise (NULL dentro do bloco principal, que se comporta como int) */
static Simbolo * funcaoAtual = NULL;
/* Contador de posicoes: variaveis globais ou variaveis da funcao em analise */
static int posicaoAtual = 0;

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

/* ---------- Declaracoes de variaveis (globais e locais) ---------- */

static void declaraIdentificadores(Toperador * noLstId, TipoDado tipo, int linha) {
    if (!noLstId) return;

    Toperador * id = noLstId->filho1;
    if (id && id->lexema) {
        TipoDado tipoVar = tipo;
        int tamanho = 0;
        int slots = 1;

        if (id->tipoOperador == noIdVet) {
            if (id->numero <= 0) {
                erroSemantico("vetor deve ter tamanho maior que zero", linha);
            }
            tipoVar = tipoVetorDe(tipo);
            tamanho = id->numero;
            slots = tamanho;   /* vetor de n elementos ocupa n posicoes */
        }

        if (!inserirVariavel(pilhaSemantica, id->lexema, tipoVar, posicaoAtual + 1, tamanho, linha)) {
            char msg[200];
            snprintf(msg, sizeof(msg), "variável '%s' redeclarada", id->lexema);
            erroSemantico(msg, linha);
        }
        posicaoAtual += slots;
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

/* ---------- Expressoes ---------- */

static TipoDado checarExpressao(Toperador * expr);
static void checarComando(Toperador * cmd);

static Simbolo * buscaVariavel(Toperador * noId) {
    Simbolo * s = pesquisarNome(pilhaSemantica, noId->lexema);
    if (!s) {
        char msg[200];
        snprintf(msg, sizeof(msg), "variável '%s' não declarada", noId->lexema);
        erroSemantico(msg, noId->linha);
    }
    if (s->categoria == CAT_FUNC) {
        char msg[200];
        snprintf(msg, sizeof(msg), "'%s' é uma função e não pode ser usada como variável", noId->lexema);
        erroSemantico(msg, noId->linha);
    }
    return s;
}

/* Tipo de um LValueExpr (identificador ou elemento de vetor) */
static TipoDado checarLValue(Toperador * lv) {
    if (lv->tipoOperador == noIndice) {
        return checarExpressao(lv);
    }
    return buscaVariavel(lv)->tipo;
}

static TipoDado checarChamada(Toperador * chamada) {
    Simbolo * f = pesquisarNome(pilhaSemantica, chamada->lexema);
    char msg[256];

    if (!f) {
        snprintf(msg, sizeof(msg), "função '%s' não declarada", chamada->lexema);
        erroSemantico(msg, chamada->linha);
    }
    if (f->categoria != CAT_FUNC) {
        snprintf(msg, sizeof(msg), "'%s' não é uma função", chamada->lexema);
        erroSemantico(msg, chamada->linha);
    }

    Parametro * p = f->params;
    Toperador * arg = chamada->filho1;
    int n = 0;

    while (arg) {
        n++;
        TipoDado tArg = checarExpressao(arg->filho1);
        if (!p) {
            snprintf(msg, sizeof(msg), "número de argumentos incompatível na chamada de '%s'", chamada->lexema);
            erroSemantico(msg, chamada->linha);
        }
        if (tArg != p->tipo) {
            snprintf(msg, sizeof(msg), "tipo do argumento %d incompatível na chamada de '%s'", n, chamada->lexema);
            erroSemantico(msg, chamada->linha);
        }
        p = p->prox;
        arg = arg->filho2;
    }

    if (p) {
        snprintf(msg, sizeof(msg), "número de argumentos incompatível na chamada de '%s'", chamada->lexema);
        erroSemantico(msg, chamada->linha);
    }

    return f->tipo;
}

static TipoDado checarExpressao(Toperador * expr) {
    if (!expr) return TIPO_INDEFINIDO;

    switch (expr->tipoOperador) {
        case noIntConst:
            return TIPO_INT;

        case noCarConst:
            return TIPO_CAR;

        case noId:
            return buscaVariavel(expr)->tipo;

        case noIndice: {
            Simbolo * s = buscaVariavel(expr->filho1);
            if (!ehVetor(s->tipo)) {
                char msg[200];
                snprintf(msg, sizeof(msg), "'%s' não é um vetor e não pode ser indexado", s->nome);
                erroSemantico(msg, expr->linha);
            }
            if (checarExpressao(expr->filho2) != TIPO_INT) {
                erroSemantico("índice de vetor deve ser do tipo int", expr->linha);
            }
            return tipoBase(s->tipo);
        }

        case noChamada:
            return checarChamada(expr);

        case noAtrib: {
            Toperador * lv = expr->filho1;
            TipoDado tipoEsq = checarLValue(lv);
            TipoDado tipoDir = checarExpressao(expr->filho2);

            if (tipoEsq != tipoDir) {
                erroSemantico("tipos incompatíveis na atribuição", expr->linha);
            }
            if (ehVetor(tipoEsq)) {
                /* atribuicao de vetor copia o endereco: so e possivel em
                   variaveis que sao ponteiros, isto e, parametros vetor */
                Simbolo * s = buscaVariavel(lv);
                if (s->categoria != CAT_PARAM) {
                    erroSemantico("vetor declarado não pode receber o endereço de outro vetor", expr->linha);
                }
            }
            return tipoEsq;
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
            if (t1 != t2 || ehVetor(t1)) {
                erroSemantico("operador relacional exige operandos de mesmo tipo básico", expr->linha);
            }
            return TIPO_INT;
        }

        default:
            return TIPO_INDEFINIDO;
    }
}

/* ---------- Comandos e blocos ---------- */

/* O bloco mais externo de uma funcao reutiliza a tabela que ja contem os
   parametros (variavel local nao pode repetir o nome de um parametro).
   Os demais blocos so criam tabela se tiverem declaracao de variaveis. */
static void checarBloco(Toperador * bloco, int reutilizaTabela) {
    if (!bloco) return;

    int empilhou = 0;
    int posicaoAnterior = posicaoAtual;

    if (bloco->filho1 != NULL) {
        if (!reutilizaTabela) {
            empilharTabela(pilhaSemantica);
            empilhou = 1;
        }
        processaDeclaracoes(bloco->filho1->filho1);
    }

    checarComando(bloco->filho2);

    if (empilhou) {
        desempilharTabela(pilhaSemantica);
        posicaoAtual = posicaoAnterior;
    }
}

static void exigeInt(Toperador * cond, const char * mensagem, int linha) {
    if (checarExpressao(cond) != TIPO_INT) {
        erroSemantico(mensagem, linha);
    }
}

static void checarComando(Toperador * cmd) {
    if (!cmd) return;

    switch (cmd->tipoOperador) {
        case noLstCmd:
            checarComando(cmd->filho1);
            checarComando(cmd->filho2);
            break;

        case noCmdVazio:
        case noNovalinha:
        case noEscrevaCadeia:
            break;

        case noLeia: {
            TipoDado t = checarLValue(cmd->filho1);
            if (ehVetor(t)) {
                erroSemantico("leia não pode ler um vetor inteiro", cmd->linha);
            }
            break;
        }

        case noEscreva: {
            TipoDado t = checarExpressao(cmd->filho1);
            if (ehVetor(t)) {
                erroSemantico("escreva não pode imprimir um vetor inteiro", cmd->linha);
            }
            break;
        }

        case noRetorne: {
            TipoDado tEsperado = funcaoAtual ? funcaoAtual->tipo : TIPO_INT;
            TipoDado t = checarExpressao(cmd->filho1);
            if (t != tEsperado) {
                erroSemantico("tipo da expressão retornada difere do tipo da função", cmd->linha);
            }
            break;
        }

        case noSe:
            exigeInt(cmd->filho1, "condicao do 'se' deve ser do tipo int", cmd->linha);
            checarComando(cmd->filho2);
            break;

        case noSeSenao:
            exigeInt(cmd->filho1, "condicao do 'se' deve ser do tipo int", cmd->linha);
            checarComando(cmd->filho2);
            checarComando(cmd->filho3);
            break;

        case noEnquanto:
            exigeInt(cmd->filho1, "condicao do 'enquanto' deve ser do tipo int", cmd->linha);
            checarComando(cmd->filho2);
            break;

        case noBloco:
            checarBloco(cmd, 0);
            break;

        default:
            checarExpressao(cmd);
            break;
    }
}

/* ---------- Funcoes ---------- */

static TipoDado tipoParametro(Toperador * param) {
    TipoDado base = extraiTipoAST(param->filho1);
    return param->numero ? tipoVetorDe(base) : base;
}

static void checarFuncao(Toperador * no) {
    int nParams = 0;
    for (Toperador * l = no->filho1; l; l = l->filho2) nParams++;

    Simbolo * f = inserirFuncao(pilhaSemantica, no->lexema, nParams, extraiTipoAST(no->filho2), no->linha);
    if (!f) {
        char msg[200];
        snprintf(msg, sizeof(msg), "'%s' redeclarado (nome de função repetido)", no->lexema);
        erroSemantico(msg, no->linha);
    }

    /* a tabela da funcao guarda parametros e variaveis do bloco mais externo */
    empilharTabela(pilhaSemantica);
    int pos = 0;
    for (Toperador * l = no->filho1; l; l = l->filho2) {
        Toperador * p = l->filho1;
        TipoDado t = tipoParametro(p);
        pos++;
        adicionarParametroFuncao(f, p->lexema, t, pos);
        if (!inserirParametro(pilhaSemantica, p->lexema, t, pos, p->linha)) {
            char msg[200];
            snprintf(msg, sizeof(msg), "parâmetro '%s' redeclarado", p->lexema);
            erroSemantico(msg, p->linha);
        }
    }

    funcaoAtual = f;
    posicaoAtual = 0;
    checarBloco(no->filho3, 1);
    funcaoAtual = NULL;

    desempilharTabela(pilhaSemantica);
}

void analiseSemantica(Toperador * raiz) {
    if (!raiz || raiz->tipoOperador != noProg) return;

    pilhaSemantica = iniciarPilha();
    empilharTabela(pilhaSemantica);     /* escopo 0: globais e nomes de funcoes */

    posicaoAtual = 0;
    if (raiz->filho1) {
        processaDeclaracoes(raiz->filho1->filho1);
    }

    for (Toperador * l = raiz->filho2; l; l = l->filho2) {
        checarFuncao(l->filho1);
    }

    funcaoAtual = NULL;
    posicaoAtual = 0;
    /* bloco principal: sempre em escopo proprio, acima do global */
    empilharTabela(pilhaSemantica);
    checarBloco(raiz->filho3, 1);
    desempilharTabela(pilhaSemantica);

    destruirPilha(pilhaSemantica);
}
