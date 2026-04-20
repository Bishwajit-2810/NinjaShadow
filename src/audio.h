#ifndef AUDIO_H
#define AUDIO_H

/* ── Sound effect IDs ───────────────────────────────────── */
typedef enum
{
    SFX_JUMP = 0,
    SFX_DOUBLE_JUMP,
    SFX_LAND,
    SFX_WALL_SLIDE,
    SFX_DASH,
    SFX_ATTACK_SLASH,
    SFX_ATTACK_HIT,
    SFX_SHURIKEN_THROW,
    SFX_SHURIKEN_HIT,
    SFX_PLAYER_HURT,
    SFX_PLAYER_DEATH,
    SFX_ENEMY_DEATH,
    SFX_GEM_COLLECT,
    SFX_STAR_COLLECT,
    SFX_POT_BREAK,
    SFX_BLADE_SPIN,
    SFX_FIRE_VENT,
    SFX_PLATFORM_CRUMBLE,
    SFX_LEVEL_COMPLETE,
    SFX_GAME_OVER,
    SFX_MENU_SELECT,
    SFX_MENU_CONFIRM,
    SFX_COUNT /* keep last */
} SfxId;

/* ── BGM track IDs ──────────────────────────────────────── */
typedef enum
{
    BGM_FOREST = 0, /* theme 0 RUINS  */
    BGM_TEMPLE,     /* theme 1 TEMPLE */
    BGM_VILLAGE,    /* theme 2 VILLAGE*/
    BGM_BAMBOO,     /* theme 3 BAMBOO */
    BGM_FORTRESS,   /* theme 4 FORTRESS*/
    BGM_COUNT       /* keep last */
} BgmId;

/* ── API ────────────────────────────────────────────────── */

/* Call once at startup (after glutInit). Returns 1 on success, 0 if OpenAL
   is unavailable — all subsequent calls become silent no-ops in that case. */
int audio_init(void);

/* Play a one-shot sound effect. Safe to call even if audio_init failed. */
void audio_play(SfxId id);

/* Start a BGM track looping. Stops any currently-playing BGM first.
   Passing bgm_id = -1 stops all BGM. */
void audio_play_bgm(int bgm_id);

/* Stop the currently-playing BGM. */
void audio_stop_bgm(void);

/* Stop all currently playing SFX sources (used on hard state transitions). */
void audio_stop_all_sfx(void);

/* Free all OpenAL resources. Call on clean exit. */
void audio_cleanup(void);

#endif /* AUDIO_H */
