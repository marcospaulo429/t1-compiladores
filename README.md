# T1 – Compiladores (UFG)

Compilador construído etapa por etapa, em C, para a linguagem **G-V1**, acompanhando o conteúdo da disciplina de Compiladores (UFG). Cada diretório corresponde a uma fase do compilador e foi criado em ordem cronológica, conforme o conteúdo era estudado. Cada fase parte do que a anterior produziu.

## Etapas

| # | Diretório | Fase | Estado |
|---|-----------|------|--------|
| 1 | [analise lexica/](analise%20lexica/) | Análise léxica com Flex | Concluída |
| 2 | [analise lexica e sintatica com flex e bison/](analise%20lexica%20e%20sintatica%20com%20flex%20e%20bison/) | Análise léxica + sintática com Flex e Bison | Concluída |
| 3 | [AST/](AST/) | Árvore sintática abstrata (AST) | Concluída |
| 4 | [tabela de simbolos/](tabela%20de%20simbolos/) | Tabela de símbolos com pilha de escopos | Concluída |
| 5 | [analise semantica/](analise%20semantica/) | Análise semântica (escopos e tipos) | Concluída |
| 6 | [geracao de codigo/](geracao%20de%20codigo/) | Geração de código assembly MIPS | Concluída |
| 7 | `makefile e testes` (renomeado para [final/](final/)) | Makefile e testes automatizados | Concluída |

O diretório [final/](final/) é a versão para entrega: o código completo, o Makefile e os testes.

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
[symble_table.h](tabela%20de%20simbolos/symble_table.h) e [symble_table.c](tabela%20de%20simbolos/symble_table.c):

- pilha de escopos, em que cada tabela guarda os símbolos de um escopo;
- operações `push`/`pop` de escopo, `insert` (retorna erro se o identificador já existe no escopo atual), `lookup` na pilha inteira e `lookup_current` só no escopo atual;
- cada símbolo guarda nome, tipo, linha de declaração e `offset`/`size`, usados na geração de código.

### 5. Análise semântica
Um percurso na AST que mantém a pilha de escopos. Cada bloco com declarações abre um escopo novo, que é removido ao final do bloco. Verifica:

- identificadores declarados antes do uso e sem redeclaração no mesmo escopo;
- atribuição com tipos iguais;
- operadores aritméticos e lógicos só sobre `int`, relacionais e de igualdade sobre operandos do mesmo tipo;
- condições de `se` e `enquanto` do tipo `int`.

Os erros saem como `ERRO: mensagem linha N`.

### 6. Geração de código
[codegen.c](geracao%20de%20codigo/codegen.c) percorre a AST e gera assembly MIPS que roda no SPIM ou no MARS:

- um único quadro de pilha guarda todas as variáveis. Cada símbolo recebe um `offset` a partir de `$fp`, com 4 bytes para `int` e 1 byte para `car`. Blocos aninhados usam o espaço logo depois do bloco externo e o liberam ao terminar;
- expressões deixam o resultado em `$t0`. Em operações binárias, o operando da esquerda é guardado na pilha enquanto o da direita é calculado;
- `||` e `&` avaliam o lado direito só se for necessário;
- `se`, `senao` e `enquanto` viram desvios com rótulos únicos;
- `leia`, `escreva` e `novalinha` usam `syscall` (inteiro ou caractere, conforme o tipo);
- cadeias de caracteres vão para a seção `.data`.

### 7. Makefile e testes
O [Makefile](final/Makefile) gera o `g-v1` com `make`. O script [rodar_testes.sh](final/rodar_testes.sh) compila os programas de [final/testes/ok/](final/testes/ok/), executa o assembly no SPIM e compara a saída esperada. Também confere que os programas de [final/testes/erro/](final/testes/erro/) são rejeitados com a mensagem certa.

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

Requer `flex`, `bison` e `gcc`. A especificação usa Flex 2.6.4 e Bison 3.8.2.

```sh
cd final
make                       # gera o executavel g-v1
./g-v1 programa.g          # gera programa.s
./g-v1 programa.g saida.s  # escolhe o nome do arquivo de saida
./g-v1 --ast programa.g    # tambem imprime a AST
spim -file programa.s      # executa o assembly gerado
```

Testes (precisam do `spim`, por exemplo `brew install spim`):

```sh
make test
make clean
```

No macOS o Bison do sistema é o 2.3. Para usar o 3.8.2 do Homebrew: `make BISON=/opt/homebrew/opt/bison/bin/bison`.

As etapas anteriores seguem o mesmo fluxo (`bison -d`, `flex`, `gcc`), sem os arquivos das fases seguintes.

## Estrutura

```
.
├── analise lexica/                              # 1. scanner (Flex)
├── analise lexica e sintatica com flex e bison/ # 2. scanner + parser (Flex + Bison)
├── AST/                                         # 3. construção e impressão da AST
├── tabela de simbolos/                          # 4. tabela de símbolos (escopos)
├── analise semantica/                           # 5. análise semântica
├── geracao de codigo/                           # 6. geração de código MIPS
└── final/                                       # 7. versão final: Makefile e testes
```
