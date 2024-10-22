#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define DEBUG

#include "./game.h"
#include "./aids.h"

Level create_level(uint8_t level) {
    assert("Level must be > 0" && level > 0);

    float gravity = pow(0.9 - (float) (level - 1) * 0.007, (float) (level - 1));
    uint64_t max_lines = (uint64_t) level * 10;

    return (Level) {
        .gravity = gravity,
        .max_lines = max_lines,
        .num = level,
    };
}

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
        .should_rerender = true,
        .input_tap_state = {0},
        .input_repeat_state = {0},
        .current_level = create_level(1),
    };

    drop_new_tetromino(&game, get_next_tetromino());

    return game;
}



TetrominoType get_next_tetromino() {
    return TETRO_O;

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

Tetromino rotate_tetromino(Tetromino *tetromino, bool clockwise) {
    Tetromino rotated_tetro = {
        .origin = tetromino->origin,
        .type = tetromino->type,
        .squares = {0},
    };

    float rads = degrees_to_radians(clockwise ? (float) 90 : -90);
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

            /**
             * Move the point back to the top left corner of the cell
             *
             * .__  90 deg rotation   __.
             * |_|  ---------------> |_|
             * 
             * The following code basically always moves back the point
             * of origin to top left
             *
             */
            if (clockwise) {
                fx -= 1;
            } else {
                fy -= 1;
            }

            /*
            DEBUG_PRINTF("final fx = %d, fy = %d", fx, fy);
            DEBUG_PRINT("-------------");
            */
            
            int idx = fy * 4 + fx;

            assert(idx >= 0 && "After rotation idx should be positive");

            rotated_tetro.squares[idx] = true;
        }
    }


    return rotated_tetro;
}

void drop_new_tetromino(Game *game, TetrominoType tetro_type) {
    memcpy(&game->active_tetromino.tetromino, &TETROMINOS[tetro_type], sizeof(Tetromino));
    game->active_tetromino.rotation = TETRO_R_000;

    game->active_tetromino.x = (game->cols / 2) - 2;
    game->active_tetromino.y = 0;

    double time = glfwGetTime();

    game->active_tetromino.simulated_time = time;
}


bool check_collision(
    Game *game,
    int32_t x,
    int32_t y,
    Tetromino *tetromino,
    CollisionDir dir
) {
    int32_t by = y;
    int32_t bx = x;
    
    switch (dir) {
        case DIR_UP: {
            by -= 1;
            break;
        }
        case DIR_DOWN: {
            by += 1;
            break;
        }
        case DIR_LEFT: {
            bx -= 1;
            break;
        }
        case DIR_RIGHT: {
            bx += 1;
            break;
        }
        case DIR_STATIC: {
            break;
        }
    }

    for (int32_t py = 0; py < 4; py += 1) {
        for (int32_t px = 0; px < 4; px += 1) {
            int32_t i = py * 4 + px;

            if (! tetromino->squares[i]) {
                continue;
            }
            
            int32_t x = bx + px;
            int32_t y = by + py;
            
            if (y < 0 || y >= game->rows || x < 0 || x >= game->cols) {
                return true;
            }

            if (game->board[y * game->cols + x] != TETRO_EMPTY) {
                return true;
            }
        }
    }

    return false;
}

TetrominoRotation get_next_tetromino_rotation(TetrominoRotation rotation, bool clockwise) {
    TetrominoRotation rotations[4] = {
        TETRO_R_000,
        TETRO_R_090,
        TETRO_R_180,
        TETRO_R_270
    };

    size_t len = sizeof(rotations) / sizeof(rotations[0]);
    
    size_t i = 0;

    while (i < len) {
        if (rotation == rotations[i]) {
            break;
        }
        
        i += 1;
    }

    if (clockwise) {
        i = (i + 1) % len;
    } else {
        i = i == 0 ? len - 1 : i - 1;
    }
    
    return rotations[i];
}

WallkickData *get_wallkick_data(TetrominoRotation rotation_from, TetrominoRotation rotation_to, TetrominoType tetromino_type) { 
    WallkickData **wallkick_data = tetromino_type == TETRO_I
        ? TETROMINO_WALLKICK_DATA_I_PIECE
        : TETROMINO_WALLKICK_DATA_GENERIC;
    
    size_t i = 0;

    while (i < 8) {
        if (wallkick_data[i]->rotation_from == rotation_from &&
            wallkick_data[i]->rotation_to == rotation_to) {
            break;
        }

        i += 1;
    }

    return wallkick_data[i];
}

