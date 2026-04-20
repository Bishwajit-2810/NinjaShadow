#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── OpenAL headers ─────────────────────────────────────── */
#include <AL/al.h>
#include <AL/alc.h>

/* ── Internal state ─────────────────────────────────────── */
static int audio_ok = 0; /* 0 until audio_init succeeds */

static ALCdevice *al_device = NULL;
static ALCcontext *al_context = NULL;

/* One OpenAL buffer per SFX + BGM track */
static ALuint sfx_buf[SFX_COUNT];
static ALuint bgm_buf[BGM_COUNT];

/* Pool of short-lived sources for SFX (fire-and-forget) */
#define SFX_SOURCE_POOL 16
static ALuint sfx_sources[SFX_SOURCE_POOL];

/* Single dedicated source for BGM */
static ALuint bgm_source = 0;

/* ── Asset path mapping ─────────────────────────────────── */
static const char *sfx_paths[SFX_COUNT] = {
    "assets/audio/sfx_jump.wav",
    "assets/audio/sfx_double_jump.wav",
    "assets/audio/sfx_land.wav",
    "assets/audio/sfx_wall_slide.wav",
    "assets/audio/sfx_dash.wav",
    "assets/audio/sfx_attack_slash.wav",
    "assets/audio/sfx_attack_hit.wav",
    "assets/audio/sfx_shuriken_throw.wav",
    "assets/audio/sfx_shuriken_hit.wav",
    "assets/audio/sfx_player_hurt.wav",
    "assets/audio/sfx_player_death.wav",
    "assets/audio/sfx_enemy_death.wav",
    "assets/audio/sfx_gem_collect.wav",
    "assets/audio/sfx_star_collect.wav",
    "assets/audio/sfx_pot_break.wav",
    "assets/audio/sfx_blade_spin.wav",
    "assets/audio/sfx_fire_vent.wav",
    "assets/audio/sfx_platform_crumble.wav",
    "assets/audio/sfx_level_complete.wav",
    "assets/audio/sfx_game_over.wav",
    "assets/audio/sfx_menu_select.wav",
    "assets/audio/sfx_menu_confirm.wav",
};

static const char *bgm_paths[BGM_COUNT] = {
    "assets/audio/bgm_forest.wav",
    "assets/audio/bgm_temple.wav",
    "assets/audio/bgm_village.wav",
    "assets/audio/bgm_bamboo.wav",
    "assets/audio/bgm_fortress.wav",
};

/* ── Minimal WAV loader ─────────────────────────────────── */
/* Loads a PCM WAV file into an OpenAL buffer.
   Returns 1 on success, 0 if the file is missing or unsupported. */
static int load_wav(const char *path, ALuint buf)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0; /* file not yet provided — silent no-op */

    /* Read RIFF header */
    char riff[4];
    fread(riff, 1, 4, f);
    if (memcmp(riff, "RIFF", 4) != 0)
    {
        fclose(f);
        return 0;
    }
    fseek(f, 4, SEEK_CUR); /* chunk size */
    char wave[4];
    fread(wave, 1, 4, f);
    if (memcmp(wave, "WAVE", 4) != 0)
    {
        fclose(f);
        return 0;
    }

    /* Find fmt and data sub-chunks */
    short num_channels = 0, bits_per_sample = 0;
    int sample_rate = 0;
    unsigned char *pcm = NULL;
    int data_size = 0;

    while (!feof(f))
    {
        char id[4];
        unsigned int sz;
        if (fread(id, 1, 4, f) != 4)
            break;
        if (fread(&sz, 4, 1, f) != 1)
            break;

        if (memcmp(id, "fmt ", 4) == 0)
        {
            short audio_fmt;
            fread(&audio_fmt, 2, 1, f);
            fread(&num_channels, 2, 1, f);
            fread(&sample_rate, 4, 1, f);
            fseek(f, 4, SEEK_CUR); /* byte rate */
            fseek(f, 2, SEEK_CUR); /* block align */
            fread(&bits_per_sample, 2, 1, f);
            /* skip any extra fmt bytes */
            int extra = (int)sz - 16;
            if (extra > 0)
                fseek(f, extra, SEEK_CUR);
        }
        else if (memcmp(id, "data", 4) == 0)
        {
            data_size = (int)sz;
            pcm = (unsigned char *)malloc(data_size);
            if (!pcm)
            {
                fclose(f);
                return 0;
            }
            fread(pcm, 1, data_size, f);
        }
        else
        {
            fseek(f, sz, SEEK_CUR); /* skip unknown chunks */
        }
    }
    fclose(f);

    if (!pcm || num_channels == 0 || sample_rate == 0)
    {
        free(pcm);
        return 0;
    }

    ALenum format;
    if (num_channels == 1 && bits_per_sample == 8)
        format = AL_FORMAT_MONO8;
    else if (num_channels == 1 && bits_per_sample == 16)
        format = AL_FORMAT_MONO16;
    else if (num_channels == 2 && bits_per_sample == 8)
        format = AL_FORMAT_STEREO8;
    else if (num_channels == 2 && bits_per_sample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        free(pcm);
        return 0;
    } /* unsupported depth */

    alBufferData(buf, format, pcm, data_size, sample_rate);
    free(pcm);
    return (alGetError() == AL_NO_ERROR) ? 1 : 0;
}

