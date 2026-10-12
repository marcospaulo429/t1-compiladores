CC = gcc-14
TEST_DIR = ../testes_G-v1

g-v1: g-v1.tab.o lex.yy.o
	$(CC) g-v1.tab.o lex.yy.o -o g-v1

g-v1.tab.o: g-v1.tab.c
	$(CC) -c g-v1.tab.c -o g-v1.tab.o

g-v1.tab.c g-v1.tab.h: g-v1.y
	bison -d g-v1.y

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c -o lex.yy.o

lex.yy.c: g-v1.l g-v1.tab.h
	flex g-v1.l

test: g-v1
	@echo "=== TESTANDO PROGRAMAS CORRETOS (devem passar sem erros) ==="
	@for arq in $(TEST_DIR)/Corretos/*.g; do \
		echo "Executando: $$arq"; \
		./g-v1 $$arq || exit 1; \
	done
	@echo "\n=== TESTANDO ERROS SINTATICOS (devem acusar ERRO) ==="
	@for arq in $(TEST_DIR)/ErroSintatico/*.g; do \
		echo "Executando: $$arq"; \
		./g-v1 $$arq || true; \
	done

clean:
	rm -f *.o g-v1.tab.c g-v1.tab.h lex.yy.c g-v1