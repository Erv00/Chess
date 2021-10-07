CC=gcc
CFLAGS=-I./include -ggdb
LDFLAGS=-lm
sources=$(wildcard src/*.c)
objects=$(sources:src/%.c=obj/%.o)

run: bin
	./chess

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