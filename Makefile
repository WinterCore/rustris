OS := $(shell uname)
CFLAGS = -std=c2x -Wall -Wextra -Iinclude
LDFLAGS = -Iinclude

ifeq ($(OS),Darwin)
	CFLAGS += `pkg-config --cflags glfw3`
	LDFLAGS += `pkg-config --static --libs glfw3`
else
	CFLAGS += `pkg-config --cflags glfw3 gl`
	LDFLAGS += `pkg-config --static --libs glfw3 gl`
endif

.PHONY: clean debug all executable

all: release

debug: CFLAGS += -ggdb -O0
debug: Rustris

release: CFLAGS += -DNDEBUG
release: Rustris

executable: Rustris
	./Rustris

src/aids.o: src/aids.c src/aids.h
	cc $(CFLAGS) -c -o src/aids.o src/aids.c

src/glad.o: src/glad.c
	cc $(CFLAGS) -c -o src/glad.o src/glad.c

src/draw.o: src/draw.c src/draw.h src/game.o src/aids.o
	cc $(CFLAGS) -c -o src/draw.o src/draw.c

src/game.o: src/game.c src/game.h
	cc $(CFLAGS) -c -o src/game.o src/game.c

clean:
	rm -rf Rustris ./src/*.o

Rustris: src/main.c src/glad.o src/aids.o src/draw.o src/game.o
	cc $(CFLAGS) -o Rustris src/main.c src/glad.o src/game.o src/aids.o src/draw.o $(LDFLAGS)
