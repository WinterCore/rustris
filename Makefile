OS := $(shell uname)
CFLAGS = -std=c2x -Wall -Wextra -Iinclude
LDFLAGS = -Iinclude
SRCS = $(wildcard src/*.c)

ifeq ($(OS),Darwin)
	CFLAGS += `pkg-config --cflags glfw3`
	LDFLAGS += `pkg-config --static --libs glfw3`
else
	CFLAGS += `pkg-config --cflags glfw3 gl openal sndfile`
	LDFLAGS += `pkg-config --static --libs glfw3 gl openal sndfile`
endif

.PHONY: clean debug release all

all: release

debug: CFLAGS += -ggdb -O0
debug: Rustris

release: CFLAGS += -DNDEBUG
release: Rustris

Rustris: $(SRCS)
	cc $(CFLAGS) -o Rustris $(SRCS) $(LDFLAGS)

clean:
	rm -rf Rustris
