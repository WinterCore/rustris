#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef enum TetrominoRotation {
    TETRO_R_000,    
    TETRO_R_090,    
    TETRO_R_180,    
    TETRO_R_270,    
    TETRO_R_360,    
} TetrominoRotation;

typedef enum TetrominoType {
    TETRO_I = 0,
    TETRO_J,
    TETRO_L,
    TETRO_O,
    TETRO_S,
    TETRO_T,
    TETRO_Z,

    TETRO_EMPTY = 696969,
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

Game create_game(uint8_t cols, uint8_t rows);

#endif
