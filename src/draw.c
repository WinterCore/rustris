#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG

#include "game.h"
#include "draw.h"
#include "aids.h"

void update_board_dimensions(App *app) {
    float real_width = app->viewport_width;
    float real_height = app->viewport_height;

    // float y_padding = 80.0f;

    float board_width = fmaxf(600.0f, real_width / 2.0f);
    float board_height = fmaxf(800.0f, real_height / 1.2f);

    float height_based_square_width = board_height / (float) app->game->rows;
    float width_based_square_width = board_width / (float) app->game->cols;
    float square_width = fminf(height_based_square_width, width_based_square_width);
    float square_height = square_width;

    // Update board dimensions based on the limiting factor (width or height)
    if (width_based_square_width < height_based_square_width) {
        // If the width is the limiting factor, we can use the full height of the board
        board_height = square_width * (float) app->game->rows;
    } else {
        // If the height is the limiting factor, we can use the full width of the board
        board_width = square_width * (float) app->game->cols;
    }


    float x = (real_width / 2.0f) - (board_width / 2.0f);
    float y = (real_height / 2.0f) - (board_height / 2.0f);

    app->ui_board->x = x;
    app->ui_board->y = y;
    app->ui_board->square_width = square_width;
    app->ui_board->square_height = square_height;
}

uint32_t create_shader_program(unsigned int shaders[], int len) {
    uint32_t shader_program = glCreateProgram();

    for (int i = 0; i < len; i += 1) {
        glAttachShader(shader_program, shaders[i]);
    }

    glLinkProgram(shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (! success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINK_ERROR\n%s\n", infoLog);
        exit(1);
    }

    DEBUG_PRINTF("SHADER PROGRAM CREATED SUCCESSFULLY %s", "");

    return shader_program;
}

uint32_t compile_shader(GLenum type, const char *path) {
    Kyle shader_source = kyle_from_file(path);

    uint32_t shader = glCreateShader(type);

    int shader_lengths[] = { shader_source.length };
    glShaderSource(shader, 1, &shader_source.data, shader_lengths);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        exit(1);
    }

    DEBUG_PRINTF("SHADER COMPILED SUCCESSFULLY %s", "");

    return shader;
}

Renderer create_renderer(Game *game) {
    // Shaders
    uint32_t vertex_shader = compile_shader(GL_VERTEX_SHADER, "./shaders/main.vert");
    uint32_t fragment_shader = compile_shader(GL_FRAGMENT_SHADER, "./shaders/main.frag");

    uint32_t shaders[] = { vertex_shader, fragment_shader };

    uint32_t shader_program = create_shader_program(
        shaders,
        sizeof(shaders) / sizeof(uint32_t)
    );

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /*
     * UI Board cell buffers
     */

    // Generate buffers
    uint32_t board_vbo, board_vao, board_ebo;
    glGenVertexArrays(1, &board_vao);
    glGenBuffers(1, &board_vbo);
    glGenBuffers(1, &board_ebo);

    // Configure UI board vao
    glBindVertexArray(board_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, board_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, board_vbo);

    // X, Y positions of the cell
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);

    // Color index of the cell
    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) (2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind VAO
    glBindVertexArray(0);


    /*
     * Pieces cell buffers
     */

    // Generate buffers
    uint32_t pieces_vao, pieces_vbo, pieces_ebo;
    glGenVertexArrays(1, &pieces_vao);
    glGenBuffers(1, &pieces_vbo);
    glGenBuffers(1, &pieces_ebo);

    // Configure UI board vao
    glBindVertexArray(pieces_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pieces_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, pieces_vbo);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) (2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind VAO
    glBindVertexArray(0);

    Renderer renderer = {
        .shader_program = shader_program,
        .screen_size_loc = glGetUniformLocation(shader_program, "screenSize"),
        .board_vao = board_vao,
        .board_vbo = board_vbo,
        .board_ebo = board_ebo,
        .pieces_vao = pieces_vao,
        .pieces_vbo = pieces_vbo,
        .pieces_ebo = pieces_ebo,
        .pieces_vertex_data = create_vertex_data(
            (
                // For the pieces, we can't share vertices between squares since they can have different colors and positions, so we need to allocate 4 vertices per square (including active piece)
                (game->rows + 1) * (game->cols + 1)
                * 4 // 4 vertices per square (including active piece)
                * 3 // x, y, color_idx
            ) + (
                // Active piece vertices, we need to allocate separately since they won't share vertices with the board (different colors and positions)
                4 // 4 vertices per square
                * 3 // x, y, color_idx
                * 4 // 4 squares in the active piece
            ),
            (
                // For the pieces
                game->rows * game->cols
                * 2 // 2 triangles per square
                * 3 // 3 vertices per triangle
            ) + (
                // Active piece
                6 // 6 elements per square
                * 4 // 4 squares in the active piece
            )
        ),
        .ui_board_vertex_data = create_vertex_data(
            // Here we can share vertices between adjacent squares since they have the same position and color, so we only need (rows + 1) * (cols + 1) vertices
            (game->rows + 1) * (game->cols + 1) * 3, // x, y, color_idx
            game->rows * game->cols
            * 2 // 2 triangles per square
            * 3 // 3 vertices per triangle
        ),
    };

    return renderer;
}

