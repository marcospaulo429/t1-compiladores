#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerador.h"
#include "tabela_simbolos.h"

static FILE *arqSaida = NULL;
static PilhaTabelas *pilhaCodegen = NULL;

/* Numero de posicoes (palavras) de variaveis ja alocadas na funcao em geracao */
static int posicaoAtual = 0;
/* Numero de parametros da funcao em geracao (-1 dentro do bloco principal) */
static int nParamsAtual = -1;

static int contadorRotulos = 0;
static int novoRotulo(void) {
    return ++contadorRotulos;
}

/* Tabela estática de strings literais para a seção .data */
typedef struct NodoString {
    int id;
    char *texto;
    struct NodoString *prox;
} NodoString;

static NodoString *listaStrings = NULL;
static int contadorStrings = 0;

static int registraString(const char *texto) {
    NodoString *nova = (NodoString*) malloc(sizeof(NodoString));
    nova->id = ++contadorStrings;
    nova->texto = strdup(texto);
    nova->prox = listaStrings;
    listaStrings = nova;
    return nova->id;
}

static TipoDado extraiTipoAST(Toperador *noTipo) {
    if (!noTipo) return TIPO_INDEFINIDO;
    if (noTipo->tipoOperador == noTipoInt) return TIPO_INT;
    if (noTipo->tipoOperador == noTipoCar) return TIPO_CAR;
    return TIPO_INDEFINIDO;
}

/* Aloca (na tabela de simbolos) as variaveis de uma lista de declaracao.
   Um vetor de n elementos conta como n posicoes. Devolve o total de posicoes. */
static int alocaVariaveis(Toperador *noLstId, TipoDado tipo, int linha) {
    if (!noLstId) return 0;

    int slots = 0;
    Toperador *id = noLstId->filho1;
    if (id && id->lexema) {
        TipoDado tipoVar = tipo;
        int tamanho = 0;
        slots = 1;
        if (id->tipoOperador == noIdVet) {
            tipoVar = tipoVetorDe(tipo);
            tamanho = id->numero;
            slots = tamanho;
        }
        inserirVariavel(pilhaCodegen, id->lexema, tipoVar, posicaoAtual + 1, tamanho, linha);
        posicaoAtual += slots;
    }
    return slots + alocaVariaveis(noLstId->filho2, tipo, linha);
}

static int processaDeclaracoesCodegen(Toperador *noLstDecl) {
    if (!noLstDecl) return 0;

    int total = 0;
    Toperador *decl = noLstDecl->filho1;
    if (decl && decl->tipoOperador == noDeclVar) {
        TipoDado tipo = extraiTipoAST(decl->filho2);
        total = alocaVariaveis(decl->filho1, tipo, decl->linha);
    }
    return total + processaDeclaracoesCodegen(noLstDecl->filho2);
}

/* Coleta antecipadamente todas as cadeias de caracteres do programa para a seção .data */
static void coletaStrings(Toperador *no) {
    if (!no) return;
    if (no->tipoOperador == noCadConst && no->lexema) {
        registraString(no->lexema);
    }
    coletaStrings(no->filho1);
    coletaStrings(no->filho2);
    coletaStrings(no->filho3);
}

static void cgenExpr(Toperador *expr);
static void cgenCmd(Toperador *cmd);
static void cgenBloco(Toperador *bloco, int reutilizaTabela);

static void emiteEmpilha(void) {
    fprintf(arqSaida, "    sw $s0, 0($sp)\n");
    fprintf(arqSaida, "    addiu $sp, $sp, -4\n");
}

static void emiteDesempilha(const char *reg) {
    fprintf(arqSaida, "    lw %s, 4($sp)\n", reg);
    fprintf(arqSaida, "    addiu $sp, $sp, 4\n");
}

/* Acesso a variaveis.
   - parametro: acima do $fp, o i-esimo parametro fica em 4*i($fp)
   - global (escopo 0): a partir de $s1, posicao p em -(p-1)*4($s1)
   - local (escopo > 0): a partir de $fp, posicao p em -4*p($fp) */
static const char *registradorBase(Simbolo *s) {
    return (s->categoria == CAT_VAR && s->escopo == 0) ? "$s1" : "$fp";
}

