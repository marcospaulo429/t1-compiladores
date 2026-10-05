#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "ast.h"


/*
 * Gera código assembly MIPS (SPIM / MARS)
 * a partir da AST.
 *
 * A AST precisa ter passado pela análise semântica
 * sem erros.
 *
 * Retorna:
 *
 * 1 -> código gerado
 * 0 -> AST inválida
 */

int codegen_generate(
    ASTNode *root,
    FILE *out
);

#endif
