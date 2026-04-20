# Audio System Documentation

## Overview

The Ninja Shadow game uses **OpenAL** (Open Audio Library) for sound effects and background music. The audio system is implemented in `src/audio.c` and `src/audio.h` and is integrated into the game initialization (`main.c:446`) and cleanup (`main.c:462`).

**Dependencies:**

- `libopenal` (linked via `-lopenal` in Makefile)
- All audio files must be in **WAV format** (PCM, mono or stereo, 8/16-bit)

---

## Audio Files Required

### Directory Structure

All audio files must be placed in: **`assets/audio/`**

```
ninja_shadow/
└── assets/
    └── audio/
        ├── sfx_*.wav           (22 sound effects)
        └── bgm_*.wav           (5 background music tracks)
```

---

## Sound Effects (SFX) - 22 Files

These are one-shot sound effects triggered during gameplay.

| File Name                  | ID                        | Trigger                                   | Description                                                    |
| -------------------------- | ------------------------- | ----------------------------------------- | -------------------------------------------------------------- |
| `sfx_jump.wav`             | SFX_JUMP (0)              | Player jumps                              | Short, light sound for initial jump                            |
| `sfx_double_jump.wav`      | SFX_DOUBLE_JUMP (1)       | Player double jumps                       | Distinctive sound for second jump                              |
| `sfx_land.wav`             | SFX_LAND (2)              | Player lands                              | Optional: soft ground impact (not currently triggered in code) |
| `sfx_wall_slide.wav`       | SFX_WALL_SLIDE (3)        | Player wall slides                        | Optional: sliding friction sound (not currently triggered)     |
| `sfx_dash.wav`             | SFX_DASH (4)              | Player dashes (E/L/Enter)                 | Quick, energetic whoosh sound                                  |
| `sfx_attack_slash.wav`     | SFX_ATTACK_SLASH (5)      | Player draws sword (Z/J)                  | Sword unsheathe or swing start sound                           |
| `sfx_attack_hit.wav`       | SFX_ATTACK_HIT (6)        | Sword hits enemy                          | Impact/collision sound                                         |
| `sfx_shuriken_throw.wav`   | SFX_SHURIKEN_THROW (7)    | Player throws shuriken (X/K)              | Release/throw sound                                            |
| `sfx_shuriken_hit.wav`     | SFX_SHURIKEN_HIT (8)      | Shuriken hits enemy/object                | Impact sound for projectile                                    |
| `sfx_player_hurt.wav`      | SFX_PLAYER_HURT (9)       | Player takes damage                       | Pain/hurt grunt                                                |
| `sfx_player_death.wav`     | SFX_PLAYER_DEATH (10)     | Player dies                               | Death/game-over sound                                          |
| `sfx_enemy_death.wav`      | SFX_ENEMY_DEATH (11)      | Enemy dies                                | Enemy death sound                                              |
| `sfx_gem_collect.wav`      | SFX_GEM_COLLECT (12)      | Player collects gem                       | Sparkly/chime sound                                            |
| `sfx_star_collect.wav`     | SFX_STAR_COLLECT (13)     | Player collects star                      | Different chime/success sound                                  |
| `sfx_pot_break.wav`        | SFX_POT_BREAK (14)        | Pot is destroyed                          | Smash/breaking ceramic sound                                   |
| `sfx_blade_spin.wav`       | SFX_BLADE_SPIN (15)       | Optional: blade spinning attack           | Not currently triggered in code                                |
| `sfx_fire_vent.wav`        | SFX_FIRE_VENT (16)        | Optional: fire trap activation            | Not currently triggered in code                                |
| `sfx_platform_crumble.wav` | SFX_PLATFORM_CRUMBLE (17) | Optional: crumbling platform              | Not currently triggered in code                                |
| `sfx_level_complete.wav`   | SFX_LEVEL_COMPLETE (18)   | Player reaches exit/completes level       | Triumphant/celebration sound                                   |
| `sfx_game_over.wav`        | SFX_GAME_OVER (19)        | Game over (all lives lost)                | Sad/defeat sound                                               |
| `sfx_menu_select.wav`      | SFX_MENU_SELECT (20)      | Menu navigation                           | Optional: selection cursor sound (not currently triggered)     |
| `sfx_menu_confirm.wav`     | SFX_MENU_CONFIRM (21)     | Menu confirmation (Start, Continue, etc.) | Selection/confirmation beep                                    |

**Total SFX:** 22 files

---

## Background Music (BGM) - 5 Tracks

Background music loops continuously during gameplay. Each level theme uses one track.

| File Name          | ID               | Level Theme       | Description                         |
| ------------------ | ---------------- | ----------------- | ----------------------------------- |
| `bgm_forest.wav`   | BGM_FOREST (0)   | Theme 0: RUINS    | Atmospheric, mysterious ruins theme |
| `bgm_temple.wav`   | BGM_TEMPLE (1)   | Theme 1: TEMPLE   | Asian temple/pagoda theme           |
| `bgm_village.wav`  | BGM_VILLAGE (2)  | Theme 2: VILLAGE  | Rural/village exploration theme     |
| `bgm_bamboo.wav`   | BGM_BAMBOO (3)   | Theme 3: BAMBOO   | Serene bamboo forest theme          |
| `bgm_fortress.wav` | BGM_FORTRESS (4) | Theme 4: FORTRESS | Action-packed fortress/combat theme |

**Total BGM:** 5 files (each should be loopable)

---

## Audio System Details

### How It Works

