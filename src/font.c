#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "font.h"
#include "aids.h"

typedef struct PPMParser {
    long pos;
    uint8_t *buffer;
} PPMParser;

char parser_consume_token(PPMParser *parser) {
    char token = parser->buffer[parser->pos];

    parser->pos += 1;

    return token;
}

void parser_consume_whitespace(PPMParser *parser) {
    char token = parser->buffer[parser->pos];

    if (token == ' ' || token == '\t' || token == '\r' || token == '\n') {
        parser->pos += 1;
        return;
    }

    DEBUG_PRINTF("PPMParser expected whitespace got %c %ld", token, parser->pos);
    exit(EXIT_FAILURE);
}

int parser_consume_decimal(PPMParser *parser) {
    uint8_t *start = &parser->buffer[parser->pos];
    uint8_t *end;

    long decimal = strtol((char *) start, (char **) &end, 10);

    if (start == end) {
        exit(EXIT_FAILURE);
    }

    parser->pos += end - start;
    
    return decimal;
}

FontBitmap load_font() {
    FILE *file = fopen("assets/font.ppm", "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    PPMParser parser = {
        .pos = 0,
        .buffer = malloc(size),
    };

    fread(parser.buffer, sizeof(uint8_t), size, file);

    // PPM Magic number
    assert(parser_consume_token(&parser) == 'P');
    assert(parser_consume_token(&parser) == '6');

    // Whitespace
    parser_consume_whitespace(&parser);

    long width = parser_consume_decimal(&parser);

    // Whitespace
    parser_consume_whitespace(&parser);

    long height = parser_consume_decimal(&parser);

    // Whitespace
    parser_consume_whitespace(&parser);

    long max_color = parser_consume_decimal(&parser);

    // Whitespace
    parser_consume_whitespace(&parser);

    // Data
    FontBitmap bitmap = {
        .width = width,
        .height = height,
        .data = &parser.buffer[parser.pos],
    };

    return bitmap;
}
