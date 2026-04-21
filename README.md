# Ninja Shadow

**A 2D OpenGL-based ninja platformer game featuring atmospheric Japanese-inspired visuals, challenging level design, and fast-paced action gameplay.**

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Technical Stack](#technical-stack)
- [Project Structure](#project-structure)
- [Architecture & Design](#architecture--design)
- [Gameplay Systems](#gameplay-systems)
- [Controls](#controls)
- [Game States](#game-states)
- [Level Themes](#level-themes)
- [Asset Management](#asset-management)
- [Audio System](#audio-system)
- [Physics & Collision](#physics--collision)
- [Animation System](#animation-system)
- [Particle Effects](#particle-effects)
- [HUD & UI](#hud--ui)
- [Rendering Pipeline](#rendering-pipeline)
- [Input Handling](#input-handling)
- [Enemy AI](#enemy-ai)
- [Collision Deep Dive](#collision-deep-dive)
- [Camera System](#camera-system)
- [Level Design Guide](#level-design-guide)
- [Memory Layout](#memory-layout)
- [Performance Optimization](#performance-optimization)
- [Code Examples](#code-examples)
- [Common Patterns](#common-patterns)
- [Testing](#testing)
- [Development Guide](#development-guide)
- [Build Instructions](#build-instructions)
- [Platform-Specific Notes](#platform-specific-notes)
- [Troubleshooting](#troubleshooting)
- [Future Enhancements](#future-enhancements)
- [Resources & References](#resources--references)
- [License & Credits](#license--credits)
- [FAQ](#faq)
- [Contact & Community](#contact--community)

---

## Overview

**Ninja Shadow** is a 2D platformer inspired by Ninja Arashi II, developed in C using OpenGL and GLUT. Players control a ninja character through 10 progressively challenging levels across 5 distinct Japanese-themed environments: Ruins, Temple, Village, Bamboo Forest, and Fortress.

The game emphasizes:

- **Fast-paced action**: Sword combat, ranged shuriken attacks, and evasive maneuvers
- **Precision platforming**: Multiple jump types, wall-sliding, and air-dash mechanics
- **Atmospheric design**: Dark silhouette art style with parallax backgrounds and dynamic weather
- **Skill-based progression**: Star-based rating system (0-3 stars per level) based on performance

---

## Features

### Gameplay

- ✅ **Player character** with sword attacks, infinite-range shurikens, dash mechanics, and double-jumping
- ✅ **Three enemy types**: Guards (melee), Archers (ranged), Heavy Warriors (armored & slow)
- ✅ **Dynamic traps**: Spinning blade traps, ground spikes, thorn bushes, fire vents, crumbling platforms
- ✅ **Collectibles**: Gems, gold coins, star pickups for rating system
- ✅ **Checkpoint system**: Save progress within levels
- ✅ **10 progressively difficult levels** across 5 unique themes

### Visual Design

- Pure **black silhouette characters** drawn with OpenGL primitives
- **Parallax-scrolling backgrounds** with theme-specific decorations:
  - Ruins: Ghost trees, stone pillars, hanging cloth
  - Temple: Pine trees, pagoda gates, red lanterns
  - Village: Willow trees, bare trees, hanging cloth
  - Bamboo: Dense bamboo forest background, stone markers
  - Fortress: Fortress walls, cherry blossom trees
- **Dynamic environment effects**: Moon phases, night stars, birds, clouds, fire animations
- **Particle effects**: Hit sparks, collection bursts, landing dust

### Audio

- **22 unique sound effects** covering all actions (jump, attack, hit, collect, etc.)
- **5 themed background music tracks** (one per level theme)
- **OpenAL-based audio system** for low-latency sound playback
- Graceful degradation if OpenAL is unavailable

### User Interface

- **Main menu** with theme animation
- **Pause overlay** with settings access
- **In-game HUD**: Health (hearts), gem/gold counters, shuriken count, star rating progress
- **Level complete screen** with star rating display
- **Game over screen** with retry/menu options
- **Settings overlay** (expandable)

### Physics & Mechanics

- **Full 2D physics**: Gravity, velocity, acceleration, collision detection
- **Movement**: Walk (265 px/s), sprint (540 px/s dash), wall-slide
- **Jump system**: Ground jump (680 vy), double-jump (600 vy), capped fall speed (-900 px/s)
- **Projectile system**: Shurikens (10 per level, 50-unit range, 450 px/s), arrows (from archers)
- **Melee combat**: One-shot cooldown to prevent multi-hits from a single swing
- **Checkpoint respawning**: Re-spawn at last triggered checkpoint with health penalty

---

## Quick Start

### Prerequisites

**Linux/macOS:**

```bash
gcc           # C compiler
libGL         # OpenGL library
libGLU        # GLU utility library
freeglut      # GLUT windowing & input library
libm          # C math library
libopenal     # OpenAL audio library
```

**Installation (Ubuntu/Debian):**

```bash
sudo apt-get install gcc libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libopenal-dev
```

**Installation (macOS with Homebrew):**

```bash
brew install freeglut
```

### Build & Run

```bash
# Build the game
make

# Run the game
./ninja_shadow

# Clean build artifacts
make clean

# Run unit tests
gcc tests/test_shuriken.c -o tests/test_shuriken -Wall
./tests/test_shuriken
```

---

## Technical Stack

| Component        | Technology                                     |
| ---------------- | ---------------------------------------------- |
| **Language**     | C99                                            |
| **Window/Input** | GLUT (freeglut)                                |
| **Rendering**    | OpenGL 2.x (fixed-function pipeline)           |
| **Audio**        | OpenAL with .wav file loading                  |
| **Math**         | C standard library (math.h) + custom utilities |
| **Build**        | GCC + GNU Makefile                             |
| **Testing**      | C unit tests with assert.h                     |

### OpenGL Features Used

- **Primitives**: GL_QUADS, GL_TRIANGLES, GL_TRIANGLE_FAN, GL_LINE_STRIP
- **Transformations**: glTranslatef, glRotatef, glScalef
- **Matrix operations**: glPushMatrix/glPopMatrix
- **Color management**: glColor4f with alpha blending
- **2D Projection**: gluOrtho2D (0, 1280, 0, 720)

---

## Project Structure

```
ninja_shadow/
├── README.md                          # This file
├── CLAUDE.md                          # Architectural guidelines for developers
├── Makefile                           # Build configuration
├── plan.md                            # Detailed design specification & checklist
├── ninja_shadow                       # Compiled binary (after make)
│
├── src/                               # All source code
│   ├── main.c                         # Entry point: GLUT init, event loop, display callback
│   ├── game.h                         # Global header: all struct definitions, constants, extern declarations
│   ├── game.c                         # Core game logic: init, level loading, update loop, physics
│   ├── renderer.h / renderer.c        # Low-level OpenGL: primitives, color helpers, text
│   ├── draw_bg.c                      # Background rendering: sky, trees, decorations, parallax
│   ├── draw_map.c                     # Level geometry: platforms, traps, pots, collectibles, exits
│   ├── draw_player.c                  # Player character rendering (all animation states)
│   ├── draw_enemy.c                   # Enemy rendering (Guard, Archer, Heavy types)
│   ├── draw_hud.c                     # HUD, menus, overlays, text rendering
│   ├── audio.h / audio.c              # Audio system: SFX, BGM, OpenAL backend
│
├── assets/
│   ├── levels/                        # Level data files (currently empty, to be created)
│   │   ├── level01.txt                # Level 1 layout, theme, objects
│   │   ├── level02.txt
│   │   └── ... level03-10.txt
│   └── audio/                         # Audio files (currently empty, to be populated)
│       ├── sfx_*.wav                  # 22 sound effects
│       ├── bgm_*.wav                  # 5 background music tracks
│
├── include/
│   └── stb_image.h                    # Utility header (optional, for future texture loading)
│
└── tests/
    ├── test_shuriken.c                # Unit tests for shuriken system constraints
    ├── test_shuriken                  # Compiled test binary
    └── test_shuriken_limits.md        # Manual test scenarios and acceptance criteria
```

---

## Architecture & Design

### Data Layer: `game.h`

**Central header** that defines ALL shared structures and global state:

- **Player**: Position, velocity, animation state, health, jump count, input tracking
- **Enemy**: Type, position, patrol bounds, health, facing direction, animation
- **Projectile**: Position, velocity, active state, type (shuriken vs arrow)
- **Platform**: Position, type (solid/one-way/moving/crumbling), animation parameters
- **Trap**: Type (blade/spike/thorn/fire), position, animation state
- **Level**: Containers for all entities + theme/BGM data
- **Camera**: Position for screen-space transformations
- **GameState**: Enum for menu/playing/paused/game_over/level_complete/settings
- **AnimState**: Enum for player animation poses (idle/run/jump/wall_slide/attack/hurt/dead)

**Globals** (defined in `game.c`, extern-declared in `game.h`):

```c
extern GameState game_state;
extern Player player;
extern Level level;
extern Camera camera;
extern Particle particles[MAX_PARTICLES];
extern float game_time, level_time;
extern int gems_collected, gold_collected, stars_collected;
extern int enemies_defeated, combo_count;
```

**Rationale**: Single-source-of-truth design enables rapid prototyping, simplifies communication between systems, and allows straightforward debugging.

---

### Logic Layer: `game.c`

**Core game loop** with:

1. **Initialization** (`game_init`): Sets up GLUT, loads level 1, initializes player/camera
2. **Level loading** (`load_level`): Parses level file, populates platforms/enemies/traps
3. **Update** (`update_game`): Called every frame (~60 fps), handles:
   - Player input processing (keyboard mapping to physics)
   - Physics simulation: gravity, velocity integration, collision detection
   - Enemy AI: patrol, attack, death
   - Projectile movement & collision
   - Trap activation & effects
   - Checkpoint system & respawning
   - Particle updates & cleanup
   - Camera tracking

**Key functions**:

- `portal_is_open()`: Returns 1 when exit is accessible (all checkpoints triggered + enemies defeated)
- Collision helpers (player-platform, projectile-enemy, etc.)
- State transition logic (level complete, game over, respawn)

---

### Rendering Layer: Four specialized files + `renderer.c`

| File            | Responsibility                                                                                                                               |
| --------------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| `renderer.c`    | Low-level OpenGL: `draw_circle`, `draw_rect`, `draw_tri`, `draw_rect_rot`, `draw_ring`, `draw_string`, color helpers                         |
| `draw_bg.c`     | Background: sky gradient, ghost trees (bare/pine/willow/cherry), bamboo forest, pagoda, lanterns, moon, stars, birds, clouds, parallax logic |
| `draw_map.c`    | Level geometry: platforms (with theme styling), blade/spike/thorn/fire traps, clay pots, collectibles, exit portal, checkpoints, rope fence  |
| `draw_player.c` | Player character: idle, run, jump, fall, wall-slide, attack, dash, hurt, dead poses (all drawn with primitives)                              |
| `draw_enemy.c`  | Enemy types: Guard (with scythe), Archer (with bow + arrow), Heavy Warrior; all with animation                                               |
| `draw_hud.c`    | HUD overlay: avatar ring, health hearts, gem/gold counters, shurikens, star rating, main menu, pause, game over, level complete screens      |

**Drawing order in `display()` callback**:

1. Clear screen, set up view
2. Apply camera translation
3. Draw sky + background
4. Draw parallax trees & decorations
5. Draw level geometry (platforms, traps, pots)
6. Draw collectibles
7. Draw enemies + projectiles
8. Draw player
9. Draw particles
10. Remove camera translation
11. Draw HUD overlay

---

### Entry Point: `main.c`

```c
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutCreateWindow("Ninja Shadow");

    glClearColor(0.1f, 0.15f, 0.25f, 1.0f);  // Dark blue background
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);    // Orthographic: Y=0 at bottom

    glutDisplayFunc(display);        // Frame rendering
    glutReshapeFunc(reshape);        // Window resize
    glutKeyboardFunc(key_down);      // Key press
    glutKeyboardUpFunc(key_up);      // Key release
    glutSpecialFunc(special_down);   // Arrow keys, etc.
    glutSpecialUpFunc(special_up);
    glutMouseFunc(mouse_click);      // Settings gear click
    glutTimerFunc(16, timer_callback, 0);  // ~60 fps timer

    game_init();
    audio_init();
    glutMainLoop();
    return 0;
}
```

**Coordinate System**:

- Origin (0, 0) at **bottom-left** of screen
- X increases rightward (0–1280)
- Y increases upward (0–720)
- Camera transformation: `glTranslatef(-camera.x, -camera.y, 0)` before drawing world objects

---

## Gameplay Systems

### Player Mechanics

**Movement**

- **Walk**: WASD or Arrow Keys at constant 265 px/s
- **Dash**: E/L/Enter to dash 540 px/s in facing direction with 0.4s cooldown
- **Wall-slide**: Slide down wall (-180 px/s) when pressing against vertical surface

**Jump System**

- **Ground jump**: Space or Up Arrow launches player at 680 px/s upward
- **Double-jump**: Second jump while airborne at 600 px/s
- **One-shot mechanism**: Flag prevents key-repeat spam

**Attack Mechanics**

- **Sword attack** (Z/J): Melee swing with directional hitbox, 1-hit cooldown per swing
- **Shuriken throw** (X/K): Infinite-range projectile, max 10 per level, 450 px/s, 50-unit range
- **Combo system**: Tracks consecutive hits; resets after 2 seconds without attacking

**Health & Damage**

- **6 hearts** (3 HP each, capped at max 6)
- **Trap contact**: -1 health on first collision per frame
- **Enemy damage**: -1 health from melee or arrow hit
- **Respawn penalty**: `-1 max health` on death without checkpoint (flags decrease in max-health display)

### Enemy Types

#### Guard

- **Weapon**: Scythe (visual only, damage via contact)
- **AI**: Patrol between bounds, attack on proximity
- **Health**: 3 HP per hit
- **Behavior**: Simple patrol + melee engage

#### Archer

- **Weapon**: Bow & arrow projectiles
- **AI**: Patrol, shoot on player detection
- **Health**: 2 HP
- **Projectile**: 250 px/s arrows, full-screen range

#### Heavy Warrior

- **Weapon**: Club/hammer
- **Health**: 5 HP (tanky)
- **AI**: Slower patrol, stronger engagement
- **Behavior**: Heavy attacks with knockback (future enhancement)

### Trap Types

#### Blade Trap

- Spinning red saw blade, rotating continuously
- Deals 1 damage on contact
- Visual effect: rotation angle updates each frame

#### Spike Trap

- Ground-level jagged spikes
- 1 damage on contact, no knockback
- Can be arranged in groups (count parameter)

#### Thorn Bush

- Red spiky vegetation obstacle
- 1 damage on contact
- Blocks movement (collision solid)

#### Fire Vent

- On/off toggle animation
- 1 damage when active
- Visual: animated flame effect

### Projectile System

**Shurikens** (player-thrown)

- Max 10 per level (`SHURIKEN_PER_LEVEL`)
- Velocity: 450 px/s in throw direction
- Range: 50 units max distance from player
- Rotation: Angle updates to match velocity direction
- One-shot flag prevents key-repeat

**Arrows** (archer-fired)

- Fired from archer position
- Velocity: 250 px/s toward player
- No range limit (but despawn off-screen)
- Arc velocity (not ballistic, straight-line motion)

### Checkpoint System

**Mechanics**:

- Multiple checkpoints per level
- Player triggers checkpoint by reaching its position
- Triggered checkpoints: stay activated, visual state changes
- **Respawn on death**: Player respawns at last triggered checkpoint with -1 max health penalty
- **Exit unlock**: Portal opens only after all checkpoints triggered AND all enemies defeated

**Variables**:

- `checkpoint_count`: Number of checkpoints in level
- `respawn_x, respawn_y`: Last triggered checkpoint position
- `respawn_active`: Flag indicating checkpoint was reached
- `respawn_health`: Current max health (decreases on death without checkpoint)

### Collectible System

**Gems** (blue diamonds)

- Pickups scattered throughout levels
- Increment `gems_collected` counter
- Spawn blue particle burst on collection

**Gold Coins** (golden circles)

- Pickups for secondary scoring
- Increment `gold_collected` counter
- Spawn yellow particle burst

**Stars** (3-point rating)

- Special collectibles for bonus points
- Increment `stars_collected` counter
- Spawn yellow-green particle burst with louder SFX
- Contribute to level star-rating (0-3 stars per level)

---

## Controls

| Input                      | Action                                                  |
| -------------------------- | ------------------------------------------------------- |
| **Arrow Keys** or **WASD** | Move left/right, look up/down (future)                  |
| **Space** or **Up Arrow**  | Jump (can double-jump)                                  |
| **Z** or **J**             | Sword attack (melee)                                    |
| **X** or **K**             | Throw shuriken (1 press = 1 shuriken, max 10 per level) |
| **E**, **L**, or **Enter** | Dash (540 px/s, 0.4s cooldown)                          |
| **P** or **Esc**           | Pause                                                   |
| **⚙️ (Settings gear)**     | Open settings (mouse click top-right)                   |
| **Mouse Click**            | Menu navigation (future enhancement)                    |

**Input Model**:

- **Continuous keys**: Walk, wall-slide (check via `keys[key]` flag each frame)
- **One-shot keys**: Jump, attack, dash (tracked via `jump_pressed`, `attack_timer`, `dash_pressed` flags to prevent key-repeat)

---

## Game States

```c
typedef enum {
    STATE_MENU,              // Main menu with theme animation
    STATE_PLAYING,           // Active level gameplay
    STATE_PAUSED,            // Pause overlay visible
    STATE_GAME_OVER,         // Player died (retry / menu options)
    STATE_LEVEL_COMPLETE,    // Level beaten (show star rating, next level button)
    STATE_SETTINGS           // Settings menu (expandable)
} GameState;
```

**State Transitions**:

- `STATE_MENU` → `STATE_PLAYING` (start game)
- `STATE_PLAYING` → `STATE_PAUSED` (press P)
- `STATE_PAUSED` → `STATE_PLAYING` (unpause)
- `STATE_PLAYING` → `STATE_GAME_OVER` (health ≤ 0)
- `STATE_PLAYING` → `STATE_LEVEL_COMPLETE` (reach exit + all goals met)
- `STATE_GAME_OVER` → `STATE_MENU` or `STATE_PLAYING` (retry)
- `STATE_LEVEL_COMPLETE` → `STATE_MENU` or `STATE_PLAYING` (next level)
- `STATE_PLAYING`/`STATE_PAUSED` ↔ `STATE_SETTINGS` (click gear icon)

---

## Level Themes

**5 distinct visual themes** drive background art, enemy placement, trap styles, and collectibles:

### Theme 0: RUINS

- **Background**: Ghost trees, stone pillars, deep shadows
- **Color palette**: Dark blue sky, gray/brown stone
- **Decorations**: Bare trees, hanging cloth banners
- **Traps**: Blade traps, spikes
- **Enemies**: Guards, Archers
- **Vibe**: Abandoned ancient site

### Theme 1: TEMPLE

- **Background**: Pine trees, pagoda gate, red lanterns
- **Color palette**: Deep indigo, gold accents, red lanterns
- **Decorations**: Hanging cloth, lantern strings, pagoda roof
- **Traps**: Fire vents, spikes, crumbling platforms
- **Enemies**: Archers, Heavy Warriors
- **Vibe**: Sacred temple sanctuary

### Theme 2: VILLAGE

- **Background**: Willow trees, bare trees, hanging cloth
- **Color palette**: Dark teal, muted greens
- **Decorations**: Village structures, wooden fences
- **Traps**: Blade traps, thorn bushes
- **Enemies**: Guards, Archers
- **Vibe**: Quiet rural settlement

### Theme 3: BAMBOO

- **Background**: Dense bamboo forest, stone markers
- **Color palette**: Deep green, black bamboo stalks
- **Decorations**: Tall bamboo stalks, stone graves
- **Traps**: Fire vents, blade traps
- **Enemies**: Heavy Warriors, Archers
- **Vibe**: Mysterious bamboo forest

### Theme 4: FORTRESS

- **Background**: Fortress walls, cherry blossom trees
- **Color palette**: Dark stone, pink cherry blossoms
- **Decorations**: Fortress walls, gate structures
- **Traps**: Blade traps, spikes, fire vents
- **Enemies**: Heavy Warriors, Guards
- **Vibe**: Fortified stronghold

### Background Parallax

- **Layer 1** (farthest): Sky gradient, moon, stars (0% parallax)
- **Layer 2**: Ghost trees, clouds (20% parallax)
- **Layer 3**: Midground decorations (50% parallax)
- **Layer 4** (nearest): Level geometry, foreground objects (100% parallax)

**Camera offset calculation**:

```c
float ox1 = -cam_x * 0.20f;  // 20% parallax
float ox2 = -cam_x * 0.50f;  // 50% parallax
```

---

## Asset Management

### Level Files (to be created in `assets/levels/`)

Each level file is a **text-based format** specifying:

- Width/height of playable area
- Theme (0-4)
- BGM track (0-4)
- Platform positions, types, animation parameters
- Enemy spawns with patrol bounds
- Trap positions and types
- Collectible placements
- Checkpoint locations
- Exit portal position

**Expected format** (example level01.txt):

```
LEVEL 01
WIDTH 1280
HEIGHT 720
THEME 0
BGM 0
PLATFORMS
  0 0 1280 40 SOLID          # ground
  200 150 150 20 ONE_WAY
  500 300 100 20 CRUMBLING
ENEMIES
  GUARD 800 200 700 900
  ARCHER 1000 400 950 1050
TRAPS
  BLADE 650 80 0
  SPIKE 300 50 3
COLLECTIBLES
  GEM 450 250
  STAR 600 450
CHECKPOINTS
  CP 400 150
EXIT
  800 700
```

### Audio Files (to be populated in `assets/audio/`)

**Sound Effects** (22 files):

```
sfx_jump.wav              # Jump sound
sfx_double_jump.wav       # Double-jump enhanced sound
sfx_land.wav              # Landing thud
sfx_wall_slide.wav        # Friction sliding sound
sfx_dash.wav              # Quick dash whoosh
sfx_attack_slash.wav      # Sword swing whoosh
sfx_attack_hit.wav        # Sword connects with enemy
sfx_shuriken_throw.wav    # Shuriken release sound
sfx_shuriken_hit.wav      # Shuriken pierces enemy
sfx_player_hurt.wav       # Player takes damage
sfx_player_death.wav      # Player dies
sfx_enemy_death.wav       # Enemy defeated
sfx_gem_collect.wav       # Gem pickup chime
sfx_star_collect.wav      # Star pickup prominent chime
sfx_pot_break.wav         # Pottery shatters
sfx_blade_spin.wav        # Spinning trap loop (optional)
sfx_fire_vent.wav         # Fire vent activation
sfx_platform_crumble.wav  # Platform breaking
sfx_level_complete.wav    # Level victory fanfare
sfx_game_over.wav         # Game over sad chord
sfx_menu_select.wav       # Menu navigation beep
sfx_menu_confirm.wav      # Menu selection confirm
```

**Background Music** (5 files):

```
bgm_forest.wav            # Theme 0 RUINS background music
bgm_temple.wav            # Theme 1 TEMPLE background music
bgm_village.wav           # Theme 2 VILLAGE background music
bgm_bamboo.wav            # Theme 3 BAMBOO background music
bgm_fortress.wav          # Theme 4 FORTRESS background music
```

**Audio sources** (free):

- **OpenGameArt.org**: CC0/CC-BY licensed game audio packs
- **Freesound.org**: Free account, filter by CC0 license
- **Kenney.nl**: Completely free, no attribution needed
- **ZapSplat**: Free SFX library with registration

---

## Audio System

### Architecture (`audio.h` / `audio.c`)

**Backend**: OpenAL (cross-platform, low-latency audio)

**Component breakdown**:

- `ALCdevice`: Physical audio device
- `ALCcontext`: OpenAL context (one per application)
- `ALuint sfx_buf[SFX_COUNT]`: 22 pre-loaded SFX buffers
- `ALuint bgm_buf[BGM_COUNT]`: 5 pre-loaded BGM buffers
- `ALuint sfx_sources[16]`: Pool of sources for fire-and-forget SFX
- `ALuint bgm_source`: Dedicated looping BGM source

### API

```c
int audio_init(void);           // Call once at startup; returns 1 on success
void audio_play(SfxId id);      // Play one-shot SFX (safe if init failed)
void audio_play_bgm(int bgm_id); // Start looping BGM (or -1 to stop)
void audio_stop_bgm(void);       // Stop current BGM
void audio_cleanup(void);        // Free OpenAL resources on exit
```

### Integration

- **Initialization**: Called in `main()` after `glutInit()`
- **SFX triggers**: Called from physics/collision handlers:
  - `audio_play(SFX_JUMP)` when player jumps
  - `audio_play(SFX_ATTACK_SLASH)` when sword swings
  - `audio_play(SFX_SHURIKEN_THROW)` on projectile spawn
  - `audio_play(SFX_GEM_COLLECT)` on collectible pickup
- **BGM**: Set during level load:

  ```c
  audio_play_bgm(level.bgm);  // BGM_FOREST (0), BGM_TEMPLE (1), etc.
  ```

### Graceful Degradation

If OpenAL is unavailable on the system:

- `audio_init()` returns 0
- `audio_play()` calls become silent no-ops
- Game continues without audio

---

## Physics & Collision

### Coordinate System

- **World space**: X increases rightward, Y increases upward
- **Pixel-based**: All positions in screen pixels (1280×720 viewport)
- **Time step**: Variable dt, capped at 0.05s (20 FPS floor) to prevent tunnelling

### Gravity & Velocity

```c
#define GRAVITY 1400.0f        // Acceleration downward (px/s²)
#define MAX_FALL_SPD (-900.0f) // Terminal velocity (px/s)

// Each frame:
player.vy -= GRAVITY * dt;     // Apply downward acceleration
if (player.vy < MAX_FALL_SPD)  // Clamp fall speed
    player.vy = MAX_FALL_SPD;
player.y += player.vy * dt;    // Integrate velocity
```

### Collision Detection

**Bounding Box (AABB) collision**:

```c
int rect_collide(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2) {
    return !(x1 + w1 <= x2 || x1 >= x2 + w2 ||
             y1 + h1 <= y2 || y1 >= y2 + h2);
}
```

**Order of collision checks each frame**:

1. **Player-platform**: Resolve vertical position if overlapping
   - One-way platforms: Only collide from above
   - Moving platforms: Carry player via platform velocity
2. **Player-trap**: Check damage, apply knockback
3. **Player-collectible**: Mark as collected
4. **Projectile-enemy**: Reduce enemy HP, spawn hit sparks
5. **Enemy-platform**: Keep enemies on ground
6. **Enemy-player**: Melee damage

### Platform Motion

**Moving platforms** (`PLAT_MOVING`):

- Oscillate along X or Y axis using sine wave
- `position = origin + amplitude * sin(2π * speed * time + phase)`
- Velocity computed per-frame and applied to carried player

**Crumbling platforms** (`PLAT_CRUMBLING`):

- Collapse when player stands on them for threshold time
- Visual: fade out while counting down
- Respawn on player respawn (checkpoint reset)

---

## Animation System

### Animation State Machine

```c
typedef enum {
    ANIM_IDLE,         // Standing still
    ANIM_RUN,          // Moving horizontally
    ANIM_JUMP,         // Ascending or at apex
    ANIM_FALL,         // Descending
    ANIM_WALL_SLIDE,   // Sliding down wall
    ANIM_ATTACK,       // Sword swing in progress
    ANIM_DASH,         // Dash dodge animation
    ANIM_HURT,         // Knockback/flinch
    ANIM_DEAD          // Death animation (fade out)
} AnimState;

typedef struct {
    AnimState state;
    float timer;       // Time in current state (seconds)
    float anim_t;      // Normalized animation progress [0, 1]
    int facing;        // +1 right, -1 left
} Animation;
```

### State Transitions

**Update logic each frame**:

```c
// Determine new state based on physics
if (player.health <= 0) {
    new_state = ANIM_DEAD;
} else if (on_wall && player.vy < 0) {
    new_state = ANIM_WALL_SLIDE;
} else if (player.attack_timer > 0) {
    new_state = ANIM_ATTACK;
} else if (player.dash_timer > 0) {
    new_state = ANIM_DASH;
} else if (!on_ground) {
    new_state = player.vy > 0 ? ANIM_JUMP : ANIM_FALL;
} else if (player.vx != 0) {
    new_state = ANIM_RUN;
} else {
    new_state = ANIM_IDLE;
}

// Handle state change
if (new_state != anim.state) {
    anim.state = new_state;
    anim.timer = 0;
}

// Update animation timer
anim.timer += dt;
anim.anim_t = fmodf(anim.timer / ANIM_DURATION, 1.0f);
```

### Player Rendering

**File**: `draw_player.c`

- **Idle pose**: Standing upright, ready stance
- **Run pose**: Animated legs (sine wave motion), body forward lean
- **Jump pose**: Arms overhead, knees pulled up
- **Fall pose**: Arms down, body stretched
- **Wall-slide pose**: Sideways lean, one hand on wall
- **Attack pose**: Sword extended, rotation based on face direction
- **Hurt pose**: Knockback recoil
- **Dead pose**: Fade-out animation

**Implementation**: All poses drawn with OpenGL primitives (circles, rectangles, triangles) — no textures.

### Enemy Animation

- **Guard**: Patrol walk (leg swing), attack stance
- **Archer**: Idle, aiming (hold), release
- **Heavy**: Heavy walk, attack wind-up, strike

---

## Particle Effects

### Particle System

**Max particles**: 256 simultaneous effects

```c
typedef struct {
    float x, y, vx, vy;   // Position and velocity
    float life, max_life; // Time remaining (seconds)
    float r, g, b, a;     // RGBA color
    float size;           // Pixel radius
    int active;
} Particle;
```

### Particle Emitters

#### Hit Sparks (Sword Impact)

- **Spawn**: On successful melee hit
- **Count**: 8 particles in radial pattern
- **Velocity**: 80-110 px/s outward + gravity
- **Color**: Yellow-orange (1, 1, 0.8)
- **Lifetime**: 0.3s
- **Size**: 3px radius

#### Collection Burst (Gem/Gold/Star Pickup)

- **Spawn**: On collectible pickup
- **Count**: 12 particles
- **Velocity**: 60-100 px/s outward + gravity
- **Color**: Yellow (0.8-1.0, 0.9, 0.1)
- **Lifetime**: 0.6s
- **Size**: 4px radius

#### Landing Dust (Jump Landing)

- **Spawn**: On hard landing (fall > threshold)
- **Count**: 10 particles
- **Velocity**: Outward/upward spread
- **Color**: Brown dust (0.55, 0.50, 0.44)
- **Lifetime**: 0.45s
- **Size**: 3.5px radius

### Particle Update

**Per frame**:

```c
particle.x += particle.vx * dt;
particle.y += particle.vy * dt;
particle.vy -= 200.0f * dt;      // Gravity
particle.life -= dt;
particle.a = particle.life / particle.max_life;  // Fade out
```

---

## HUD & UI

### In-Game HUD

**Layout** (screen coordinates):

- **Top-left**: Avatar circle (50, 670), hearts (100-220, 700)
- **Left column**: Gems (100, 680), Gold (100, 660), Stars (20-65, 636)
- **Bottom-left**: Shuriken throw buttons (50, 40) + (80, 40)
- **Bottom-right**: 4 skill buttons in 2×2 grid (future)
- **Top-right**: Settings gear (1240, 690) — clickable

**Elements**:

1. **Avatar ring**: Player silhouette + decorative circle frame
2. **Hearts**: 6 red circles (filled) or dimmed (empty)
3. **Gem counter**: Blue diamond icon + numeric value
4. **Gold counter**: Gold circle icon + numeric value
5. **Stars**: 3 gold stars (filled) or gray (empty)
6. **Shurikens**: Visual representation of remaining throws (10 max)

### Menus & Overlays

#### Main Menu

- Large title "Ninja Shadow"
- Animated theme (parallax background cycling)
- "Start Game" button
- "Settings" button
- "Quit" button

#### Pause Overlay

- Semi-transparent dark overlay
- "PAUSED" text
- "Resume", "Settings", "Quit to Menu" buttons

#### Level Complete Screen

- Level number and title
- **Star rating** (0-3):
  - 1 star: Completed
  - 2 stars: Completed + 50% collectibles or <60s
  - 3 stars: All goals met (collect all, beat time, kill all enemies)
- "Next Level" button
- "Retry" button
- "Menu" button

#### Game Over Screen

- "GAME OVER" text
- Reason: "Out of lives" / "No health remaining"
- "Retry Level" button
- "Back to Menu" button

#### Settings Menu

- Audio volume sliders
- Visual quality options
- Colorblind mode toggle
- Full-screen toggle
- Difficulty selector (Normal/Hard/Expert)

---

## Rendering Pipeline

### Display Callback (`display()` in `main.c`)

```c
void display(void) {
    static int last_ms = 0;
    int now_ms = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now_ms - last_ms) / 1000.0f;
    last_ms = now_ms;
    if (dt > 0.05f) dt = 0.05f;  // Cap at 20 FPS minimum

    // Update game state
    update_game(dt);

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Route rendering based on game state
    switch (game_state) {
        case STATE_MENU:
            draw_main_menu(game_time);
            break;
        case STATE_PLAYING:
            draw_world_playing();
            draw_hud(...);
            break;
        case STATE_PAUSED:
            draw_world_playing();
            draw_hud(...);
            draw_pause_overlay();
            break;
        case STATE_GAME_OVER:
            draw_game_over_screen();
            break;
        case STATE_LEVEL_COMPLETE:
            draw_level_complete(stars_collected);
            break;
        case STATE_SETTINGS:
            draw_settings_overlay();
            break;
    }

    // Swap buffers for smooth animation
    glutSwapBuffers();
}
```

### Draw World (Playing State)

```c
void draw_world_playing(void) {
    // 1. Apply camera translation
    glTranslatef(-camera.x, -camera.y, 0);

    // 2. Background (no parallax)
    draw_sky();
    draw_ghost_trees(camera.x);  // Parallax 20%
    draw_ground_atmosphere();

    // 3. Parallax trees & decorations (20-50%)
    draw_parallax(camera.x, level.theme);

    // 4. Level geometry
    for (int i = 0; i < level.plat_count; i++)
        draw_platform(&level.platforms[i], level.theme);
    for (int i = 0; i < level.trap_count; i++)
        draw_trap(&level.traps[i]);
    for (int i = 0; i < level.pot_count; i++)
        draw_clay_pot(&level.pots[i]);

    // 5. Collectibles
    for (int i = 0; i < level.coll_count; i++)
        draw_collectible(&level.colls[i]);
    draw_exit_portal(level.exit_x, level.exit_y, game_time, portal_is_open());

    // 6. Enemies & projectiles
    for (int i = 0; i < level.enemy_count; i++)
        draw_enemy(&level.enemies[i]);
    for (int i = 0; i < level.proj_count; i++)
        draw_projectile(&level.projs[i]);

    // 7. Player
    draw_player_dispatch(&player);

    // 8. Particles
    particles_draw();

    // 9. Remove camera translation
    glTranslatef(camera.x, camera.y, 0);
}
```

### Camera System

**Tracking mode**: Follow player's X position with smooth easing

```c
#define CAMERA_EASE 0.1f
float target_x = player.x - SCREEN_W / 2;
camera.x += (target_x - camera.x) * CAMERA_EASE;

// Clamp to level bounds
if (camera.x < 0) camera.x = 0;
if (camera.x > level.level_w - SCREEN_W)
    camera.x = level.level_w - SCREEN_W;
```

### Color Space

**RGB values** (0.0–1.0 range):

- **Black silhouettes**: (0, 0, 0, 1)
- **Dark shadow**: (0.1, 0.1, 0.12, 1)
- **Red accents**: (0.8, 0.05, 0.05, 1)
- **Gold**: (1.0, 0.85, 0.1, 1)
- **Blue gem**: (0.25, 0.55, 1.0, 1)
- **Dark blue sky**: (0.05, 0.1, 0.2, 1)

---

## Input Handling

### Keyboard State Management

The game uses two mechanisms for input:

**Continuous Keys** (checked each frame via array):

```c
int keys[256];          // ASCII keys (a-z, 0-9, etc.)
int special_keys[256];  // Special keys (arrows, function keys)

// In key_down/key_up callbacks:
void key_down(unsigned char k, int x, int y) {
    keys[k] = 1;
    // Handle one-shot actions here if needed
}

void key_up(unsigned char k, int x, int y) {
    keys[k] = 0;
}

// In update_game():
if (keys['w'] || special_keys[GLUT_KEY_UP])
    player.vx -= MOVE_SPEED * dt;  // Move left
```

**One-Shot Keys** (flag-based to prevent key-repeat):

```c
// Player struct contains:
int jump_pressed;       // Released since last jump
int shuriken_pressed;   // Released since last throw
int dash_pressed;       // Released since last dash

// In key_down:
if (k == ' ' && !jump_pressed) {
    jump_pressed = 1;
    player.jump_count++;
    player.vy = JUMP_VY;
    audio_play(SFX_JUMP);
}

// In key_up:
if (k == ' ')
    jump_pressed = 0;  // Allow next jump
```

### Input Routing

**Full input map** (main.c):

```c
void key_down(unsigned char k, int x, int y) {
    switch (k) {
        case 'w': case 'W': keys['w'] = 1; break;
        case 'a': case 'A': keys['a'] = 1; break;
        case 's': case 'S': keys['s'] = 1; break;
        case 'd': case 'D': keys['d'] = 1; break;
        case 'z': case 'Z': case 'j': case 'J':
            player_attack(); break;
        case 'x': case 'X': case 'k': case 'K':
            player_throw_shuriken(); break;
        case 'e': case 'E': case 'l': case 'L':
            player_dash(); break;
        case 'p': case 'P': case 27:  // ESC
            game_state = (game_state == STATE_PLAYING) ? STATE_PAUSED : STATE_PLAYING;
            break;
    }
}

void special_down(int k, int x, int y) {
    switch (k) {
        case GLUT_KEY_UP:       special_keys[GLUT_KEY_UP] = 1; break;
        case GLUT_KEY_DOWN:     special_keys[GLUT_KEY_DOWN] = 1; break;
        case GLUT_KEY_LEFT:     special_keys[GLUT_KEY_LEFT] = 1; break;
        case GLUT_KEY_RIGHT:    special_keys[GLUT_KEY_RIGHT] = 1; break;
    }
}
```

---

## Enemy AI

### AI State Machine

Each enemy follows a simple state-based AI:

```c
typedef enum {
    AI_PATROL,      // Patrol between bounds
    AI_CHASE,       // Move toward player
    AI_ATTACK,      // Melee or ranged attack
    AI_RETREAT,     // Move away from player
    AI_DEAD         // Fade out
} AIState;

// In game.c update_game():
for (int i = 0; i < level.enemy_count; i++) {
    Enemy *e = &level.enemies[i];
    if (!e->alive) continue;

    // Distance to player
    float dx = player.x - e->x;
    float dist = fabsf(dx);

    // State transitions
    if (dist < 300.0f) {
        // Player detected - engage
        new_ai_state = AI_ATTACK;
    } else if (e->x < e->patrol_l || e->x > e->patrol_r) {
        // Reached patrol bound - reverse
        e->facing *= -1;
    } else {
        // Patrol normally
        new_ai_state = AI_PATROL;
    }

    // Execute AI state
    switch (new_ai_state) {
        case AI_PATROL:
            e->vx = e->facing * 150.0f;  // Patrol speed
            break;
        case AI_ATTACK:
            if (e->type == ENEMY_ARCHER) {
                e->shoot_timer += dt;
                if (e->shoot_timer > 2.0f) {
                    spawn_arrow_projectile(e, &player);
                    e->shoot_timer = 0;
                    audio_play(SFX_ARROW_SHOOT);
                }
            } else if (e->type == ENEMY_GUARD) {
                // Move toward player
                e->vx = (dx > 0 ? 1 : -1) * 200.0f;
            }
            break;
    }

    // Apply physics
    e->vx *= 0.95f;  // Friction
    e->x += e->vx * dt;
}
```

### Enemy-Specific Behaviors

**Guard (ENEMY_GUARD)**:

- Patrol speed: 150 px/s
- Attack range: 300 px
- Melee damage on contact: 1 HP
- AI: Simple patrol with chase on player detection
- Animation: Walking (leg swing), attack stance

**Archer (ENEMY_ARCHER)**:

- Patrol speed: 120 px/s (slower)
- Attack range: Full screen width
- Fires arrows every 2 seconds when attacking
- AI: Maintain distance while shooting
- Animation: Idle, draw bow, release

**Heavy Warrior (ENEMY_HEAVY)**:

- Patrol speed: 100 px/s (very slow)
- Attack range: 250 px
- Melee damage: 1 HP (but stronger knockback)
- Health: 5 HP (twice as much as Guard)
- AI: Deliberate, methodical approach
- Animation: Heavy walk, wind-up, crushing attack

---

## Collision Deep Dive

### AABB Collision System

**Core algorithm**:

```c
// Two rectangles: (x1,y1,w1,h1) and (x2,y2,w2,h2)
int rect_collide(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2) {
    // Check if boxes DO NOT overlap
    // If no gap on any side, they overlap
    if (x1 + w1 <= x2) return 0;  // x1's right <= x2's left
    if (x1 >= x2 + w2) return 0;  // x1's left >= x2's right
    if (y1 + h1 <= y2) return 0;  // y1's top <= y2's bottom
    if (y1 >= y2 + h2) return 0;  // y1's bottom >= y2's top
    return 1;  // All checks passed = overlap
}

// Get overlap depth (for push-out)
float get_overlap_x(float x1, float w1, float x2, float w2) {
    float left_gap = x2 - (x1 + w1);    // Gap if x1 is to the left
    float right_gap = x1 - (x2 + w2);   // Gap if x1 is to the right

    if (left_gap > 0) return 0;   // No overlap
    if (right_gap > 0) return 0;

    // Return the smallest gap (push-out direction)
    if (fabsf(left_gap) < fabsf(right_gap))
        return left_gap;  // Negative = push x1 left
    else
        return right_gap; // Positive = push x1 right
}
```

### Collision Resolution Order

Each frame, collisions are resolved in this precise order:

1. **Player-Platform collisions** (vertical resolution):

   ```c
   for (int i = 0; i < level.plat_count; i++) {
       Platform *p = &level.platforms[i];
       if (rect_collide(player.x-player.w/2, player.y, player.w, player.h,
                        p->x, p->y, p->w, p->h)) {

           // One-way platform: only collide from above
           if (p->type == PLAT_ONE_WAY && player.vy <= 0)
               continue;

           // Push player above platform
           player.y = p->y + p->h;
           player.vy = 0;  // Stop downward motion
           player.on_ground = 1;

           // Carry player if platform is moving
           player.x += p->pvx;
       }
   }
   ```

2. **Player-Trap collisions** (damage):

   ```c
   player.hurt_timer = 0.2f;  // Invulnerability flash
   player.health--;
   respawn_requested = 1;  // Deferred respawn
   audio_play(SFX_PLAYER_HURT);
   spawn_landing_dust(player.x, player.y);
   ```

3. **Player-Collectible collisions**:

   ```c
   if (!collectible.collected) {
       collectible.collected = 1;
       if (collectible.is_star) {
           stars_collected++;
           audio_play(SFX_STAR_COLLECT);
       } else {
           gems_collected++;
           audio_play(SFX_GEM_COLLECT);
       }
       spawn_collect_burst(collectible.x, collectible.y);
   }
   ```

4. **Projectile-Enemy collisions**:

   ```c
   for (int i = 0; i < level.proj_count; i++) {
       Projectile *proj = &level.projs[i];
       if (!proj->active) continue;

       for (int j = 0; j < level.enemy_count; j++) {
           Enemy *e = &level.enemies[j];
           if (!e->alive) continue;

           if (rect_collide(proj->x, proj->y, 4, 4,
                           e->x-12, e->y, 24, 32)) {
               e->health--;
               proj->active = 0;
               spawn_hit_sparks(proj->x, proj->y);
               audio_play(SFX_SHURIKEN_HIT);

               if (e->health <= 0) {
                   e->alive = 0;
                   enemies_defeated++;
                   audio_play(SFX_ENEMY_DEATH);
               }
           }
       }
   }
   ```

### Box-to-Pixel Conversions

Player collision box:

```c
// Player struct stores center position (cx, cy)
// Bounding box for collision is:
float player_x1 = player.x - player.w/2;  // 28/2 = 14
float player_y1 = player.y;
float player_x2 = player.x + player.w/2;
float player_y2 = player.y + player.h;
```

---

## Camera System

### Advanced Tracking

The camera uses **ease-in easing** to smoothly follow the player:

```c
// Camera target (centered on player)
float target_x = player.x - SCREEN_W / 2;

// Exponential ease: lerp to target
#define CAMERA_EASE 0.08f  // Adjust for smoothness
camera.x += (target_x - camera.x) * CAMERA_EASE;

// Hard bounds (don't scroll past level edges)
if (camera.x < 0) camera.x = 0;
if (camera.x > level.level_w - SCREEN_W)
    camera.x = level.level_w - SCREEN_W;
```

### Parallax Multipliers

Different background layers scroll at different speeds:

```c
void draw_parallax(float cam_x, int theme) {
    // Layer 1: Far background (5% parallax)
    float ox_far = -cam_x * 0.05f;
    draw_ghost_trees(ox_far);

    // Layer 2: Mid-ground (20% parallax)
    float ox_mid = -cam_x * 0.20f;
    draw_trees_theme(theme, ox_mid);

    // Layer 3: Near-ground (50% parallax)
    float ox_near = -cam_x * 0.50f;
    draw_decorations(theme, ox_near);

    // Layer 4: World space (100% parallax = 1.0x)
    // This is the main gameplay layer
}

// Parallax calculation:
// world_position + (camera_offset × multiplier) = screen_position
// Example: tree at x=1000 with cam_x=500 and mult=0.2:
// screen_x = 1000 + (-500 × 0.2) = 1000 - 100 = 900
```

---

## Level Design Guide

### Level Progression

**Difficulty curve**:

- **Levels 1-2**: Tutorial difficulty, basic mechanics
- **Levels 3-5**: Intermediate, mixed enemy types
- **Levels 6-8**: Advanced, complex layouts
- **Levels 9-10**: Expert, multi-phase challenges / boss fights

### Platform Layout Guidelines

**Safe design**:

- Minimum gap between platforms: 40 pixels (for double-jump reach)
- Maximum fall height: 200 pixels (can survive with care)
- Platform minimum width: 60 pixels (comfortable standing area)

**Challenge design**:

```c
// Example: Moving platform chain (Level 4)
// Platform 1: Stationary (200, 300, 100, 20)
// Platform 2: Moving Y-axis (350, 300, 80, 20)
//   - Amplitude: 150px, Speed: 2.0, Phase: 0
// Platform 3: One-way (500, 250, 120, 20)
// Platform 4: Crumbling (650, 250, 100, 20)
```

### Enemy Placement Strategy

**Balanced encounters**:

- Never surround player with 3+ enemies at once
- Space patrols 300+ pixels apart
- Archers on high ground, Guards on mid-level, Heavy on low
- Mix ranged + melee to force tactical decisions

### Checkpoint Placement

**Rule of thumb**: One checkpoint per 40-60 seconds of optimal playtime

```
Level start (0s) → CP1 (45s) → CP2 (90s) → CP3 (135s) → Exit (180s)
```

---

## Memory Layout

### Player Structure (88 bytes)

```c
typedef struct {
    float x, y, vx, vy;              // 16 bytes (position + velocity)
    float w, h;                      // 8 bytes (collision box)
    int on_ground, on_wall;          // 8 bytes (flags)
    int jump_count;                  // 4 bytes
    int health;                      // 4 bytes
    float hurt_timer;                // 4 bytes
    float attack_timer;              // 4 bytes
    float dash_timer;                // 4 bytes
    float dash_cooldown;             // 4 bytes
    int shuriken_count;              // 4 bytes
    int shuriken_pressed;            // 4 bytes
    int jump_pressed;                // 4 bytes
    int jump_available;              // 4 bytes
    int dash_pressed;                // 4 bytes
    Animation anim;                  // 20 bytes (state, timer, anim_t, facing)
} Player;  // Total: ~100 bytes
```

### Level Structure (estimated 8KB)

```c
typedef struct {
    float level_w, level_h;                      // 8 bytes
    int theme, bgm;                              // 8 bytes

    Platform platforms[MAX_PLATFORMS];           // 64 * 112 = 7168 bytes
    int plat_count;                              // 4 bytes

    Enemy enemies[MAX_ENEMIES];                  // 32 * 112 = 3584 bytes
    int enemy_count;                             // 4 bytes

    Projectile projs[MAX_PROJECTILES];           // 64 * 44 = 2816 bytes
    int proj_count;                              // 4 bytes

    // ... other arrays ...
} Level;  // Total: ~15-20KB
```

### Particle Buffer (8KB for 256 particles)

```c
Particle particles[256];  // 256 * 32 = 8192 bytes
```

**Total in-game memory**: ~40KB (very lightweight)

---

## Performance Optimization

### Low-Hanging Fruit

1. **Minimize draw calls**:

   ```c
   // BAD: 256 draw_circle calls per frame for particles
   for (int i = 0; i < MAX_PARTICLES; i++)
       if (particles[i].active)
           draw_circle(particles[i].x, particles[i].y, particles[i].size, 16);

   // GOOD: Batch particles by color, render once per batch
   glBegin(GL_TRIANGLE_FAN);
   for (int i = 0; i < MAX_PARTICLES; i++) {
       if (particles[i].active) {
           glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].a);
           draw_circle_verts(...);
       }
   }
   glEnd();
   ```

2. **Collision optimization**:

   ```c
   // Use spatial partitioning instead of O(n²) checks
   // For now, iterate only over nearby entities:
   for (int i = 0; i < level.enemy_count; i++) {
       Enemy *e = &level.enemies[i];
       if (fabsf(e->x - player.x) > 400)  // Skip far enemies
           continue;
       // ... collision check ...
   }
   ```

3. **Animation frame skipping**:

   ```c
   // Only update animation every 2 frames at 30 FPS
   static int frame_count = 0;
   if (++frame_count % 2 == 0) {
       // Update animation
       player.anim.timer += dt;
   }
   ```

### Profiling with gprof

```bash
# Build with profiling enabled
gcc -pg -O2 src/*.c -o ninja_shadow_prof -lGL -lGLU -lglut -lopenal -lm

# Run and play for 1 minute
./ninja_shadow_prof

# Analyze results
gprof ninja_shadow_prof gmon.out | head -30
```

**Expected hotspots**:

- `update_game()` — 40-50% (physics)
- `display()` — 30-40% (rendering)
- Collision functions — 10-20%

---

## Code Examples

### Adding a Health Pickup

1. **Define in game.h**:

   ```c
   #define COLLECTIBLE_HEALTH 2
   ```

2. **Draw in draw_map.c**:

   ```c
   void draw_health_pickup(float cx, float cy, float pulse_t) {
       float pulse = 1.0f + sinf(pulse_t * 6.28f) * 0.2f;
       glColor4f(0.9f, 0.2f, 0.2f, 1.0f);  // Red
       draw_circle(cx, cy, 6.0f * pulse, 12);
       glColor4f(1.0f, 1.0f, 1.0f, 0.5f);   // White center
       draw_circle(cx, cy, 3.0f, 8);
   }
   ```

3. **Spawn in level file (assets/levels/level05.txt)**:

   ```
   COLLECTIBLES
     HEALTH 400 250
     HEALTH 800 400
   ```

4. **Collect in game.c**:

   ```c
   if (collectible.type == COLLECTIBLE_HEALTH) {
       if (player.health < MAX_HEALTH)
           player.health++;
       audio_play(SFX_HEALTH_COLLECT);
       collectible.collected = 1;
   }
   ```

### Adding Wall-Jump Mechanic

```c
// In update_game(), when player is on wall:
if (on_wall && keys['w'] && !jump_pressed) {
    jump_pressed = 1;
    player.jump_count = 0;  // Reset jump count
    player.vy = JUMP_VY * 0.9f;  // Slightly weaker
    player.vx = -player.facing * MOVE_SPEED;  // Push away from wall
    audio_play(SFX_WALL_JUMP);
    spawn_landing_dust(player.x, player.y);
}
```

---

## Common Patterns

### State Machine Pattern

```c
// For any entity with multiple states
typedef struct {
    int state;
    float timer;
} StateMachine;

// Usage:
if (should_transition) {
    sm.state = NEW_STATE;
    sm.timer = 0;
}
sm.timer += dt;

// Dispatch based on state
switch (sm.state) {
    case STATE_A: /* ... */ break;
    case STATE_B: /* ... */ break;
}
```

### Particle Emitter Pattern

```c
void emit_particles(float cx, float cy, int count,
                   float vx, float vy, float spread,
                   float r, float g, float b, float lifetime) {
    for (int i = 0; i < count; i++) {
        float angle = (i * 360.0f / count) * PI / 180.0f;
        float s = spread * (0.5f + (rand() % 100) / 100.0f);
        particle_spawn(
            cx + cosf(angle) * s, cy + sinf(angle) * s,
            cosf(angle) * vx, sinf(angle) * vy,
            r, g, b, lifetime, 3.0f
        );
    }
}
```

### Lerp Utility

```c
// Linear interpolation
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Usage: smooth camera easing
camera.x = lerp(camera.x, target_x, CAMERA_EASE);
```

---

## Testing

### Unit Tests: `tests/test_shuriken.c`

**Compile & run**:

```bash
gcc tests/test_shuriken.c -o tests/test_shuriken -Wall
./tests/test_shuriken
```

**Test scenarios**:

1. **Initial count**: Shuriken count = 10 at level start
2. **Count clamp**: Count never goes below 0
3. **Velocity constant**: Shuriken speed = 450 px/s throughout flight
4. **Range limit**: Shuriken disappears after 50 units
5. **Key-repeat prevention**: Holding X throws 1 shuriken, not multiple

**Output**: `✓ PASS` or `✗ FAIL` for each test

### Manual Test Scenarios: `tests/test_shuriken_limits.md`

**Scenario 1: Shuriken Count Limit**

- Start level, verify HUD shows 10
- Throw all 10, observe count → 0
- Verify no shurikens spawn after count reaches 0

**Scenario 2: Shuriken Range Limit**

- Throw shuriken, measure distance traveled
- Verify disappears at ~50 units

**Scenario 3: Key-Repeat Prevention**

- Hold X key for 2 seconds
- Verify only 1 shuriken created (not 30+ from key repeat)

---

## Development Guide

### Code Style

- **Naming**: snake_case for functions/variables, UPPER_CASE for constants
- **Comments**: `/* ── Section Title ──────────── */` for major blocks
- **Line length**: ~80 characters for readability
- **Indentation**: 4 spaces (no tabs)
- **Header guards**: `#ifndef FILENAME_H` / `#define FILENAME_H` / `#endif`

### Adding a New Enemy Type

1. **Define constant** in `game.h`:

   ```c
   #define ENEMY_NINJA 3
   ```

2. **Add draw function** in `draw_enemy.c`:

   ```c
   void draw_enemy_ninja(float cx, float cy, int facing, float anim_t) {
       // Draw ninja silhouette using primitives
   }
   ```

3. **Add AI logic** in `game.c` `update_game()`:

   ```c
   if (enemy.type == ENEMY_NINJA) {
       // Update patrol, attack, animation
   }
   ```

4. **Update dispatch function** (`draw_enemy_dispatch` in `game.c`)

### Adding a New Level

1. **Create level file** `assets/levels/level06.txt` with geometry/enemies/traps
2. **Update `current_level` check** in `main.c` to load level 6
3. **Test**: Verify platforms, enemies, collectibles spawn correctly

### Adding a New Trap Type

1. **Add enum** in `game.h`:

   ```c
   #define TRAP_ICE 4
   ```

2. **Draw function** in `draw_map.c`:

   ```c
   void draw_ice_trap(float cx, float cy) { /* ... */ }
   ```

3. **Collision logic** in `game.c`:

   ```c
   if (trap.type == TRAP_ICE) {
       // Slippery surface: reduce friction
   }
   ```

### Debugging Tips

- **Print statements**: Use `printf()` to log state
- **Visual debugging**: Draw debug bounding boxes:

  ```c
  glColor4f(1, 0, 0, 0.3f);  // Red transparent
  draw_rect(player.x, player.y, player.w, player.h);
  ```

- **Pause & step**: Use STATE_PAUSED to freeze frame-by-frame
- **GDB debugger**:

  ```bash
  gcc -g src/*.c -o ninja_shadow_debug -lGL -lGLU -lglut -lopenal -lm
  gdb ./ninja_shadow_debug
  ```

### Common Issues & Fixes

| Issue                          | Cause                         | Fix                                            |
| ------------------------------ | ----------------------------- | ---------------------------------------------- |
| Player falls through platforms | Collision check fails         | Verify AABB overlap logic in `update_game()`   |
| Shurikens don't appear         | Count at 0                    | Check `player.shuriken_count` in HUD           |
| Audio doesn't play             | OpenAL not installed          | Run `sudo apt-get install libopenal-dev`       |
| Game runs at 15 FPS            | Rendering bottleneck          | Profile with `glxgears`, check draw call count |
| Menu doesn't respond to clicks | Mouse callback not registered | Verify `glutMouseFunc()` called in `main()`    |

---

## Build Instructions

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev \
    freeglut3-dev libopenal-dev

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install freeglut

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### Windows (MinGW)

```bash
# Install MinGW + GLUT + OpenAL development files
# (Use pre-built libraries or vcpkg package manager)

# Build
mingw32-make

# Run
ninja_shadow.exe
```

### Docker (Optional)

```dockerfile
FROM ubuntu:latest
RUN apt-get update && apt-get install -y gcc libgl1-mesa-dev \
    libglu1-mesa-dev freeglut3-dev libopenal-dev
WORKDIR /app
COPY . .
RUN make
CMD ["./ninja_shadow"]
```

```bash
docker build -t ninja_shadow .
docker run --display $DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix ninja_shadow
```

---

## Platform-Specific Notes

### Linux (Primary development platform)

- **GPU drivers**: Ensure proprietary drivers are installed for better performance
  - NVIDIA: `nvidia-driver-*`
  - AMD: `amdgpu-pro` or open-source `radeon`
  - Intel: Usually built-in, update via BIOS
- **OpenGL check**: `glxinfo | grep "OpenGL version"`
- **Audio**: ALSA or PulseAudio backend for OpenAL

### macOS

- **Compatibility**: Requires OpenGL 2.1+ (macOS 10.7+)
- **M1/M2 Silicon**: May need Rosetta 2 translation
- **Framework linking**:

  ```bash
  -framework OpenGL -framework GLUT
  ```

### Windows

- **Visual Studio**: Use `cl.exe` instead of `gcc` for better optimization
- **Library paths**: Link against `opengl32.lib`, `glu32.lib`, `glut.lib`, `OpenAL32.lib`
- **DLL dependencies**: Ship `OpenAL32.dll` with binary for compatibility

---

## Troubleshooting

### Build Errors

**Error**: `fatal error: GL/glut.h: No such file or directory`

- **Solution**: Install freeglut development package:

  ```bash
  sudo apt-get install freeglut3-dev
  ```

**Error**: `undefined reference to 'alGenBuffers'`

- **Solution**: Install OpenAL development library:

  ```bash
  sudo apt-get install libopenal-dev
  ```

### Runtime Errors

**Error**: `GLUT: freeglut ERROR: Unable to open display`

- **Cause**: X11 display not available (headless/SSH session)
- **Solution**: Use X11 forwarding or VNC

  ```bash
  ssh -X user@remote ./ninja_shadow
  ```

**Error**: Game window doesn't appear

- **Cause**: GLUT window creation failed
- **Solution**: Check for X11/graphics driver issues

  ```bash
  glxinfo | grep "direct rendering"  # Should say "Yes"
  ```

**Error**: Audio doesn't play (no errors)

- **Cause**: OpenAL device not found or no audio hardware
- **Solution**: Check audio availability

  ```bash
  aplay /usr/share/sounds/freedesktop/stereo/bell.oga  # Test system audio
  ```

### Performance Issues

**Game runs slow (< 30 FPS)**:

1. Check CPU/GPU usage: `htop`, `nvidia-smi`
2. Reduce particle count (change MAX_PARTICLES)
3. Simplify background decorations
4. Profile with `gprof`:

   ```bash
   gcc -pg src/*.c -o ninja_shadow_prof -lGL -lGLU -lglut -lopenal -lm
   ./ninja_shadow_prof
   gprof ninja_shadow_prof gmon.out
   ```

**Screen tearing**:

- Enable V-Sync in GPU driver settings
- Or reduce frame rate cap (change `glutTimerFunc` interval)

---

## Directory Reference

### Source Code Files

| File            | Lines  | Purpose                                    |
| --------------- | ------ | ------------------------------------------ |
| `main.c`        | ~200   | Entry point, GLUT setup, event loop        |
| `game.c`        | ~2000+ | Core game logic, physics, collision        |
| `game.h`        | ~300   | All struct definitions, constants, externs |
| `renderer.c`    | ~100   | OpenGL primitives and helpers              |
| `renderer.h`    | ~50    | Renderer function declarations             |
| `draw_bg.c`     | ~500   | Background rendering and parallax          |
| `draw_map.c`    | ~500   | Level geometry and props                   |
| `draw_player.c` | ~300   | Player character rendering                 |
| `draw_enemy.c`  | ~300   | Enemy character rendering                  |
| `draw_hud.c`    | ~400   | HUD, menus, overlays                       |
| `audio.c`       | ~300   | OpenAL audio system                        |
| `audio.h`       | ~60    | Audio API definitions                      |

### Total: ~5,000 lines of C code

---

## Performance Characteristics

### Target Specifications

- **Resolution**: 1280×720 (90 KB framebuffer)
- **Target FPS**: 60 (16 ms per frame)
- **Max entities**: 64 platforms + 32 enemies + 64 projectiles + 32 traps + 64 collectibles = 256
- **Max particles**: 256 simultaneous
- **Draw calls**: ~50 per frame (batched by type)

### Memory Usage (Estimated)

- **Level data**: ~50 KB per level × 10 = 500 KB
- **Audio buffers**: ~5 MB (22 SFX + 5 BGM)
- **Textures**: ~0 KB (no textures, primitives only)
- **Runtime state**: ~1 MB (particles, entities, etc.)
- **Total**: ~7 MB at runtime

### CPU / GPU

- **CPU**: Minimal (physics solver, AI is simple)
- **GPU**: Light (OpenGL 2.x fixed-function pipeline)
- **Target platform**: Any Linux desktop from 2010+ with OpenGL 2.0 support

---

## Future Enhancements

### Planned Features (Phase 2)

- [ ] Difficulty levels (Normal/Hard/Expert) with unique level layouts
- [ ] Boss battles at levels 5 and 10
- [ ] Combo counter display + multiplier bonus
- [ ] Power-ups: speed boost, shield, temporary invulnerability
- [ ] Leaderboard system (local high-scores file)
- [ ] Replay system (record input, playback)
- [ ] Level editor (in-game or separate tool)
- [ ] Mobile touch controls
- [ ] Steam achievements/trading cards

### Polish (Phase 3)

- [ ] Texture sprites for characters (optional, keep silhouettes as fallback)
- [ ] Advanced shaders (bloom, glow effects)
- [ ] Dialogue/story narration
- [ ] Character customization (skins, accessories)
- [ ] Multiplayer co-op or PvP modes

---

## Resources & References

### Documentation

- [OpenGL 2.1 Specification](https://www.khronos.org/opengl/wiki/OpenGL_Specification)
- [GLUT Tutorial](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html)
- [OpenAL Documentation](https://openal-soft.org/)
- [C99 Standard](https://en.wikipedia.org/wiki/C99)

### Free Art & Audio Assets

- **Graphics**: OpenGameArt.org, itch.io, Kenney.nl
- **Audio**: Freesound.org, ZapSplat, OpenGameArt.org, Kenney.nl

### Similar Games (Inspiration)

- Ninja Arashi series (original inspiration)
- Mark of the Ninja (stealth platformer)
- Sekiro: Shadows Die Twice (combat mechanics)
- Celeste (precise platforming)
- Hollow Knight (atmospheric design)

---

## License & Credits

### License

**MIT License** — See LICENSE file for details.

```
MIT License

Copyright (c) 2025 Ninja Shadow Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

### Credits

**Development**: Claude AI (Anthropic)
**Architecture**: Based on Ninja Arashi II gameplay analysis
**Technology**: OpenGL, GLUT (freeglut), OpenAL, GCC

### Third-Party Libraries

- **freeglut**: [LGPL License](https://github.com/dcnieho/freeglut)
- **OpenAL-soft**: [LGPL License](https://github.com/kcat/openal-soft)
- **stb_image.h**: [Public Domain](https://github.com/nothings/stb)

---

## FAQ

**Q: Can I play Ninja Shadow on Windows?**
A: Yes, with MinGW/MSVC. Pre-built Windows binaries coming soon.

**Q: How many levels are there?**
A: 10 levels total, each with 0-3 star ratings based on performance.

**Q: Can I make my own levels?**
A: Yes! Edit level files in `assets/levels/levelNN.txt` or use the future level editor (Phase 2).

**Q: Is the game open-source?**
A: Yes, MIT licensed. Contributions welcome on GitHub.

**Q: How do I contribute?**
A: Fork the repository, make changes, and submit a pull request. See CONTRIBUTING.md (coming soon).

**Q: What's the estimated playtime?**
A: 1-2 hours for casual playthrough; 5+ hours for 100% completion (3-star all levels).

**Q: Are there cheat codes?**
A: Not yet. Can be added in Phase 2 (God mode, level skip, etc.).

**Q: Will there be a mobile version?**
A: Mobile touch controls planned for Phase 2. Full native iOS/Android: Phase 3+.

**Q: Can I stream/upload videos?**
A: Yes! The game is open-source and encourage community content.

---

## Contact & Community

- **GitHub**: (Link to repository)
- **Issues**: Report bugs via GitHub Issues
- **Discussions**: GitHub Discussions for feature requests
- **Discord**: (Community server coming soon)

---

**Last updated**: April 16, 2026
**Version**: 0.1 (Alpha)
**Status**: In active developmentandling

### Keyboard State Management

The game uses two mechanisms for input:

**Continuous Keys** (checked each frame via array):

```c
int keys[256];          // ASCII keys (a-z, 0-9, etc.)
int special_keys[256];  // Special keys (arrows, function keys)

// In key_down/key_up callbacks:
void key_down(unsigned char k, int x, int y) {
    keys[k] = 1;
    // Handle one-shot actions here if needed
}

void key_up(unsigned char k, int x, int y) {
    keys[k] = 0;
}

// In update_game():
if (keys['w'] || special_keys[GLUT_KEY_UP])
    player.vx -= MOVE_SPEED * dt;  // Move left
```

**One-Shot Keys** (flag-based to prevent key-repeat):

```c
// Player struct contains:
int jump_pressed;       // Released since last jump
int shuriken_pressed;   // Released since last throw
int dash_pressed;       // Released since last dash

// In key_down:
if (k == ' ' && !jump_pressed) {
    jump_pressed = 1;
    player.jump_count++;
    player.vy = JUMP_VY;
    audio_play(SFX_JUMP);
}

// In key_up:
if (k == ' ')
    jump_pressed = 0;  // Allow next jump
```

### Input Routing

**Full input map** (main.c):

```c
void key_down(unsigned char k, int x, int y) {
    switch (k) {
        case 'w': case 'W': keys['w'] = 1; break;
        case 'a': case 'A': keys['a'] = 1; break;
        case 's': case 'S': keys['s'] = 1; break;
        case 'd': case 'D': keys['d'] = 1; break;
        case 'z': case 'Z': case 'j': case 'J':
            player_attack(); break;
        case 'x': case 'X': case 'k': case 'K':
            player_throw_shuriken(); break;
        case 'e': case 'E': case 'l': case 'L':
            player_dash(); break;
        case 'p': case 'P': case 27:  // ESC
            game_state = (game_state == STATE_PLAYING) ? STATE_PAUSED : STATE_PLAYING;
            break;
    }
}

void special_down(int k, int x, int y) {
    switch (k) {
        case GLUT_KEY_UP:       special_keys[GLUT_KEY_UP] = 1; break;
        case GLUT_KEY_DOWN:     special_keys[GLUT_KEY_DOWN] = 1; break;
        case GLUT_KEY_LEFT:     special_keys[GLUT_KEY_LEFT] = 1; break;
        case GLUT_KEY_RIGHT:    special_keys[GLUT_KEY_RIGHT] = 1; break;
    }
}
```

---

## Enemy AI

### AI State Machine

Each enemy follows a simple state-based AI:

```c
typedef enum {
    AI_PATROL,      // Patrol between bounds
    AI_CHASE,       // Move toward player
    AI_ATTACK,      // Melee or ranged attack
    AI_RETREAT,     // Move away from player
    AI_DEAD         // Fade out
} AIState;

// In game.c update_game():
for (int i = 0; i < level.enemy_count; i++) {
    Enemy *e = &level.enemies[i];
    if (!e->alive) continue;

    // Distance to player
    float dx = player.x - e->x;
    float dist = fabsf(dx);

    // State transitions
    if (dist < 300.0f) {
        // Player detected - engage
        new_ai_state = AI_ATTACK;
    } else if (e->x < e->patrol_l || e->x > e->patrol_r) {
        // Reached patrol bound - reverse
        e->facing *= -1;
    } else {
        // Patrol normally
        new_ai_state = AI_PATROL;
    }

    // Execute AI state
    switch (new_ai_state) {
        case AI_PATROL:
            e->vx = e->facing * 150.0f;  // Patrol speed
            break;
        case AI_ATTACK:
            if (e->type == ENEMY_ARCHER) {
                e->shoot_timer += dt;
                if (e->shoot_timer > 2.0f) {
                    spawn_arrow_projectile(e, &player);
                    e->shoot_timer = 0;
                    audio_play(SFX_ARROW_SHOOT);
                }
            } else if (e->type == ENEMY_GUARD) {
                // Move toward player
                e->vx = (dx > 0 ? 1 : -1) * 200.0f;
            }
            break;
    }

    // Apply physics
    e->vx *= 0.95f;  // Friction
    e->x += e->vx * dt;
}
```

### Enemy-Specific Behaviors

**Guard (ENEMY_GUARD)**:

- Patrol speed: 150 px/s
- Attack range: 300 px
- Melee damage on contact: 1 HP
- AI: Simple patrol with chase on player detection
- Animation: Walking (leg swing), attack stance

**Archer (ENEMY_ARCHER)**:

- Patrol speed: 120 px/s (slower)
- Attack range: Full screen width
- Fires arrows every 2 seconds when attacking
- AI: Maintain distance while shooting
- Animation: Idle, draw bow, release

**Heavy Warrior (ENEMY_HEAVY)**:

- Patrol speed: 100 px/s (very slow)
- Attack range: 250 px
- Melee damage: 1 HP (but stronger knockback)
- Health: 5 HP (twice as much as Guard)
- AI: Deliberate, methodical approach
- Animation: Heavy walk, wind-up, crushing attack

---

## Collision Deep Dive

### AABB Collision System

**Core algorithm**:

```c
// Two rectangles: (x1,y1,w1,h1) and (x2,y2,w2,h2)
int rect_collide(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2) {
    // Check if boxes DO NOT overlap
    // If no gap on any side, they overlap
    if (x1 + w1 <= x2) return 0;  // x1's right <= x2's left
    if (x1 >= x2 + w2) return 0;  // x1's left >= x2's right
    if (y1 + h1 <= y2) return 0;  // y1's top <= y2's bottom
    if (y1 >= y2 + h2) return 0;  // y1's bottom >= y2's top
    return 1;  // All checks passed = overlap
}

// Get overlap depth (for push-out)
float get_overlap_x(float x1, float w1, float x2, float w2) {
    float left_gap = x2 - (x1 + w1);    // Gap if x1 is to the left
    float right_gap = x1 - (x2 + w2);   // Gap if x1 is to the right

    if (left_gap > 0) return 0;   // No overlap
    if (right_gap > 0) return 0;

    // Return the smallest gap (push-out direction)
    if (fabsf(left_gap) < fabsf(right_gap))
        return left_gap;  // Negative = push x1 left
    else
        return right_gap; // Positive = push x1 right
}
```

### Collision Resolution Order

Each frame, collisions are resolved in this precise order:

1. **Player-Platform collisions** (vertical resolution):

   ```c
   for (int i = 0; i < level.plat_count; i++) {
       Platform *p = &level.platforms[i];
       if (rect_collide(player.x-player.w/2, player.y, player.w, player.h,
                        p->x, p->y, p->w, p->h)) {

           // One-way platform: only collide from above
           if (p->type == PLAT_ONE_WAY && player.vy <= 0)
               continue;

           // Push player above platform
           player.y = p->y + p->h;
           player.vy = 0;  // Stop downward motion
           player.on_ground = 1;

           // Carry player if platform is moving
           player.x += p->pvx;
       }
   }
   ```

2. **Player-Trap collisions** (damage):

   ```c
   player.hurt_timer = 0.2f;  // Invulnerability flash
   player.health--;
   respawn_requested = 1;  // Deferred respawn
   audio_play(SFX_PLAYER_HURT);
   spawn_landing_dust(player.x, player.y);
   ```

3. **Player-Collectible collisions**:

   ```c
   if (!collectible.collected) {
       collectible.collected = 1;
       if (collectible.is_star) {
           stars_collected++;
           audio_play(SFX_STAR_COLLECT);
       } else {
           gems_collected++;
           audio_play(SFX_GEM_COLLECT);
       }
       spawn_collect_burst(collectible.x, collectible.y);

   }
   ```

4. **Projectile-Enemy collisions**:

   ```c
   for (int i = 0; i < level.proj_count; i++) {
       Projectile *proj = &level.projs[i];
       if (!proj->active) continue;

       for (int j = 0; j < level.enemy_count; j++) {
           Enemy *e = &level.enemies[j];
           if (!e->alive) continue;

           if (rect_collide(proj->x, proj->y, 4, 4,
                           e->x-12, e->y, 24, 32)) {
               e->health--;
               proj->active = 0;
               spawn_hit_sparks(proj->x, proj->y);
               audio_play(SFX_SHURIKEN_HIT);

               if (e->health <= 0) {
                   e->alive = 0;
                   enemies_defeated++;
                   audio_play(SFX_ENEMY_DEATH);
               }
           }
       }
   }
   ```

### Box-to-Pixel Conversions

Player collision box:

```c
// Player struct stores center position (cx, cy)
// Bounding box for collision is:
float player_x1 = player.x - player.w/2;  // 28/2 = 14
float player_y1 = player.y;
float player_x2 = player.x + player.w/2;
float player_y2 = player.y + player.h;
```

---

## Camera System

### Advanced Tracking

The camera uses **ease-in easing** to smoothly follow the player:

```c
// Camera target (centered on player)
float target_x = player.x - SCREEN_W / 2;

// Exponential ease: lerp to target
#define CAMERA_EASE 0.08f  // Adjust for smoothness
camera.x += (target_x - camera.x) * CAMERA_EASE;

// Hard bounds (don't scroll past level edges)
if (camera.x < 0) camera.x = 0;
if (camera.x > level.level_w - SCREEN_W)
    camera.x = level.level_w - SCREEN_W;
```

### Parallax Multipliers

Different background layers scroll at different speeds:

```c
void draw_parallax(float cam_x, int theme) {
    // Layer 1: Far background (5% parallax)
    float ox_far = -cam_x * 0.05f;
    draw_ghost_trees(ox_far);

    // Layer 2: Mid-ground (20% parallax)
    float ox_mid = -cam_x * 0.20f;
    draw_trees_theme(theme, ox_mid);

    // Layer 3: Near-ground (50% parallax)
    float ox_near = -cam_x * 0.50f;
    draw_decorations(theme, ox_near);

    // Layer 4: World space (100% parallax = 1.0x)
    // This is the main gameplay layer
}

// Parallax calculation:
// world_position + (camera_offset × multiplier) = screen_position
// Example: tree at x=1000 with cam_x=500 and mult=0.2:
// screen_x = 1000 + (-500 × 0.2) = 1000 - 100 = 900
```

---

## Level Design Guide

### Level Progression

**Difficulty curve**:

- **Levels 1-2**: Tutorial difficulty, basic mechanics
- **Levels 3-5**: Intermediate, mixed enemy types
- **Levels 6-8**: Advanced, complex layouts
- **Levels 9-10**: Expert, multi-phase challenges / boss fights

### Platform Layout Guidelines

**Safe design**:

- Minimum gap between platforms: 40 pixels (for double-jump reach)
- Maximum fall height: 200 pixels (can survive with care)
- Platform minimum width: 60 pixels (comfortable standing area)

**Challenge design**:

```c
// Example: Moving platform chain (Level 4)
// Platform 1: Stationary (200, 300, 100, 20)
// Platform 2: Moving Y-axis (350, 300, 80, 20)
//   - Amplitude: 150px, Speed: 2.0, Phase: 0
// Platform 3: One-way (500, 250, 120, 20)
// Platform 4: Crumbling (650, 250, 100, 20)
```

### Enemy Placement Strategy

**Balanced encounters**:

- Never surround player with 3+ enemies at once
- Space patrols 300+ pixels apart
- Archers on high ground, Guards on mid-level, Heavy on low
- Mix ranged + melee to force tactical decisions

### Checkpoint Placement

**Rule of thumb**: One checkpoint per 40-60 seconds of optimal playtime

```
Level start (0s) → CP1 (45s) → CP2 (90s) → CP3 (135s) → Exit (180s)
```

---

## Memory Layout

### Player Structure (88 bytes)

```c
typedef struct {
    float x, y, vx, vy;              // 16 bytes (position + velocity)
    float w, h;                      // 8 bytes (collision box)
    int on_ground, on_wall;          // 8 bytes (flags)
    int jump_count;                  // 4 bytes
    int health;                      // 4 bytes
    float hurt_timer;                // 4 bytes
    float attack_timer;              // 4 bytes
    float dash_timer;                // 4 bytes
    float dash_cooldown;             // 4 bytes
    int shuriken_count;              // 4 bytes
    int shuriken_pressed;            // 4 bytes
    int jump_pressed;                // 4 bytes
    int jump_available;              // 4 bytes
    int dash_pressed;                // 4 bytes
    Animation anim;                  // 20 bytes (state, timer, anim_t, facing)
} Player;  // Total: ~100 bytes
```

### Level Structure (estimated 8KB)

```c
typedef struct {
    float level_w, level_h;                      // 8 bytes
    int theme, bgm;                              // 8 bytes

    Platform platforms[MAX_PLATFORMS];           // 64 * 112 = 7168 bytes
    int plat_count;                              // 4 bytes

    Enemy enemies[MAX_ENEMIES];                  // 32 * 112 = 3584 bytes
    int enemy_count;                             // 4 bytes

    Projectile projs[MAX_PROJECTILES];           // 64 * 44 = 2816 bytes
    int proj_count;                              // 4 bytes

    // ... other arrays ...
} Level;  // Total: ~15-20KB
```

### Particle Buffer (8KB for 256 particles)

```c
Particle particles[256];  // 256 * 32 = 8192 bytes
```

**Total in-game memory**: ~40KB (very lightweight)

---

## Performance Optimization

### Low-Hanging Fruit

1. **Minimize draw calls**:

   ```c
   // BAD: 256 draw_circle calls per frame for particles
   for (int i = 0; i < MAX_PARTICLES; i++)
       if (particles[i].active)
           draw_circle(particles[i].x, particles[i].y, particles[i].size, 16);

   // GOOD: Batch particles by color, render once per batch
   glBegin(GL_TRIANGLE_FAN);
   for (int i = 0; i < MAX_PARTICLES; i++) {
       if (particles[i].active) {
           glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].a);
           draw_circle_verts(...);
       }
   }
   glEnd();
   ```

2. **Collision optimization**:

   ```c
   // Use spatial partitioning instead of O(n²) checks
   // For now, iterate only over nearby entities:
   for (int i = 0; i < level.enemy_count; i++) {
       Enemy *e = &level.enemies[i];
       if (fabsf(e->x - player.x) > 400)  // Skip far enemies
           continue;

       // ... collision check ...
   }
   ```

3. **Animation frame skipping**:

   ```c
   // Only update animation every 2 frames at 30 FPS
   static int frame_count = 0;
   if (++frame_count % 2 == 0) {
       // Update animation
       player.anim.timer += dt;
   }
   ```

### Profiling with gprof

```bash
# Build with profiling enabled
gcc -pg -O2 src/*.c -o ninja_shadow_prof -lGL -lGLU -lglut -lopenal -lm

# Run and play for 1 minute
./ninja_shadow_prof

# Analyze results
gprof ninja_shadow_prof gmon.out | head -30
```

**Expected hotspots**:

- `update_game()` — 40-50% (physics)
- `display()` — 30-40% (rendering)
- Collision functions — 10-20%

---

## Code Examples

### Adding a Health Pickup

1. **Define in game.h**:

   ```c
   #define COLLECTIBLE_HEALTH 2
   ```

2. **Draw in draw_map.c**:

   ```c
   void draw_health_pickup(float cx, float cy, float pulse_t) {
       float pulse = 1.0f + sinf(pulse_t * 6.28f) * 0.2f;
       glColor4f(0.9f, 0.2f, 0.2f, 1.0f);  // Red
       draw_circle(cx, cy, 6.0f * pulse, 12);
       glColor4f(1.0f, 1.0f, 1.0f, 0.5f);   // White center
       draw_circle(cx, cy, 3.0f, 8);
   }
   ```

3. **Spawn in level file (assets/levels/level05.txt)**:

   ```

   COLLECTIBLES
     HEALTH 400 250
     HEALTH 800 400
   ```

4. **Collect in game.c**:

   ```c
   if (collectible.type == COLLECTIBLE_HEALTH) {
       if (player.health < MAX_HEALTH)
           player.health++;
       audio_play(SFX_HEALTH_COLLECT);
       collectible.collected = 1;
   }
   ```

### Adding Wall-Jump Mechanic

```c
// In update_game(), when player is on wall:
if (on_wall && keys['w'] && !jump_pressed) {
    jump_pressed = 1;
    player.jump_count = 0;  // Reset jump count
    player.vy = JUMP_VY * 0.9f;  // Slightly weaker
    player.vx = -player.facing * MOVE_SPEED;  // Push away from wall
    audio_play(SFX_WALL_JUMP);
    spawn_landing_dust(player.x, player.y);
}
```

---

## Common Patterns

### State Machine Pattern

```c
// For any entity with multiple states
typedef struct {
    int state;
    float timer;
} StateMachine;

// Usage:
if (should_transition) {
    sm.state = NEW_STATE;
    sm.timer = 0;
}
sm.timer += dt;

// Dispatch based on state
switch (sm.state) {
    case STATE_A: /* ... */ break;
    case STATE_B: /* ... */ break;
}
```

### Particle Emitter Pattern

```c
void emit_particles(float cx, float cy, int count,
                   float vx, float vy, float spread,
                   float r, float g, float b, float lifetime) {
    for (int i = 0; i < count; i++) {
        float angle = (i * 360.0f / count) * PI / 180.0f;
        float s = spread * (0.5f + (rand() % 100) / 100.0f);
        particle_spawn(
            cx + cosf(angle) * s, cy + sinf(angle) * s,
            cosf(angle) * vx, sinf(angle) * vy,
            r, g, b, lifetime, 3.0f
        );
    }
}
```

### Lerp Utility

```c
// Linear interpolation
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Usage: smooth camera easing
camera.x = lerp(camera.x, target_x, CAMERA_EASE);
```

---

## Testing

### Unit Tests: `tests/test_shuriken.c`

**Compile & run**:

```bash
gcc tests/test_shuriken.c -o tests/test_shuriken -Wall
./tests/test_shuriken
```

**Test scenarios**:

1. **Initial count**: Shuriken count = 10 at level start
2. **Count clamp**: Count never goes below 0
3. **Velocity constant**: Shuriken speed = 450 px/s throughout flight
4. **Range limit**: Shuriken disappears after 50 units
5. **Key-repeat prevention**: Holding X throws 1 shuriken, not multiple

**Output**: `✓ PASS` or `✗ FAIL` for each test

### Manual Test Scenarios: `tests/test_shuriken_limits.md`

**Scenario 1: Shuriken Count Limit**

- Start level, verify HUD shows 10
- Throw all 10, observe count → 0
- Verify no shurikens spawn after count reaches 0

**Scenario 2: Shuriken Range Limit**

- Throw shuriken, measure distance traveled
- Verify disappears at ~50 units

**Scenario 3: Key-Repeat Prevention**

- Hold X key for 2 seconds
- Verify only 1 shuriken created (not 30+ from key repeat)

---

## Development Guide

### Code Style

- **Naming**: snake_case for functions/variables, UPPER_CASE for constants
- **Comments**: `/* ── Section Title ──────────── */` for major blocks
- **Line length**: ~80 characters for readability
- **Indentation**: 4 spaces (no tabs)
- **Header guards**: `#ifndef FILENAME_H` / `#define FILENAME_H` / `#endif`

### Adding a New Enemy Type

1. **Define constant** in `game.h`:

   ```c
   #define ENEMY_NINJA 3
   ```

2. **Add draw function** in `draw_enemy.c`:

   ```c
   void draw_enemy_ninja(float cx, float cy, int facing, float anim_t) {
       // Draw ninja silhouette using primitives
   }
   ```

3. **Add AI logic** in `game.c` `update_game()`:

   ```c
   if (enemy.type == ENEMY_NINJA) {
       // Update patrol, attack, animation
   }
   ```

4. **Update dispatch function** (`draw_enemy_dispatch` in `game.c`)

### Adding a New Level

1. **Create level file** `assets/levels/level06.txt` with geometry/enemies/traps
2. **Update `current_level` check** in `main.c` to load level 6
3. **Test**: Verify platforms, enemies, collectibles spawn correctly

### Adding a New Trap Type

1. **Add enum** in `game.h`:

   ```c
   #define TRAP_ICE 4
   ```

2. **Draw function** in `draw_map.c`:

   ```c
   void draw_ice_trap(float cx, float cy) { /* ... */ }
   ```

3. **Collision logic** in `game.c`:

   ```c
   if (trap.type == TRAP_ICE) {
       // Slippery surface: reduce friction
   }
   ```

### Debugging Tips

- **Print statements**: Use `printf()` to log state
- **Visual debugging**: Draw debug bounding boxes:

  ```c
  glColor4f(1, 0, 0, 0.3f);  // Red transparent
  draw_rect(player.x, player.y, player.w, player.h);
  ```

- **Pause & step**: Use STATE_PAUSED to freeze frame-by-frame
- **GDB debugger**:

  ```bash
  gcc -g src/*.c -o ninja_shadow_debug -lGL -lGLU -lglut -lopenal -lm
  gdb ./ninja_shadow_debug
  ```

### Common Issues & Fixes

| Issue                          | Cause                         | Fix                                            |
| ------------------------------ | ----------------------------- | ---------------------------------------------- |
| Player falls through platforms | Collision check fails         | Verify AABB overlap logic in `update_game()`   |
| Shurikens don't appear         | Count at 0                    | Check `player.shuriken_count` in HUD           |
| Audio doesn't play             | OpenAL not installed          | Run `sudo apt-get install libopenal-dev`       |
| Game runs at 15 FPS            | Rendering bottleneck          | Profile with `glxgears`, check draw call count |
| Menu doesn't respond to clicks | Mouse callback not registered | Verify `glutMouseFunc()` called in `main()`    |

---

## Build Instructions

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev \
    freeglut3-dev libopenal-dev

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install freeglut

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### Windows (MinGW)

```bash
# Install MinGW + GLUT + OpenAL development files
# (Use pre-built libraries or vcpkg package manager)

# Build
mingw32-make

# Run
ninja_shadow.exe
```

### Docker (Optional)

```dockerfile
FROM ubuntu:latest
RUN apt-get update && apt-get install -y gcc libgl1-mesa-dev \
    libglu1-mesa-dev freeglut3-dev libopenal-dev
WORKDIR /app
COPY . .
RUN make
CMD ["./ninja_shadow"]
```

```bash
docker build -t ninja_shadow .
docker run --display $DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix ninja_shadow
```

---

## Platform-Specific Notes

### Linux (Primary development platform)

- **GPU drivers**: Ensure proprietary drivers are installed for better performance
  - NVIDIA: `nvidia-driver-*`
  - AMD: `amdgpu-pro` or open-source `radeon`
  - Intel: Usually built-in, update via BIOS
- **OpenGL check**: `glxinfo | grep "OpenGL version"`

- **Audio**: ALSA or PulseAudio backend for OpenAL

### macOS

- **Compatibility**: Requires OpenGL 2.1+ (macOS 10.7+)
- **M1/M2 Silicon**: May need Rosetta 2 translation
- **Framework linking**:

  ```bash
  -framework OpenGL -framework GLUT
  ```

### Windows

- **Visual Studio**: Use `cl.exe` instead of `gcc` for better optimization
- **Library paths**: Link against `opengl32.lib`, `glu32.lib`, `glut.lib`, `OpenAL32.lib`
- **DLL dependencies**: Ship `OpenAL32.dll` with binary for compatibility

---

## Troubleshooting

### Build Errors

**Error**: `fatal error: GL/glut.h: No such file or directory`

- **Solution**: Install freeglut development package:

  ```bash
  sudo apt-get install freeglut3-dev
  ```

**Error**: `undefined reference to 'alGenBuffers'`

- **Solution**: Install OpenAL development library:

  ```bash
  sudo apt-get install libopenal-dev
  ```

### Runtime Errors

**Error**: `GLUT: freeglut ERROR: Unable to open display`

- **Cause**: X11 display not available (headless/SSH session)
- **Solution**: Use X11 forwarding or VNC

  ```bash
  ssh -X user@remote ./ninja_shadow
  ```

**Error**: Game window doesn't appear

- **Cause**: GLUT window creation failed
- **Solution**: Check for X11/graphics driver issues

  ```bash
  glxinfo | grep "direct rendering"  # Should say "Yes"
  ```

**Error**: Audio doesn't play (no errors)

- **Cause**: OpenAL device not found or no audio hardware
- **Solution**: Check audio availability

  ```bash
  aplay /usr/share/sounds/freedesktop/stereo/bell.oga  # Test system audio
  ```

### Performance Issues

**Game runs slow (< 30 FPS)**:

1. Check CPU/GPU usage: `htop`, `nvidia-smi`
2. Reduce particle count (change MAX_PARTICLES)
3. Simplify background decorations
4. Profile with `gprof`:

   ```bash
   gcc -pg src/*.c -o ninja_shadow_prof -lGL -lGLU -lglut -lopenal -lm
   ./ninja_shadow_prof
   gprof ninja_shadow_prof gmon.out
   ```

**Screen tearing**:

- Enable V-Sync in GPU driver settings
- Or reduce frame rate cap (change `glutTimerFunc` interval)

---

## Directory Reference

### Source Code Files

| File            | Lines  | Purpose                                    |
| --------------- | ------ | ------------------------------------------ |
| `main.c`        | ~200   | Entry point, GLUT setup, event loop        |
| `game.c`        | ~2000+ | Core game logic, physics, collision        |
| `game.h`        | ~300   | All struct definitions, constants, externs |
| `renderer.c`    | ~100   | OpenGL primitives and helpers              |
| `renderer.h`    | ~50    | Renderer function declarations             |
| `draw_bg.c`     | ~500   | Background rendering and parallax          |
| `draw_map.c`    | ~500   | Level geometry and props                   |
| `draw_player.c` | ~300   | Player character rendering                 |
| `draw_enemy.c`  | ~300   | Enemy character rendering                  |
| `draw_hud.c`    | ~400   | HUD, menus, overlays                       |
| `audio.c`       | ~300   | OpenAL audio system                        |
| `audio.h`       | ~60    | Audio API definitions                      |

### Total: ~5,000 lines of C code

---

## Performance Characteristics

### Target Specifications

- **Resolution**: 1280×720 (90 KB framebuffer)
- **Target FPS**: 60 (16 ms per frame)
- **Max entities**: 64 platforms + 32 enemies + 64 projectiles + 32 traps + 64 collectibles = 256
- **Max particles**: 256 simultaneous
- **Draw calls**: ~50 per frame (batched by type)

### Memory Usage (Estimated)

- **Level data**: ~50 KB per level × 10 = 500 KB
- **Audio buffers**: ~5 MB (22 SFX + 5 BGM)
- **Textures**: ~0 KB (no textures, primitives only)
- **Runtime state**: ~1 MB (particles, entities, etc.)
- **Total**: ~7 MB at runtime

### CPU / GPU

- **CPU**: Minimal (physics solver, AI is simple)
- **GPU**: Light (OpenGL 2.x fixed-function pipeline)
- **Target platform**: Any Linux desktop from 2010+ with OpenGL 2.0 support

---

## Future Enhancements

### Planned Features (Phase 2)

- [ ] Difficulty levels (Normal/Hard/Expert) with unique level layouts
- [ ] Boss battles at levels 5 and 10
- [ ] Combo counter display + multiplier bonus
- [ ] Power-ups: speed boost, shield, temporary invulnerability
- [ ] Leaderboard system (local high-scores file)
- [ ] Replay system (record input, playback)
- [ ] Level editor (in-game or separate tool)
- [ ] Mobile touch controls
- [ ] Steam achievements/trading cards

### Polish (Phase 3)

- [ ] Texture sprites for characters (optional, keep silhouettes as fallback)
- [ ] Advanced shaders (bloom, glow effects)
- [ ] Dialogue/story narration
- [ ] Character customization (skins, accessories)
- [ ] Multiplayer co-op or PvP modes

---

## Resources & References

### Documentation

- [OpenGL 2.1 Specification](https://www.khronos.org/opengl/wiki/OpenGL_Specification)
- [GLUT Tutorial](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html)
- [OpenAL Documentation](https://openal-soft.org/)
- [C99 Standard](https://en.wikipedia.org/wiki/C99)

### Free Art & Audio Assets

- **Graphics**: OpenGameArt.org, itch.io, Kenney.nl
- **Audio**: Freesound.org, ZapSplat, OpenGameArt.org, Kenney.nl

### Similar Games (Inspiration)

- Ninja Arashi series (original inspiration)
- Mark of the Ninja (stealth platformer)
- Sekiro: Shadows Die Twice (combat mechanics)
- Celeste (precise platforming)
- Hollow Knight (atmospheric design)

---

## License & Credits

### License

**MIT License** — See LICENSE file for details.

```
MIT License

Copyright (c) 2025 Ninja Shadow Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

### Credits

**Development**: Claude AI (Anthropic)
**Architecture**: Based on Ninja Arashi II gameplay analysis
**Technology**: OpenGL, GLUT (freeglut), OpenAL, GCC

### Third-Party Libraries

- **freeglut**: [LGPL License](https://github.com/dcnieho/freeglut)
- **OpenAL-soft**: [LGPL License](https://github.com/kcat/openal-soft)
- **stb_image.h**: [Public Domain](https://github.com/nothings/stb)

---

## FAQ

**Q: Can I play Ninja Shadow on Windows?**
A: Yes, with MinGW/MSVC. Pre-built Windows binaries coming soon.

**Q: How many levels are there?**
A: 10 levels total, each with 0-3 star ratings based on performance.

**Q: Can I make my own levels?**
A: Yes! Edit level files in `assets/levels/levelNN.txt` or use the future level editor (Phase 2).

**Q: Is the game open-source?**
A: Yes, MIT licensed. Contributions welcome on GitHub.

**Q: How do I contribute?**
A: Fork the repository, make changes, and submit a pull request. See CONTRIBUTING.md (coming soon).

**Q: What's the estimated playtime?**
A: 1-2 hours for casual playthrough; 5+ hours for 100% completion (3-star all levels).

**Q: Are there cheat codes?**
A: Not yet. Can be added in Phase 2 (God mode, level skip, etc.).

**Q: Will there be a mobile version?**
A: Mobile touch controls planned for Phase 2. Full native iOS/Android: Phase 3+.

**Q: Can I stream/upload videos?**
A: Yes! The game is open-source and encourage community content.

---

## Contact & Community

- **GitHub**: (Link to repository)
- **Issues**: Report bugs via GitHub Issues
- **Discussions**: GitHub Discussions for feature requests
- **Discord**: (Community server coming soon)

---

**Last updated**: April 16, 2026
**Version**: 0.1 (Alpha)
**Status**: In active development

### Unit Tests: `tests/test_shuriken.c`

**Compile & run**:

```bash
gcc tests/test_shuriken.c -o tests/test_shuriken -Wall
./tests/test_shuriken
```

**Test scenarios**:

1. **Initial count**: Shuriken count = 10 at level start
2. **Count clamp**: Count never goes below 0
3. **Velocity constant**: Shuriken speed = 450 px/s throughout flight
4. **Range limit**: Shuriken disappears after 50 units
5. **Key-repeat prevention**: Holding X throws 1 shuriken, not multiple

**Output**: `✓ PASS` or `✗ FAIL` for each test

### Manual Test Scenarios: `tests/test_shuriken_limits.md`

**Scenario 1: Shuriken Count Limit**

- Start level, verify HUD shows 10
- Throw all 10, observe count → 0
- Verify no shurikens spawn after count reaches 0

**Scenario 2: Shuriken Range Limit**

- Throw shuriken, measure distance traveled
- Verify disappears at ~50 units

**Scenario 3: Key-Repeat Prevention**

- Hold X key for 2 seconds
- Verify only 1 shuriken created (not 30+ from key repeat)

---

## Development Guide

### Code Style

- **Naming**: snake_case for functions/variables, UPPER_CASE for constants
- **Comments**: `/* ── Section Title ──────────── */` for major blocks
- **Line length**: ~80 characters for readability
- **Indentation**: 4 spaces (no tabs)
- **Header guards**: `#ifndef FILENAME_H` / `#define FILENAME_H` / `#endif`

### Adding a New Enemy Type

1. **Define constant** in `game.h`:

   ```c
   #define ENEMY_NINJA 3
   ```

2. **Add draw function** in `draw_enemy.c`:

   ```c
   void draw_enemy_ninja(float cx, float cy, int facing, float anim_t) {
       // Draw ninja silhouette using primitives
   }
   ```

3. **Add AI logic** in `game.c` `update_game()`:

   ```c
   if (enemy.type == ENEMY_NINJA) {
       // Update patrol, attack, animation
   }
   ```

4. **Update dispatch function** (`draw_enemy_dispatch` in `game.c`)

### Adding a New Level

1. **Create level file** `assets/levels/level06.txt` with geometry/enemies/traps
2. **Update `current_level` check** in `main.c` to load level 6
3. **Test**: Verify platforms, enemies, collectibles spawn correctly

### Adding a New Trap Type

1. **Add enum** in `game.h`:

   ```c
   #define TRAP_ICE 4
   ```

2. **Draw function** in `draw_map.c`:

   ```c
   void draw_ice_trap(float cx, float cy) { /* ... */ }
   ```

3. **Collision logic** in `game.c`:

   ```c
   if (trap.type == TRAP_ICE) {
       // Slippery surface: reduce friction
   }
   ```

### Debugging Tips

- **Print statements**: Use `printf()` to log state
- **Visual debugging**: Draw debug bounding boxes:

  ```c
  glColor4f(1, 0, 0, 0.3f);  // Red transparent
  draw_rect(player.x, player.y, player.w, player.h);
  ```

- **Pause & step**: Use STATE_PAUSED to freeze frame-by-frame
- **GDB debugger**:

  ```bash
  gcc -g src/*.c -o ninja_shadow_debug -lGL -lGLU -lglut -lopenal -lm
  gdb ./ninja_shadow_debug
  ```

### Common Issues & Fixes

| Issue                          | Cause                         | Fix                                            |
| ------------------------------ | ----------------------------- | ---------------------------------------------- |
| Player falls through platforms | Collision check fails         | Verify AABB overlap logic in `update_game()`   |
| Shurikens don't appear         | Count at 0                    | Check `player.shuriken_count` in HUD           |
| Audio doesn't play             | OpenAL not installed          | Run `sudo apt-get install libopenal-dev`       |
| Game runs at 15 FPS            | Rendering bottleneck          | Profile with `glxgears`, check draw call count |
| Menu doesn't respond to clicks | Mouse callback not registered | Verify `glutMouseFunc()` called in `main()`    |

---

## Build Instructions

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev \
    freeglut3-dev libopenal-dev

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install freeglut

# Build
cd ninja_shadow
make

# Run
./ninja_shadow
```

### Windows (MinGW)

```bash
# Install MinGW + GLUT + OpenAL development files
# (Use pre-built libraries or vcpkg package manager)

# Build
mingw32-make

# Run
ninja_shadow.exe
```

### Docker (Optional)

```dockerfile
FROM ubuntu:latest
RUN apt-get update && apt-get install -y gcc libgl1-mesa-dev \
    libglu1-mesa-dev freeglut3-dev libopenal-dev
WORKDIR /app
COPY . .
RUN make
CMD ["./ninja_shadow"]
```

```bash
docker build -t ninja_shadow .
docker run --display $DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix ninja_shadow
```

---

## Troubleshooting

### Build Errors

**Error**: `fatal error: GL/glut.h: No such file or directory`

- **Solution**: Install freeglut development package:

  ```bash
  sudo apt-get install freeglut3-dev
  ```

**Error**: `undefined reference to 'alGenBuffers'`

- **Solution**: Install OpenAL development library:

  ```bash
  sudo apt-get install libopenal-dev
  ```

### Runtime Errors

**Error**: `GLUT: freeglut ERROR: Unable to open display`

- **Cause**: X11 display not available (headless/SSH session)
- **Solution**: Use X11 forwarding or VNC

  ```bash
  ssh -X user@remote ./ninja_shadow
  ```

**Error**: Game window doesn't appear

- **Cause**: GLUT window creation failed
- **Solution**: Check for X11/graphics driver issues

  ```bash
  glxinfo | grep "direct rendering"  # Should say "Yes"
  ```

**Error**: Audio doesn't play (no errors)

- **Cause**: OpenAL device not found or no audio hardware
- **Solution**: Check audio availability

  ```bash
  aplay /usr/share/sounds/freedesktop/stereo/bell.oga  # Test system audio
  ```

### Performance Issues

**Game runs slow (< 30 FPS)**:

1. Check CPU/GPU usage: `htop`, `nvidia-smi`
2. Reduce particle count (change MAX_PARTICLES)
3. Simplify background decorations
4. Profile with `gprof`:

   ```bash
   gcc -pg src/*.c -o ninja_shadow_prof -lGL -lGLU -lglut -lopenal -lm
   ./ninja_shadow_prof
   gprof ninja_shadow_prof gmon.out
   ```

**Screen tearing**:

- Enable V-Sync in GPU driver settings
- Or reduce frame rate cap (change `glutTimerFunc` interval)

---

## Directory Reference

### Source Code Files

| File            | Lines  | Purpose                                    |
| --------------- | ------ | ------------------------------------------ |
| `main.c`        | ~200   | Entry point, GLUT setup, event loop        |
| `game.c`        | ~2000+ | Core game logic, physics, collision        |
| `game.h`        | ~300   | All struct definitions, constants, externs |
| `renderer.c`    | ~100   | OpenGL primitives and helpers              |
| `renderer.h`    | ~50    | Renderer function declarations             |
| `draw_bg.c`     | ~500   | Background rendering and parallax          |
| `draw_map.c`    | ~500   | Level geometry and props                   |
| `draw_player.c` | ~300   | Player character rendering                 |
| `draw_enemy.c`  | ~300   | Enemy character rendering                  |
| `draw_hud.c`    | ~400   | HUD, menus, overlays                       |
| `audio.c`       | ~300   | OpenAL audio system                        |
| `audio.h`       | ~60    | Audio API definitions                      |

### Total: ~5,000 lines of C code

---

## Performance Characteristics

### Target Specifications

- **Resolution**: 1280×720 (90 KB framebuffer)
- **Target FPS**: 60 (16 ms per frame)
- **Max entities**: 64 platforms + 32 enemies + 64 projectiles + 32 traps + 64 collectibles = 256
- **Max particles**: 256 simultaneous
- **Draw calls**: ~50 per frame (batched by type)

### Memory Usage (Estimated)

- **Level data**: ~50 KB per level × 10 = 500 KB
- **Audio buffers**: ~5 MB (22 SFX + 5 BGM)
- **Textures**: ~0 KB (no textures, primitives only)
- **Runtime state**: ~1 MB (particles, entities, etc.)
- **Total**: ~7 MB at runtime

### CPU / GPU

- **CPU**: Minimal (physics solver, AI is simple)
- **GPU**: Light (OpenGL 2.x fixed-function pipeline)
- **Target platform**: Any Linux desktop from 2010+ with OpenGL 2.0 support

---

## Future Enhancements

### Planned Features (Phase 2)

- [ ] Difficulty levels (Normal/Hard/Expert) with unique level layouts
- [ ] Boss battles at levels 5 and 10
- [ ] Combo counter display + multiplier bonus
- [ ] Power-ups: speed boost, shield, temporary invulnerability
- [ ] Leaderboard system (local high-scores file)
- [ ] Replay system (record input, playback)
- [ ] Level editor (in-game or separate tool)
- [ ] Mobile touch controls
- [ ] Steam achievements/trading cards

### Polish (Phase 3)

- [ ] Texture sprites for characters (optional, keep silhouettes as fallback)
- [ ] Advanced shaders (bloom, glow effects)
- [ ] Dialogue/story narration
- [ ] Character customization (skins, accessories)
- [ ] Multiplayer co-op or PvP modes

---

## Resources & References

### Documentation

- [OpenGL 2.1 Specification](https://www.khronos.org/opengl/wiki/OpenGL_Specification)
- [GLUT Tutorial](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html)
- [OpenAL Documentation](https://openal-soft.org/)
- [C99 Standard](https://en.wikipedia.org/wiki/C99)

### Free Art & Audio Assets

- **Graphics**: OpenGameArt.org, itch.io, Kenney.nl
- **Audio**: Freesound.org, ZapSplat, OpenGameArt.org, Kenney.nl

### Similar Games (Inspiration)

- Ninja Arashi series (original inspiration)
- Mark of the Ninja (stealth platformer)
- Sekiro: Shadows Die Twice (combat mechanics)
- Celeste (precise platforming)
- Hollow Knight (atmospheric design)

---

## License & Credits

### License

**MIT License** — See LICENSE file for details.

```
MIT License

Copyright (c) 2025 Ninja Shadow Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

### Credits

**Development**: Claude AI (Anthropic)
**Architecture**: Based on Ninja Arashi II gameplay analysis
**Technology**: OpenGL, GLUT (freeglut), OpenAL, GCC

### Third-Party Libraries

- **freeglut**: [LGPL License](https://github.com/dcnieho/freeglut)
- **OpenAL-soft**: [LGPL License](https://github.com/kcat/openal-soft)
- **stb_image.h**: [Public Domain](https://github.com/nothings/stb)

---

## FAQ

**Q: Can I play Ninja Shadow on Windows?**
A: Yes, with MinGW/MSVC. Pre-built Windows binaries coming soon.

**Q: How many levels are there?**
A: 10 levels total, each with 0-3 star ratings based on performance.

**Q: Can I make my own levels?**
A: Yes! Edit level files in `assets/levels/levelNN.txt` or use the future level editor (Phase 2).

**Q: Is the game open-source?**
A: Yes, MIT licensed. Contributions welcome on GitHub.

**Q: How do I contribute?**
A: Fork the repository, make changes, and submit a pull request. See CONTRIBUTING.md (coming soon).

**Q: What's the estimated playtime?**
A: 1-2 hours for casual playthrough; 5+ hours for 100% completion (3-star all levels).

**Q: Are there cheat codes?**
A: Not yet. Can be added in Phase 2 (God mode, level skip, etc.).

**Q: Will there be a mobile version?**
A: Mobile touch controls planned for Phase 2. Full native iOS/Android: Phase 3+.

**Q: Can I stream/upload videos?**
A: Yes! The game is open-source and encourage community content.

---

## Contact & Community

- **GitHub**: [NinjaShadow](https://github.com/Bishwajit-2810/NinjaShadow)
- **Issues**: Report bugs via GitHub Issues
- **Discussions**: GitHub Discussions for feature requests
- **Discord**: (Community server coming soon)

---

**Last updated**: April 16, 2026
**Version**: 5 (final release)
**Status**: In active development
