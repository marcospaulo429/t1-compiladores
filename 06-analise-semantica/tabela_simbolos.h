#ifndef _tabela_simbolos_h
#define _tabela_simbolos_h

#define TAM_HASH 211

typedef enum {
    TIPO_INT,
    TIPO_CAR,
    TIPO_INDEFINIDO
} TipoDado;

typedef struct Simbolo {
    char * nome;
    TipoDado tipo;
    int linha;
    struct Simbolo * prox;
} Simbolo;

/* Tabela Hash com encadeamento (buckets) */
typedef struct TabelaSimbolos {
    Simbolo * baldes[TAM_HASH];
    struct TabelaSimbolos * anterior;
} TabelaSimbolos;

typedef struct PilhaTabelas {
    TabelaSimbolos * topo;
} PilhaTabelas;


PilhaTabelas * iniciarPilha(void);
void empilharTabela(PilhaTabelas * pilha);
Simbolo * pesquisarNome(PilhaTabelas * pilha, const char * nome);
void desempilharTabela(PilhaTabelas * pilha);

Simbolo * pesquisarNoEscopoAtual(PilhaTabelas * pilha, const char * nome);
int inserirSimbolo(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int linha);
void destruirPilha(PilhaTabelas * pilha);

#endif