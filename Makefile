OS := $(shell uname)
CFLAGS = -std=c11 -Wall -Wextra
LDFLAGS = -Iinclude

ifeq ($(OS),Darwin)
	CFLAGS += `pkg-config --cflags glfw3`
	LDFLAGS += `pkg-config --static --libs glfw3`
else
	CFLAGS += `pkg-config --cflags glfw3 gl`
	LDFLAGS += `pkg-config --static --libs glfw3 gl`
endif

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
