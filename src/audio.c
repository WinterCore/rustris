#include <stdlib.h>
#include <stdio.h>
#include <miniaudio/miniaudio.h>

#include "audio.h"
#include "aids.h"

char *SFX_FILES[SFX_COUNT] = {
    "assets/drop.wav",
    NULL,
    "assets/double.wav",
    "assets/triple.wav",
    "assets/tetris.wav",
    "assets/pause.wav",
    "assets/pause.wav",
};

Audio create_audio() {
    Audio audio = {0};
    ma_result engine_init_result = ma_engine_init(NULL, &audio.engine);

    if (engine_init_result != MA_SUCCESS) {
        DEBUG_PRINTF("Failed to initialize audio engine: %d\n", engine_init_result);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SFX_COUNT; i++) {
        // Skip loading if effect has no associated file
        if (SFX_FILES[i] == NULL) {
            continue;
        }

        ma_result sound_init_result = ma_sound_init_from_file(
            &audio.engine,
            SFX_FILES[i],
            MA_SOUND_FLAG_DECODE,
            NULL,
            NULL,
            &audio.sounds[i]
        );

        if (sound_init_result != MA_SUCCESS) {
            DEBUG_PRINTF("Failed to initialize sound effect %d: %d\n", i, sound_init_result);
            exit(EXIT_FAILURE);
        }

        audio.sounds_initialized[i] = true;
    }

    return audio;
}

void play_sound_effect(Audio *audio, SoundEffect effect) {
    if (! audio->sounds_initialized[effect]) {
        return;
    }

    ma_sound_seek_to_pcm_frame(&audio->sounds[effect], 0);
    ma_sound_start(&audio->sounds[effect]);
}

void play_lines_cleared_sound_effect(Audio *audio, int lines_cleared) {
    switch (lines_cleared) {
        case 1:
            play_sound_effect(audio, SFX_SINGLE_LINE);
            break;
        case 2:
            play_sound_effect(audio, SFX_DOUBLE_LINE);
            break;
        case 3:
            play_sound_effect(audio, SFX_TRIPLE_LINE);
            break;
        case 4:
            play_sound_effect(audio, SFX_TETRIS);
            break;
        default:
            // No sound effect for clearing 0 lines or more than 4 lines
            break;
    }
}
