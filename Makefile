ifeq ($(origin CC),default)
  CC = gcc
endif
TEST_DIR = ./test

# Alvo principal: compila e liga o compilador g-v2 completo
g-v2: g-v2.tab.o lex.yy.o tabela_simbolos.o semantico.o gerador.o
	$(CC) g-v2.tab.o lex.yy.o tabela_simbolos.o semantico.o gerador.o -o g-v2

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
g-v2.tab.o: g-v2.tab.c tipos.h tabela_simbolos.h semantico.h gerador.h
	$(CC) -c g-v2.tab.c -o g-v2.tab.o

# Gera arquivos do Bison
g-v2.tab.c g-v2.tab.h: g-v2.y tipos.h tabela_simbolos.h
	bison -d g-v2.y

# Analisador Lexico
lex.yy.o: lex.yy.c tipos.h g-v2.tab.h
	$(CC) -c lex.yy.c -o lex.yy.o

# Gera arquivo do Flex
lex.yy.c: g-v2.l g-v2.tab.h tipos.h
	flex g-v2.l

# Testes
test: g-v2
	@echo "\n=== COMPILANDO PROGRAMAS CORRETOS PARA MIPS ==="
	@for arq in $(TEST_DIR)/Corretos/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v2 $$arq || exit 1; \
	done
	@echo "\n=== TESTANDO ERROS LEXICOS ==="
	@for arq in $(TEST_DIR)/ErroLexico/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v2 $$arq || true; \
	done
	@echo "\n=== TESTANDO ERROS SINTATICOS ==="
	@for arq in $(TEST_DIR)/ErroSintatico/*.g; do \
		echo "Compilando: $$arq"; \
		./g-v2 $$arq || true; \
	done
	@echo "\n=== TESTANDO ERROS SEMANTICOS ==="
	@for arq in $(TEST_DIR)/ErroSemantico/*.g; do \
		echo "Compilando: $$arq"; \
		if [ -f "$$arq" ]; then \
			./g-v2 $$arq || true; \
		fi \
	done

# Executa no SPIM os programas corretos que tem arquivo .saida (saida esperada)
test-saida: g-v2
	@for arq in $(TEST_DIR)/Corretos/*.g; do \
		esperado="$${arq%.g}.saida"; \
		if [ -f "$$esperado" ]; then \
			./g-v2 $$arq > /dev/null || exit 1; \
			spim -file "$${arq%.g}.asm" < /dev/null 2>&1 | tail -n +2 | diff -q - "$$esperado" > /dev/null \
				&& echo "OK   $$arq" || { echo "FALHOU $$arq"; exit 1; }; \
		fi \
	done

.PHONY: test test-saida clean

clean:
	rm -f *.o g-v2.tab.c g-v2.tab.h lex.yy.c g-v2 $(TEST_DIR)/Corretos/*.asm