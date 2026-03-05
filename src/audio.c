#include <sndfile.h>
#include <alext.h>
#include <stdlib.h>

#include "audio.h"
#include "aids.h"
#include "al.h"
#include "alc.h"

char *SFX_FILES[SFX_COUNT] = {
    "assets/break.wav",
    "assets/pipes.wav",
    "assets/fart.wav",
    "assets/fart.wav",
    "assets/fart.wav",
    "assets/fart.wav",
    "assets/fart.wav",
};

Audio create_audio() {
    Audio audio = {0};

    ALCdevice *device = alcOpenDevice(NULL);

    if (!device) {
        // TODO: Error handling
        DEBUG_PRINTF("Failed to open audio device\n");
        exit(1);
    }

    // Create an OpenAL context (like an OpenGL context, holds all audio state)
    ALCcontext *context = alcCreateContext(device, NULL);
    // Make it the active context for this thread
    alcMakeContextCurrent(context);

    for (size_t i = 0; i < SFX_COUNT; i += 1) {
        // Read the WAV file header (sample rate, channels, frames) into sfinfo
        SF_INFO sfinfo = {0};
        // Open the WAV file for reading, populates sfinfo with metadata
        SNDFILE *file = sf_open(SFX_FILES[i], SFM_READ, &sfinfo);

        if (!file) {
            // TODO: Error handling
            DEBUG_PRINTF("Failed to open audio file %s: %s\n", SFX_FILES[i], sf_strerror(NULL));
            exit(1);
        }

        // Pick the right OpenAL format based on channel count
        ALenum format;
        if (sfinfo.channels == 1) {
            format = AL_FORMAT_MONO16;
        } else if (sfinfo.channels == 2) {
            format = AL_FORMAT_STEREO16;
        } else {
            sf_close(file);
            DEBUG_PRINTF("Unsupported channel count %d in audio file %s\n", sfinfo.channels, SFX_FILES[i]);
            // TODO: Error handling
            exit(1);
        }

        // Allocate a buffer for all samples (frames * channels = total sample values)
        short *samples = malloc(sfinfo.frames * sfinfo.channels * sizeof(short));
        // Read all samples from the file as signed 16-bit integers
        sf_read_short(file, samples, sfinfo.frames * sfinfo.channels);
        sf_close(file);

        // Create an OpenAL buffer (stores audio data for playback)
        ALuint buffer;
        alGenBuffers(1, &buffer);
        // Upload the PCM data into the OpenAL buffer
        alBufferData(buffer, format, samples, sfinfo.frames * sfinfo.channels * sizeof(short), sfinfo.samplerate);
        free(samples);

        ALuint source;
        alGenSources(1, &source);
        // Attach the buffer to the source
        alSourcei(source, AL_BUFFER, buffer);

        audio.buffers[i] = buffer;
        audio.sources[i] = source;
    }

    audio.context = context;
    audio.device = device;

    return audio;
}

void play_sound_effect(Audio *audio, SoundEffect effect) {
    if (effect < 0 || effect >= SFX_COUNT) {
        DEBUG_PRINTF("Invalid sound effect %d\n", effect);
        return;
    }

    ALuint source = audio->sources[effect];
    alSourcePlay(source);
}

/*
void test_audio() {
    // Open the default audio output device (speakers/headphones)
    ALCdevice *device = alcOpenDevice(NULL);

    if (!device) {
        return;
    }

    // Create an OpenAL context (like an OpenGL context, holds all audio state)
    ALCcontext *context = alcCreateContext(device, NULL);
    // Make it the active context for this thread
    alcMakeContextCurrent(context);

    // Read the WAV file header (sample rate, channels, frames) into sfinfo
    SF_INFO sfinfo = {0};
    // Open the WAV file for reading, populates sfinfo with metadata
    SNDFILE *file = sf_open("./fart.wav", SFM_READ, &sfinfo);

    if (!file) {
        return;
    }

    // Pick the right OpenAL format based on channel count
    ALenum format;
    if (sfinfo.channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (sfinfo.channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        sf_close(file);
        return;
    }

    // Allocate a buffer for all samples (frames * channels = total sample values)
    short *samples = malloc(sfinfo.frames * sfinfo.channels * sizeof(short));
    // Read all samples from the file as signed 16-bit integers
    sf_read_short(file, samples, sfinfo.frames * sfinfo.channels);
    sf_close(file);

    // Create an OpenAL buffer (stores audio data for playback)
    ALuint buffer;
    alGenBuffers(1, &buffer);
    // Upload the PCM data into the OpenAL buffer
    alBufferData(buffer, format, samples, sfinfo.frames * sfinfo.channels * sizeof(short), sfinfo.samplerate);
    // PCM data is now copied into OpenAL, free our copy
    free(samples);

    // Create a source (an "audio player" that can play buffers)
    ALuint source;
    alGenSources(1, &source);
    // Attach the buffer to the source
    alSourcei(source, AL_BUFFER, buffer);
    // Start playback
    alSourcePlay(source);

    // Busy-wait until playback finishes (not suitable for a game loop)
    ALint state;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    alSourceRewind(source); // Rewind source to the beginning for potential replay
    // Start playback
    alSourcePlay(source);

    // Busy-wait until playback finishes (not suitable for a game loop)
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    // Cleanup: delete source, buffer, context, and close device
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
*/
