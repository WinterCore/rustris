#include "draw.h"
#include <stdbool.h>

/*
TetrominoGeometry getTetrominoGeometry(TetrominoType type) {
    
}
*/


void genTetrominoVertices(
    const Tetromino *tetro,
    unsigned int posX,
    unsigned int posY,
    unsigned int squareWidth,
    unsigned int squareHeight
) {
    float vertices[16] = {};

    for (int y = 0; y < 4; y += 1) {
        for (int x = 0; x < 4; x += 1) {
            int i = y * 4 + x;

            if (tetro->squares[i]) {
                
            }
        }
    }
}
