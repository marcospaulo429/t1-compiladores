# Trabalho 1 - Compilador para a linguagem G-V1

**Alunos:** Marcos Paulo Caetano e Giordana de Farias Franco Bueno Bucci

Disciplina: Compiladores - Instituto de Informática (UFG), 2026-2,
Prof. Thierson Couto Rosa

Repositório: <https://github.com/marcospaulo429/t1-compiladores/giordana/entrega>

Compilador para a linguagem G-V1 feito em C com Flex e Bison. Ele lê um programa `.g`,
faz a análise léxica, sintática e semântica e, se não tiver erro, gera o código em assembly
MIPS, que pode ser executado no SPIM ou no MARS. O enunciado está em `Parte1.pdf` no
repositório.

## Organização

```
.
├── Makefile              # automação de compilação e suíte de testes
├── README.md             # documentação do projeto
├── g-v1.l                # analisador léxico (flex)
├── g-v1.y                # analisador sintático + construção AST (bison) e main
├── gerador.c             # gerador de codigo MIPS
├── gerador.h
├── semantico.c           # analisador semantico
├── semantico.h
├── tabela_simbolos.c     # pilha de tabela de simbolos
├── tabela_simbolos.h
├── tipos.h
├── test                  # testes enviado pelo professor + próprios
│   ├── Corretos
│   ├── ErroLexico
│   ├── ErroSemantico
│   └── ErroSintatico
```

## Como compilar e rodar

### 1. Gerar o executável `g-v1`
```bash
make
```

### 2. Gerar o código Assembly MIPS
```bash
# Gera automaticamente o arquivo <nome>.asm no mesmo diretório
./g-v1 programa.g

# Ou especificando manualmente o arquivo de saída
./g-v1 programa.g saida.asm
```

### 3. Executar o código MIPS gerado
```bash
spim -file programa.asm
``` 

Se o programa tiver erro, o compilador mostra uma linha começando com `ERRO:` e o número da linha, não gera o `.asm` e termina com código de saída 1.

## Testes

Para executar toda a bateria de testes automatizados (programas corretos e todas as categorias de erros):
```bash
make test
```


## O que o enunciado pede e onde está

| Item do enunciado | Onde está |
|---|---|
| 3.1 Analisador léxico com Flex | `g-v1.l` |
| Reconhecimento de identificadores, números inteiros, caracteres e cadeias | `g-v1.l` |
| Comentários de múltiplas linhas `/* ... */` com erro caso não terminem | `g-v1.l` (estado exclusivo `COMENTARIO`) |
| Mensagens `ERRO: CARACTERE INVÁLIDO`, `COMENTÁRIO NAO TERMINA` e `CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA` + linha | `g-v1.l` |
| 3.2 Analisador sintático com Bison (`extern yylineno`, `extern yytext`, `extern yylex`, `extern FILE *yyin`) | `g-v1.y` |
| `yyerror` exibindo `ERRO: syntax error linha <linha>` | `g-v1.y` |
| Função `main` manipulando `argv[1]` através de `yyin` e fechando o ficheiro | `g-v1.y` |
| 4. Árvore Sintática Abstrata (AST) compacta com registo da linha de cada nó | `tipos.h`, `g-v1.y` |
| 5. Pilha de tabelas de símbolos (iniciar, empilhar, pesquisar topo para a base, pesquisar no topo, desempilhar) | `tabela_simbolos.c`, `tabela_simbolos.h` |
| 6. Analisador semântico: resolução de escopo e checagem rigorosa de tipos, parando no primeiro erro | `semantico.c`, `semantico.h` |
| 7. Geração de código assembly MIPS percorrendo a AST e calculando deslocamentos na pilha | `gerador.c`, `gerador.h` |
| 8. Makefile que compila e liga todos os módulos com o comando `make` | `Makefile` |

## Como funciona

**Léxico.** O analisador reconhece as palavras reservadas, operadores aritméticos, relacionais e lógicos, delimitadores, literais numéricos, caracteres e cadeias. Comentários de múltiplas linhas são processados através do estado `%x COMENTARIO`. Erros de caracteres inválidos, comentários não fechados e cadeias de caracteres não terminadas na linha produzem saída padronizada e encerram a execução de imediato.