static int deslocamento(Simbolo *s) {
    if (s->categoria == CAT_PARAM) return 4 * s->posicao;
    if (s->escopo == 0) return -(s->posicao - 1) * 4;
    return -4 * s->posicao;
}

/* Coloca em $reg o endereco do elemento 0 do vetor s. Vetor declarado:
   endereco da sua primeira posicao. Parametro vetor: o endereco recebido. */
static void emiteBaseVetor(Simbolo *s, const char *reg) {
    if (s->categoria == CAT_PARAM) {
        fprintf(arqSaida, "    lw %s, %d($fp)    # endereco do vetor %s\n", reg, deslocamento(s), s->nome);
    } else {
        fprintf(arqSaida, "    addiu %s, %s, %d    # endereco do vetor %s\n", reg, registradorBase(s), deslocamento(s), s->nome);
    }
}

/* Calcula em $t1 o endereco de v[i]: base - 4*i. Usa $s0 (destroi). */
static void emiteEnderecoElemento(Toperador *noIndice) {
    Simbolo *s = pesquisarNome(pilhaCodegen, noIndice->filho1->lexema);
    cgenExpr(noIndice->filho2);
    fprintf(arqSaida, "    sll $s0, $s0, 2\n");
    emiteBaseVetor(s, "$t1");
    fprintf(arqSaida, "    subu $t1, $t1, $s0\n");
}

/* Guarda o valor de $s0 no lvalue (variavel ou elemento de vetor).
   O valor de $s0 e preservado ao final. */
static void emiteGuardaLValue(Toperador *lv) {
    if (lv->tipoOperador == noIndice) {
        emiteEmpilha();
        emiteEnderecoElemento(lv);
        emiteDesempilha("$s0");
        fprintf(arqSaida, "    sw $s0, 0($t1)    # %s[...] = $s0\n", lv->filho1->lexema);
    } else {
        Simbolo *s = pesquisarNome(pilhaCodegen, lv->lexema);
        fprintf(arqSaida, "    sw $s0, %d(%s)    # %s = $s0\n", deslocamento(s), registradorBase(s), s->nome);
    }
}

/* Tipo estatico de uma expressao (o programa ja passou pela analise semantica) */
static TipoDado tipoExpr(Toperador *e) {
    if (!e) return TIPO_INDEFINIDO;
    switch (e->tipoOperador) {
        case noCarConst: return TIPO_CAR;
        case noId: {
            Simbolo *s = pesquisarNome(pilhaCodegen, e->lexema);
            return s ? s->tipo : TIPO_INT;
        }
        case noIndice: {
            Simbolo *s = pesquisarNome(pilhaCodegen, e->filho1->lexema);
            return s ? tipoBase(s->tipo) : TIPO_INT;
        }
        case noChamada: {
            Simbolo *s = pesquisarNome(pilhaCodegen, e->lexema);
            return s ? s->tipo : TIPO_INT;
        }
        case noAtrib: return tipoExpr(e->filho1);
        default: return TIPO_INT;
    }
}

/* Empilha os argumentos na ordem inversa (en primeiro, e1 por ultimo) */
static void empilhaArgumentos(Toperador *lstExpr) {
    if (!lstExpr) return;
    empilhaArgumentos(lstExpr->filho2);
    cgenExpr(lstExpr->filho1);
    emiteEmpilha();
}

/* Sequencia de retorno do codigo chamado */
static void emiteRetorno(void) {
    if (nParamsAtual < 0) {
        fprintf(arqSaida, "    li $v0, 10    # retorne no principal encerra o programa\n");
        fprintf(arqSaida, "    syscall\n");
        return;
    }
    fprintf(arqSaida, "    lw $ra, 0($fp)\n");
    fprintf(arqSaida, "    move $sp, $fp\n");
    fprintf(arqSaida, "    addiu $sp, $sp, %d\n", (nParamsAtual + 1) * 4);
    fprintf(arqSaida, "    lw $fp, 0($sp)\n");
    fprintf(arqSaida, "    jr $ra\n");
}

