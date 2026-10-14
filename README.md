# Compilador para a Linguagem G-V1 (UFG)

Implementação de um compilador didático para a linguagem **G-V1**, desenvolvido para o Trabalho 1 da disciplina de Compiladores (Instituto de Informática – Universidade Federal de Goiás), sob orientação do Prof. Thierson Couto Rosa.

O repositório é estruturado em diretórios numerados correspondentes a cada seção e etapa de implementação descritas na especificação oficial do trabalho ([Parte1.pdf](Parte1.pdf)).


## Estrutura do Repositório

| Seção no PDF | Diretório | Descrição |
|:------------:|-----------|-----------|
| **Seção 3** | [03-geracao-analisadores-sintatico-lexico/](03-geracao-analisadores-sintatico-lexico/) | Analisadores léxico (Flex) e sintático (Bison) integrados |
| **Seção 4** | [04-geracao-representacao-intermediaria/](04-geracao-representacao-intermediaria/) | Construção da Árvore Sintática Abstrata (AST) durante o parsing |
| **Seção 5** | [05-tabela-simbolos/](05-tabela-simbolos/) | Pilha de tabelas de símbolos com escopos aninhados e testes unitários |
| **Seção 6** | [06-analise-semantica/](06-analise-semantica/) | Analisador semântico: verificação de escopos e checagem de tipos na AST |
| **Seção 7** | [07-geracao-de-codigo/](07-geracao-de-codigo/) | Geração de código objeto em linguagem Assembly MIPS |
| **Seção 8** | [entrega/](entrega/) | **Pasta final para submissão**, contendo estritamente os arquivos exigidos |
| **Testes**   | [testes_G-v1/](testes_G-v1/) | Suíte completa de testes sintáticos, léxicos, semânticos e programas corretos |



## Descrição das Etapas

### 1. Seção 3: Analisadores Léxico e Sintático Integrados
- **Scanner Flex ([03-geracao-analisadores-sintatico-lexico/g-v1.l](03-geracao-analisadores-sintatico-lexico/g-v1.l))**: Reconhece as palavras-chave (`principal`, `int`, `car`, `leia`, `escreva`, `novalinha`, `se`, `entao`, `senao`, `fimse`, `enquanto`), operadores (`+`, `-`, `*`, `/`, `<`, `>`, `<=`, `>=`, `==`, `!=`, `&`, `||`, `!`, `=`), comentários de múltiplas linhas (`/* ... */`), identificadores e literais. Reporta erros léxicos no formato exigido: `ERRO: CARACTERE INVÁLIDO <linha>`, `ERRO: COMENTÁRIO NAO TERMINA <linha>` e `ERRO: CADEIA DE CARACTERES OCUPA MAIS DE UMA LINHA <linha>`.
- **Parser Bison ([03-geracao-analisadores-sintatico-lexico/g-v1.y](03-geracao-analisadores-sintatico-lexico/g-v1.y))**: Implementa a gramática formal LALR de G-V1, manipula o arquivo de entrada via `yyin` e reporta erros sintáticos no formato `ERRO: syntax error linha <linha>`.

### 2. Seção 4: Representação Intermediária (AST)
- Implementa a estrutura compacta da Árvore Sintática Abstrata baseada nas definições dos slides da disciplina:
  - Estrutura `Toperador` e enumeração `TespecieOperador` em [04-geracao-representacao-intermediaria/tipos.h](04-geracao-representacao-intermediaria/tipos.h).
  - Cada nó armazena seu tipo de operador, a linha original no arquivo-fonte, lexema associado e até 3 ponteiros de filhos (`filho1`, `filho2`, `filho3`), suportando comandos de controle como `if-then-else` e expressões binárias/unárias.

### 3. Seção 5: Tabela de Símbolos
- Implementada em [05-tabela-simbolos/tabela_simbolos.h](05-tabela-simbolos/tabela_simbolos.h) e [05-tabela-simbolos/tabela_simbolos.c](05-tabela-simbolos/tabela_simbolos.c) como uma pilha de escopos (tabelas hash com encadeamento de colisões por baldes):
  - `iniciarPilha()`: Inicializa a pilha de escopos.
  - `empilharTabela()`: Cria e empilha um novo escopo.
  - `desempilharTabela()`: Remove o escopo do topo.
  - `pesquisarNome()`: Busca um símbolo do topo até a base da pilha (suportando sombreamento/shadowing entre blocos internos e externos).
  - `pesquisarNoEscopoAtual()` / `inserirSimbolo()`: Verifica e insere variáveis no escopo corrente.
- Inclui suíte de testes unitários em [05-tabela-simbolos/testa_tabela.c](05-tabela-simbolos/testa_tabela.c).

