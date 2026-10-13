#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tipos.h"
#include "tabela_simbolos.h"


/*
 * Executa análise semântica.
 *
 * Retorna:
 *
 * 1 -> programa correto
 * 0 -> erro semântico
 */

int semantic_analyze(
    ASTNode *root
);


/*
 * Quantidade de erros semânticos.
 */

int semantic_error_count(void);

#endif