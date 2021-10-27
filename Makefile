CC=gcc
CFLAGS=-I./include -ggdb -Wall -Wextra -pedantic
LDFLAGS=-lSDL2 -lSDL2_image -lm
sources=$(wildcard src/*.c)
objects=$(sources:src/%.c=obj/%.o)

bin: $(objects)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o chess

obj/%.o : src/%.c | obj
	$(CC) -c $(CFLAGS) $< -o $@

obj:
	mkdir obj

clean:
	-rm -rf obj
	-rm ./chess ./chess.zip

package:
	zip chess.zip $(sources)