#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symble_table.h"


/*
 * Cria tabela vazia.
 */

static SymbolTable *create_table(void)
{
    SymbolTable *table =
        malloc(sizeof(SymbolTable));


    if (table == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        exit(1);
    }


    table->symbols = NULL;

    table->next = NULL;


    return table;
}


/*
 * Cria entrada de símbolo.
 */

static Symbol *create_symbol(
    const char *name,
    ValueType type,
    int line
)
{
    Symbol *symbol =
        malloc(sizeof(Symbol));


    if (symbol == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        exit(1);
    }


    symbol->name =
        malloc(strlen(name) + 1);


    if (symbol->name == NULL) {

        fprintf(
            stderr,
            "ERRO: memoria insuficiente\n"
        );

        free(symbol);

        exit(1);
    }


    strcpy(
        symbol->name,
        name
    );


    symbol->type = type;

    symbol->line = line;


    /*
     * Ainda não temos geração de código.
     */

    symbol->offset = 0;

    symbol->size = 0;


    symbol->next = NULL;


    return symbol;
}


/*
 * Inicializa pilha.
 */

void symbol_table_init(
    SymbolTableStack *stack
)
{
    stack->top = NULL;
}


/*
 * Empilha um novo escopo.
 */

void symbol_table_push(
    SymbolTableStack *stack
)
{
    SymbolTable *table =
        create_table();


    table->next =
        stack->top;


    stack->top =
        table;
}


/*
 * Remove o escopo atual.
 */

void symbol_table_pop(
    SymbolTableStack *stack
)
{
    if (
        stack == NULL ||
        stack->top == NULL
    ) {
        return;
    }


    SymbolTable *table =
        stack->top;


    stack->top =
        table->next;


    Symbol *symbol =
        table->symbols;


    while (symbol != NULL) {

        Symbol *next =
            symbol->next;


        free(
            symbol->name
        );


        free(symbol);


        symbol = next;
    }


    free(table);
}


/*
 * Procura no escopo atual.
 */

Symbol *symbol_table_lookup_current(
    SymbolTableStack *stack,
    const char *name
)
{
    if (
        stack == NULL ||
        stack->top == NULL
    ) {
        return NULL;
    }


    Symbol *current =
        stack->top->symbols;


    while (current != NULL) {

        if (
            strcmp(
                current->name,
                name
            ) == 0
        ) {

            return current;
        }


        current =
            current->next;
    }


    return NULL;
}


/*
 * Procura do escopo atual para
 * os escopos externos.
 */

Symbol *symbol_table_lookup(
    SymbolTableStack *stack,
    const char *name
)
{
    if (stack == NULL) {
        return NULL;
    }


    SymbolTable *table =
        stack->top;


    while (table != NULL) {

        Symbol *symbol =
            table->symbols;


        while (symbol != NULL) {

            if (
                strcmp(
                    symbol->name,
                    name
                ) == 0
            ) {

                return symbol;
            }


            symbol =
                symbol->next;
        }


        table =
            table->next;
    }


    return NULL;
}


/*
 * Insere no escopo atual.
 */

int symbol_table_insert(
    SymbolTableStack *stack,
    const char *name,
    ValueType type,
    int line
)
{
    if (
        stack == NULL ||
        stack->top == NULL
    ) {
        return 0;
    }


    /*
     * Só é duplicado se existir
     * no escopo atual.
     */

    if (
        symbol_table_lookup_current(
            stack,
            name
        ) != NULL
    ) {

        return 0;
    }


    Symbol *symbol =
        create_symbol(
            name,
            type,
            line
        );


    symbol->next =
        stack->top->symbols;


    stack->top->symbols =
        symbol;


    return 1;
}


/*
 * Destrói a pilha inteira.
 */

void symbol_table_destroy(
    SymbolTableStack *stack
)
{
    if (stack == NULL) {
        return;
    }


    while (
        stack->top != NULL
    ) {

        symbol_table_pop(
            stack
        );
    }
}