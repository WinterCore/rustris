#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct FontBitmap {
    long width, height;
    uint8_t *data;
} FontBitmap;

FontBitmap load_font();

#endif
