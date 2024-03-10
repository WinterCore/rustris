#include "game.h"
#include <stdint.h>
#include <stdlib.h>

Game create_game(uint8_t cols, uint8_t rows) {
    uint32_t area = cols * rows;
    TetrominoType *board = malloc(sizeof(TetrominoType) * area);

    for (uint32_t i = 0; i < area; i += 1) {
        board[i] = TETRO_EMPTY;
    }

    Game game = {
        .cols = cols,
        .rows = rows,
        .board = board,
        .active_tetromino = NULL,
    };

    return game;
}
