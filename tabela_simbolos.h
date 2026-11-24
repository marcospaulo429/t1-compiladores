#ifndef _tabela_simbolos_h
#define _tabela_simbolos_h

#define TAM_HASH 211

typedef enum {
    TIPO_INT,
    TIPO_CAR,
    TIPO_VET_INT,   /* vetor de int (ponteiro para sequencia de int) */
    TIPO_VET_CAR,   /* vetor de car */
    TIPO_INDEFINIDO
} TipoDado;

typedef enum {
    CAT_VAR,        /* variavel (global ou local) */
    CAT_PARAM,      /* parametro formal de funcao */
    CAT_FUNC        /* funcao */
} Categoria;

/* Parametro formal guardado na entrada da funcao: o compilador precisa
   dele para checar os argumentos de uma chamada feita fora da funcao,
   onde os lexemas dos parametros nao sao visiveis. */
typedef struct Parametro {
    char * nome;
    TipoDado tipo;
    int posicao;            /* posicao (1..n) na lista de parametros */
    struct Parametro * prox;
} Parametro;

typedef struct Simbolo {
    char * nome;
    Categoria categoria;
    TipoDado tipo;          /* tipo da variavel/parametro ou tipo de retorno da funcao */
    int linha;
    int escopo;             /* 0 = global; >0 = nivel de aninhamento dentro da funcao */
    int posicao;            /* posicao na lista de variaveis (vetor de n conta como n) */
    int tamanho;            /* numero de elementos de vetor declarado (0 se nao for) */
    int nParams;            /* funcao: numero de parametros */
    Parametro * params;     /* funcao: lista de parametros, na ordem de declaracao */
    struct Simbolo * prox;
} Simbolo;

/* Tabela Hash com encadeamento (buckets) */
typedef struct TabelaSimbolos {
    Simbolo * baldes[TAM_HASH];
    int escopo;
    struct TabelaSimbolos * anterior;
} TabelaSimbolos;

typedef struct PilhaTabelas {
    TabelaSimbolos * topo;
} PilhaTabelas;

/* a) iniciar a pilha de tabelas (vazia) */
PilhaTabelas * iniciarPilha(void);
/* b) criar nova tabela (novo escopo) e empilhar */
void empilharTabela(PilhaTabelas * pilha);
/* c) pesquisar nome do topo para a base; NULL se nao achar */
Simbolo * pesquisarNome(PilhaTabelas * pilha, const char * nome);
Simbolo * pesquisarNoEscopoAtual(PilhaTabelas * pilha, const char * nome);
/* d) remover escopo atual */
void desempilharTabela(PilhaTabelas * pilha);
/* e) inserir funcao na tabela atual; devolve a entrada (ou NULL se o nome ja existe no escopo).
      Os parametros sao anexados em seguida com adicionarParametroFuncao. */
Simbolo * inserirFuncao(PilhaTabelas * pilha, const char * nome, int nArgs, TipoDado tipoRetorno, int linha);
void adicionarParametroFuncao(Simbolo * funcao, const char * nome, TipoDado tipo, int posicao);
/* f) inserir variavel na tabela atual (tamanho > 0 indica vetor declarado); 0 se ja existe no escopo */
int inserirVariavel(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int posicao, int tamanho, int linha);
/* g) inserir parametro na tabela atual; 0 se ja existe no escopo */
int inserirParametro(PilhaTabelas * pilha, const char * nome, TipoDado tipo, int posicao, int linha);
/* h) eliminar a pilha de tabelas */
void destruirPilha(PilhaTabelas * pilha);

/* auxiliares sobre tipos */
int ehVetor(TipoDado tipo);
TipoDado tipoBase(TipoDado tipo);       /* TIPO_VET_INT -> TIPO_INT, etc. */
TipoDado tipoVetorDe(TipoDado base);    /* TIPO_INT -> TIPO_VET_INT, etc. */

#endif
