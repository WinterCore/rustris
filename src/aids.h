#ifndef AIDS
#define AIDS
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define DEBUG_PRINT(fmt, ...) \
        if (DEBUG_TEST) { \
            fprintf(stderr, "-----DEBUG----> %s:%d:%s(): " fmt "\n\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); \
            fflush(stderr); \
        }

typedef struct Kyle {
    const char *data;
    const size_t length;
} Kyle;

Kyle kyle_from_file(const char *path);

void kyle_destroy(Kyle kyle);
#endif
