#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

#define TETRO_DROP_SECS_PER_ROW 1

typedef struct Point {
    float x;
    float y;
} Point;

typedef enum TetrominoRotation {
    TETRO_R_000 = 0,    
    TETRO_R_090 = 90,
    TETRO_R_180 = 180,
    TETRO_R_270 = 270,
} TetrominoRotation;

typedef enum TetrominoType {
    TETRO_I = 0,
    TETRO_J,
    TETRO_L,
    TETRO_O,
    TETRO_S,
    TETRO_T,
    TETRO_Z,

    TETRO_EMPTY = 6969,
} TetrominoType;

typedef enum Direction {
    DIR_RIGHT,
    DIR_LEFT,
    DIR_UP,
    DIR_DOWN,
} CollisionDir;

// All tetrominos have a widthxheight = 4x4
typedef struct Tetromino {
    bool squares[16];
    TetrominoType type;

    Point origin;
} Tetromino;

typedef struct ActiveTetromino {
    Tetromino tetromino;
    
    int32_t x;
    int32_t y;

    double simulated_time;
    double time_exists;
} ActiveTetromino;

typedef enum GameKey {
    KEY_UP = 0,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT,
} GameKey;

typedef struct Game {
    uint8_t cols;
    uint8_t rows;

    // Array
    TetrominoType *board;

    ActiveTetromino *active_tetromino;

    bool should_rerender;

    bool input_tap_state[4];
} Game;

static Tetromino TETROMINOS[7] = {
    {
        .type = TETRO_I,
        .squares = {
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 2, .y = -2 },
    },
    {
        .type = TETRO_J,
        .squares = {
            1, 0, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_L,
        .squares = {
            0, 0, 1, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_O,
        .squares = {
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 2, .y = -1 },
    },
    {
        .type = TETRO_S,
        .squares = {
            0, 1, 1, 0,
            1, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_T,
        .squares = {
            0, 1, 0, 0,
            1, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
    {
        .type = TETRO_Z,
        .squares = {
            1, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        },
        .origin = { .x = 1.5, .y = -1.5 },
    },
};

bool is_key_tapped(GLFWwindow *window, Game *game, GameKey key);

Game create_game(uint8_t cols, uint8_t rows);
TetrominoType get_next_tetromino();

Tetromino rotate_tetromino(Tetromino *tetromino, TetrominoRotation rotation);
void drop_new_tetromino(Game *game, TetrominoType tetro_type);
void handle_tetromino_vertical_movement(GLFWwindow *window, Game *game);
void handle_tetromino_horizontal_movement(GLFWwindow *window, Game *game);

#endif
