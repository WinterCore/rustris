#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <stdint.h>

#define TETRO_DROP_SECS_PER_ROW 1


#define KEY_REPEAT_INITIAL_DELAY_SECS 0.2
#define KEY_REPEAT_RATE 20 // per second
#define LOCK_DELAY_SECS 0.5
#define LOCK_DELAY_MOVE_RESET_LIMIT 15

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

    /**
     * Timestamp (in seconds, from glfwGetTime()) tracking the last time
     * gravity was applied. Gravity ticks are consumed by advancing this
     * value by the gravity interval, so the piece drops at a steady rate
     * independent of frame timing. Reset when a new piece spawns or when
     * the player manually moves the piece down.
     */
    double simulated_time;


    /**
     * Timestamp (from glfwGetTime()) of when the piece first touched a surface and the lock
     * delay window began. Reset when a new piece spawns.
     */
    double lock_delay_start_time;
    /**
     * Number of move/rotation resets remaining before the lock delay can no longer be extended.
     * Decremented on each move or rotation while grounded, locked at 0.
     */
    int lock_delay_resets_remaining;
} ActiveTetromino;

typedef enum GameKey {
    KEY_UP = 0,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT,
    KEY_C,
    KEY_X,
    KEY_SPACE,

    // Used to size input state arrays, must be last
    KEY_COUNT,
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

typedef enum GameState {
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER,
} GameState;

/**
 * Implements the "7-bag" randomizer: pieces are shuffled in groups of 7,
 * guaranteeing that the player will get one of each piece every 7 pieces.
 */
typedef struct NextPieceBag {
    TetrominoType pieces[7];
    uint8_t next_piece_index;
} NextPieceBag;

typedef struct Game {
    uint8_t cols;
    uint8_t rows;

    GameState state;

    uint64_t score;
    uint64_t lines_cleared;
    Level current_level;

    // Array[cols * rows]
    TetrominoType *board;

    ActiveTetromino active_tetromino;

    bool should_rerender;

    NextPieceBag next_piece_bag;

    /**
     * Tracks whether each GameKey is currently held down, indexed by GameKey.
     * Used by is_key_tapped() to fire exactly once per key press — set to true
     * on the first frame a key is detected as pressed, and cleared on release.
     * Prevents a held key from registering as multiple taps.
     */
    bool input_hold_state[KEY_COUNT];

    /**
     * Tracks the custom key-repeat state for each GameKey, indexed by GameKey.
     * Used by get_held_key_repeats() to implement DAS (Delayed Auto Shift):
     * after an initial delay of KEY_REPEAT_INITIAL_DELAY_MS, the key fires
     * repeats at KEY_REPEAT_RATE per second. Reset when the key is released.
     */
    InputRepeatState input_repeat_state[KEY_COUNT];
} Game;

extern Tetromino TETROMINOS[7];
extern WallkickData *TETROMINO_WALLKICK_DATA_GENERIC[8];
extern WallkickData *TETROMINO_WALLKICK_DATA_I_PIECE[8];

bool is_key_tapped(GLFWwindow *window, Game *game, GameKey key);
uint32_t get_held_key_repeats(GLFWwindow *window, Game *game, GameKey key);

Game create_game(uint8_t cols, uint8_t rows);
TetrominoType next_tetromino_consume(Game *game);
TetrominoType next_tetromino_peek(Game *game);

Tetromino rotate_tetromino(Tetromino *tetromino, bool clockwise);
void drop_new_tetromino(Game *game, TetrominoType tetro_type);
bool check_collision(
    Game *game,
    int32_t x,
    int32_t y,
    Tetromino *tetromino,
    CollisionDir dir
);

void handle_tetromino_rotation(GLFWwindow *window, Game *game);
void handle_tetromino_vertical_movement(GLFWwindow *window, Game *game);
void handle_tetromino_horizontal_movement(GLFWwindow *window, Game *game);

#endif
