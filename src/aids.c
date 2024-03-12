#include <stdio.h>
#include <stdlib.h>
#include "aids.h"

Kyle kyle_from_file(const char *path) {
    FILE *fd = fopen(path, "rb");

    if (fd == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    size_t length = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    char *buffer = malloc(length + 1);

    if (buffer) {
        fread(buffer, 1, length, fd);
    }

    fclose(fd);

    Kyle fileData = {
        .data = buffer,
        .length = length,
    };

    return fileData;
}

void kyle_destroy(Kyle kyle) {
    free((void *) kyle.data);
}


float clamp(float min, float max, float v) {
  const float t = v < min ? min : v;
  return t > max ? max : t;
}

float lerp(float a, float b, float v) {
    return a + v * (b - a);
}

float remap(float source_a, float source_b, float dest_a, float dest_b, float value) {
    return dest_a + (value - source_a) * (dest_b - dest_a) / (source_b - source_a);
}
