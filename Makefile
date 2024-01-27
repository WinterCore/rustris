CFLAGS = -std=c11 -Wall -Wextra `pkg-config --cflags glfw3 gl`
LDFLAGS = -Iinclude `pkg-config --static --libs glfw3 gl`

.PHONY: clean debug all executable

all: executable

debug: CFLAGS += -ggdb
debug: executable

release: CFLAGS += -DNDEBUG
release: executable

executable: Rustris
	./Rustris

src/aids.o: src/aids.c src/aids.h
	cc $(CFLAGS) -c -o src/aids.o src/aids.c $(LDFLAGS)

src/glad.o: src/glad.c
	cc $(CFLAGS) -c -o src/glad.o src/glad.c $(LDFLAGS)

clean:
	rm -rf Rustris ./src/*.o

Rustris: src/main.c src/glad.o src/aids.o
	cc $(CFLAGS) -o Rustris src/main.c src/glad.o src/aids.o $(LDFLAGS)
