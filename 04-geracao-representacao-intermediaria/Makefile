ifeq ($(origin CC),default)
  CC = gcc
endif
TEST_DIR = ../testes_G-v1

# Alvo principal: compila e liga o executavel g-v1
g-v1: g-v1.tab.o lex.yy.o
	$(CC) g-v1.tab.o lex.yy.o -o g-v1

g-v1.tab.o: g-v1.tab.c tipos.h
	$(CC) -c g-v1.tab.c -o g-v1.tab.o

g-v1.tab.c g-v1.tab.h: g-v1.y tipos.h
	bison -d g-v1.y

lex.yy.o: lex.yy.c tipos.h g-v1.tab.h
	$(CC) -c lex.yy.c -o lex.yy.o

lex.yy.c: g-v1.l g-v1.tab.h tipos.h
	flex g-v1.l

# Alvo de testes da AST
test: g-v1
	@echo "=== TESTANDO GERACAO DA AST (PROGRAMAS CORRETOS) ==="
	@for arq in $(TEST_DIR)/Corretos/*.g; do \
		echo "\n----------------------------------------"; \
		echo "Arquivo: $$arq"; \
		echo "----------------------------------------"; \
		./g-v1 $$arq || exit 1; \
	done
	@echo "\n=== TESTANDO ERROS LÉXICOS (devem acusar ERRO) ==="
	@for arq in $(TEST_DIR)/ErroLexico/*.g; do \
		echo "Arquivo: $$arq"; \
		./g-v1 $$arq || true; \
	done
	@echo "\n=== TESTANDO ERROS SINTATICOS (devem acusar ERRO) ==="
	@for arq in $(TEST_DIR)/ErroSintatico/*.g; do \
		echo "Arquivo: $$arq"; \
		./g-v1 $$arq || true; \
	done

# Limpeza dos arquivos gerados
clean:
	rm -f *.o g-v1.tab.c g-v1.tab.h lex.yy.c g-v1