ActiveTetromino try_rotate_tetromino(Game *game, ActiveTetromino *at, bool clockwise) {
    Tetromino rotated_tetromino = rotate_tetromino(&at->tetromino, clockwise);

    TetrominoRotation next_rotation = get_next_tetromino_rotation(at->rotation, clockwise);
    WallkickData *wallkick_data = get_wallkick_data(at->rotation, next_rotation, at->tetromino.type);
    
    Point *tests = wallkick_data->tests;

    ActiveTetromino rotated_active_tetromino = {
        .tetromino = rotated_tetromino,
        .rotation = next_rotation,
        .simulated_time = at->simulated_time,
        .x = at->x,
        .y = at->y,
    };

    for (size_t i = 0; i < 5; i += 1) {
        Point *test = &tests[i];
        
        int32_t x = rotated_active_tetromino.x + (int32_t) test->x;
        int32_t y = rotated_active_tetromino.y + (int32_t) test->y;

        if (! check_collision(game, x, y, &rotated_active_tetromino.tetromino, DIR_STATIC)) {
            rotated_active_tetromino.x = x;
            rotated_active_tetromino.y = y;
            return rotated_active_tetromino;
        }
    }

    return *at;
}

void clear_lines(Game *game, int32_t y, size_t count) {
    size_t row_start = y * game->cols;
    memcpy(&game->board[game->cols * count], &game->board[0], row_start * sizeof(TetrominoType));
    game->should_rerender = true;
}

uint64_t try_clear_lines(Game *game, int32_t by) {
    uint64_t lines_cleared = 0;

    for (int32_t y = 0; y < 4; y += 1) {
        bool has_gaps = false;

        if (by + y >= game->rows) {
            break;
        }

        for (int32_t x = 0; x < game->cols; x += 1) {
            if (game->board[(by + y) * game->cols + x] == TETRO_EMPTY) {
                has_gaps = true;
                break;
            }
        }
        
        if (has_gaps) {
            continue;
        }

        // Clear line
        clear_lines(game, by + y, 1);
        lines_cleared += 1;
    }

    return lines_cleared;
}

uint64_t get_cleared_lines_score(Game *game, uint64_t num_lines) {
    assert("Lines should be between 0 and 4" && num_lines <= 4);

    // TODO: Add scoring for T spins, perfect clears and combos
    switch (num_lines) {
        case 1:
            return 100 * game->current_level.num;
        case 2:
            return 300 * game->current_level.num;
        case 3:
            return 500 * game->current_level.num;
        case 5:
            return 800 * game->current_level.num;
    }
    
    return 0;
}

void settle_active_tetromino_on_board(Game *game) {
    int32_t bx = game->active_tetromino.x;
    int32_t by = game->active_tetromino.y;
    Tetromino *tetromino = &game->active_tetromino.tetromino;

    for (int32_t ty = 0; ty < 4; ty += 1) {
        for (int32_t tx = 0; tx < 4; tx += 1) {
            if (! tetromino->squares[ty * 4 + tx]) {
                continue;
            }

            int32_t x = bx + tx;
            int32_t y = by + ty;

            game->board[y * game->cols + x] = tetromino->type;
        }
    }

    uint64_t lines_cleared = try_clear_lines(game, by);

    if (lines_cleared > 0) {
        game->lines_cleared += lines_cleared;
        uint64_t score_delta = get_cleared_lines_score(game, lines_cleared);
        game->score += score_delta;

        if (game->lines_cleared >= game->current_level.max_lines) {
            game->current_level = create_level(game->current_level.num + 1);
        }

        DEBUG_PRINTF("LEVEL = %hu, CLEARED LINES LINES = %lu, SCORE_DELTA = %lu, SCORE = %lu", game->current_level.num, lines_cleared, score_delta, game->score);
    }
}

