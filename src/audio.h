#ifndef AUDIO_H
#define AUDIO_H

#include <miniaudio/miniaudio.h>

typedef enum SoundEffect {
    SFX_DROP = 0,
    SFX_SINGLE_LINE,
    SFX_DOUBLE_LINE,
    SFX_TRIPLE_LINE,
    SFX_TETRIS,
    SFX_PAUSE,
    SFX_UNPAUSE,

    // Used to track the number of effects
    SFX_COUNT
} SoundEffect;

typedef struct Audio {
    ma_engine engine;
    bool sounds_initialized[SFX_COUNT];
    ma_sound sounds[SFX_COUNT];
} Audio;

Audio create_audio();
void play_sound_effect(Audio *audio, SoundEffect effect);
void play_lines_cleared_sound_effect(Audio *audio, int lines_cleared);

extern char *SFX_FILES[SFX_COUNT];

#endif
