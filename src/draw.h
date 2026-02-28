#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include "game.h"

typedef enum Color {
    COLOR_CYAN = 0,
    COLOR_BLUE,
    COLOR_ORANGE,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_PURPLE,
    COLOR_RED,
    COLOR_EMPTY,
    COLOR_GHOST,

    COLOR_COUNT,
} Color;

typedef struct UIBoard {
    float square_width;
    float square_height;

    float x;
    float y;

    float border_thickness;
} UIBoard;

typedef struct VertexData {
    // Total allocated capacity (number of vertices the buffer can hold)
    uint32_t max_vertices_count;
    // Number of vertices currently in use
    uint32_t vertices_count;
    // Vertex attribute data (positions, colors, etc.)
    float *vertex_data;

    // Total allocated capacity (number of elements the buffer can hold)
    uint32_t max_elements_count;
    // Number of elements currently in use
    uint32_t elements_count;
    // Element index data for indexed drawing
    uint32_t *elements_data;
} VertexData;

VertexData create_vertex_data(uint32_t vertices_count, uint32_t elements_count);

typedef struct {
    uint32_t shader_program;
    int32_t screen_size_loc;

    uint32_t board_vao, board_vbo, board_ebo;
    uint32_t pieces_vao, pieces_vbo, pieces_ebo;

    VertexData ui_board_vertex_data;
    VertexData pieces_vertex_data;
} Renderer;

typedef struct App {
    uint32_t viewport_width;
    uint32_t viewport_height;

    UIBoard *ui_board;
    Game *game;
    Renderer *renderer;

    float fps;
} App;

void update_board_dimensions(App *app);

Renderer create_renderer(Game *game);

void generate_ui_board_vertex_data(Renderer *renderer, Game *game, UIBoard *ui_board);
void generate_pieces_vertex_data(Renderer *renderer, Game *game, UIBoard *ui_board);

void draw_ui_board(Renderer *renderer);
void draw_pieces(Renderer *renderer);

void calculateFPS(App *app);

#endif
