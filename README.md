# Trabalho 2 - Compilador para a linguagem G-V2

**Alunos:** Marcos Paulo Caetano e Giordana de Farias Franco Bueno Bucci

Disciplina: Compiladores - Instituto de Informática (UFG), 2026-1,
Prof. Thierson Couto Rosa

Repositório: <https://github.com/marcospaulo429/t1-compiladores/tree/entrega_t2>

Compilador para a linguagem G-V2 feito em C com Flex e Bison. G-V2 estende a G-V1 do
trabalho 1 com **variáveis globais**, **funções** e o tipo **vetor** (de `int` e de `car`).
O compilador lê um programa `.g`, faz as análises léxica, sintática e semântica e, se não
houver erro, gera código assembly MIPS (SPIM/MARS). O enunciado está em `docs/trabalho2.pdf`.

## Organização

```
.
├── Makefile              # compilação e testes
├── README.md
├── g-v2.l                # analisador léxico (flex)
├── g-v2.y                # analisador sintático + AST (bison) e main
├── tipos.h               # nós da AST (Toperador)
├── tabela_simbolos.c/.h  # pilha de tabelas de símbolos
├── semantico.c/.h        # analisador semântico
├── gerador.c/.h          # gerador de código MIPS
└── test/                 # Corretos, ErroLexico, ErroSintatico, ErroSemantico
```

## Como compilar e rodar

```bash
make                          # gera o executável g-v2
./g-v2 programa.g             # gera programa.asm
./g-v2 programa.g saida.asm   # ou escolhe o arquivo de saída
spim -file programa.asm       # executa o código gerado
make test                     # roda todos os programas de teste
make test-saida               # roda no SPIM os corretos que têm .saida e compara
```

Se houver erro, o compilador imprime `ERRO: ... linha N`, não gera o `.asm` e termina com
código 1.

## O que mudou do T1 para o T2

| Item | O que foi feito | Onde |
|---|---|---|
| Léxico | novos tokens `global`, `funcao`, `retorne`, `[` e `]` | `g-v2.l` |
| Sintático | seção `global [...]`, seção `funcao [...]`, parâmetros (inclusive `v[]`), vetores `x[n]`, `retorne`, índice em expressões, chamadas, declarações de bloco em `[...]` | `g-v2.y` |
| AST | nós para lista de funções, função (nome, parâmetros, tipo, bloco), parâmetro, vetor, índice, chamada e `retorne` | `tipos.h` |
| Tabela de símbolos | `inserirFuncao`, `inserirVariavel`, `inserirParametro`; entradas com categoria, escopo, posição e tamanho; função guarda a lista de parâmetros | `tabela_simbolos.c` |
| Semântico | escopo global, função na tabela antes do corpo (recursão), parâmetros no escopo do bloco mais externo da função, checagem de chamadas, índices, vetores e `retorne` | `semantico.c` |
| Geração de código | registro de ativação, sequência de chamada/retorno, globais por `$s1`, vetores por endereço | `gerador.c` |

## Como funciona

**Semântica.** Variáveis globais ficam no escopo 0, junto com os nomes das funções. Cada função
cria uma tabela com seus parâmetros; as variáveis do bloco mais externo entram nessa mesma
tabela, então uma variável local não pode ter o nome de um parâmetro (em bloco aninhado pode).
Uma chamada exige o mesmo número de argumentos e o mesmo tipo em cada posição; um vetor só
casa com parâmetro vetor do mesmo tipo básico, e um elemento de vetor casa com parâmetro simples.
Todo vetor é tratado como ponteiro (enunciado, p.9): a atribuição `y = x` entre vetores copia o
endereço de `x` para `y` e só exige que os dois tenham o mesmo tipo básico.

**Geração de código.**
- Chamada: o chamador empilha `$fp` e os argumentos na ordem inversa e faz `jal f_<nome>`; o
  chamado empilha `$ra`, faz `$fp` apontar para ele e reserva as locais. O resultado fica em `$s0`.
  O parâmetro `i` fica em `4*i($fp)` e a local de posição `p` em `-4*p($fp)`.
- Globais: `$s1` aponta para o início delas; a posição `p` fica em `-(p-1)*4($s1)`.
- Um vetor declarado `v[n]` na posição `p` ocupa `n+1` posições: a posição `p` guarda o ponteiro
  (endereço do elemento 0, preenchido na alocação) e `p+1 .. p+n` guardam os elementos. `v[i]` fica
  em ponteiro `- 4*i`. Passar um vetor como argumento passa esse endereço (por referência), e um
  parâmetro vetor é usado da mesma forma.
- `&` e `||` são lógicos, como em C: qualquer valor diferente de zero é verdadeiro e o resultado é 0 ou 1.
- O bloco `principal` tem um frame próprio, como as funções, e `retorne` nele encerra o programa.

## Observações

- Nos erros sintáticos e semânticos a linha informada é a do token em que o analisador estava ao
  detectar o problema (pode ser a linha seguinte à do erro).
- Variáveis não são inicializadas: ler uma variável antes de atribuir devolve lixo da pilha.
- `||` e `&` avaliam os dois operandos (sem curto-circuito), como no T1.
- Todas as mensagens de erro (léxico, sintático e semântico) vão para a saída padrão.
- INTCONST sem sinal no léxico: a Tabela 1 permite sinal, mas `[+-]?[0-9]+` faria `a-1` virar
  `a` seguido de `-1` (erro sintático em expressão válida). `-5` funciona pelo menos unário;
  `+5` não é aceito, pois a gramática não tem `+` unário.
- Os testes `.txt` em `test/` (fornecidos junto ao enunciado) não são rodados pelo `make test`.
  Alguns dos que estão em `Corretos/` têm erro semântico real (ex.: `i` não declarado, vetor `car`
  passado para `int[]`), e o compilador acusa esse primeiro erro. Ver `docs/revisao_trabalho2.md`.
- Divisão por zero não é verificada.
