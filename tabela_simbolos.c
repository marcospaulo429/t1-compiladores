#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabela_simbolos.h"

static unsigned int funcaoHash(const char * str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TAM_HASH;
}

PilhaTabelas * iniciarPilha(void) {
    PilhaTabelas * pilha = (PilhaTabelas *) malloc(sizeof(PilhaTabelas));
    if (!pilha) {
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }
    pilha->topo = NULL;
    return pilha;
}

void empilharTabela(PilhaTabelas * pilha) {
    if (!pilha) return;

    TabelaSimbolos * tabela = (TabelaSimbolos *) malloc(sizeof(TabelaSimbolos));
    if (!tabela) {
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }

    for (int i = 0; i < TAM_HASH; i++) {
        tabela->baldes[i] = NULL;
    }

    tabela->escopo = pilha->topo ? pilha->topo->escopo + 1 : 0;
    tabela->anterior = pilha->topo;
    pilha->topo = tabela;
}

void desempilharTabela(PilhaTabelas * pilha) {
    if (!pilha || !pilha->topo) return;

    TabelaSimbolos * tabela = pilha->topo;
    pilha->topo = tabela->anterior;

    for (int i = 0; i < TAM_HASH; i++) {
        Simbolo * s = tabela->baldes[i];
        while (s != NULL) {
            Simbolo * prox = s->prox;
            Parametro * p = s->params;
            while (p != NULL) {
                Parametro * proxP = p->prox;
                free(p->nome);
                free(p);
                p = proxP;
            }
            free(s->nome);
            free(s);
            s = prox;
        }
    }
    free(tabela);
}

Simbolo * pesquisarNoEscopoAtual(PilhaTabelas * pilha, const char * nome) {
    if (!pilha || !pilha->topo || !nome) return NULL;

    unsigned int h = funcaoHash(nome);
    Simbolo * s = pilha->topo->baldes[h];

    while (s != NULL) {
        if (strcmp(s->nome, nome) == 0) {
            return s;
        }
        s = s->prox;
    }
    return NULL;
}

Simbolo * pesquisarNome(PilhaTabelas * pilha, const char * nome) {
    if (!pilha || !nome) return NULL;

    unsigned int h = funcaoHash(nome);
    TabelaSimbolos * tabela = pilha->topo;

    while (tabela != NULL) {
        Simbolo * s = tabela->baldes[h];
        while (s != NULL) {
            if (strcmp(s->nome, nome) == 0) {
                return s;
            }
            s = s->prox;
        }
        tabela = tabela->anterior;
    }
    return NULL;
}

static Simbolo * novoSimbolo(PilhaTabelas * pilha, const char * nome, Categoria cat, TipoDado tipo, int linha) {
    if (!pilha || !pilha->topo || !nome) return NULL;

    if (pesquisarNoEscopoAtual(pilha, nome) != NULL) return NULL;

    unsigned int h = funcaoHash(nome);
    Simbolo * novo = (Simbolo *) calloc(1, sizeof(Simbolo));
    if (!novo) {
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }

    novo->nome = strdup(nome);
    if (!novo->nome) {
        free(novo);
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }

    novo->categoria = cat;
    novo->tipo = tipo;
    novo->linha = linha;
    novo->escopo = pilha->topo->escopo;
    novo->prox = pilha->topo->baldes[h];
    pilha->topo->baldes[h] = novo;
    return novo;
}

Simbolo * inserirFuncao(PilhaTabelas * pilha, const char * nome, int nArgs, TipoDado tipoRetorno, int linha) {
    Simbolo * f = novoSimbolo(pilha, nome, CAT_FUNC, tipoRetorno, linha);
    if (f) f->nParams = nArgs;
    return f;
}

void adicionarParametroFuncao(Simbolo * funcao, const char * nome, TipoDado tipo, int posicao) {
    if (!funcao) return;

    Parametro * p = (Parametro *) calloc(1, sizeof(Parametro));
    if (!p) {
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }
    p->nome = strdup(nome);
    p->tipo = tipo;
    p->posicao = posicao;

    /* anexa no fim para manter a ordem de declaracao */
    if (!funcao->params) {
        funcao->params = p;
    } else {
        Parametro * ult = funcao->params;
        while (ult->prox) ult = ult->prox;
        ult->prox = p;
    }
}

int inserirVariavel(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int posicao, int tamanho, int linha) {
    Simbolo * v = novoSimbolo(pilha, nome, CAT_VAR, tipo, linha);
    if (!v) return 0;
    v->posicao = posicao;
    v->tamanho = tamanho;
    return 1;
}

int inserirParametro(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int posicao, int linha) {
    Simbolo * p = novoSimbolo(pilha, nome, CAT_PARAM, tipo, linha);
    if (!p) return 0;
    p->posicao = posicao;
    return 1;
}

int ehVetor(TipoDado tipo) {
    return tipo == TIPO_VET_INT || tipo == TIPO_VET_CAR;
}

TipoDado tipoBase(TipoDado tipo) {
    if (tipo == TIPO_VET_INT) return TIPO_INT;
    if (tipo == TIPO_VET_CAR) return TIPO_CAR;
    return tipo;
}

TipoDado tipoVetorDe(TipoDado base) {
    if (base == TIPO_INT) return TIPO_VET_INT;
    if (base == TIPO_CAR) return TIPO_VET_CAR;
    return TIPO_INDEFINIDO;
}

void destruirPilha(PilhaTabelas * pilha) {
    if (!pilha) return;
    while (pilha->topo != NULL) {
        desempilharTabela(pilha);
    }
    free(pilha);
}