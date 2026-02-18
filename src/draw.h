#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include "game.h"

typedef struct UIBoard {
    float square_gwidth;
    float square_gheight;

    float gx;
    float gy;
} UIBoard;

typedef struct VertexData {
    uint32_t vertices_count;
    float *vertex_data;

    uint32_t elements_count;
    uint32_t *elements_data;
} VertexData;

typedef struct App {
    uint32_t viewport_width;
    uint32_t viewport_height;

    UIBoard ui_board;

    Game game;

    float fps;
} App;

void update_board_dimensions(App *app);

VertexData generate_ui_board_vertex_data(App *app);
VertexData generate_pieces_vertex_data(App *app);
void calculateFPS(App *app);

#endif