void draw_ui_board(Renderer *renderer) {
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->board_vao);


    // TODO: Maybe should move into generate_ui_board_vertex_data() and only update when necessary
    glBindBuffer(GL_ARRAY_BUFFER, renderer->board_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        renderer->ui_board_vertex_data.vertices_count * sizeof(float),
        renderer->ui_board_vertex_data.vertex_data,
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->board_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        renderer->ui_board_vertex_data.elements_count * sizeof(uint32_t),
        renderer->ui_board_vertex_data.elements_data,
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawElements(GL_TRIANGLES, renderer->ui_board_vertex_data.elements_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void draw_pieces(Renderer *renderer) {
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->pieces_vao);


    // TODO: Maybe should move into generate_ui_pieces_vertex_data() and only update when necessary
    glBindBuffer(GL_ARRAY_BUFFER, renderer->pieces_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        renderer->pieces_vertex_data.vertices_count * sizeof(float),
        renderer->pieces_vertex_data.vertex_data,
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->pieces_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        renderer->pieces_vertex_data.elements_count * sizeof(uint32_t),
        renderer->pieces_vertex_data.elements_data,
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawElements(GL_TRIANGLES, renderer->pieces_vertex_data.elements_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

VertexData create_vertex_data(uint32_t max_vertices_count, uint32_t max_elements_count) {
    float *vertex_data = malloc(max_vertices_count * sizeof(float));
    uint32_t *elements_data = malloc(max_elements_count * sizeof(uint32_t));

    VertexData vertex_data_struct = {
        .max_vertices_count = max_vertices_count,
        .vertices_count = 0,
        .vertex_data = vertex_data,
        .max_elements_count = max_elements_count,
        .elements_count = 0,
        .elements_data = elements_data,
    };

    return vertex_data_struct;
}

void generate_ui_board_vertex_data(Renderer *renderer, Game *game, UIBoard *ui_board) {
    uint32_t rows = game->rows;
    uint32_t cols = game->cols;

    float board_x = ui_board->x;
    float board_y = ui_board->y;
    float square_width = ui_board->square_width;
    float square_height = ui_board->square_height;

    uint32_t vertex_count = (rows + 1) * (cols + 1);

    uint32_t vertices_count = vertex_count * 3; // x, y, color_idx
    uint32_t elements_count = rows * cols * 2 * 3; // 2 triangles per square
    
    float *vertex_data = renderer->ui_board_vertex_data.vertex_data;
    uint32_t *elements_data = renderer->ui_board_vertex_data.elements_data;

    // DEBUG_PRINTF("Generating UI board x=%.2f, y=%.2f, square_width=%.2f, square_height=%.2f", board_x, board_y, square_width, square_height);

    for (uint32_t i = 0, si = 0; i < vertices_count; i += 3, si += 1) {
        uint32_t row = si / (cols + 1);
        uint32_t col = si % (cols + 1);

        vertex_data[i + 0] = board_x + (square_width * col);
        vertex_data[i + 1] = board_y + (square_height * row);
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

    renderer->ui_board_vertex_data.vertices_count = vertices_count;
    renderer->ui_board_vertex_data.elements_count = elements_count;
}

void generate_pieces_vertex_data(Renderer *renderer, Game *game, UIBoard *ui_board) {
    size_t vertices_count = 0;
    size_t elements_count = 0;

    size_t area = game->cols * game->rows;

    for (size_t i = 0; i < area; i += 1) {
        if (game->board[i] == TETRO_EMPTY) {
            continue;
        }

        vertices_count += 4 * 3; // 4 (vertices per square) * 3 (x, y, color)
        elements_count += 6;
    }


    vertices_count += 4 * 3 * 4;
    elements_count += 6 * 4;

    float *vertex_data = renderer->pieces_vertex_data.vertex_data;
    uint32_t *elements_data = renderer->pieces_vertex_data.elements_data;

    uint32_t rows = game->rows;
    uint32_t cols = game->cols;
    float board_x = ui_board->x;
    float board_y = ui_board->y;
    float square_width = ui_board->square_width;
    float square_height = ui_board->square_height;


    size_t vi = 0, ei = 0;

    for (size_t i = 0; i < area; i += 1) {
        size_t x = i % cols;
        size_t y = i / cols;

        size_t ri = (rows - 1 - y) * cols + x;

        if (game->board[ri] == TETRO_EMPTY) {
            continue;
        }


        // top left
        vertex_data[vi + 0] = board_x + (square_width * x);
        vertex_data[vi + 1] = board_y + (square_height * y);
        vertex_data[vi + 2] = game->board[ri];
        
        // top right
        vertex_data[vi + 3] = board_x + (square_width * x) + square_width;
        vertex_data[vi + 4] = board_y + (square_height * y);
        vertex_data[vi + 5] = game->board[ri];
        
        // bottom left
        vertex_data[vi + 6] = board_x + (square_width * x);
        vertex_data[vi + 7] = board_y + (square_height * y) + square_height;
        vertex_data[vi + 8] = game->board[ri];

        // bottom right
        vertex_data[vi + 9] = board_x + (square_width * x) + square_width;
        vertex_data[vi + 10] = board_y + (square_height * y) + square_height;
        vertex_data[vi + 11] = game->board[ri];

        size_t vertex_idx = (vi / 12) * 4;

        elements_data[ei + 0] =  vertex_idx + 0;
        elements_data[ei + 1] =  vertex_idx + 1;
        elements_data[ei + 2] =  vertex_idx + 2;
        elements_data[ei + 3] =  vertex_idx + 1;
        elements_data[ei + 4] =  vertex_idx + 2;
        elements_data[ei + 5] =  vertex_idx + 3;


        vi += 12;
        ei += 6;
    }


    // Active piece
    ActiveTetromino *active_tetromino = &game->active_tetromino;
    Tetromino *tetromino = &active_tetromino->tetromino;

    size_t bx = active_tetromino->x;
    size_t by = active_tetromino->y;

    for (size_t py = 0; py < 4; py += 1) {
        for (size_t px = 0; px < 4; px += 1) {
            if (! tetromino->squares[py * 4 + px]) {
                continue;
            }


            size_t x = bx + px;
            size_t y = rows - (by + py) - 1;


            // top left
            vertex_data[vi + 0] = board_x + (square_width * x);
            vertex_data[vi + 1] = board_y + (square_height * y);
            vertex_data[vi + 2] = tetromino->type;

            // top right
            vertex_data[vi + 3] = board_x + (square_width * x) + square_width;
            vertex_data[vi + 4] = board_y + (square_height * y);
            vertex_data[vi + 5] = tetromino->type;
            
            // bottom left
            vertex_data[vi + 6] = board_x + (square_width * x);
            vertex_data[vi + 7] = board_y + (square_height * y) + square_height;
            vertex_data[vi + 8] = tetromino->type;

            // bottom right
            vertex_data[vi + 9] = board_x + (square_width * x) + square_width;
            vertex_data[vi + 10] = board_y + (square_height * y) + square_height;
            vertex_data[vi + 11] = tetromino->type;

            size_t vertex_idx = (vi / 12) * 4;
            elements_data[ei + 0] =  vertex_idx + 0;
            elements_data[ei + 1] =  vertex_idx + 1;
            elements_data[ei + 2] =  vertex_idx + 2;
            elements_data[ei + 3] =  vertex_idx + 1;
            elements_data[ei + 4] =  vertex_idx + 2;
            elements_data[ei + 5] =  vertex_idx + 3;

            vi += 12;
            ei += 6;
        }
    }

    renderer->pieces_vertex_data.vertices_count = vertices_count;
    renderer->pieces_vertex_data.elements_count = elements_count;
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
