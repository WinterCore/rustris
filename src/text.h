#ifndef TEXT_H
#define TEXT_H

#include "draw.h"
#include <stdint.h>

typedef struct FontBitmap {
    long width, height;
    uint8_t *data;
} FontBitmap;

typedef struct CharacterInfo {
    char character;
    double x, y;
} CharacterInfo;

typedef struct Rect {
    int x, y;
    int width, height;
} Rect;

extern CharacterInfo character_info[128];
extern double character_width;
extern double character_height;

FontBitmap load_font();


void render_text(
    Renderer *renderer,
    Rect screen_rect,
    char *text
);

#endif
