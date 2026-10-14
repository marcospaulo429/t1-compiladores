ifeq ($(origin CC),default)
  CC = gcc
endif
TEST_DIR = ../testes_G-v1

# Alvo principal: compila e liga o compilador g-v1 completo
g-v1: g-v1.tab.o lex.yy.o tabela_simbolos.o semantico.o gerador.o
	$(CC) g-v1.tab.o lex.yy.o tabela_simbolos.o semantico.o gerador.o -o g-v1

# Gerador de Codigo MIPS
gerador.o: gerador.c gerador.h tipos.h tabela_simbolos.h
	$(CC) -c gerador.c -o gerador.o

# Analisador Semantico
semantico.o: semantico.c semantico.h tipos.h tabela_simbolos.h
	$(CC) -c semantico.c -o semantico.o

# Modulo da Tabela de Simbolos
tabela_simbolos.o: tabela_simbolos.c tabela_simbolos.h
	$(CC) -c tabela_simbolos.c -o tabela_simbolos.o

# Analisador Sintatico
g-v1.tab.o: g-v1.tab.c tipos.h tabela_simbolos.h semantico.h gerador.h
	$(CC) -c g-v1.tab.c -o g-v1.tab.o

# Gera arquivos do Bison
g-v1.tab.c g-v1.tab.h: g-v1.y tipos.h tabela_simbolos.h
	bison -d g-v1.y

# Analisador Lexico
lex.yy.o: lex.yy.c tipos.h g-v1.tab.h
	$(CC) -c lex.yy.c -o lex.yy.o

# Gera arquivo do Flex
lex.yy.c: g-v1.l g-v1.tab.h tipos.h
	flex g-v1.l

# Testes
test: g-v1
	@echo "\n=== COMPILANDO PROGRAMAS CORRETOS PARA MIPS ==="
	@for arq in $(TEST_DIR)/Corretos/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v1 $$arq || exit 1; \
	done
	@echo "\n=== TESTANDO ERROS LEXICOS ==="
	@for arq in $(TEST_DIR)/ErroLexico/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v1 $$arq || true; \
	done
	@echo "\n=== TESTANDO ERROS SINTATICOS ==="
	@for arq in $(TEST_DIR)/ErroSintatico/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v1 $$arq || true; \
	done
	@echo "\n=== TESTANDO ERROS SEMANTICOS ==="
	@for arq in $(TEST_DIR)/ErroSemantico/*.g; do \
		echo "Compilando: $$arq"; \
		if [ -f "$$arq" ]; then \
			./g-v1 $$arq || true; \
		fi \
	done

clean:
	rm -f *.o g-v1.tab.c g-v1.tab.h lex.yy.c g-v1 $(TEST_DIR)/Corretos/*.asm