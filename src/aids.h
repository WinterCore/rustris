#ifndef AIDS_H
#define AIDS_H
#include <stdio.h>
#define PI 3.14159265359

#define DEBUG_PRINT_ENABLED 1
#define DEBUG_PRINTF(...) \
    do { if (DEBUG_PRINT_ENABLED) fprintf(stderr, __VA_ARGS__); } while (0)

#define UNIMPLEMENTED \
    fprintf(stderr, "UNIMPLEMENTED"); \
    exit(EXIT_FAILURE); \

#define UNREACHABLE \
    fprintf(stderr, "UNREACHABLE"); \
    exit(EXIT_FAILURE); \

#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
    _a > _b ? _b : _a; })


typedef struct Kyle {
    const char *data;
    const size_t length;
} Kyle;

Kyle kyle_from_file(const char *path);

void kyle_destroy(Kyle kyle);

float clamp(float min, float max, float v);
float lerp(float a, float b, float v);
float remap(float source_a, float source_b, float dest_a, float dest_b, float value);

#endif
