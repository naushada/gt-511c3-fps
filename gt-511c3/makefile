BIN = UART0 
CC = arm-linux-gcc

OBJ_DIR = ../obj
CFLAGS = -g -c -D__DEBUG__

INC = -I./gt-511c3/inc -I./pi3-uart/inc

SRC = $(shell find . -type f -name '*.c')

#OBJ = $(SRC:.c=.o)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(SRC) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@


clean:
	rm -fr ./gt-511c3/src/*.o
	rm -fr ./pi3-uart/src/*.o
	rm -fr $(BIN)
