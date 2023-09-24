
CC=gcc
AR=ar

LIBRARIES = -Llib -lglfw -pthread -ldl -lrt -lXrandr -lXxf86vm -lX11 -lm

SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o))) 

CFLAGS = -I ./include -std=gnu99 -Wall -Werror -Wno-unused -g
LFLAGS =  $(LIBRARIES) -shared -g

DYNAMIC = libhengine.so
STATIC = libhengine.a
CFLAGS += -fPIC


all: $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@
	
$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)
	
obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/*/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@
	
obj:
	mkdir obj
	
hengine.res: hengine.rc
	windres $< -O coff -o $@
	
clean:
	rm $(OBJ) $(STATIC) $(DYNAMIC)
  
install_unix: $(STATIC)
	cp $(STATIC) /usr/local/lib/$(STATIC)
 