### 4. Seção 6: Analisador Semântico
- Implementado em [06-analise-semantica/semantico.h](06-analise-semantica/semantico.h) e [06-analise-semantica/semantico.c](06-analise-semantica/semantico.c).
- Percorre a AST da esquerda para a direita, gerenciando a pilha de escopos:
  - Bloco com declarações abre um novo escopo e insere os identificadores.
  - Valida uso exclusivo após declaração prévia e proíbe redeclarações no mesmo bloco.
  - Checagem estrita de tipos:
    - Atribuição exige tipos idênticos em ambos os lados.
    - Operações aritméticas (`+`, `-`, `*`, `/`) e unárias (`-`) exigem operandos `int`.
    - Operações lógicas (`||`, `&`, `!`) exigem operandos `int` e produzem `int`.
    - Operações relacionais (`<`, `>`, `<=`, `>=`, `==`, `!=`) exigem operandos do mesmo tipo (`int` com `int` ou `car` com `car`) e produzem `int`.
    - Expressões de controle em `se` e `enquanto` exigem tipo `int`.
  - Ao encontrar erro semântico, emite `ERRO: <mensagem> linha <linha>` e encerra a compilação com código de erro.

### 5. Seção 7: Geração de Código Assembly MIPS
- Implementado em [07-geracao-de-codigo/gerador.h](07-geracao-de-codigo/gerador.h) e [07-geracao-de-codigo/gerador.c](07-geracao-de-codigo/gerador.c).
- Gera código Assembly MIPS compatível com simuladores como SPIM e MARS:
  - Seção `.data`: Aloca cadeias literais com rótulos `str_N`.
  - Seção `.text`: Inicializa o frame pointer (`move $fp, $sp`) no ponto de entrada `main`.
  - Alocação de variáveis na pilha com cálculo de `offset` negativo em relação a `$fp`.
  - Blocos aninhados alocam e desalocam espaço no frame da pilha via `$sp`.
  - Avaliação de expressões com resultado acumulado em `$s0`, usando a pilha (`$sp`) para salvar subexpressões.
  - Suporte completo a chamadas de sistema MIPS (`syscall`):
    - `leia` (inteiro: syscall 5; caractere: syscall 12);
    - `escreva` (inteiro: syscall 1; caractere: syscall 11; cadeia: syscall 4);
    - `novalinha` (syscall 11 com caractere `'\n'`);
    - Encerramento do programa com syscall 10 (exit).


## Pasta de Entrega (`entrega/`)

A pasta [entrega/](entrega/) foi preparada rigorosamente de acordo com as exigências da **Seção 8** do `Parte1.pdf` para ser compactada e submetida na plataforma:

```
entrega/
├── Makefile            # Script de compilação compatível com Linux e GCC 14
├── g-v1.l              # Especificação léxica para Flex
├── g-v1.y              # Especificação sintática/AST para Bison
├── tipos.h             # Definições da AST e nós
├── tabela_simbolos.h   # Cabeçalho da tabela de símbolos
├── tabela_simbolos.c   # Implementação da pilha de escopos
├── semantico.h         # Cabeçalho do analisador semântico
├── semantico.c         # Implementação das regras e checagem semântica
├── gerador.h           # Cabeçalho do gerador de código MIPS
└── gerador.c           # Implementação da geração de código MIPS
```


## Como Compilar e Executar

### Pré-requisitos
- **GCC / G++**: versão 14 (conforme especificado no trabalho);
- **Flex**: versão 2.6.4;
- **Bison**: versão 3.8.2;
- **SPIM** (opcional, para execução do assembly gerado).

### Compilação do Compilador

Basta entrar no diretório `entrega/` (ou `07-geracao-de-codigo/`) e executar `make`:

```bash
cd entrega
make
```

O comando gerará o executável `g-v1`.

### Execução do Compilador

O compilador aceita o arquivo de entrada em G-V1 e, opcionalmente, o nome do arquivo MIPS de saída:

```bash
# Gera automaticamente <arquivo>.asm
./g-v1 arquivo.g

# Ou especificando o arquivo de saída
./g-v1 arquivo.g saida.asm
```

### Execução do Código MIPS no SPIM

```bash
spim -file arquivo.asm
```


## Bateria de Testes (`testes_G-v1/`)

Os testes estão organizados em subpastas dentro de [testes_G-v1/](testes_G-v1/) seguindo a convenção de nomenclatura do projeto:

- `testes_G-v1/Corretos/`: Programas sintática e semanticamente corretos (`Aritmetica.g`, `Caracteres.g`, `Comentarios.g`, `Controle.g`, `CurtoCircuito.g`, `Escopo.g`, `FatorialSimples.g`, `Leitura.g`, `Minimo.g`, `NotaEmConceito.g`, `PA.g`, `Relacionais.g`, `SeqOrdenada.g`, `Soma.g`, `fatorial.g`).
- `testes_G-v1/ErroLexico/`: Testes que exercitam erros léxicos no padrão `<Base>-<DescricaoErro>Lin<N>.g`.
- `testes_G-v1/ErroSintatico/`: Testes que exercitam erros de sintaxe no padrão `<Base>-<DescricaoErro>Lin<N>.g`.
- `testes_G-v1/ErroSemantico/`: Testes que exercitam erros de escopo, tipos e operadores no padrão `<Base>-<DescricaoErro>Lin<N>.g`.

Para rodar todos os testes automatizados a partir da pasta de entrega:

```bash
cd entrega
make test
```
