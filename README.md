# Trabalho 1 - Compilador para a linguagem G-V1

Compiladores - Instituto de Informática (UFG), 2026-2
Prof. Thierson Couto Rosa

Alunos: Marcos Paulo e Giordana

Compilador para a linguagem G-V1 feito em C com Flex e Bison. Ele lê um programa `.g`,
faz a análise léxica, sintática e semântica e, se não tiver erro, gera o código em assembly
MIPS, que pode ser executado no SPIM ou no MARS.

O enunciado está em [Parte1.pdf](Parte1.pdf). O histórico de como fomos fazendo cada etapa
está na branch `desenvolvimento`.

## Organização

```
.
├── Parte1.pdf          enunciado
├── Dockerfile          ambiente Linux para compilar e testar
├── trabalho-final/     codigo do compilador (pasta que e entregue)
│   ├── Makefile
│   ├── g-v1.l          analisador lexico (Flex)
│   ├── g-v1.y          analisador sintatico + construcao da AST (Bison) e main
│   ├── ast.h / ast.c   arvore sintatica abstrata
│   ├── symble_table.h / symble_table.c   pilha de tabelas de simbolos
│   ├── semantic.h / semantic.c           analisador semantico
│   └── codegen.h / codegen.c             gerador de codigo MIPS
└── testes/
    ├── rodar_testes.sh
    ├── ok/             programas corretos (.g), entrada (.in) e saida esperada (.out)
    └── erro/           programas com erro (.g) e a mensagem esperada (.err)
```

## Como compilar e rodar

Precisa de `gcc`, `flex` e `bison` (testamos com gcc 14, Flex 2.6.4 e Bison 3.8.2, que são as
versões do enunciado).

```sh
cd trabalho-final
make                         # gera o executavel g-v1
./g-v1 programa.g            # gera programa.s
./g-v1 programa.g saida.s    # escolhendo o nome do arquivo de saida
./g-v1 --ast programa.g      # tambem mostra a AST na tela
spim -file programa.s        # executa o codigo gerado
make clean
```

Se o programa tiver erro, o compilador mostra uma linha começando com `ERRO:` e o número
da linha, não gera o `.s` e termina com código de saída 1.

No macOS o `bison` que vem no sistema é o 2.3. Dá pra usar o do Homebrew com
`make BISON=/opt/homebrew/opt/bison/bin/bison`.

## Testes

```sh
cd trabalho-final
make test
```

O script [testes/rodar_testes.sh](testes/rodar_testes.sh) compila cada programa de `testes/ok`,
executa o assembly no SPIM (com o `.in` como entrada, quando tem) e compara com o `.out`.
Para os programas de `testes/erro`, confere se o compilador recusa o programa, não gera
assembly e mostra a mensagem do `.err`.

São 15 programas corretos e 26 com erro (6 léxicos, 7 sintáticos e 13 semânticos). Os de
nota em conceito, PA, soma, sequência ordenada e fatorial vieram dos testes que a Giordana
juntou; os outros a gente escreveu para testar cada parte da linguagem.

Rodamos tudo em Linux (container `gcc:14`) e no macOS: `41 passaram, 0 falharam`.

### Testando com Docker

Para testar no mesmo ambiente do enunciado sem instalar nada, tem um `Dockerfile` na raiz.
Ele usa a imagem `gcc:14` e instala Flex, Bison e SPIM:

```sh
docker build -t g-v1 .
docker run --rm g-v1              # compila e roda os testes
docker run --rm -it g-v1 bash     # abre um terminal no container
```

## O que o enunciado pede e onde está

| Item do enunciado | Onde está |
|-------------------|-----------|
| 3.1 Analisador léxico com Flex | `g-v1.l` |
| Comentários `/* */` em várias linhas, com erro se não fechar | `g-v1.l` (estado `COMENTARIO`) |
| Mensagens `ERRO: CARACTERE INVÁLIDO`, `COMENTÁRIO NAO TERMINA`, `CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA` + linha | `g-v1.l` |
| 3.2 Analisador sintático com Bison, `extern yylineno`, `yytext`, `yylex` | `g-v1.y` |
| `yyerror` com `ERRO:` e a linha | `g-v1.y` |
| `main` abre `argv[1]` em `yyin` e fecha no final | `g-v1.y` |
| 4. AST montada pelas ações semânticas, com a linha de cada item | `g-v1.y`, `ast.c` |
| 5. Pilha de tabelas de símbolos: iniciar, empilhar escopo, pesquisar do topo para a base, remover escopo | `symble_table.c` |
| 6. Análise semântica (escopo e tipos), parando no primeiro erro | `semantic.c` |
| 7. Geração de código percorrendo a AST e usando a tabela de símbolos para achar a posição das variáveis | `codegen.c` |
| 8. Makefile que gera o executável `g-v1` só com `make` | `Makefile` |

## Como funciona

**Léxico.** Além dos tokens da tabela do enunciado, o scanner guarda o lexema em `yylval`
para o parser. A `INTCONST` é só a sequência de dígitos: o sinal fica com a regra
`UnExpr -> '-' PrimExpr`. Antes a gente colocava o sinal na `INTCONST`, mas aí `n-1` (sem
espaço) virava `n` seguido do número `-1` e dava erro de sintaxe.

**Sintático e AST.** A gramática é a do enunciado. Cada nó da AST tem o tipo do nó, a linha,
o lexema, o primeiro filho e o próximo irmão. Um bloco fica como `BLOCK -> [VAR_SECTION]
COMMAND_LIST`, então as declarações aparecem antes dos comandos, como o enunciado descreve.

**Tabela de símbolos.** Uma pilha de escopos. Cada símbolo guarda nome, tipo, linha da
declaração e o `offset`/tamanho usados na geração de código.

**Semântico.** Percorre a AST da raiz para as folhas, da esquerda para a direita. Todo bloco
com declarações empilha um escopo novo e desempilha no final. Verifica:

- variável declarada antes de usar e sem redeclaração no mesmo escopo;
- atribuição com os dois lados do mesmo tipo;
- `+ - * /`, `||`, `&`, `!` e `-` unário só com `int`;
- relacionais e `== !=` com operandos do mesmo tipo (resultado `int`);
- condição de `se` e `enquanto` do tipo `int`.

**Geração de código.** Faz outro percurso na AST empilhando as tabelas de novo, agora para
calcular onde cada variável fica na memória:

- todas as variáveis ficam em um quadro na pilha, acessadas por `offset($fp)`
  (`int` com 4 bytes, `car` com 1 byte). Um bloco interno usa o espaço logo depois do bloco
  de fora e as variáveis começam com zero quando o bloco começa;
- o resultado de uma expressão fica em `$t0`. Numa operação binária o operando da esquerda
  é empilhado enquanto o da direita é calculado;
- `||` e `&` só calculam o lado direito quando precisa (curto-circuito);
- `se`/`senao`/`enquanto` viram desvios com rótulos numerados;
- `leia`, `escreva` e `novalinha` usam `syscall` (5/12 para ler int/car, 1/11/4 para
  escrever int/car/cadeia). As cadeias ficam na seção `.data`.

## Observações

- Nos erros sintáticos a linha mostrada é a do token em que o Bison percebeu o erro. Por
  exemplo, se falta o `;` no fim da linha 20, o erro aparece na linha 21, no comando seguinte.
- Divisão por zero não é verificada pelo compilador.
- No SPIM, um `leia` de inteiro logo depois de um `leia` de caractere pode ler 0 se o número
  estiver na linha seguinte da entrada (é uma limitação do simulador).
