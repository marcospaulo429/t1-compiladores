/* testa_tabela.c - Teste Unitario da Secao 5 */
#include <stdio.h>
#include <assert.h>
#include "tabela_simbolos.h"

int main(void) {
    printf("=== TESTE UNITARIO DA TABELA DE SIMBOLOS ===\n");

    /* a) Iniciar pilha vazia */
    PilhaTabelas *pilha = iniciarPilha();
    assert(pilha != NULL);
    assert(pilha->topo == NULL);

    /* b) Criar nova tabela e empilhar (Escopo 1 - Bloco Principal) */
    empilharTabela(pilha);
    assert(pilha->topo != NULL);

    /* Insercao no escopo 1 */
    assert(inserirSimbolo(pilha, "x", TIPO_INT, 10) == 1);
    assert(inserirSimbolo(pilha, "y", TIPO_CAR, 11) == 1);

    /* Teste de redeclaracao no mesmo escopo (deve falhar) */
    assert(inserirSimbolo(pilha, "x", TIPO_CAR, 12) == 0);

    /* b) Novo escopo empilhado (Escopo 2 - Bloco Interno) */
    empilharTabela(pilha);

    /* Sombreamento: permite declarar 'x' no bloco interno */
    assert(inserirSimbolo(pilha, "x", TIPO_CAR, 20) == 1);

    /* c) Pesquisar nome: 'x' deve resolver para o TIPO_CAR do topo */
    Simbolo *s = pesquisarNome(pilha, "x");
    assert(s != NULL && s->tipo == TIPO_CAR && s->linha == 20);

    /* 'y' nao existe no topo, mas deve ser encontrado no escopo anterior */
    s = pesquisarNome(pilha, "y");
    assert(s != NULL && s->tipo == TIPO_CAR && s->linha == 11);

    /* 'z' nao foi declarada em nenhum escopo (deve retornar NULL) */
    assert(pesquisarNome(pilha, "z") == NULL);

    /* d) Desempilhar escopo atual (sai do bloco interno) */
    desempilharTabela(pilha);

    /* Agora 'x' deve resolver para o TIPO_INT do escopo 1 */
    s = pesquisarNome(pilha, "x");
    assert(s != NULL && s->tipo == TIPO_INT && s->linha == 10);

    /* Limpeza final */
    destruirPilha(pilha);
    printf("Todos os testes da Tabela de Simbolos passaram com sucesso!\n");
    return 0;
}