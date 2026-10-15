CC := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Wpedantic
TARGET := pesquisa

SOURCES := main.c \
	geradorDeArquivos.c \
	ACESSO-SEQUENCIAL-INDEXADO/src/paginacao.c \
	ABP/abp.c \
	ARVORE-B/arvoreb.c \
	ArvoreB_Estrela/src/arvoreB_estrela.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	$(RM) $(TARGET) registros.bin abp.bin