1. **Initialization** (`audio_init()` in main.c:446)
   - Opens OpenAL device and creates context
   - Pre-loads all 27 audio files into OpenAL buffers
   - Creates:
     - 16 SFX sources (fire-and-forget pool)
     - 1 dedicated BGM source (looping)
   - Missing files are silently skipped (graceful degradation)

2. **Sound Effects**
   - Triggered via `audio_play(SfxId)` calls throughout game logic
   - Uses a pool of 16 simultaneous sources
   - If all sources are busy, steals the oldest sound
   - Plays at full gain (1.0f)

3. **Background Music**
   - Triggered via `audio_play_bgm(int bgm_id)` when level is loaded
   - Loops continuously
   - Plays at 60% gain (0.6f) to avoid overpowering SFX
   - Can be stopped with `audio_stop_bgm()`
   - Theme selected from `level.theme` (values 0-4)

4. **Cleanup**
   - `audio_cleanup()` called on exit (main.c:462)
   - Releases all OpenAL resources

### WAV File Specifications

The minimal WAV loader supports:

- **Format:** PCM (uncompressed)
- **Channels:** Mono (1) or Stereo (2)
- **Bit Depth:** 8-bit or 16-bit
- **Sample Rate:** Any (common: 44100 Hz, 48000 Hz, 22050 Hz)

**Recommended specs:**

- **SFX:** Mono, 16-bit, 44100 Hz (fast playback, low memory)
- **BGM:** Stereo, 16-bit, 44100 Hz (better quality for looping tracks)

---

## Where to Find Audio Files

### Free Resources for Creating/Finding Sounds

#### Sound Effects Libraries

1. **Freesound.org** (https://freesound.org)
   - Huge collection of sound effects
   - Filter by license and duration
   - Search: "jump sound", "sword slash", "collectible chime", etc.

2. **Zapsplat** (https://www.zapsplat.com)
   - Free sound effects and music
   - No account required
   - Wide variety of game sounds

3. **OpenGameArt.org** (https://opengameart.org)
   - Game-specific assets
   - CC0 and open-source friendly

4. **BBC Sound Library** (https://sound-effects.bbcrewind.co.uk)
   - Quality sound effects
   - Open access

#### Music/BGM Resources

1. **OpenGameArt.org** - Game music section
2. **Incompetech** (https://incompetech.com)
   - Free background music
   - Great for looping game tracks
3. **itch.io** (https://itch.io/game-assets/music)
   - Many free game music packs
   - Filter by license
4. **YouTube Audio Library** (requires YouTube account)
   - Background music collection

#### Audio Editing Tools (Free/Open Source)

1. **Audacity** (https://www.audacityteam.org)
   - Edit, convert WAV files
   - Normalize levels
   - Trim/loop sounds
2. **FFmpeg** (command-line)
   - Convert formats to WAV
   - Adjust sample rate/bit depth
   ```bash
   ffmpeg -i input.mp3 -acodec pcm_s16le -ar 44100 output.wav
   ```

---

## Setup Steps

### 1. Create Directory

```bash
mkdir -p assets/audio
```

### 2. Obtain Audio Files

- Download or create 27 audio files (22 SFX + 5 BGM)
- Ensure all are in WAV format

### 3. Place Files

Copy all files to `assets/audio/` with exact names (case-sensitive):

```bash
# SFX files
cp sfx_jump.wav assets/audio/
cp sfx_double_jump.wav assets/audio/
cp sfx_land.wav assets/audio/
... (repeat for all 22 SFX files)

# BGM files
cp bgm_forest.wav assets/audio/
cp bgm_temple.wav assets/audio/
cp bgm_village.wav assets/audio/
cp bgm_bamboo.wav assets/audio/
cp bgm_fortress.wav assets/audio/
```

### 4. Test

```bash
make clean && make
./ninja_shadow
```

The game will:

- Load all audio files (missing files produce silent no-op)
- Play sounds when triggered
- Output to stderr if OpenAL is unavailable: `audio: no OpenAL device found — running silent`

---

## Notes

- **Graceful Degradation:** Game runs normally even if audio files are missing (silent mode)
- **OpenAL Required:** Linux desktop systems typically have OpenAL pre-installed
  - Check: `ldconfig -p | grep openal`
  - Install if missing: `sudo apt install libopenal1 libopenal-dev` (Ubuntu/Debian)
- **File Naming:** Exact filenames and paths are hardcoded in `src/audio.c` (lines 28-59)
- **Untriggered Sounds:** Some SFX are defined but not called in current code:
  - `sfx_land.wav`
  - `sfx_wall_slide.wav`
  - `sfx_menu_select.wav`
  - `sfx_blade_spin.wav`
  - `sfx_fire_vent.wav`
  - `sfx_platform_crumble.wav`

---

## Source Code References

| File          | Lines    | Purpose                                      |
| ------------- | -------- | -------------------------------------------- |
| `src/audio.h` | 1-62     | Header file: enum IDs, function declarations |
| `src/audio.c` | 1-257    | Implementation: OpenAL wrapper, WAV loader   |
| `src/main.c`  | 446, 462 | Init/cleanup calls                           |
| `src/game.c`  | 474-2475 | Audio trigger calls (24 audio_play calls)    |
| `Makefile`    | 3        | Linker flag: `-lopenal`                      |

---

## Summary Table

| Category        | Count  | File Prefix | Format  | Location        |
| --------------- | ------ | ----------- | ------- | --------------- |
| Sound Effects   | 22     | `sfx_*.wav` | PCM WAV | `assets/audio/` |
| BGM Tracks      | 5      | `bgm_*.wav` | PCM WAV | `assets/audio/` |
| **Total Files** | **27** |             |         |                 |
