# Trabalho 2 - Compilador para a linguagem G-V2

**Alunos:** Marcos Paulo Caetano e Giordana de Farias Franco Bueno Bucci

Disciplina: Compiladores - Instituto de Informática (UFG), 2026-1,
Prof. Thierson Couto Rosa

Repositório: <https://github.com/marcospaulo429/t1-compiladores/tree/giordana/entrega>

Compilador para a linguagem G-V2 feito em C com Flex e Bison. G-V2 estende a G-V1 do
trabalho 1 com **variáveis globais**, **funções** e o tipo **vetor** (de `int` e de `car`).
O compilador lê um programa `.g`, faz as análises léxica, sintática e semântica e, se não
houver erro, gera código assembly MIPS (SPIM/MARS). O enunciado está em `slides/Parte2.pdf`.

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
Atribuição entre vetores copia o endereço, por isso o destino precisa ser um parâmetro vetor
(um vetor declarado com tamanho fixo não pode ser reatribuído).

**Geração de código.**
- Chamada: o chamador empilha `$fp` e os argumentos na ordem inversa e faz `jal f_<nome>`; o
  chamado empilha `$ra`, faz `$fp` apontar para ele e reserva as locais. O resultado fica em `$s0`.
  O parâmetro `i` fica em `4*i($fp)` e a local de posição `p` em `-4*p($fp)`.
- Globais: `$s1` aponta para o início delas; a posição `p` fica em `-(p-1)*4($s1)`.
- Um vetor de `n` elementos ocupa `n` posições; `v[i]` fica em endereço-base `- 4*i`. Passar um
  vetor como argumento passa o endereço do primeiro elemento (por referência).
- O bloco `principal` tem um frame próprio, como as funções, e `retorne` nele encerra o programa.

## Observações

- Nos erros sintáticos e semânticos a linha informada é a do token em que o analisador estava ao
  detectar o problema (pode ser a linha seguinte à do erro).
- Variáveis não são inicializadas: ler uma variável antes de atribuir devolve lixo da pilha.
- `||` e `&` avaliam os dois operandos (sem curto-circuito), como no T1.
- Divisão por zero não é verificada.
