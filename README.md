# T1 Compiladores - histórico de desenvolvimento

Essa branch guarda tudo que fizemos até chegar no trabalho final do compilador da G-V1.
A versão que foi entregue está na branch `master`.

Cada um de nós foi fazendo o compilador por etapas, então separamos em duas pastas:

## marcos/

Uma pasta por etapa, na ordem em que fomos estudando a matéria:

| Pasta | O que tem |
|-------|-----------|
| `analise lexica/` | scanner em Flex |
| `analise lexica e sintatica com flex e bison/` | scanner + parser em Bison |
| `AST/` | montagem e impressão da árvore sintática abstrata |
| `tabela de simbolos/` | pilha de tabelas de símbolos (escopos) |
| `analise semantica/` | verificação de escopo e tipos |
| `geracao de codigo/` | geração de assembly MIPS |
| `final/` | versão com Makefile e testes automáticos (`make test`) |

## giordana/

Pastas numeradas pela seção do enunciado (Parte1.pdf):

| Pasta | O que tem |
|-------|-----------|
| `03-geracao-analisadores-sintatico-lexico/` | Flex + Bison |
| `04-geracao-representacao-intermediaria/` | AST |
| `05-tabela-simbolos/` | tabela de símbolos com hash, com testes unitários (`testa_tabela.c`) |
| `06-analise-semantica/` | analisador semântico |
| `07-geracao-de-codigo/` | gerador de código MIPS |
| `entrega/` | cópia da seção 7 preparada para entrega |
| `testes_G-v1/` | programas de teste (corretos e com erro léxico, sintático e semântico) |

## O que foi pro trabalho final

Comparando as duas versões, a gente juntou assim:

- o código do compilador é o de `marcos/final`, porque o gerador de código da `giordana/07` estava
  sobrescrevendo uma variável quando empilhava valores temporários (o fatorial de 5 dava 1, por exemplo);
- a correção da `INTCONST` veio da versão da Giordana: o número não leva mais o sinal, o `-` fica
  com a gramática. Antes `n-1` (sem espaço) dava erro de sintaxe;
- os testes da `testes_G-v1` entraram junto com os nossos na pasta `testes/` da master.
