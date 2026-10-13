CC = gcc-14
TEST_DIR = ../testes_G-v1

# Alvo principal: compila e liga o executavel g-v1
g-v1: g-v1.tab.o lex.yy.o tabela_simbolos.o
	$(CC) g-v1.tab.o lex.yy.o tabela_simbolos.o -o g-v1

# Modulo da Tabela de Simbolos
tabela_simbolos.o: tabela_simbolos.c tabela_simbolos.h
	$(CC) -c tabela_simbolos.c -o tabela_simbolos.o

# Modulo do Analisador Sintatico (depende de tipos.h e tabela_simbolos.h)
g-v1.tab.o: g-v1.tab.c tipos.h tabela_simbolos.h
	$(CC) -c g-v1.tab.c -o g-v1.tab.o

# Gera arquivos do Bison
g-v1.tab.c g-v1.tab.h: g-v1.y tipos.h tabela_simbolos.h
	bison -d g-v1.y

# Modulo do Analisador Lexico
lex.yy.o: lex.yy.c tipos.h g-v1.tab.h
	$(CC) -c lex.yy.c -o lex.yy.o

# Gera arquivo do Flex
lex.yy.c: g-v1.l g-v1.tab.h tipos.h
	flex g-v1.l

# Alvo de testes automatizados
test: g-v1
	@echo "=== TESTANDO PROGRAMAS CORRETOS (AST + Tabela de Simbolos) ==="
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
		echo "\nArquivo: $$arq"; \
		./g-v1 $$arq || true; \
	done

# Limpeza
clean:
	rm -f *.o g-v1.tab.c g-v1.tab.h lex.yy.c g-v1