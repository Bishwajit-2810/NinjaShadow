# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
make          # build → produces ./ninja_shadow binary
make clean    # remove binary
./ninja_shadow
```

Requires: `gcc`, `libGL`, `libGLU`, `freeglut`, `libm` (standard on most Linux desktops).

## Tests

The only test suite lives in `tests/` and is standalone (no game headers needed):

```bash
gcc tests/test_shuriken.c -o tests/test_shuriken -Wall
./tests/test_shuriken
```

Manual/scenario tests are documented in `tests/test_shuriken_limits.md`.

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys / WASD | Move |
| Space / Up Arrow | Jump (double jump) |
| Z / J | Sword attack |
| X / K | Throw shuriken (infinite range, straight line) |
| E / L / Enter | Dash |
| P / Esc | Pause |

## Architecture

### Data layer — `src/game.h`
Single header that owns **all** shared structs and global state declarations: `Player`, `Enemy`, `Level`, `Platform`, `Projectile`, `Trap`, `Pot`, `Collectible`, `Particle`, `Camera`, `GameState`, `AnimState`. Every `.c` file includes this. Globals are *defined* in `game.c` and `extern`-declared here.

### Logic — `src/game.c`
Implements `game_init()`, `load_level(int num)`, and `update_game(float dt)`. All physics, collision, enemy AI, projectile movement, and game-state transitions live here.

### Rendering — split across four draw files + shared primitives

| File | Responsibility |
|------|---------------|
| `src/renderer.h` / `src/renderer.c` | Low-level GL helpers (`draw_rect`, `draw_circle`, `set_color`, etc.); also contains the GLUT display callback, parallax draw logic, and `draw_world()` |
| `src/draw_bg.c` | Sky, ghost trees, bamboo, pagoda, lanterns, moon, birds |
| `src/draw_map.c` | Platforms, traps, pots, collectibles, exit portal, grass, rope fence |
| `src/draw_player.c` | Player silhouette in all `AnimState` poses |
| `src/draw_enemy.c` | Guard, Archer, Heavy enemy types; arrows |
| `src/draw_hud.c` | HUD (health hearts, gem/gold/shuriken counts), menus, overlays |

All draw function signatures are forward-declared together in `renderer.h` regardless of which file implements them.

### Entry point — `src/main.c`
GLUT init, window setup, orthographic projection (`gluOrtho2D(0, 1280, 0, 720)` — Y=0 at bottom), registers `display` callback, then calls `game_init()` and enters `glutMainLoop()`. The `display` function computes `dt`, calls `update_game`, then routes rendering through the `GameState` switch.

### Coordinate system
Origin (0,0) is **bottom-left**. All positions are in screen pixels for world objects; camera translation is applied with `glTranslatef(-camera.x, -camera.y, 0)` before drawing world-space objects.

### Level themes
`level.theme`: `0=RUINS`, `1=TEMPLE`, `2=VILLAGE`, `3=BAMBOO`, `4=FORTRESS`. Theme controls background drawing path, platform art style, and which mid-ground decorations (pagoda, lanterns, bamboo) appear. Level data files: `assets/levels/level01.txt` … `level10.txt`.

### Key constraints
- Characters use **only OpenGL primitives** — no texture sprites for player/enemies.
- Shurikens: 10 per level (`SHURIKEN_PER_LEVEL`), range 50 units (`SHURIKEN_RANGE`), velocity 450 (`SHURIKEN_VELOCITY`). Key-repeat prevention via `player.shuriken_pressed` flag.
- `MAX_PROJECTILES 64` — throwing is blocked (without decrementing ammo) if the buffer is full.
- `dt` is capped at 0.05 s (20 FPS floor) to prevent physics tunnelling on frame spikes.
