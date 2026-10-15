CC = gcc
# O -I avisa ao compilador onde procurar os arquivos .h
CFLAGS = -Wall -I./headers -I./gerador -I.

SRC = main.c src/abp.c src/arvoreB_estrela.c src/arvoreb.c gerador/geradorDeArquivos.c
OBJ = $(SRC:.c=.o)
EXEC = pesquisa

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC) *.bin