void handle_tetromino_vertical_movement(GLFWwindow *window, Game *game) {
    // TODO: Implement loss condition
    double *simulated_time = &game->active_tetromino.simulated_time;

    double time = glfwGetTime();

    ActiveTetromino *at = &game->active_tetromino;

    if (is_key_tapped(window, game, KEY_DOWN)) {
        game->should_rerender = true;

        if (check_collision(game, at->x, at->y, &game->active_tetromino.tetromino, DIR_DOWN)) {
            settle_active_tetromino_on_board(game);
            drop_new_tetromino(game, get_next_tetromino());

            return;
        } else {
            at->y += 1;
            *simulated_time = time;
        }
    }

    while (time > *simulated_time + game->current_level.gravity) {
        game->should_rerender = true;

        if (check_collision(game, at->x, at->y, &at->tetromino, DIR_DOWN)) {
            settle_active_tetromino_on_board(game);
            drop_new_tetromino(game, get_next_tetromino());

            break;
        }

        game->active_tetromino.y += 1;
        DEBUG_PRINTF("TETRO Y = %hu", game->active_tetromino.y);
        *simulated_time += game->current_level.gravity;        
    }

    if (is_key_tapped(window, game, KEY_UP)) {
        game->active_tetromino = try_rotate_tetromino(game, &game->active_tetromino, TETRO_R_090);
        game->should_rerender = true;
    }

    uint32_t down_repeats = get_held_key_repeats(window, game, KEY_DOWN);

    while (down_repeats > 0) {
        game->should_rerender = true;

        if (check_collision(game, at->x, at->y, &game->active_tetromino.tetromino, DIR_DOWN)) {
            settle_active_tetromino_on_board(game);
            drop_new_tetromino(game, get_next_tetromino());

            return;
        } else {
            at->y += 1;
            *simulated_time = time;
            game->score += 1;
        }

        down_repeats -= 1;
    }
}


void handle_tetromino_horizontal_movement(GLFWwindow *window, Game *game) {
    ActiveTetromino *at = &game->active_tetromino;

    if (is_key_tapped(window, game, KEY_LEFT) && ! check_collision(game, at->x, at->y, &at->tetromino, DIR_LEFT)) {
        game->active_tetromino.x -= 1;
        game->should_rerender = true;
    }
    
    if (is_key_tapped(window, game, KEY_RIGHT) && ! check_collision(game, at->x, at->y, &at->tetromino, DIR_RIGHT)) {
        game->active_tetromino.x += 1;
        game->should_rerender = true;
    }
    
    uint32_t right_repeats = get_held_key_repeats(window, game, KEY_RIGHT);

    while (right_repeats > 0) {
        if (! check_collision(game, at->x, at->y, &at->tetromino, DIR_RIGHT)) {
            game->should_rerender = true;
            game->active_tetromino.x += right_repeats;
        }

        right_repeats -= 1;
    }

    uint32_t left_repeats = get_held_key_repeats(window, game, KEY_LEFT);

    while (left_repeats > 0) {
        if (! check_collision(game, at->x, at->y, &at->tetromino, DIR_LEFT)) {
            game->should_rerender = true;
            game->active_tetromino.x -= left_repeats;
        }

        left_repeats -= 1;
    }
}


int game_key_to_glfw_key(GameKey key) {
    switch (key) {
        case KEY_DOWN: return GLFW_KEY_DOWN;
        case KEY_RIGHT: return GLFW_KEY_RIGHT;
        case KEY_UP: return GLFW_KEY_UP;
        case KEY_LEFT: return GLFW_KEY_LEFT;
    }

    UNREACHABLE;
}


bool is_key_tapped(GLFWwindow *window, Game *game, GameKey key) {
    int glfw_key = game_key_to_glfw_key(key);
    int key_state = glfwGetKey(window, glfw_key);
    
    if (key_state == GLFW_PRESS) {
        if (! game->input_tap_state[key]) {
            game->input_tap_state[key] = true;
            return true;
        }
        
        return false;
    } else if (key_state == GLFW_RELEASE) {
        game->input_tap_state[key] = false;

        return false;
    } else {
        UNREACHABLE;
    }
}

/**
 * Get the number of repeats since the last call to this function
 *
 *
 * IMPORTANT: DO NOT call this function from multiple places or you'll
 * get unexpected results
 */
uint32_t get_held_key_repeats(GLFWwindow *window, Game *game, GameKey key) {
    int glfw_key = game_key_to_glfw_key(key);
    int key_state = glfwGetKey(window, glfw_key);
    InputRepeatState *state = &game->input_repeat_state[key];

    if (key_state == GLFW_RELEASE) {
        state->simulated_time = 0;
        state->finished_initial_delay = false;

        return 0;
    }
    
    if (state->simulated_time == 0) {
        state->simulated_time = glfwGetTime();

        return 0;
    }

    double time = glfwGetTime();

    if (! state->finished_initial_delay) {
        double delay = (float) KEY_REPEAT_INITIAL_DELAY_MS / 1000.0f;

        if (state->simulated_time + delay < time) {
            state->simulated_time += delay;
            state->finished_initial_delay = true;
            // Fall through
        } else {
            return 0;
        }
    }

    double repeat_rate_delay = 1.0f / (float) KEY_REPEAT_RATE;
    
    uint32_t repeats = 0;

    while (state->simulated_time + repeat_rate_delay < time) {
        repeats += 1;

        state->simulated_time += repeat_rate_delay;
    }

    return repeats;
}
