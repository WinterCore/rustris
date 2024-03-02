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
        .elements = (unsigned int []) {
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
        .elements = (unsigned int []) {
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
        .elements = (unsigned int []) {
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
        .elements = (unsigned int []){
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
