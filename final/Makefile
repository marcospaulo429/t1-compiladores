# Compilador da linguagem G-V1
#
#   make         gera o executavel g-v1
#   make test    roda os testes (precisa do simulador spim)
#   make clean   remove os arquivos gerados

CC      = gcc
CFLAGS  = -Wall -Wno-unused-function -g
FLEX    = flex
BISON   = bison

OBJS = g-v1.tab.o lex.yy.o ast.o symble_table.o semantic.o codegen.o

g-v1: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# analisador lexico (flex) e sintatico (bison)
lex.yy.c: g-v1.l g-v1.tab.h
	$(FLEX) g-v1.l

g-v1.tab.c g-v1.tab.h: g-v1.y
	$(BISON) -d g-v1.y

# dependencias dos demais arquivos
g-v1.tab.o: g-v1.tab.c ast.h semantic.h codegen.h symble_table.h
lex.yy.o: lex.yy.c g-v1.tab.h
ast.o: ast.c ast.h
symble_table.o: symble_table.c symble_table.h ast.h
semantic.o: semantic.c semantic.h symble_table.h ast.h
codegen.o: codegen.c codegen.h symble_table.h ast.h

test: g-v1
	./rodar_testes.sh

clean:
	rm -f g-v1 $(OBJS) g-v1.tab.c g-v1.tab.h lex.yy.c
	rm -f testes/ok/*.s testes/erro/*.s

.PHONY: test clean
