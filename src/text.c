#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "text.h"
#include "aids.h"

double character_width = 0.0554;
double character_height = 0.0930;

// x0=0.0664, y0=0.1093, x_stride=0.0665, y_stride=0.1116
#define CI(ch, col, row) (CharacterInfo) { .x = 0.0664 + (col) * 0.0665, .y = 0.1093 + (row) * 0.1116, .character = (ch) }

CharacterInfo character_info[128] = {
    // Row 0: A-M
    CI('A',0,0), CI('B',1,0), CI('C',2,0), CI('D',3,0), CI('E',4,0), CI('F',5,0), CI('G',6,0),
    CI('H',7,0), CI('I',8,0), CI('J',9,0), CI('K',10,0), CI('L',11,0), CI('M',12,0),
    // Row 1: N-Z
    CI('N',0,1), CI('O',1,1), CI('P',2,1), CI('Q',3,1), CI('R',4,1), CI('S',5,1), CI('T',6,1),
    CI('U',7,1), CI('V',8,1), CI('W',9,1), CI('X',10,1), CI('Y',11,1), CI('Z',12,1),
    // Row 2: a-m
    CI('a',0,2), CI('b',1,2), CI('c',2,2), CI('d',3,2), CI('e',4,2), CI('f',5,2), CI('g',6,2),
    CI('h',7,2), CI('i',8,2), CI('j',9,2), CI('k',10,2), CI('l',11,2), CI('m',12,2),
    // Row 3: n-z
    CI('n',0,3), CI('o',1,3), CI('p',2,3), CI('q',3,3), CI('r',4,3), CI('s',5,3), CI('t',6,3),
    CI('u',7,3), CI('v',8,3), CI('w',9,3), CI('x',10,3), CI('y',11,3), CI('z',12,3),
    // Row 4: 0-9, +, -, =
    CI('0',0,4), CI('1',1,4), CI('2',2,4), CI('3',3,4), CI('4',4,4), CI('5',5,4), CI('6',6,4),
    CI('7',7,4), CI('8',8,4), CI('9',9,4), CI('+',10,4), CI('-',11,4), CI('=',12,4),
    // Row 5: ( ) [ ] { } < > / * : # %
    CI('(',0,5), CI(')',1,5), CI('[',2,5), CI(']',3,5), CI('{',4,5), CI('}',5,5), CI('<',6,5),
    CI('>',7,5), CI('/',8,5), CI('*',9,5), CI(':',10,5), CI('#',11,5), CI('%',12,5),
    // Row 6: ! ? . , ' " @ & $
    CI('!',0,6), CI('?',1,6), CI('.',2,6), CI(',',3,6), CI('\'',4,6), CI('"',5,6), CI('@',6,6),
    CI('&',7,6), CI('$',8,6),
};

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
    FILE *file = fopen("assets/font.ppm", "rb");

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

void render_text(Renderer *renderer, Rect screen_rect, char *text) {

}
