#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>
#include "game.h"

typedef struct UIBoard {
    float square_gwidth;
    float square_gheight;

    float gx;
    float gy;
} UIBoard;

typedef struct VertexMeta {
    uint32_t vertices_count;
    uint32_t elements_count;
} VertexMeta;

typedef struct App {
    uint32_t viewport_width;
    uint32_t viewport_height;

    UIBoard ui_board;

    Game game;

    float fps;
} App;

void update_board_dimensions(App *app);

VertexMeta ui_generate_and_copy_board_vertex_buffer_data(App *app, int32_t vao, int32_t vbo, int32_t ebo);
VertexMeta ui_generate_and_copy_pieces_vertex_buffer_data(App *app, int32_t vao, int32_t vbo, int32_t ebo);
void calculateFPS(App *app);

#endif