/* Geração de código para expressões (resultado sempre em $s0) */
static void cgenExpr(Toperador *expr) {
    if (!expr) return;

    switch (expr->tipoOperador) {
        case noIntConst:
            fprintf(arqSaida, "    li $s0, %s\n", expr->lexema);
            break;

        case noCarConst:
            if (strcmp(expr->lexema, "'\\n'") == 0) {
                fprintf(arqSaida, "    li $s0, 10\n");
            } else if (strcmp(expr->lexema, "'\\t'") == 0) {
                fprintf(arqSaida, "    li $s0, 9\n");
            } else {
                fprintf(arqSaida, "    li $s0, %d\n", (int)expr->lexema[1]);
            }
            break;

        case noId: {
            Simbolo *s = pesquisarNome(pilhaCodegen, expr->lexema);
            if (ehVetor(s->tipo)) {
                /* um vetor usado sem indice vale o seu endereco */
                emiteBaseVetor(s, "$s0");
            } else {
                fprintf(arqSaida, "    lw $s0, %d(%s)    # carrega %s\n", deslocamento(s), registradorBase(s), s->nome);
            }
            break;
        }

        case noIndice:
            emiteEnderecoElemento(expr);
            fprintf(arqSaida, "    lw $s0, 0($t1)    # %s[...]\n", expr->filho1->lexema);
            break;

        case noChamada: {
            /* sequencia de chamada do codigo chamador */
            fprintf(arqSaida, "    sw $fp, 0($sp)    # empilha $fp do chamador\n");
            fprintf(arqSaida, "    addiu $sp, $sp, -4\n");
            empilhaArgumentos(expr->filho1);
            fprintf(arqSaida, "    jal f_%s\n", expr->lexema);
            break;
        }

        case noAtrib:
            cgenExpr(expr->filho2);
            emiteGuardaLValue(expr->filho1);
            break;

        /* Operadores Binários Aritméticos, Lógicos e Relacionais */
        case noMais:
        case noMenos:
        case noMult:
        case noDiv:
        case noOU:
        case noE:
        case noIgual:
        case noDiferente:
        case noMenor:
        case noMaior:
        case noMenorIgual:
        case noMaiorIgual: {
            /* 1. Computa filho1 (e1) */
            cgenExpr(expr->filho1);
            /* 2. Empilha $s0 */
            emiteEmpilha();
            /* 3. Computa filho2 (e2) */
            cgenExpr(expr->filho2);
            /* 4. Desempilha e1 em $t1 */
            emiteDesempilha("$t1");

            /* 5. Opera $t1 (esq) com $s0 (dir), guardando o resultado em $s0 */
            switch (expr->tipoOperador) {
                case noMais:       fprintf(arqSaida, "    add $s0, $t1, $s0\n"); break;
                case noMenos:      fprintf(arqSaida, "    sub $s0, $t1, $s0\n"); break;
                case noMult:       fprintf(arqSaida, "    mul $s0, $t1, $s0\n"); break;
                case noDiv:        fprintf(arqSaida, "    div $s0, $t1, $s0\n"); break;
                case noOU:         fprintf(arqSaida, "    or $s0, $t1, $s0\n"); break;
                case noE:          fprintf(arqSaida, "    and $s0, $t1, $s0\n"); break;
                case noIgual:      fprintf(arqSaida, "    seq $s0, $t1, $s0\n"); break;
                case noDiferente:  fprintf(arqSaida, "    sne $s0, $t1, $s0\n"); break;
                case noMenor:      fprintf(arqSaida, "    slt $s0, $t1, $s0\n"); break;
                case noMaior:      fprintf(arqSaida, "    sgt $s0, $t1, $s0\n"); break;
                case noMenorIgual: fprintf(arqSaida, "    sle $s0, $t1, $s0\n"); break;
                case noMaiorIgual: fprintf(arqSaida, "    sge $s0, $t1, $s0\n"); break;
                default: break;
            }
            break;
        }

        /* Operadores Unários */
        case noMenosUnario:
            cgenExpr(expr->filho1);
            fprintf(arqSaida, "    neg $s0, $s0\n");
            break;

        case noNao:
            cgenExpr(expr->filho1);
            fprintf(arqSaida, "    seq $s0, $s0, $zero\n");
            break;

        default:
            break;
    }
}

static int buscaIdString(const char *texto) {
    NodoString *atual = listaStrings;
    while (atual) {
        if (strcmp(atual->texto, texto) == 0) return atual->id;
        atual = atual->prox;
    }
    return 1;
}

