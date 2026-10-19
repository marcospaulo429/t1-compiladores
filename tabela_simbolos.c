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

int inserirSimbolo(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int linha, int offset) {
    if (!pilha || !pilha->topo || !nome) return 0;

    if (pesquisarNoEscopoAtual(pilha, nome) != NULL) return 0;

    unsigned int h = funcaoHash(nome);
    Simbolo * novo = (Simbolo *) malloc(sizeof(Simbolo));
    if (!novo) {
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }

    novo->nome = strdup(nome);
    if(!novo->nome) {
        free(novo);
        fprintf(stderr, "ERRO: memoria insuficiente\n");
        exit(1);
    }

    novo->tipo = tipo;
    novo->linha = linha;
    novo->offset = offset;
    novo->prox = pilha->topo->baldes[h];
    pilha->topo->baldes[h] = novo;

    return 1;
}

void destruirPilha(PilhaTabelas * pilha) {
    if (!pilha) return;
    while (pilha->topo != NULL) {
        desempilharTabela(pilha);
    }
    free(pilha);
}