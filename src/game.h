#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <stdint.h>

#define TETRO_DROP_SECS_PER_ROW 1


#define KEY_REPEAT_INITIAL_DELAY_MS 200
#define KEY_REPEAT_RATE 20 // per second

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

typedef struct WallKickData {
    TetrominoRotation rotation_from;
    TetrominoRotation rotation_to;

    Point tests[5];
} WallkickData;

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

typedef enum CollisionDirection {
    DIR_STATIC,
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
    TetrominoRotation rotation;
    
    int32_t x;
    int32_t y;

    double simulated_time;
} ActiveTetromino;

typedef enum GameKey {
    KEY_UP = 0,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT,
} GameKey;

/**
 * Holds the per-key state used by get_held_key_repeats() to implement
 * DAS (Delayed Auto Shift): an initial delay before repeats begin, followed
 * by repeats firing at a fixed rate for as long as the key is held.
 */
typedef struct InputRepeatState {
    /**
     * Timestamp (in seconds, from glfwGetTime()) used as a moving cursor to
     * count elapsed intervals. Set to the current time when the key is first
     * pressed, then advanced by each delay/repeat interval as they are
     * consumed. Reset to 0 on key release.
     */
    double simulated_time;

    /**
     * Whether the initial delay (KEY_REPEAT_INITIAL_DELAY_MS) has already
     * elapsed for the current key press. False while waiting out the initial
     * delay, true once it has passed and repeat firing has begun.
     * Reset to false on key release.
     */
    bool finished_initial_delay;
} InputRepeatState;

typedef struct Level {
    uint8_t num;

    /**
     * Time spent per row in seconds
     */
    float gravity;

    uint64_t max_lines;
} Level;

typedef struct Game {
    uint8_t cols;
    uint8_t rows;

    uint64_t score;
    uint64_t lines_cleared;
    Level current_level;

    // Array[cols * rows]
    TetrominoType *board;

    ActiveTetromino active_tetromino;

    bool should_rerender;

    /**
     * Tracks whether each GameKey is currently held down, indexed by GameKey.
     * Used by is_key_tapped() to fire exactly once per key press — set to true
     * on the first frame a key is detected as pressed, and cleared on release.
     * Prevents a held key from registering as multiple taps.
     */
    bool input_hold_state[4];

    /**
     * Tracks the custom key-repeat state for each GameKey, indexed by GameKey.
     * Used by get_held_key_repeats() to implement DAS (Delayed Auto Shift):
     * after an initial delay of KEY_REPEAT_INITIAL_DELAY_MS, the key fires
     * repeats at KEY_REPEAT_RATE per second. Reset when the key is released.
     */
    InputRepeatState input_repeat_state[4];
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

/**
 * Wall kick data for the pieces: J, L, T, S, Z
 */
static WallkickData *TETROMINO_WALLKICK_DATA_GENERIC[8] = {
    &(WallkickData) {
        .rotation_from = TETRO_R_000,
        .rotation_to = TETRO_R_090,
        .tests = {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_090,
        .rotation_to = TETRO_R_000,
        .tests = {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_090,
        .rotation_to = TETRO_R_180,
        .tests = {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_180,
        .rotation_to = TETRO_R_090,
        .tests = {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_180,
        .rotation_to = TETRO_R_270,
        .tests = {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_270,
        .rotation_to = TETRO_R_180,
        .tests = {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_270,
        .rotation_to = TETRO_R_000,
        .tests = {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_000,
        .rotation_to = TETRO_R_270,
        .tests = {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    },
};

/**
 * Wall kick data for the I piece
 */
static WallkickData *TETROMINO_WALLKICK_DATA_I_PIECE[8] = {
    &(WallkickData) {
        .rotation_from = TETRO_R_000,
        .rotation_to = TETRO_R_090,
        .tests = {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_090,
        .rotation_to = TETRO_R_000,
        .tests = {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_090,
        .rotation_to = TETRO_R_180,
        .tests = {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_180,
        .rotation_to = TETRO_R_090,
        .tests = {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_180,
        .rotation_to = TETRO_R_270,
        .tests = {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_270,
        .rotation_to = TETRO_R_180,
        .tests = {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
    },

    &(WallkickData) {
        .rotation_from = TETRO_R_270,
        .rotation_to = TETRO_R_000,
        .tests = {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
    },
    &(WallkickData) {
        .rotation_from = TETRO_R_000,
        .rotation_to = TETRO_R_270,
        .tests = {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
    },
};

bool is_key_tapped(GLFWwindow *window, Game *game, GameKey key);
uint32_t get_held_key_repeats(GLFWwindow *window, Game *game, GameKey key);

Game create_game(uint8_t cols, uint8_t rows);
TetrominoType get_next_tetromino();

Tetromino rotate_tetromino(Tetromino *tetromino, bool clockwise);
void drop_new_tetromino(Game *game, TetrominoType tetro_type);
void handle_tetromino_vertical_movement(GLFWwindow *window, Game *game);
void handle_tetromino_horizontal_movement(GLFWwindow *window, Game *game);

#endif
