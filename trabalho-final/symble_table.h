#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"


typedef struct Symbol Symbol;

typedef struct SymbolTable SymbolTable;

typedef struct SymbolTableStack SymbolTableStack;


/*
 * Uma entrada da tabela de símbolos.
 */

struct Symbol {

    char *name;

    ValueType type;

    int line;


    /*
     * Serão utilizados posteriormente
     * na geração de código.
     */

    int offset;

    int size;


    /*
     * Próximo símbolo da tabela.
     */

    Symbol *next;
};


/*
 * Uma tabela representa um escopo.
 */

struct SymbolTable {

    Symbol *symbols;

    /*
     * Próximo escopo externo.
     */

    SymbolTable *next;
};


/*
 * Pilha de escopos.
 */

struct SymbolTableStack {

    SymbolTable *top;
};


/*
 * Inicializa a pilha.
 */

void symbol_table_init(
    SymbolTableStack *stack
);


/*
 * Cria e empilha novo escopo.
 */

void symbol_table_push(
    SymbolTableStack *stack
);


/*
 * Remove o escopo atual.
 */

void symbol_table_pop(
    SymbolTableStack *stack
);


/*
 * Insere no escopo atual.
 *
 * Retorna:
 *
 * 1 -> sucesso
 * 0 -> identificador já existe no escopo atual
 */

int symbol_table_insert(
    SymbolTableStack *stack,
    const char *name,
    ValueType type,
    int line
);


/*
 * Procura em toda a pilha.
 */

Symbol *symbol_table_lookup(
    SymbolTableStack *stack,
    const char *name
);


/*
 * Procura somente no escopo atual.
 */

Symbol *symbol_table_lookup_current(
    SymbolTableStack *stack,
    const char *name
);


/*
 * Libera toda a pilha.
 */

void symbol_table_destroy(
    SymbolTableStack *stack
);

#endif