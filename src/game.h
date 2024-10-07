#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Point {
    float x;
    float y;
} Point;

typedef enum TetrominoRotation {
    TETRO_R_000 = 0,    
    TETRO_R_090 = 90,
    TETRO_R_180 = 180,
    TETRO_R_270 = 270,
    TETRO_R_360 = 360,
} TetrominoRotation;

typedef enum TetrominoType {
    TETRO_I = 0,
    TETRO_J,
    TETRO_L,
    TETRO_O,
    TETRO_S,
    TETRO_T,
    TETRO_Z,

    TETRO_EMPTY = 6969,
} TetrominoType;

typedef struct ActiveTetromino {
    TetrominoType tetromino_type;
    
    uint8_t x;
    uint8_t y;

    TetrominoRotation rotation;
} ActiveTetromino;

typedef struct Game {
    uint8_t cols;
    uint8_t rows;

    // Array
    TetrominoType *board;

    ActiveTetromino *active_tetromino;
} Game;

// All tetrominos have a widthxheight = 4x4
typedef struct Tetromino {
    bool squares[16];
    TetrominoType type;

    Point origin;
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
        .origin = { .x = 2, .y = -2 },
    },
    {
        .type = TETRO_J,
        .squares = {
            1, 0, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,

            /*
            0 0 0 1
            0 0 0 1
            0 0 0 1
            1 0 0 0
            */
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_L,
        .squares = {
            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_O,
        .squares = {
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 2, .y = -1 },
    },
    {
        .type = TETRO_S,
        .squares = {
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_T,
        .squares = {
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_Z,
        .squares = {
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
};

Game create_game(uint8_t cols, uint8_t rows);
TetrominoType get_next_tetromino();
Tetromino rotate_tetromino(Tetromino *tetromino, TetrominoRotation rotation);

#endif