static void cgenCmd(Toperador *cmd) {
    if (!cmd) return;

    switch (cmd->tipoOperador) {
        case noLstCmd:
            cgenCmd(cmd->filho1);
            cgenCmd(cmd->filho2);
            break;

        case noCmdVazio:
            break;

        case noNovalinha:
            fprintf(arqSaida, "    li $a0, 10\n");
            fprintf(arqSaida, "    li $v0, 11\n");
            fprintf(arqSaida, "    syscall\n");
            break;

        case noEscrevaCadeia: {
            int strId = buscaIdString(cmd->filho1->lexema);
            fprintf(arqSaida, "    la $a0, str_%d\n", strId);
            fprintf(arqSaida, "    li $v0, 4\n");
            fprintf(arqSaida, "    syscall\n");
            break;
        }

        case noEscreva:
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    move $a0, $s0\n");
            fprintf(arqSaida, "    li $v0, %d\n", tipoExpr(cmd->filho1) == TIPO_CAR ? 11 : 1);
            fprintf(arqSaida, "    syscall\n");
            break;

        case noRetorne:
            cgenExpr(cmd->filho1);   /* valor retornado fica em $s0 */
            emiteRetorno();
            break;

        case noLeia: {
            int car = (tipoExpr(cmd->filho1) == TIPO_CAR);
            fprintf(arqSaida, "    li $v0, %d\n", car ? 12 : 5);
            fprintf(arqSaida, "    syscall\n");
            fprintf(arqSaida, "    move $s0, $v0\n");
            emiteGuardaLValue(cmd->filho1);
            break;
        }

        case noSe: {
            int lFim = novoRotulo();
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_fim_%d\n", lFim);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "L_fim_%d:\n", lFim);
            break;
        }

        case noSeSenao: {
            int lElse = novoRotulo();
            int lFim = novoRotulo();
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_else_%d\n", lElse);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "    j L_fim_%d\n", lFim);
            fprintf(arqSaida, "L_else_%d:\n", lElse);
            cgenCmd(cmd->filho3);
            fprintf(arqSaida, "L_fim_%d:\n", lFim);
            break;
        }

        case noEnquanto: {
            int lInicio = novoRotulo();
            int lFim = novoRotulo();
            fprintf(arqSaida, "L_while_%d:\n", lInicio);
            cgenExpr(cmd->filho1);
            fprintf(arqSaida, "    beq $s0, $zero, L_fim_while_%d\n", lFim);
            cgenCmd(cmd->filho2);
            fprintf(arqSaida, "    j L_while_%d\n", lInicio);
            fprintf(arqSaida, "L_fim_while_%d:\n", lFim);
            break;
        }

        case noBloco:
            cgenBloco(cmd, 0);
            break;

        default:
            cgenExpr(cmd);
            break;
    }
}

/* Bloco com declaracao de variaveis: cria tabela (exceto no bloco mais externo
   da funcao, que reaproveita a tabela dos parametros) e reserva espaco na pilha. */
static void cgenBloco(Toperador *bloco, int reutilizaTabela) {
    if (!bloco) return;

    int empilhou = 0;
    int bytesAlocados = 0;
    int posicaoAnterior = posicaoAtual;

    if (bloco->filho1 != NULL) {
        if (!reutilizaTabela) {
            empilharTabela(pilhaCodegen);
            empilhou = 1;
        }
        bytesAlocados = 4 * processaDeclaracoesCodegen(bloco->filho1->filho1);
        if (bytesAlocados > 0) {
            fprintf(arqSaida, "    addiu $sp, $sp, -%d   # aloca variaveis do bloco\n", bytesAlocados);
        }
    }

    cgenCmd(bloco->filho2);

    if (bloco->filho1 != NULL && bytesAlocados > 0) {
        fprintf(arqSaida, "    addiu $sp, $sp, %d    # desaloca variaveis do bloco\n", bytesAlocados);
    }
    if (empilhou) {
        desempilharTabela(pilhaCodegen);
    }
    posicaoAtual = posicaoAnterior;
}

static TipoDado tipoDoParametro(Toperador *p) {
    TipoDado base = extraiTipoAST(p->filho1);
    return p->numero ? tipoVetorDe(base) : base;
}

