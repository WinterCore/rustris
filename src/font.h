#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct FontBitmap {
    long width, height;
    uint8_t *data;
} FontBitmap;

typedef struct CharacterInfo {
    char character;
    int x, y, width, height;
} CharacterInfo;

extern CharacterInfo character_info[128];

FontBitmap load_font();

#endif