/* ── audio_init ─────────────────────────────────────────── */
int audio_init(void)
{
    al_device = alcOpenDevice(NULL); /* default output device */
    if (!al_device)
    {
        fprintf(stderr, "audio: no OpenAL device found — running silent\n");
        return 0;
    }

    al_context = alcCreateContext(al_device, NULL);
    if (!al_context || !alcMakeContextCurrent(al_context))
    {
        fprintf(stderr, "audio: failed to create OpenAL context — running silent\n");
        alcCloseDevice(al_device);
        al_device = NULL;
        return 0;
    }

    alGetError(); /* clear error state */

    /* Generate buffers */
    alGenBuffers(SFX_COUNT, sfx_buf);
    alGenBuffers(BGM_COUNT, bgm_buf);

    /* Generate SFX source pool */
    alGenSources(SFX_SOURCE_POOL, sfx_sources);

    /* Generate dedicated BGM source */
    alGenSources(1, &bgm_source);
    alSourcei(bgm_source, AL_LOOPING, AL_TRUE);
    alSourcef(bgm_source, AL_GAIN, 0.6f); /* BGM slightly quieter than SFX */

    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "audio: OpenAL source/buffer allocation failed — running silent\n");
        audio_cleanup();
        return 0;
    }

    /* Load SFX files (missing files are silently skipped) */
    for (int i = 0; i < SFX_COUNT; i++)
        load_wav(sfx_paths[i], sfx_buf[i]);

    /* Load BGM files */
    for (int i = 0; i < BGM_COUNT; i++)
        load_wav(bgm_paths[i], bgm_buf[i]);

    audio_ok = 1;
    return 1;
}

/* ── audio_play ─────────────────────────────────────────── */
void audio_play(SfxId id)
{
    if (!audio_ok)
        return;
    if (id < 0 || id >= SFX_COUNT)
        return;

    /* Check that the buffer has data (file may not have been provided yet) */
    ALint buf_size = 0;
    alGetBufferi(sfx_buf[id], AL_SIZE, &buf_size);
    if (buf_size == 0)
        return;

    /* Find a free (not currently playing) source from the pool */
    for (int i = 0; i < SFX_SOURCE_POOL; i++)
    {
        ALint state;
        alGetSourcei(sfx_sources[i], AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING)
        {
            alSourcei(sfx_sources[i], AL_BUFFER, (ALint)sfx_buf[id]);
            alSourcef(sfx_sources[i], AL_GAIN, 1.0f);
            alSourcePlay(sfx_sources[i]);
            return;
        }
    }
    /* All sources busy — steal the first one (oldest sound) */
    alSourceStop(sfx_sources[0]);
    alSourcei(sfx_sources[0], AL_BUFFER, (ALint)sfx_buf[id]);
    alSourcePlay(sfx_sources[0]);
}

/* ── audio_play_bgm ─────────────────────────────────────── */
void audio_play_bgm(int bgm_id)
{
    if (!audio_ok)
        return;

    alSourceStop(bgm_source);

    if (bgm_id < 0 || bgm_id >= BGM_COUNT)
        return;

    /* Check that the buffer has data */
    ALint buf_size = 0;
    alGetBufferi(bgm_buf[bgm_id], AL_SIZE, &buf_size);
    if (buf_size == 0)
        return;

    alSourcei(bgm_source, AL_BUFFER, (ALint)bgm_buf[bgm_id]);
    alSourcePlay(bgm_source);
}

/* ── audio_stop_bgm ─────────────────────────────────────── */
void audio_stop_bgm(void)
{
    if (!audio_ok)
        return;
    alSourceStop(bgm_source);
}

/* ── audio_stop_all_sfx ────────────────────────────────── */
void audio_stop_all_sfx(void)
{
    if (!audio_ok)
        return;
    for (int i = 0; i < SFX_SOURCE_POOL; i++)
        alSourceStop(sfx_sources[i]);
}

/* ── audio_cleanup ──────────────────────────────────────── */
void audio_cleanup(void)
{
    if (!audio_ok && !al_context)
        return;

    if (bgm_source)
    {
        alSourceStop(bgm_source);
        alDeleteSources(1, &bgm_source);
        bgm_source = 0;
    }
    alDeleteSources(SFX_SOURCE_POOL, sfx_sources);
    alDeleteBuffers(SFX_COUNT, sfx_buf);
    alDeleteBuffers(BGM_COUNT, bgm_buf);

    alcMakeContextCurrent(NULL);
    if (al_context)
    {
        alcDestroyContext(al_context);
        al_context = NULL;
    }
    if (al_device)
    {
        alcCloseDevice(al_device);
        al_device = NULL;
    }

    audio_ok = 0;
}
