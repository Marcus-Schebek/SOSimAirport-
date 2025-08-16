CC = gcc
CFLAGS = -Wall -Iinclude -pthread
SRC = src/aviao.c src/pistas.c src/portoes.c src/torre.c src/monitoramento.c src/relatorio.c main.c
OBJ = $(SRC:.c=.o)
TARGET = controle_trafego

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
