#include "game.h"
#include <GLFW/glfw3.h>
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
    float real_width = app->viewport_width;
    float real_height = app->viewport_height;

    float square_height = real_height / (float) app->game.rows;
    float square_width = real_width / (float) app->game.cols;

    float gx = remap(
        0.0f, real_width,
        -1.0f, 1.0f,
        0
    );

    float gy = remap(
        0.0f, real_height,
        -1.0f, 1.0f,
        0
    );

    float square_gwidth = remap(
        0.0f, real_width,
        0.0f, 2.0f,
        square_width
    );

    float square_gheight = remap(
        0.0f, real_height,
        0.0f, 2.0f,
        square_height
    );

    app->ui_board.gx = gx;
    app->ui_board.gy = gy;
    app->ui_board.square_gwidth = square_gwidth;
    app->ui_board.square_gheight = square_gheight;
}

VertexData generate_ui_board_vertex_data(App *app) {
    uint32_t rows = app->game.rows;
    uint32_t cols = app->game.cols;

    float gx = app->ui_board.gx;
    float gy = app->ui_board.gy;
    float square_gwidth = app->ui_board.square_gwidth;
    float square_gheight = app->ui_board.square_gheight;

    uint32_t vertex_count = (rows + 1) * (cols + 1);


    // TODO: Remember to deallloc when done 
    uint32_t vertices_count = vertex_count * 3; // x, y, color_idx
    float *vertex_data = malloc(vertices_count * sizeof(float));

    // TODO: Remember to deallloc when done 
    uint32_t elements_count = rows * cols * 2 * 3; // 2 triangles per square
    uint32_t *elements_data = malloc(elements_count * sizeof(uint32_t));


    for (uint32_t i = 0, si = 0; i < vertices_count; i += 3, si += 1) {
        uint32_t row = si / (cols + 1);
        uint32_t col = si % (cols + 1);

        vertex_data[i + 0] = gx + (square_gwidth * col);
        vertex_data[i + 1] = gy + (square_gheight * row);
        vertex_data[i + 2] = TETRO_EMPTY; 
    }

    for (uint32_t i = 0, si = 0; i < elements_count; i += 6, si += 1) {
        int32_t vert_y = si / cols;
        int32_t vert_x = (si + vert_y) % (cols + 1);
        
        int32_t a = (vert_y * (cols + 1)) + vert_x;
        int32_t b = ((vert_y + 1) * (cols + 1)) + vert_x;
        int32_t c = (vert_y * (cols + 1)) + (vert_x + 1);
        int32_t d = ((vert_y + 1) * (cols + 1)) + (vert_x + 1);
        

        // Top left triangle
        elements_data[i + 0] = a;
        elements_data[i + 1] = b;
        elements_data[i + 2] = c;

        // Bottom right triangle
        elements_data[i + 3] = b;
        elements_data[i + 4] = c;
        elements_data[i + 5] = d;
    }

    VertexData ui_board_vertex_data = {
        .vertices_count = vertices_count,
        .vertex_data = vertex_data,
        .elements_data = elements_data,
        .elements_count = elements_count,
    };

    return ui_board_vertex_data;
}

