ifeq ($(origin CC),default)
  CC = gcc
endif
CFLAGS = -Wall -Wextra -std=c99
TARGET = testa_tabela

# Alvo padrão: compila e gera o executável de teste
all: $(TARGET)

# Ligação dos objetos
$(TARGET): testa_tabela.o tabela_simbolos.o
	$(CC) $(CFLAGS) testa_tabela.o tabela_simbolos.o -o $(TARGET)

# Compilação do teste unitário
testa_tabela.o: testa_tabela.c tabela_simbolos.h
	$(CC) $(CFLAGS) -c testa_tabela.c -o testa_tabela.o

# Compilação da estrutura da tabela de símbolos
tabela_simbolos.o: tabela_simbolos.c tabela_simbolos.h
	$(CC) $(CFLAGS) -c tabela_simbolos.c -o tabela_simbolos.o

# Compila e executa o teste diretamente
test: $(TARGET)
	@echo "Executando teste unitário da Tabela de Símbolos..."
	@./$(TARGET)

# Limpeza dos binários gerados
clean:
	rm -f *.o $(TARGET)