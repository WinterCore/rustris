#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG

#include "draw.h"
#include "aids.h"
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
    DEBUG_PRINT("HERERERERERERE %s\n", "world");
    uint32_t square_width = app->ui_board.square_width;

    uint32_t rows = app->game.rows;
    uint32_t cols = app->game.cols;
    /*
    uint32_t vertex_count =
        // Inner vertices
        (rows - 1) * (cols - 1)
        // Middle border vertices
        + (rows - 1) * 2
        + (cols - 1) * 2
        // 4 corner edges
        + 4;
    */

    uint32_t vertex_count = (rows + 1) * (cols + 1);


    uint32_t vertex_data_size = vertex_count * 3; // x, y, color_idx
    float *vertex_data = malloc(vertex_data_size * sizeof(float));

    uint32_t elements_size = rows * cols * 2 * 3; // 2 triangles per square
    uint32_t *elements_data = malloc(elements_size * sizeof(uint32_t));

    float board_gx = remap(
        0.0f, (float) app->viewport_width,
        -1.0f, 1.0f,
        (app->viewport_width - (square_width * cols)) / 2.0f
    );

    float board_gy = remap(
        0.0f, (float) app->viewport_height,
        1.0f, -1.0f,
        app->ui_board.padding_y
    );

    float square_gwidth = remap(
        0.0f, (float) app->viewport_width,
        0.0f, 2.0f,
        square_width
    );

    DEBUG_PRINT("--- vs=%u, es=%u", vertex_data_size, elements_size);

    for (uint32_t i = 0, si = 0; i < vertex_data_size; i += 3, si += 1) {
        uint32_t row = si / (cols + 1);
        uint32_t col = si % (cols + 1);

        vertex_data[i + 0] = board_gx + (square_gwidth * col);
        vertex_data[i + 1] = board_gy - (square_gwidth * row);
        vertex_data[i + 2] = 0; 

        DEBUG_PRINT("Vertex x=%f, y=%f\n", vertex_data[i + 0], vertex_data[i + 1]);
    }

    for (uint32_t i = 0, si = 0; i < elements_size; i += 6, si += 1) {
        int32_t vert_y = si / cols;
        int32_t vert_x = (si + vert_y) % (cols + 1);
        
        int32_t a = (vert_y * (cols + 1)) + vert_x;
        int32_t b = ((vert_y + 1) * (cols + 1)) + vert_x;
        int32_t c = (vert_y * (cols + 1)) + (vert_x + 1);
        int32_t d = ((vert_y + 1) * (cols + 1)) + (vert_x + 1);
        
        DEBUG_PRINT("Vertex a=%u, b=%u, c=%u, d=%u\n", a, b, c, d);

        // Top left triangle
        elements_data[i + 0] = a;
        elements_data[i + 1] = b;
        elements_data[i + 2] = c;

        // Bottom right triangle
        elements_data[i + 3] = b;
        elements_data[i + 4] = c;
        elements_data[i + 5] = d;
    }

    UIBoardVertexData ui_board_vertex_data = {
        .vertex_data_size = vertex_data_size,
        .vertex_data = vertex_data,
        .elements_data = elements_data,
        .elements_size = elements_size,
    };

    return ui_board_vertex_data;
}