/* Declara a funcao (e seus parametros) na tabela global antes de gerar codigo */
static void declaraFuncao(Toperador *f) {
    int n = 0;
    for (Toperador *l = f->filho1; l; l = l->filho2) n++;

    Simbolo *sf = inserirFuncao(pilhaCodegen, f->lexema, n, extraiTipoAST(f->filho2), f->linha);
    int pos = 0;
    for (Toperador *l = f->filho1; l; l = l->filho2) {
        pos++;
        adicionarParametroFuncao(sf, l->filho1->lexema, tipoDoParametro(l->filho1), pos);
    }
}

static void cgenFuncao(Toperador *f) {
    int n = 0;
    for (Toperador *l = f->filho1; l; l = l->filho2) n++;

    fprintf(arqSaida, "\nf_%s:\n", f->lexema);
    /* sequencia de chamada do codigo chamado */
    fprintf(arqSaida, "    move $fp, $sp\n");
    fprintf(arqSaida, "    sw $ra, 0($sp)\n");
    fprintf(arqSaida, "    addiu $sp, $sp, -4\n");

    empilharTabela(pilhaCodegen);
    int pos = 0;
    for (Toperador *l = f->filho1; l; l = l->filho2) {
        pos++;
        inserirParametro(pilhaCodegen, l->filho1->lexema, tipoDoParametro(l->filho1), pos, l->filho1->linha);
    }

    nParamsAtual = n;
    posicaoAtual = 0;
    cgenBloco(f->filho3, 1);
    emiteRetorno();   /* fim da funcao sem retorne */

    desempilharTabela(pilhaCodegen);
    nParamsAtual = -1;
}

void geraCodigoMIPS(Toperador *raiz, FILE *saida) {
    if (!raiz || !saida || raiz->tipoOperador != noProg) return;

    arqSaida = saida;
    pilhaCodegen = iniciarPilha();
    empilharTabela(pilhaCodegen);   /* escopo 0: globais e funcoes */
    posicaoAtual = 0;
    nParamsAtual = -1;
    contadorRotulos = 0;
    listaStrings = NULL;
    contadorStrings = 0;

    /* Coleta e declara todas as strings no segmento .data */
    coletaStrings(raiz);

    fprintf(arqSaida, "    .data\n");
    NodoString *s = listaStrings;
    while (s) {
        fprintf(arqSaida, "str_%d: .asciiz %s\n", s->id, s->texto);
        s = s->prox;
    }

    /* Inicia o segmento .text */
    fprintf(arqSaida, "\n    .text\n");
    fprintf(arqSaida, "    .globl main\n");
    fprintf(arqSaida, "main:\n");
    fprintf(arqSaida, "    move $s1, $sp         # $s1 aponta para o inicio das variaveis globais\n");

    /* variaveis globais: escopo 0, posicao a partir de 1 */
    if (raiz->filho1) {
        int bytes = 4 * processaDeclaracoesCodegen(raiz->filho1->filho1);
        if (bytes > 0) {
            fprintf(arqSaida, "    addiu $sp, $sp, -%d   # aloca variaveis globais\n", bytes);
        }
    }

    /* todas as funcoes ficam na tabela global antes de gerar qualquer codigo */
    for (Toperador *l = raiz->filho2; l; l = l->filho2) {
        declaraFuncao(l->filho1);
    }

    /* o bloco principal tem frame como as funcoes: $fp aponta para o slot do "$ra" */
    fprintf(arqSaida, "    move $fp, $sp         # Frame Pointer do bloco principal\n");
    fprintf(arqSaida, "    addiu $sp, $sp, -4\n");

    posicaoAtual = 0;
    empilharTabela(pilhaCodegen);
    cgenBloco(raiz->filho3, 1);
    desempilharTabela(pilhaCodegen);

    /* Encerra o programa com syscall 10 (exit) */
    fprintf(arqSaida, "\n    # Encerra execucao\n");
    fprintf(arqSaida, "    li $v0, 10\n");
    fprintf(arqSaida, "    syscall\n");

    /* codigo das funcoes */
    for (Toperador *l = raiz->filho2; l; l = l->filho2) {
        cgenFuncao(l->filho1);
    }

    destruirPilha(pilhaCodegen);
}
