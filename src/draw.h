#ifndef DRAW_H
#define DRAW_H
#include <stdbool.h>

typedef enum TetrominoType {
    TETRO_I = 0,
    TETRO_J,
    TETRO_L,
    TETRO_O,
    TETRO_S,
    TETRO_T,
    TETRO_Z,
} TetrominoType;

// All tetrominos have a widthxheight = 4x4
typedef struct Tetromino {
    bool squares[16];
    TetrominoType type;

    unsigned int vertices_count;
    float *vertices;

    unsigned int elements_count;
    unsigned int *elements;
} Tetromino;

static const Tetromino TETROMINOS[7] = {
    {
        .type = TETRO_I,
        .squares = {
            false, false, false, false,
             true,  true,  true,  true,
            false, false, false, false,
            false, false, false, false,
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
        .elements = (unsigned int []) {
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
             true, false, false, false,
             true,  true,  true,  true,
            false, false, false, false,
            false, false, false, false,
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
        .elements = (unsigned int []) {
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
            false, false, false,  true,
             true,  true,  true,  true,
            false, false, false, false,
            false, false, false, false,
        },
    },
    {
        .type = TETRO_O,
        .squares = {
            false, false, false, false,
            false,  true,  true, false,
            false,  true,  true, false,
            false, false, false, false,
        },
    },
    {
        .type = TETRO_S,
        .squares = {
            false, false, false, false,
            false,  true,  true, false,
             true,  true, false, false,
            false, false, false, false,
        },
    },
    {
        .type = TETRO_T,
        .squares = {
            false, false, false, false,
            false,  true, false, false,
             true,  true,  true, false,
            false, false, false, false,
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
        .elements = (unsigned int []) {
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
            false, false, false, false,
             true,  true, false, false,
            false,  true,  true, false,
            false, false, false, false,
        },
    },
};

typedef struct TetrominoGeometry {
    unsigned int vertices_count;
    float *vertices;

    unsigned int elements_count;
    unsigned int *elements_indices;
} TetrominoGeometry;

void genTetrominoVao();
void genTetrominoVertices(
    const Tetromino *tetro,
    unsigned int posX,
    unsigned int posY,
    unsigned int squareWidth,
    unsigned int squareHeight
);

#endif
