#include "draw.h"
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
    },
    {
        .type = TETRO_J,
        .squares = {
             true, false, false, false,
             true,  true,  true,  true,
            false, false, false, false,
            false, false, false, false,
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



