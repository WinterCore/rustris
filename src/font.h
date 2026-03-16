#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct FontBitmap {
    long width, height;
    uint8_t *data;
} FontBitmap;

typedef struct CharacterInfo {
    char character;
    double x, y;
} CharacterInfo;

extern CharacterInfo character_info[128];
extern double character_width;
extern double character_height;

FontBitmap load_font();

#endif
