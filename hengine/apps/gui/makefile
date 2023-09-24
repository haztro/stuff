
APP=game
CC=gcc -g


CFLAGS= -I../../include -std=gnu99 -Wall

OUT=$(APP)
LFLAGS= -L../../ -lhengine -lglfw -pthread -ldl -lrt -lXrandr -lXxf86vm -lX11 -lm

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	
clean:
	-rm $(OUT) $(OBJ)
