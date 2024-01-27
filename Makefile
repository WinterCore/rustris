CFLAGS = -std=c11 -Wall -Wextra `pkg-config --cflags glfw3`
LDFLAGS = -Iinclude `pkg-config --static --libs glfw3`

.PHONY: clean debug all executable

all: executable

debug: CFLAGS += -ggdb
debug: executable

release: CFLAGS += -DNDEBUG
release: executable

executable: Rustris
	./Rustris

#src/aids.o: src/aids.c src/aids.h
#	cc $(CFLAGS) -c -o src/aids.o $<

clean:
	rm -rf Rustris ./src/*.o

Rustris: src/main.c src/glad.o
	cc $(CFLAGS) -o Rustris src/main.c $(LDFLAGS)
