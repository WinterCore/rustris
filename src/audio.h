#ifndef AUDIO_H
#define AUDIO_H

#include "al.h"
#include "alc.h"

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
    ALCcontext *context;
    ALCdevice *device;

    ALuint buffers[SFX_COUNT];
    ALuint sources[SFX_COUNT];
} Audio;

Audio create_audio();
void play_sound_effect(Audio *audio, SoundEffect effect);

extern char *SFX_FILES[SFX_COUNT];

#endif
