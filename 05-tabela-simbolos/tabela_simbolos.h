#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct Symbol Symbol;
typedef struct SymbolTable SymbolTable;
typedef struct SymbolTableStack SymbolTableStack;


/*
 * Entrada da tabela de símbolos.
 */
struct Symbol {
    char *name;

    ValueType type;

    int line;

    /*
     * Será utilizado posteriormente
     * na geração de código.
     */
    int offset;
    int size;

    Symbol *next;
};


/*
 * Uma tabela representa um único escopo.
 */
struct SymbolTable {
    Symbol *symbols;

    SymbolTable *next;
};


/*
 * Pilha de escopos.
 *
 * top aponta para o escopo atual.
 */
struct SymbolTableStack {
    SymbolTable *top;
};


/*
 * Cria uma pilha inicialmente vazia.
 */
void symbol_table_init(SymbolTableStack *stack);


/*
 * Cria uma nova tabela e coloca no topo.
 */
void symbol_table_push(SymbolTableStack *stack);


/*
 * Remove o escopo atual.
 */
void symbol_table_pop(SymbolTableStack *stack);


/*
 * Insere um identificador no escopo atual.
 *
 * Retorna:
 *   1 → sucesso
 *   0 → identificador já existe no escopo atual
 */
int symbol_table_insert(
    SymbolTableStack *stack,
    const char *name,
    ValueType type,
    int line
);


/*
 * Procura um identificador na pilha inteira.
 *
 * Retorna NULL caso não encontre.
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
void symbol_table_destroy(SymbolTableStack *stack);

#endif