**Sintático e AST.** A gramática livre de contexto LALR é processada pelo Bison. Durante a análise sintática, as ações acopladas às produções instanciam os nós da AST através da função `criaOperador`. Cada nó (`Toperador`) contém o tipo de operação (`TespecieOperador`), o número da linha original, o lexema associado e até três ponteiros de subárvores (`filho1`, `filho2`, `filho3`), permitindo representar estruturas como `se-entao-senao` e operações binárias.

**Tabela de Símbolos.** Implementada como uma pilha de tabelas hash com tratamento de colisões por encadeamento nos baldes (`TAM_HASH 211`) e função hash `djb2`. Permite o aninhamento dinâmico de blocos, sombreamento (*shadowing*) de variáveis homônimas em escopos internos e busca progressiva do topo até à base da pilha. A estrutura armazena o nome, tipo (`int`/`car`), linha de declaração e o deslocamento de memória (`offset`) para a geração de código.

**Semântico.** Realiza um percurso na AST da esquerda para a direita a partir da raiz. Ao encontrar declarações num bloco, empilha uma tabela na pilha de escopos e insere os identificadores, acusando erro em caso de duplicidade no mesmo bloco. Em seguida, percorre os comandos e expressões validando: 
- Uso estrito de variáveis previamente declaradas
- Atribuição entre variáveis e expressões de tipos rigorosamente iguais 
- Operadores aritméticos (`+`, `-`, `*`, `/`, `-` unário) aplicados exclusivamente a operandos `int`
- Operadores lógicos (`||`, `&`, `!`) operando sobre `int` e produzindo resultado `int`
- Operadores relacionais (`==`, `!=`, `<`, `>`, `<=`, `>=`) exigindo operandos de mesmo tipo (`int` com `int` ou `car` com `car`) e resultando em `int`
- Expressões condicionais de `se` e `enquanto` obrigatoriamente do tipo `int`

Ao detectar a primeira irregularidade, imprime a mensagem de erro acompanhada da linha e aborta com `exit(1)`

**Geração de Código.** Efetua um percurso na AST gerando instruções para uma máquina de pilha com acumulador simulada sobre a arquitetura MIPS:
- O registrador `$s0` atua como o acumulador de expressões;
- Nas operações binárias, o valor do operando esquerdo é calculado em `$s0` e colocado na pilha (`sw $s0, 0($sp); addiu $sp, $sp, -4`), o operando direito é avaliado e o valor da pilha é descarregado em `$t1` para a operação final;
- O registrador `$fp` (*frame pointer*) marca o início do quadro de dados no ponto de entrada `main` (`move $fp, $sp`). Cada variável é acedida através de deslocamento negativo relativo a `$fp` (`-%d($fp)`);
- Blocos aninhados que possuem variáveis reservam espaço adicional na pilha deslocando `$sp` e desalocam ao término do bloco;
- Comandos condicionais (`se`, `se-senao`) e de repetição (`enquanto`) utilizam desvios com rótulos gerados de forma sequencial e unívoca;
- Chamadas de sistema (`syscall`) do SPIM/MARS implementam `leia` (5 para `int`, 12 para `car`), `escreva` (1 para `int`, 11 para `car`, 4 para cadeias literais alocadas em `.data`) e `novalinha` (11 com código ASCII 10). O programa é finalizado com `syscall` 10

## Observações

- Nos erros sintáticos, a linha assinalada corresponde à do token no qual o analisador detectou a impossibilidade de continuidade da derivação sintática. Por exemplo, se falta o `;` no fim da linha 20, o erro aparece na linha 21, no comando seguinte.
- A pasta de testes (`test/`) encontra-se no mesmo diretório dos ficheiros-fonte, permitindo a execução imediata do comando `make test`
- Divisão por zero não é verificada pelo compilador.
- O simulador SPIM pode reter carateres de quebra de linha no buffer do terminal entre operações sequenciais de leitura de carateres e inteiros; a execução nos simuladores gráficos como o MARS não apresenta essa particularidade de buffering.