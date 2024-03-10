#include "draw.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
TetrominoGeometry getTetrominoGeometry(TetrominoType type) {
    
}
*/

void update_board_dimensions(App *app) {
    uint32_t real_width = app->viewport_width - app->ui_board.padding_x;
    uint32_t real_height = app->viewport_height - app->ui_board.padding_y;

    uint32_t square_width = real_height / app->game.rows;

    app->ui_board.square_height = square_width;
    app->ui_board.square_width = square_width;
}

UIBoardVertexData generate_ui_board_vertex_data(App *app) {
    uint32_t square_width = app->ui_board.square_width;

    uint32_t rows = app->game.rows;
    uint32_t cols = app->game.cols;
    uint32_t vertex_count =
        // Inner vertices
        (rows - 1) * (cols - 1)
        // Middle border vertices
        + (rows - 1) * 2
        + (cols - 1) * 2
        // 4 corner edges
        + 4;


    uint32_t vertex_data_size = vertex_count * 3; // x, y, color_idx
    float *vertex_data = malloc(vertex_data_size * sizeof(float));

    uint32_t elements_size = rows * cols * 2; // 2 triangles per square
    uint32_t *elements_data = malloc(elements_size * sizeof(uint32_t));

    for (uint32_t i = 0; i < vertex_data_size; i += 3) {
    }


    UIBoardVertexData ui_board_vertex_data = {
        .vertex_data_size = vertex_data_size,
        .vertex_data = vertex_data,
        .elements_data = elements_data,
        .elements_size = elements_size,
    };

    return ui_board_vertex_data;
}
