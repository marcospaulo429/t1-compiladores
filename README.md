# T1 – Compiladores (UFG)

Compilador construído etapa por etapa, em C, para a linguagem **G-V1**, acompanhando o conteúdo da disciplina de Compiladores (UFG). Cada diretório corresponde a uma fase do compilador e foi criado em ordem cronológica, conforme o conteúdo era estudado. Cada fase parte do que a anterior produziu.

## Etapas

| # | Diretório | Fase | Estado |
|---|-----------|------|--------|
| 1 | [analise lexica/](analise%20lexica/) | Análise léxica com Flex | Concluída |
| 2 | [analise lexica e sintatica com flex e bison/](analise%20lexica%20e%20sintatica%20com%20flex%20e%20bison/) | Análise léxica + sintática com Flex e Bison | Concluída |
| 3 | [AST/](AST/) | Árvore sintática abstrata (AST) | Concluída |
| 4 | [tabela de simbolos/](tabela%20de%20simbolos/) | Tabela de símbolos com pilha de escopos | Em andamento |

### 1. Análise léxica
Scanner em Flex ([g-v1.l](analise%20lexica/g-v1.l)) que reconhece os tokens da linguagem:

- palavras reservadas: `principal`, `int`, `car`, `leia`, `escreva`, `novalinha`, `se`, `entao`, `senao`, `fimse`, `enquanto`;
- operadores: `+ - * /`, `< > <= >= == !=`, `&`, `||`, `!`, `=`;
- delimitadores: `{ } ( ) , : ;`;
- identificadores, constantes inteiras, caractere (`'a'`) e cadeias (`"texto"`);
- comentários `/* ... */`, com erro se o comentário não for fechado.

### 2. Análise sintática
O scanner passa a alimentar um parser Bison ([g-v1.y](analise%20lexica%20e%20sintatica%20com%20flex%20e%20bison/g-v1.y)). Nessa etapa a gramática da linguagem é definida e o programa é apenas validado (aceito ou rejeitado, com erro de sintaxe).

### 3. AST
O parser passa a construir uma árvore sintática abstrata durante a análise:

- [ast.h](AST/ast.h) e [ast.c](AST/ast.c): nós (`ASTNode`) com tipo, linha, lexema, tipo de valor, primeiro filho e próximo irmão, além de funções de construção, impressão (`ast_print`) e liberação (`ast_free`);
- [g-v1.y](AST/g-v1.y): ações semânticas que montam a árvore, com uma camada de expressões por precedência (`||`, `&`, igualdade, relacionais, soma/subtração, multiplicação/divisão, operadores unários e primários);
- [g-v1.l](AST/g-v1.l): scanner adaptado para devolver o lexema ao parser.

Ao final da análise, o programa imprime a AST.

### 4. Tabela de símbolos
Em desenvolvimento em [symble_table.h](tabela%20de%20simbolos/symble_table.h) e [symble_table.c](tabela%20de%20simbolos/symble_table.c):

- pilha de escopos, em que cada tabela guarda os símbolos de um escopo;
- operações `push`/`pop` de escopo, `insert` (retorna erro se o identificador já existe no escopo atual), `lookup` na pilha inteira e `lookup_current` só no escopo atual;
- cada símbolo guarda nome, tipo, linha de declaração e campos `offset`/`size` reservados para a geração de código.

Os próximos passos previstos são integrar a tabela ao parser e fazer a análise semântica (declaração e tipos), seguidos pela geração de código.

## A linguagem G-V1

Os programas têm extensão `.g`. Um programa tem a palavra `principal`, um bloco opcional de declaração de variáveis e um bloco de comandos:

```
principal {
    x: int;
    y: int;
} {
    x = 10 + y;
    escreva x;
    novalinha;
}
```

Tipos: `int` e `car`. Comandos: atribuição, `leia`, `escreva` (expressão ou cadeia), `novalinha`, `se ... entao ... [senao ...] fimse` e `enquanto`.

## Como compilar e executar

Requer `flex`, `bison` e `gcc`. Exemplo com a etapa da AST:

```sh
cd AST
bison -d g-v1.y
flex g-v1.l
gcc -o g-v1 g-v1.tab.c lex.yy.c ast.c
./g-v1 teste.g
```

As etapas anteriores seguem o mesmo fluxo, sem os arquivos da AST. A etapa 1 usa só o Flex.

## Estrutura

```
.
├── analise lexica/                              # 1. scanner (Flex)
├── analise lexica e sintatica com flex e bison/ # 2. scanner + parser (Flex + Bison)
├── AST/                                         # 3. construção e impressão da AST
└── tabela de simbolos/                          # 4. tabela de símbolos (escopos)
```
