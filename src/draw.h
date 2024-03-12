#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>
#include "game.h"

// All tetrominos have a widthxheight = 4x4
typedef struct Tetromino {
    bool squares[16];
    TetrominoType type;

    uint32_t vertices_count;
    float *vertices;

    uint32_t elements_count;
    uint32_t *elements;
} Tetromino;

static const Tetromino TETROMINOS[7] = {
    {
        .type = TETRO_I,
        .squares = {
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 20,
        .vertices = (float []) {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            2.0f, 1.0f,
            2.0f, 0.0f,
            3.0f, 1.0f,
            3.0f, 0.0f,
            4.0f, 1.0f,
            4.0f, 0.0f,
        },
        .elements_count = 24,
        .elements = (uint32_t []) {
            0, 1, 2,
            1, 2, 3,
            2, 3, 4,
            3, 4, 5,
            4, 5, 6,
            5, 6, 7,
            6, 7, 8,
            7, 8, 9,
        },
    },
    {
        .type = TETRO_J,
        .squares = {
            1, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 24,
        .vertices = (float []) {
            0.0f, 2.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            3.0f, 0.0f,
            4.0f, 0.0f,
            4.0f, 1.0f,
            3.0f, 1.0f,
            2.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 2.0f,
        },
        .elements_count = 30,
        .elements = (uint32_t []) {
            0,  1, 11,
            1, 10, 11,
            1,  2, 10,
            2,  3, 10,
            3,  9, 10,
            3,  4,  9,
            4,  8,  9,
            4,  5,  8,
            5,  7,  8,
            5,  6,  7,
        },
    },
    {
        .type = TETRO_L,
        .squares = {
            0, 0, 0, 1,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 24,
        .vertices = (float []) {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            3.0f, 0.0f,
            4.0f, 0.0f,
            4.0f, 1.0f,
            4.0f, 2.0f,
            3.0f, 2.0f,
            3.0f, 1.0f,
            2.0f, 1.0f,
            1.0f, 1.0f,
        },
        .elements_count = 30,
        .elements = (uint32_t []) {
            0,  1, 11,
            1,  2, 11,
            2, 10, 11,
            2,  3, 10,
            3,  9, 10,
            3,  4,  9,
            4,  6,  9,
            4,  5,  6,
            6,  7,  9,
            7,  8,  9,
        },
    },
    {
        .type = TETRO_O,
        .squares = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 18,
        .vertices = (float []) {
            0.0f, 2.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            2.0f, 1.0f,
            2.0f, 2.0f,
            1.0f, 2.0f,
            1.0f, 1.0f,
        },
        .elements_count = 24,
        .elements = (uint32_t []) {
            0, 1, 7,
            1, 7, 8,
            1, 2, 8,
            2, 3, 8,
            6, 7, 8,
            5, 6, 8,
            3, 5, 8,
            3, 4, 5,
        },
    },
    {
        .type = TETRO_S,
        .squares = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 20,
        .vertices = (float []) {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            2.0f, 1.0f,
            3.0f, 1.0f,
            3.0f, 2.0f,
            2.0f, 2.0f,
            1.0f, 2.0f,
            1.0f, 1.0f,
        },
        .elements_count = 24,
        .elements = (uint32_t []) {
            0, 1, 9,
            1, 2, 9,
            2, 4, 9,
            2, 3, 4,
            4, 6, 7,
            4, 5, 6,
            7, 8, 9,
            4, 7, 9,
        },
    },
    {
        .type = TETRO_T,
        .squares = {
            0, 0, 0, 0,
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 20,
        .vertices = (float []) {
            1.0f, 2.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            3.0f, 0.0f,
            3.0f, 1.0f,
            2.0f, 1.0f,
            2.0f, 2.0f
        },
        .elements_count = 24,
        .elements = (uint32_t []) {
            1, 2, 3,
            1, 3, 4,
            1, 4, 8,
            4, 5, 8,
            5, 7, 8,
            5, 6, 7,
            0, 1, 9,
            1, 8, 9,
        },
    },
    {
        .type = TETRO_Z,
        .squares = {
            0, 0, 0, 0,
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
        },
        .vertices_count = 20,
        .vertices = (float []) {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            2.0f, 0.0f,
            3.0f, 0.0f,
            3.0f, 1.0f,
            2.0f, 1.0f,
            2.0f, 2.0f,
            1.0f, 2.0f,
            0.0f, 2.0f,
        },
        .elements_count = 24,
        .elements = (uint32_t []){
            0, 8, 9,
            0, 1, 8,
            1, 7, 8,
            1, 6, 7,
            1, 2, 6,
            2, 3, 6,
            3, 5, 6,
            3, 4, 5,
        },
    },
};

typedef struct UIBoard {
    uint32_t square_width;
    uint32_t square_height;

    uint32_t padding_y;
    uint32_t padding_x;
} UIBoard;

typedef struct UIBoardVertexData {
    uint32_t vertex_data_size;
    float *vertex_data;

    uint32_t elements_size;
    uint32_t *elements_data;
} UIBoardVertexData;

typedef struct App {
    uint32_t viewport_width;
    uint32_t viewport_height;

    UIBoard ui_board;

    Game game;
} App;

void update_board_dimensions(App *app);
UIBoardVertexData generate_ui_board_vertex_data(App *app);

#endif
