#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define DEBUG

#include "./game.h"
#include "./aids.h"

Game create_game(uint8_t cols, uint8_t rows) {
    uint32_t area = cols * rows;
    TetrominoType *board = malloc(sizeof(TetrominoType) * area);

    for (uint32_t i = 0; i < area; i += 1) {
        board[i] = TETRO_EMPTY;
    }

    // TODO: Improve this
    srand(time(NULL));

    Game game = {
        .cols = cols,
        .rows = rows,
        .board = board,
        .active_tetromino = NULL,
    };


    return game;
}



TetrominoType get_next_tetromino() {
    static TetrominoType types[] = {
        TETRO_I,
        TETRO_J,
        TETRO_L,
        TETRO_O,
        TETRO_S,
        TETRO_T,
        TETRO_Z,
    };

    size_t len = sizeof(types) / sizeof(types[0]);

    return types[rand() % len];
}

float degrees_to_radians(float degrees) {
    return degrees * PI / 180;
}

Tetromino rotate_tetromino(Tetromino *tetromino, TetrominoRotation rotation) {
    Tetromino rotated_tetro = {
        .origin = tetromino->origin,
        .type = tetromino->type,
        .squares = {0},
    };

    if (rotation == TETRO_R_000 || rotation == TETRO_R_360) {
        for (size_t i = 0; i < 16; i += 1) {
            rotated_tetro.squares[i] = tetromino->squares[i];
        }

        return rotated_tetro;
    }

    float rads = degrees_to_radians((float) rotation);
    float s = sin(-rads), c = cos(-rads);

    for (size_t y = 0; y < 4; y += 1) {
        for (size_t x = 0; x < 4; x += 1) {
            if (! tetromino->squares[y * 4 + x]) {
                continue;
            }

            // Step 1: Translate origin point to 0,0
            float tx = ((float) x) - tetromino->origin.x;
            float ty = -((float) y) - tetromino->origin.y;


            // Step 2: Rotate
            float rx = tx * c - ty * s;
            float ry = tx * s + ty * c;

            // Step 3: Translate back
            int fx = round(rx + tetromino->origin.x);
            int fy = round(-(ry + tetromino->origin.y));


            switch (rotation) {
                case TETRO_R_000:
                case TETRO_R_360: {
                    break;
                }
                case TETRO_R_090: {
                    fx -= 1;
                    break;
                }
                case TETRO_R_180: {
                    fx -= 1;
                    fy -= 1;
                    break;
                }
                case TETRO_R_270: {
                    fy -= 1;
                    break;
                }
            }

            DEBUG_PRINTF("final fx = %d, fy = %d", fx, fy);
            DEBUG_PRINT("-------------");
            
            int idx = fy * 4 + fx;

            assert(idx >= 0 && "After rotation idx should be positive");

            rotated_tetro.squares[idx] = true;
        }
    }

    return rotated_tetro;
}

void drop_new_tetromino(Game *game, TetrominoType tetro_type) {
    // TODO: Remember to free
    game->active_tetromino = malloc(sizeof(ActiveTetromino));

    game->active_tetromino->tetromino_type = tetro_type;
    game->active_tetromino->rotation = TETRO_R_000;
    game->active_tetromino->x = (game->cols / 2) - 2;
    game->active_tetromino->y = 0;

    double time = glfwGetTime();

    game->active_tetromino->time_exists = time;
    game->active_tetromino->simulated_time = time;
}

bool is_active_tetromino_grounded(Game *game) {
    const Tetromino *at = &TETROMINOS[game->active_tetromino->tetromino_type];

    for (size_t x = 0; x < 4; x += 1) {
        for (size_t by = 0; by < 4; by += 1) {
            size_t y = (4 - by) - 1;
            size_t i = y * 4 + x;

            if (! at->squares[i]) {
                continue;
            }
            
            // DEBUG_PRINTF("BEFORE DETECTED %u, %zu", game->active_tetromino->y, y);
            size_t board_x = game->active_tetromino->x + x;
            size_t board_y = game->active_tetromino->y + y;

            if (board_y + 1 == game->rows || game->board[(board_y + 1) * game->cols + board_x] != TETRO_EMPTY) {
                // DEBUG_PRINTF("DETECTED type = %d, y = %zu, boardy = %d", game->active_tetromino->tetromino_type, board_y, game->board[(board_y + 1) * game->cols + board_x]);
                return true;
            }


            break;
        }
    }

    return false;
}

void settle_active_tetromino_on_board(Game *game) {
    assert(game->active_tetromino != NULL && "Active tetromino should be present");

    size_t bx = game->active_tetromino->x;
    size_t by = game->active_tetromino->y;
    Tetromino *tetromino = &TETROMINOS[game->active_tetromino->tetromino_type];
    Tetromino rotated_tetromino = rotate_tetromino(tetromino, game->active_tetromino->rotation);

    for (size_t ty = 0; ty < 4; ty += 1) {
        for (size_t tx = 0; tx < 4; tx += 1) {
            if (! rotated_tetromino.squares[ty * 4 + tx]) {
                continue;
            }

            size_t x = bx + tx;
            size_t y = by + ty;

            game->board[y * game->cols + x] = game->active_tetromino->tetromino_type;
        }
    }
}

// TOOD: Add levels later on
void move_tetromino_down(Game *game) {
    // TODO: Implement loss condition
    assert(game->active_tetromino != NULL && "Active tetromino should be present");

    double *time_exists = &game->active_tetromino->time_exists;
    double *simulated_time = &game->active_tetromino->simulated_time;

    *time_exists = glfwGetTime();


    while (*time_exists > *simulated_time + TETRO_DROP_SECS_PER_ROW) {
        DEBUG_PRINTF("A: TETRO Y = %hu", game->active_tetromino->y);
        game->active_tetromino->y += 1;
        DEBUG_PRINTF("B: TETRO Y = %hu", game->active_tetromino->y);
        *simulated_time += TETRO_DROP_SECS_PER_ROW;
        
        game->should_rerender = true;
        game->should_check_collision = true;
    }

    if (game->should_check_collision && is_active_tetromino_grounded(game)) {
        DEBUG_PRINT("Collision detected");
        settle_active_tetromino_on_board(game);
        free(game->active_tetromino);
        drop_new_tetromino(game, get_next_tetromino());
        game->should_check_collision = false;
    }
}