VertexData generate_pieces_vertex_data(App *app) {
    size_t vertices_count = 0;
    size_t elements_count = 0;

    size_t area = app->game.cols * app->game.rows;

    for (size_t i = 0; i < area; i += 1) {
        if (app->game.board[i] == TETRO_EMPTY) {
            continue;
        }

        vertices_count += 4 * 3; // 4 (vertices per square) * 3 (x, y, color)
        elements_count += 6;
    }


    vertices_count += 4 * 3 * 4;
    elements_count += 6 * 4;



    // TODO: Remember to deallloc when done 
    float *vertices = malloc(vertices_count * sizeof(float));

    // TODO: Remember to deallloc when done 
    uint32_t *elements = malloc(elements_count * sizeof(uint32_t));


    uint32_t rows = app->game.rows;
    uint32_t cols = app->game.cols;
    float gx = app->ui_board.gx;
    float gy = app->ui_board.gy;
    float square_gwidth = app->ui_board.square_gwidth;
    float square_gheight = app->ui_board.square_gheight;


    size_t vi = 0, ei = 0;

    for (size_t i = 0; i < area; i += 1) {
        size_t x = i % cols;
        size_t y = i / cols;

        size_t ri = (rows - 1 - y) * cols + x;

        if (app->game.board[ri] == TETRO_EMPTY) {
            continue;
        }


        // top left
        vertices[vi + 0] = gx + (square_gwidth * x);
        vertices[vi + 1] = gy + (square_gheight * y);
        vertices[vi + 2] = app->game.board[ri];
        
        // top right
        vertices[vi + 3] = gx + (square_gwidth * x) + square_gwidth;
        vertices[vi + 4] = gy + (square_gheight * y);
        vertices[vi + 5] = app->game.board[ri];
        
        // bottom left
        vertices[vi + 6] = gx + (square_gwidth * x);
        vertices[vi + 7] = gy + (square_gheight * y) + square_gheight;
        vertices[vi + 8] = app->game.board[ri];

        // bottom right
        vertices[vi + 9] = gx + (square_gwidth * x) + square_gwidth;
        vertices[vi + 10] = gy + (square_gheight * y) + square_gheight;
        vertices[vi + 11] = app->game.board[ri];

        size_t vertex_idx = (vi / 12) * 4;
        elements[ei + 0] =  vertex_idx + 0;
        elements[ei + 1] =  vertex_idx + 1;
        elements[ei + 2] =  vertex_idx + 2;
        elements[ei + 3] =  vertex_idx + 1;
        elements[ei + 4] =  vertex_idx + 2;
        elements[ei + 5] =  vertex_idx + 3;


        vi += 12;
        ei += 6;
    }


    // Active piece
    ActiveTetromino *active_tetromino = &app->game.active_tetromino;
    Tetromino *tetromino = &active_tetromino->tetromino;

    size_t bx = active_tetromino->x;
    size_t by = active_tetromino->y;

    for (size_t py = 0; py < 4; py += 1) {
        for (size_t px = 0; px < 4; px += 1) {
            if (! tetromino->squares[py * 4 + px]) {
                continue;
            }


            size_t x = bx + px;
            size_t y = app->game.rows - (by + py) - 1;



            // top left
            vertices[vi + 0] = gx + (square_gwidth * x);
            vertices[vi + 1] = gy + (square_gheight * y);
            vertices[vi + 2] = tetromino->type;

            // top right
            vertices[vi + 3] = gx + (square_gwidth * x) + square_gwidth;
            vertices[vi + 4] = gy + (square_gheight * y);
            vertices[vi + 5] = tetromino->type;
            
            // bottom left
            vertices[vi + 6] = gx + (square_gwidth * x);
            vertices[vi + 7] = gy + (square_gheight * y) + square_gheight;
            vertices[vi + 8] = tetromino->type;

            // bottom right
            vertices[vi + 9] = gx + (square_gwidth * x) + square_gwidth;
            vertices[vi + 10] = gy + (square_gheight * y) + square_gheight;
            vertices[vi + 11] = tetromino->type;

            size_t vertex_idx = (vi / 12) * 4;
            elements[ei + 0] =  vertex_idx + 0;
            elements[ei + 1] =  vertex_idx + 1;
            elements[ei + 2] =  vertex_idx + 2;
            elements[ei + 3] =  vertex_idx + 1;
            elements[ei + 4] =  vertex_idx + 2;
            elements[ei + 5] =  vertex_idx + 3;

            vi += 12;
            ei += 6;
        }
    }


    VertexData vertex_data = {
        .vertices_count = vertices_count,
        .vertex_data = vertices,
        .elements_data = elements,
        .elements_count = elements_count,
    };

    return vertex_data;
}

float previousTime = 0.0;
int frameCount = 0;

void calculateFPS(App *app) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - previousTime;
    
    frameCount++;
    
    if (deltaTime >= 1.0) {
        app->fps = (float) frameCount / deltaTime;
        
        previousTime = currentTime;
        frameCount = 0;
    }
}
