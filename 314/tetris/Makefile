.PHONY: clean

OBJ = obj/def.o \
		obj/tetris.o\
		obj/keyboard.o

CFLAGS= -Iinclude -g

all: tetris

tetris: $(OBJ)
	gcc $(CFLAGS) $^ -o bin/$@ -std=gnu99

obj/%.o : src/%.c
	gcc $(CFLAGS) -c $< -o $@ -std=gnu99

clean:
	rm -rf obj/*

