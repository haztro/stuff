
APP=test
CC=gcc -g


CFLAGS= -I../../include -std=gnu99 -Wall -Werror

OUT=$(APP)
LFLAGS= -L../../ -lhengine -lglfw -pthread -ldl -lrt -lXrandr -lXxf86vm -lX11 -lm

$(OUT): *.c
	$(CC) $< $(CFLAGS) $(LFLAGS) -o $@
	
clean:
	rm $(OUT)
