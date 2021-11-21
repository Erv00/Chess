CC=gcc
CFLAGS=-I./include -ggdb -Wall -Wextra -pedantic -Wno-sign-compare -Werror -Wno-unknown-pragmas -O2 -DNO_DBGMLC
LDFLAGS=-lSDL2 -lSDL2_image -lm -lSDL2_ttf

ERRORS=-Wno-error=format-overflow -Wno-error=unused-parameter -Wno-error=stringop-truncation

sources=$(wildcard src/*.c)
objects=$(sources:src/%.c=obj/%.o)

bin: $(objects)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o chess

obj/%.o : src/%.c | obj
	$(CC) -c $(CFLAGS) $(ERRORS) $< -o $@

obj:
	mkdir obj

clean:
	-rm -rf obj
	-rm ./chess ./chess.zip

package:
	zip chess.zip src/* include/* doc/* assets/*
