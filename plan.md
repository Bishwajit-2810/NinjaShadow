# Ninja Shadow — 2D OpenGL Platformer

## Complete Agent-Ready Game Development Plan

### Based on Ninja Arashi II Screenshot Analysis

**Document Version**: 2.0
**Last Updated**: April 16, 2026
**Status**: COMPREHENSIVE & DETAILED

---

> **CRITICAL INSTRUCTION FOR THE AGENT**
> This plan was written by analysing 4 actual gameplay screenshots from Ninja Arashi II.
> Every visual element described here — characters, backgrounds, UI layout, colors,
> platforms, traps, props — is directly observed from those screenshots.
> Do NOT simplify anything to rectangles or plain colors.
> Implement every element exactly as specified. The checklist in Section 30 must
> pass 100% before the project is considered complete.

---

## IMPLEMENTATION PRIORITIES

### TIER 1 - CRITICAL (MVP Core)

**Must have for alpha release:**

- [x] Core game loop & physics
- [x] Player character with basic movement
- [x] Jump/double-jump mechanics
- [x] Basic enemy types (Guard, Archer)
- [x] Sword attack and shuriken throw
- [x] Collision detection (AABB)
- [x] 1-2 playable levels
- [x] Audio system (OpenAL)
- [x] HUD (health, ammo, score)

### TIER 2 - HIGH (Core Features)

**Essential for beta release:**

- [ ] All 10 levels with proper progression
- [ ] All 3 enemy types fully functional
- [ ] All 4-5 trap types implemented
- [ ] Checkpoint system with respawn
- [ ] Level themes (visual differentiation)
- [ ] Star rating system (0-3 stars)
- [ ] Settings/options menu
- [ ] Main menu with navigation
- [ ] Level complete/game over screens

### TIER 3 - MEDIUM (Polish)

**Nice to have for release candidate:**

- [ ] Background parallax effects
- [ ] Particle effects (hits, collections)
- [ ] Crumbling platforms
- [ ] Moving platforms with momentum
- [ ] Fire vent traps with animation
- [ ] Wall-slide mechanics
- [ ] Combo counter
- [ ] Audio balance (SFX + BGM volume)

### TIER 4 - LOW (Enhancement)

**Post-release content:**

- [ ] Leaderboard system
- [ ] Difficulty modes (Easy/Normal/Hard)
- [ ] Boss battles
- [ ] Power-ups (shield, speed boost)
- [ ] Replay recording
- [ ] Level editor
- [ ] Mobile controls
- [ ] Advanced visual effects

---

## SCREENSHOT ANALYSIS — WHAT THE AGENT MUST REPRODUCE

### From Screenshot 1 (Forest Ruins, open area)

- Player ninja stands on a mossy stone pillar, mid-jump pose
- Background: deep blue sky with faint ghost-tree silhouettes in the far distance
- Three stone pillars of varying heights in the middle ground
- A spinning red blade trap (circular saw with red blades) sits on the ground
- An enemy warrior stands on high ground top-right, holding a weapon over shoulder
- A second enemy stands at ground level centre-right, holding a long polearm
- Scattered clay pots (breakable props) on the ground near the second enemy
- Left side: tall bare silhouette trees, hanging cloth/banner between them
- HUD top-left: circular avatar frame, heart x6, blue gem count 9999864, gold coin 99984034
- HUD bottom-left: two shuriken icons (left/right directional throw buttons)
- HUD bottom-right: 4 circular skill buttons arranged in a 2x2 grid

### From Screenshot 2 (Night Village / Temple gateway)

- Large pagoda gate structure dominates the right half — massive dark roof with curved eaves
- Hanging red lanterns on strings between the gate pillars (glowing red-orange)
- Foreground: large red spiky cactus/thorn bush on the left (red spikes, very prominent)
- Player ninja clings to the side of a brown/tan cliff face wall, in wall-slide pose
- A small hanging platform (dark wood with grass/moss on top) juts from the cliff
- Enemy warrior stands at ground level below the gate
- Bridge/rope fence visible bottom-right
- Stars in HUD show only 1 star earned (first star filled, two empty)

### From Screenshot 3 (Stone ruins, darker area)

- Player grapples with an enemy at ground level (close-combat pose)
- Player has distinctive branching/antler-like hair or headpiece visible
- Stone pillars in background, same as screenshot 1 but from different angle
- Enemy on high ground top-right with weapon
- Spinning red blade trap visible on the right side ground
- Scattered pots in the background
- HUD shows same layout, 3 stars empty (0 stars this run)

### From Screenshot 4 (Bamboo forest, open area)

- Background: bamboo forest with tall thin vertical bamboo stalks
- Player mid-jump, reaching upward toward a GLOWING GOLD STAR collectible
- The star emits bright yellow-green particle sparks around it
- Enemy warrior at ground level below, holding weapon on shoulder
- Various clay pots scattered on the right side ground
- Large stone monolith / grave-marker shapes on the right (tall dark rectangles)
- Ground has tufts of grass silhouettes
- HUD shows 0 stars collected (all grey)

---

## 1. PROJECT OVERVIEW

- **Engine**: OpenGL 2.x + GLUT (freeglut), language C
- **Resolution**: 1280 x 720 (16:9 aspect ratio)
- **Frame rate**: 60 FPS target (16ms per frame)
- **Style**: Pure black silhouette characters on dark blue atmospheric backgrounds
- **Levels**: 10 levels, 5 distinct visual themes
- **Characters drawn**: All with OpenGL primitives — NO texture sprites for characters
- **Audio**: 22 distinct sound effects + 5 per-theme BGM tracks
- **Target platform**: Linux (primary), macOS, Windows (secondary)
- **Development time**: ~2-3 months for MVP to release candidate
- **Team size**: 1 developer (AI-assisted)

---

## 2. TECHNOLOGY STACK

| Component    | Technology                                                                    |
| ------------ | ----------------------------------------------------------------------------- |
| Language     | C99 (ISO/IEC 9899:1999)                                                       |
| Window/Input | GLUT (freeglut)                                                               |
| Rendering    | OpenGL fixed-function: GL_QUADS, GL_TRIANGLES, GL_TRIANGLE_FAN, GL_LINE_STRIP |
| Audio        | OpenAL (cross-platform audio library)                                         |
| Math         | math.h (sinf, cosf, atan2f, fmodf, fabsf)                                     |
| Build        | GCC + GNU Makefile                                                            |
| Testing      | C unit tests with assert.h                                                    |
| Version Ctrl | Git (GitHub for sharing)                                                      |

---

## 3. COMPLETE FOLDER STRUCTURE

```
ninja_shadow/
├── README.md                          # Comprehensive documentation (THIS file now)
├── CLAUDE.md                          # Claude-specific architectural notes
├── plan.md                            # Development plan & checklist (this file)
├── Makefile                           # Build configuration
├── .gitignore                         # Git ignore rules
├── LICENSE                            # MIT License
├── ninja_shadow                       # Compiled binary (after make)
│
├── src/                               # Source code (11 files, ~5000 LOC)
│   ├── main.c                         # Entry point: GLUT init, event loop
│   ├── game.h                         # Central header: ALL struct defs + externs
│   ├── game.c                         # Core logic: physics, collision, AI
│   ├── renderer.h / renderer.c        # OpenGL primitives & color helpers
│   ├── draw_bg.c                      # Backgrounds, trees, decorations, parallax
│   ├── draw_map.c                     # Platforms, traps, pots, collectibles
│   ├── draw_player.c                  # Player rendering (all poses)
│   ├── draw_enemy.c                   # Enemy rendering (Guard, Archer, Heavy)
│   ├── draw_hud.c                     # HUD, menus, overlays, text
│   ├── audio.h / audio.c              # OpenAL audio system + SFX management
│
├── assets/                            # Game content
│   ├── levels/                        # Level files (text-based format)
│   │   ├── level01.txt                # Level 1 (Tutorial - Ruins)
│   │   ├── level02.txt                # Level 2 (Ruins)
│   │   ├── level03.txt                # Level 3 (Temple intro)
│   │   ├── level04.txt                # Level 4 (Temple)
│   │   ├── level05.txt                # Level 5 (Village)
│   │   ├── level06.txt                # Level 6 (Village / Bamboo)
│   │   ├── level07.txt                # Level 7 (Bamboo)
│   │   ├── level08.txt                # Level 8 (Fortress intro)
│   │   ├── level09.txt                # Level 9 (Fortress)
│   │   └── level10.txt                # Level 10 (Fortress - Final boss)
│   │
│   └── audio/                         # Audio files (27 total)
│       ├── sfx_jump.wav               # (22 SFX files)
│       ├── sfx_double_jump.wav
│       ├── sfx_land.wav
│       ├── sfx_wall_slide.wav
│       ├── sfx_dash.wav
│       ├── sfx_attack_slash.wav
│       ├── sfx_attack_hit.wav
│       ├── sfx_shuriken_throw.wav
│       ├── sfx_shuriken_hit.wav
│       ├── sfx_player_hurt.wav
│       ├── sfx_player_death.wav
│       ├── sfx_enemy_death.wav
│       ├── sfx_gem_collect.wav
│       ├── sfx_star_collect.wav
│       ├── sfx_pot_break.wav
│       ├── sfx_blade_spin.wav
│       ├── sfx_fire_vent.wav
│       ├── sfx_platform_crumble.wav
│       ├── sfx_level_complete.wav
│       ├── sfx_game_over.wav
│       ├── sfx_menu_select.wav
│       ├── sfx_menu_confirm.wav
│       │
│       ├── bgm_forest.wav             # Theme 0 RUINS background music
│       ├── bgm_temple.wav             # Theme 1 TEMPLE background music
│       ├── bgm_village.wav            # Theme 2 VILLAGE background music
│       ├── bgm_bamboo.wav             # Theme 3 BAMBOO background music
│       └── bgm_fortress.wav           # Theme 4 FORTRESS background music
│
├── include/                           # Utility headers
│   └── stb_image.h                    # Optional: for future PNG loading
│
├── tests/                             # Test suite
│   ├── test_shuriken.c                # Unit tests for shuriken system
│   ├── test_shuriken_limits.md        # Manual test scenarios
│   └── test_shuriken                  # Compiled test binary
│
└── docs/                              # (Optional) Additional docs
    ├── CONTRIBUTING.md                # Contribution guidelines
    ├── ARCHITECTURE.md                # Detailed architecture notes
    └── ASSET_SOURCES.md               # Where to find audio/art assets
```

---

## 4. IMPLEMENTATION CHECKLIST

### Phase 1: Core Engine (Weeks 1-2)

- [ ] GLUT window setup (1280x720, double-buffered)
- [ ] OpenGL projection setup (gluOrtho2D)
- [ ] Input system (keyboard + mouse)
- [ ] Game loop (update → render cycle)
- [ ] OpenAL audio initialization
- [ ] Basic color & primitive drawing functions
- [ ] Frame rate limiter (60 FPS target)

### Phase 2: Player & Physics (Weeks 3-4)

- [ ] Player structure (position, velocity, health, animation)
- [ ] Gravity & velocity integration
- [ ] Ground/platform collision detection
- [ ] Jump mechanics (single + double jump)
- [ ] Player animation state machine (idle, run, jump, fall, attack)
- [ ] Sword attack with hitbox
- [ ] Shuriken throw with range limit
- [ ] Camera system with smooth easing

### Phase 3: Level System (Weeks 5-6)

- [ ] Level file format & parser
- [ ] Platform rendering (solid, one-way, moving, crumbling)
- [ ] Enemy spawning & basic AI
- [ ] Trap system (blade, spike, thorn, fire)
- [ ] Collectibles (gems, gold, stars)
- [ ] Checkpoints with respawn logic
- [ ] Exit portal & level complete detection

### Phase 4: Enemies & Combat (Weeks 7-8)

- [ ] Guard enemy (melee patrol + attack)
- [ ] Archer enemy (ranged attacks + arrows)
- [ ] Heavy warrior enemy (tank variant)
- [ ] Enemy-player collision & damage
- [ ] Enemy death & corpse cleanup
- [ ] Projectile system (shuriken + arrows)
- [ ] Combat sound effects

### Phase 5: Visual Polish (Weeks 9-10)

- [ ] Sky gradient & atmospheric effects
- [ ] Background trees (bare, pine, willow, cherry)
- [ ] Parallax scrolling (4-layer system)
- [ ] Particle effects (hit sparks, collection bursts, landing dust)
- [ ] Trap animations (blade spin, fire vent)
- [ ] Platform-specific drawing (theme variations)
- [ ] Character detail improvements

### Phase 6: Audio & HUD (Weeks 11-12)

- [ ] All 22 sound effect implementation
- [ ] All 5 BGM track looping
- [ ] HUD layout (hearts, gems, gold, stars, shurikens)
- [ ] Main menu with start/settings/quit
- [ ] Pause overlay with options
- [ ] Game over screen
- [ ] Level complete screen with star rating
- [ ] Settings menu (volume, fullscreen, etc.)

### Phase 7: All 10 Levels (Weeks 13-14)

- [ ] Level 1 (Tutorial - Ruins)
- [ ] Level 2 (Ruins difficulty ramp)
- [ ] Level 3 (Temple introduction)
- [ ] Level 4 (Temple with fire vents)
- [ ] Level 5 (Village)
- [ ] Level 6 (Bamboo introduction)
- [ ] Level 7 (Bamboo forest)
- [ ] Level 8 (Fortress start)
- [ ] Level 9 (Fortress advanced)
- [ ] Level 10 (Final boss / extreme challenge)

### Phase 8: Testing & Polish (Weeks 15-16)

- [ ] Full playthrough testing (all 10 levels)
- [ ] Unit test suite (shuriken constraints)
- [ ] Performance profiling & optimization
- [ ] Bug fixes & edge case handling
- [ ] Balance tweaking (difficulty, enemy damage, etc.)
- [ ] Cross-platform testing (Linux, macOS, Windows)
- [ ] Documentation completeness

---

## 5. DETAILED SPECIFICATIONS BY THEME

### Theme 0: RUINS

**Visual Description**: Ancient abandoned site with crumbling stone structures

- **Sky**: Deep blue (0.02, 0.05, 0.10)
- **Ground**: Gray stone with moss (0.20, 0.20, 0.22)
- **Accent color**: Brown/gold stone accents
- **Key decorations**:
  - Ghost trees (faint, 5% parallax): Massive silhouettes in far distance
  - Stone pillars: Various heights (150-300px tall)
  - Bare trees: Thin branching silhouettes on sides
  - Hanging cloth: Tattered banners between trees
  - Moss/vines: Detail on stone surfaces
- **Enemy placement**: Guards and Archers patrol
- **Traps**: Blade traps, ground spikes
- **Atmosphere**: Quiet, eerie, abandoned

### Theme 1: TEMPLE

**Visual Description**: Sacred temple with Asian architecture

- **Sky**: Deep indigo (0.03, 0.08, 0.18)
- **Ground**: Stone tiles with purple shadows
- **Accent colors**: Gold (lanterns, trim), red (lanterns, cloth)
- **Key decorations**:
  - Pagoda gate: Large curved roof structure
  - Red lanterns: Glowing on strings between pillars
  - Pine trees: Tall silhouettes in background
  - Temple pillars: Red-painted stone columns
  - Hanging cloth: Sacred banners
- **Enemy placement**: Archers on high ground, Heavy warriors below
- **Traps**: Fire vents, crumbling platforms, spikes
- **Atmosphere**: Mystical, reverent, active danger

### Theme 2: VILLAGE

**Visual Description**: Quiet rural Japanese village at night

- **Sky**: Dark teal (0.04, 0.10, 0.16)
- **Ground**: Dirt/gravel paths
- **Accent colors**: Muted greens, browns, tans
- **Key decorations**:
  - Willow trees: Drooping silhouettes
  - Village structures: Houses, fences
  - Wooden bridges: Rickety crossings
  - Grass tufts: Ground-level detail
  - Clotheslines: Hanging rope
- **Enemy placement**: Mixed patrols (Guards + Archers)
- **Traps**: Blade traps, thorn bushes
- **Atmosphere**: Peaceful but dangerous, residential

### Theme 3: BAMBOO

**Visual Description**: Dense bamboo forest with mystical atmosphere

- **Sky**: Very dark green-blue (0.02, 0.06, 0.08)
- **Ground**: Dark earth with grass
- **Accent colors**: Green (bamboo), dark gray (stones)
- **Key decorations**:
  - Bamboo stalks: Tall vertical lines (50-60px wide, 400-600px tall)
  - Stone markers: Graves/monuments (tall dark rectangles)
  - Bamboo thickets: Dense overlapping stalks
  - Morning mist: Semi-transparent fog layers
  - Stepping stones: Rock paths through forest
- **Enemy placement**: Heavy Warriors dominate, Archers in canopy
- **Traps**: Fire vents, blade traps, spike chains
- **Atmosphere**: Mysterious, oppressive, claustrophobic

### Theme 4: FORTRESS

**Visual Description**: Dark fortified stronghold with cherry blossoms

- **Sky**: Very dark (0.01, 0.03, 0.08)
- **Ground**: Stone flagstones, dark concrete
- **Accent colors**: Stone gray, pink cherry blossoms, dark red
- **Key decorations**:
  - Fortress walls: Massive dark stone structures
  - Gate towers: Tall sentinel structures
  - Cherry blossom trees: Pink silhouettes (contrast)
  - Watchtowers: High vantage points
  - Guard posts: Small fortified positions
- **Enemy placement**: Heavy Warriors everywhere, Guards on walls
- **Traps**: All trap types mixed, concentrated hazards
- **Atmosphere**: Militaristic, hostile, final challenge

---

## 6. PHYSICS CONSTANTS & TUNING

```c
// Gravity & Movement
#define GRAVITY            1400.0f    // px/s² downward acceleration
#define MAX_FALL_SPD       (-900.0f)  // px/s terminal velocity
#define MOVE_SPEED         265.0f     // px/s horizontal movement
#define DASH_SPEED         540.0f     // px/s dash sprint
#define WALL_SLIDE_VY      (-180.0f)  // px/s slide-down velocity

// Jumping
#define JUMP_VY            680.0f     // px/s initial jump velocity
#define DBL_JUMP_VY        600.0f     // px/s second jump (weaker)
#define WALL_JUMP_VY       610.0f     // px/s jump away from wall

// Player
#define PLAYER_W           28.0f      // pixels width
#define PLAYER_H           64.0f      // pixels height
#define MAX_HEALTH         6          // hearts (3 HP each)
#define INVULN_TIME        0.2f       // seconds after hit

// Shurikens
#define SHURIKEN_PER_LEVEL 10         // max per level
#define SHURIKEN_VELOCITY  450.0f     // px/s throw speed
#define SHURIKEN_RANGE     50.0f      // units max distance
#define SHURIKEN_SIZE      4.0f       // px radius

// Enemies
#define GUARD_SPEED        150.0f     // px/s patrol speed
#define ARCHER_SPEED       120.0f     // px/s (slower)
#define HEAVY_SPEED        100.0f     // px/s (very slow)
#define ENEMY_DETECT_RANGE 300.0f     // px sight range

// Projectiles
#define MAX_PROJECTILES    64         // active shurikens + arrows

// Time
#define DT_STEP            0.05f      // max dt per frame (20 FPS floor)
#define ANIM_FRAME_TIME    0.1f       // seconds per animation frame
```

---

## 7. COLOR PALETTE (RGB 0.0-1.0 range)

```c
// Environment
#define COL_SKY_TOP        (0.02f, 0.05f, 0.10f)  // Deep blue zenith
#define COL_SKY_BOT        (0.07f, 0.16f, 0.28f)  // Lighter horizon
#define COL_GROUND_FOG     (0.04f, 0.10f, 0.20f)  // Atmospheric haze

// Characters (Silhouettes)
#define COL_BLACK          (0.00f, 0.00f, 0.00f)  // Pure black
#define COL_DARK_SHADOW    (0.10f, 0.10f, 0.12f)  // Dark gray for detail

// Accents
#define COL_RED_ACCENT     (0.80f, 0.05f, 0.05f)  // Red (enemies, traps)
#define COL_RED_LANTERN    (0.95f, 0.40f, 0.20f)  // Orange-red glow
#define COL_GOLD           (1.00f, 0.85f, 0.10f)  // Gold (coins, stars)
#define COL_BLUE_GEM       (0.25f, 0.55f, 1.00f)  // Bright blue
#define COL_WHITE          (1.00f, 1.00f, 1.00f)  // White (text, accents)

// Special Effects
#define COL_SPARK_YELLOW   (1.00f, 1.00f, 0.80f)  // Hit sparks
#define COL_DUST_BROWN     (0.55f, 0.50f, 0.44f)  // Landing dust
#define COL_GREEN_PARTICLE (0.60f, 1.00f, 0.50f)  // Sparkle effect
```

---

## 8. FEATURE MATRIX BY LEVEL

| Level | Theme    | Difficulty | Key Mechanic       | Enemy Types    | Trap Types       | Checkpoints | Stars     |
| ----- | -------- | ---------- | ------------------ | -------------- | ---------------- | ----------- | --------- |
| 1     | RUINS    | Tutorial   | Basic platforming  | Guard only     | Blade, Spikes    | 1           | Easy      |
| 2     | RUINS    | Easy       | Double jump        | Guard, Archer  | Blade, Spikes    | 2           | Hard      |
| 3     | TEMPLE   | Easy       | Moving platforms   | Archer focus   | Spikes, Fire     | 2           | Hard      |
| 4     | TEMPLE   | Medium     | Wall-slide         | Guard, Archer  | Fire, Blade      | 2           | Hard      |
| 5     | VILLAGE  | Medium     | Dash mechanic      | All 3 types    | All types        | 3           | Hard      |
| 6     | VILLAGE  | Medium     | Collectible maze   | Guard, Heavy   | Thorn, Blade     | 1           | Hard      |
| 7     | BAMBOO   | Hard       | Speed run          | Heavy, Archer  | Fire, Spikes     | 3           | Extreme   |
| 8     | FORTRESS | Hard       | Crumbling floors   | All 3 types    | All types, mixed | 2           | Extreme   |
| 9     | FORTRESS | Extreme    | Multi-enemy combat | All 3 types    | All types, dense | 4           | Extreme   |
| 10    | FORTRESS | Extreme    | BOSS FIGHT         | Boss + minions | Trap gauntlet    | 1           | Legendary |

---

## REMAINING CONTENT

The document continues with the technical specifications from the original plan.md file.
(Sections 9-onwards covered in original plan.md remain unchanged)
│ ├── player.c player.h
│ ├── draw_player.c draw_player.h
│ ├── draw_enemy.c draw_enemy.h
│ ├── draw_map.c draw_map.h
│ ├── draw_hud.c draw_hud.h
│ ├── draw_bg.c draw_bg.h
│ ├── enemy.c enemy.h
│ ├── level.c level.h
│ ├── collision.c collision.h
│ ├── audio.c audio.h
│ ├── particles.c particles.h
│ └── ui.c ui.h
├── assets/
│ ├── audio/
│ │ ├── sfx_jump.wav
│ │ ├── sfx_double_jump.wav
│ │ ├── sfx_land.wav
│ │ ├── sfx_wall_slide.wav
│ │ ├── sfx_dash.wav
│ │ ├── sfx_attack_slash.wav
│ │ ├── sfx_attack_hit.wav
│ │ ├── sfx_shuriken_throw.wav
│ │ ├── sfx_shuriken_hit.wav
│ │ ├── sfx_player_hurt.wav
│ │ ├── sfx_player_death.wav
│ │ ├── sfx_enemy_death.wav
│ │ ├── sfx_gem_collect.wav
│ │ ├── sfx_star_collect.wav
│ │ ├── sfx_pot_break.wav
│ │ ├── sfx_blade_spin.wav
│ │ ├── sfx_fire_vent.wav
│ │ ├── sfx_platform_crumble.wav
│ │ ├── sfx_level_complete.wav
│ │ ├── sfx_game_over.wav
│ │ ├── sfx_menu_select.wav
│ │ ├── sfx_menu_confirm.wav
│ │ ├── bgm_forest.wav
│ │ ├── bgm_temple.wav
│ │ ├── bgm_village.wav
│ │ ├── bgm_bamboo.wav
│ │ └── bgm_fortress.wav
│ └── levels/
│ ├── level01.txt
│ ├── level02.txt
│ ├── level03.txt
│ ├── level04.txt
│ ├── level05.txt
│ ├── level06.txt
│ ├── level07.txt
│ ├── level08.txt
│ ├── level09.txt
│ └── level10.txt
├── include/
│ └── stb_image.h
└── Makefile

```

---

## 4. WHERE TO GET FREE AUDIO ASSETS

### Sound Effects — Free Sources

**Option A: OpenGameArt.org** (<https://opengameart.org>)

- Search "ninja jump" → download WAV files
- Search "sword slash" for attack sound
- Search "shuriken" for throw sound
- All CC0 (public domain) or CC-BY licensed — free to use
- Recommended pack: "512 Sound Effects (8-bit style)" by Juhani Junkala

**Option B: Freesound.org** (<https://freesound.org>)

- Create free account
- Search each sound name (e.g. "jump whoosh", "metal clang", "coin collect")
- Filter by CC0 license for zero-restriction use
- Download as WAV, rename to match the filenames in Section 3

**Option C: Kenney.nl** (<https://kenney.nl/assets>)

- Go to <https://kenney.nl/assets/interface-sounds>
- Go to <https://kenney.nl/assets/impact-sounds>
- Go to <https://kenney.nl/assets/RPG-audio>
- All completely free, no attribution required
- Already sorted by category — easiest source

**Option D: ZapSplat** (<https://zapsplat.com>)

- Free account gives access to thousands of SFX
- Good for atmospheric sounds like fire, wind, blade spin

**Specific Recommended Downloads:**

| File Needed              | Search Term on Freesound/OpenGameArt |
| ------------------------ | ------------------------------------ |
| sfx_jump.wav             | "jump whoosh" or "ninja jump"        |
| sfx_double_jump.wav      | "double jump airy"                   |
| sfx_land.wav             | "footstep landing thud"              |
| sfx_wall_slide.wav       | "slide scrape friction"              |
| sfx_dash.wav             | "dash swoosh fast"                   |
| sfx_attack_slash.wav     | "sword slash swing"                  |
| sfx_attack_hit.wav       | "sword hit flesh impact"             |
| sfx_shuriken_throw.wav   | "shuriken throw whiz"                |
| sfx_shuriken_hit.wav     | "metal clank impact"                 |
| sfx_player_hurt.wav      | "player hurt grunt"                  |
| sfx_player_death.wav     | "death scream short"                 |
| sfx_enemy_death.wav      | "enemy death groan"                  |
| sfx_gem_collect.wav      | "coin collect chime"                 |
| sfx_star_collect.wav     | "star collect sparkle jingle"        |
| sfx_pot_break.wav        | "pottery break smash"                |
| sfx_blade_spin.wav       | "blade spin mechanical loop"         |
| sfx_fire_vent.wav        | "fire whoosh burst"                  |
| sfx_platform_crumble.wav | "stone crumble rumble"               |
| sfx_level_complete.wav   | "level complete fanfare short"       |
| sfx_game_over.wav        | "game over low stinger"              |
| sfx_menu_select.wav      | "UI click tick"                      |
| sfx_menu_confirm.wav     | "UI confirm chime"                   |

### Background Music — Free Sources

**Option A: OpenGameArt.org**

- Search "ninja music loop" or "oriental ambient loop"
- Search "dark ambient loop" for fortress theme
- Recommended: "Traditional Japanese Music" by yd (CC-BY)

**Option B: Incompetech / Free Music Archive** (<https://freemusicarchive.org>)

- Search "Asian ambient" or "feudal Japan"
- Filter by CC0 or CC-BY

**Option C: Pixabay Music** (<https://pixabay.com/music>)

- No account needed, all royalty-free
- Search "japanese", "ninja", "oriental"
- Download as MP3, convert to WAV with Audacity (free)

**Recommended BGM per Theme:**

| File             | Mood / Search Terms                    |
| ---------------- | -------------------------------------- |
| bgm_forest.wav   | "dark forest ambient loop japanese"    |
| bgm_temple.wav   | "ancient temple music loop mysterious" |
| bgm_village.wav  | "japanese village night music loop"    |
| bgm_bamboo.wav   | "bamboo flute ambient loop calm tense" |
| bgm_fortress.wav | "dark fortress epic drums loop"        |

### Audio Tools (Free)

- **Audacity** (<https://audacityteam.org>) — trim, convert MP3→WAV, adjust volume
- **ffmpeg** — command line: `ffmpeg -i input.mp3 output.wav`

---

## 5. WHERE TO GET FREE GAME ART / IMAGES

The characters in this game are drawn with OpenGL primitives (no sprites needed).
However you may want reference images or PNG backgrounds.

### Background Reference / PNG Backgrounds

**Kenney.nl** (<https://kenney.nl/assets/background-elements>)

- Free PNG silhouette trees, clouds, hills — all CC0

**OpenGameArt.org**

- Search "silhouette background" or "parallax background dark"
- Search "japanese ruins tileset"

**Craftpix.net** (<https://craftpix.net/freebies>)

- Free section has full environment packs
- "Free Ninja Platformer Game Assets" is a direct match

**itch.io Free Assets** (<https://itch.io/game-assets/free>)

- Search "ninja platformer" — many free CC0 packs
- "Ninja Adventure Asset Pack" by Pixel Frog (free on itch.io)
- "Dark Forest Backgrounds" — multiple free packs

### Sprite Reference (for drawing accuracy)

Even though we draw primitives, reference these for proportions:

- Google Image Search: "ninja arashi character sprite sheet"
- <https://www.spriters-resource.com> — search mobile games for reference

---

## 6. COORDINATE SYSTEM

```

gluOrtho2D(0, 1280, 0, 720)
Origin (0,0) = bottom-left of screen
X increases rightward
Y increases upward
All draw functions: (float cx, float cy) = center of character feet (ground contact point)
Player collision box: 28 wide x 64 tall, centered on cx, bottom at cy

````

---

## 7. GLOBAL DRAW HELPER FUNCTIONS

Place all of these in renderer.c. Every other file includes renderer.h.

```c
#include <math.h>
#define PI 3.14159265f

void set_color(float r, float g, float b, float a) { glColor4f(r,g,b,a); }
void col_black(void)  { glColor4f(0.00f, 0.00f, 0.00f, 1.0f); }
void col_red(float a) { glColor4f(0.80f, 0.05f, 0.05f,   a);  }
void col_gold(void)   { glColor4f(1.00f, 0.85f, 0.10f, 1.0f); }
void col_white(float a){ glColor4f(1.0f, 1.0f, 1.0f,     a);  }

void draw_circle(float cx, float cy, float r, int segs) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; i++) {
        float a = 2*PI*i/segs;
        glVertex2f(cx + cosf(a)*r, cy + sinf(a)*r);
    }
    glEnd();
}

void draw_rect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x,   y);   glVertex2f(x+w, y);
    glVertex2f(x+w, y+h); glVertex2f(x,   y+h);
    glEnd();
}

void draw_tri(float x1,float y1, float x2,float y2, float x3,float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
    glEnd();
}

// Rotated rectangle: cx,cy = pivot center, angle = degrees
void draw_rect_rot(float cx, float cy, float w, float h, float angle) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0, 0, 1);
    draw_rect(-w/2, -h/2, w, h);
    glPopMatrix();
}

// Draw a ring / hollow circle (for glow halos)
void draw_ring(float cx, float cy, float r_in, float r_out, int segs) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; i++) {
        float a = 2*PI*i/segs;
        glVertex2f(cx + cosf(a)*r_in,  cy + sinf(a)*r_in);
        glVertex2f(cx + cosf(a)*r_out, cy + sinf(a)*r_out);
    }
    glEnd();
}
````

---

## 8. SKY AND ATMOSPHERE

### 8.1 Sky Gradient (observed: dark blue at top, slightly lighter at horizon)

```c
void draw_sky(void) {
    glBegin(GL_QUADS);
    // Horizon (bottom) — slightly lighter blue-teal
    glColor4f(0.07f, 0.16f, 0.28f, 1.0f); glVertex2f(0,    0);
    glColor4f(0.07f, 0.16f, 0.28f, 1.0f); glVertex2f(1280, 0);
    // Zenith (top) — deep near-black blue
    glColor4f(0.02f, 0.05f, 0.10f, 1.0f); glVertex2f(1280, 720);
    glColor4f(0.02f, 0.05f, 0.10f, 1.0f); glVertex2f(0,    720);
    glEnd();
}
```

### 8.2 Atmospheric Fog Band (ground level)

```c
// Draws a semi-transparent blue haze at ground level, like in screenshots
void draw_ground_atmosphere(void) {
    glBegin(GL_QUADS);
    glColor4f(0.04f, 0.10f, 0.20f, 0.55f); glVertex2f(0,    0);
    glColor4f(0.04f, 0.10f, 0.20f, 0.55f); glVertex2f(1280, 0);
    glColor4f(0.04f, 0.10f, 0.20f, 0.00f); glVertex2f(1280, 80);
    glColor4f(0.04f, 0.10f, 0.20f, 0.00f); glVertex2f(0,    80);
    glEnd();
}
```

### 8.3 Ghost Tree Background (observed in screenshots: faint large tree silhouettes in far distance)

```c
// These are the very faint ghost-like large trees visible in the screenshot backgrounds
// They are NOT the foreground trees — they are massive and semi-transparent
void draw_ghost_trees(float cam_x) {
    // These scroll at only 5% of camera speed — nearly stationary
    float ox = -cam_x * 0.05f;

    glColor4f(0.05f, 0.09f, 0.16f, 0.35f);  // very faint blue-dark

    // Ghost tree 1 — massive, ~400px tall
    float positions[] = {200, 480, 760, 1040};
    for (int i = 0; i < 4; i++) {
        float bx = fmodf(positions[i] + ox + 1280, 1280*3) - 640;
        float by = 0;
        float h  = 400.0f + i * 40.0f;
        float tw = h * 0.06f;

        // Ghost trunk
        glBegin(GL_QUADS);
        glVertex2f(bx - tw*0.5f, by);
        glVertex2f(bx + tw*0.5f, by);
        glVertex2f(bx + tw*0.3f, by + h*0.5f);
        glVertex2f(bx - tw*0.3f, by + h*0.5f);
        glEnd();

        // Ghost canopy — 3 overlapping triangles
        float cw = h * 0.45f;
        draw_tri(bx - cw,        by + h*0.35f, bx + cw,        by + h*0.35f, bx,        by + h*0.75f);
        draw_tri(bx - cw*0.7f,   by + h*0.50f, bx + cw*0.7f,   by + h*0.50f, bx,        by + h*0.85f);
        draw_tri(bx - cw*0.4f,   by + h*0.65f, bx + cw*0.4f,   by + h*0.65f, bx,        by + h*0.95f);
    }
}
```

---

## 9. PARALLAX BACKGROUND LAYERS

### 9.1 Layer System (3 depths observed in screenshots)

```
LAYER 0: Sky gradient + ghost trees     — cam multiplier 0.05
LAYER 1: Far silhouette trees           — cam multiplier 0.20
LAYER 2: Mid-ground ruins/structures    — cam multiplier 0.50
LAYER 3: Foreground (platforms/chars)  — cam multiplier 1.00 (world space)
```

### 9.2 Foreground Bare Trees (observed in screenshot 1 left side)

These are the tall thin BARE trees on the left of screenshot 1 with hanging cloth.

```c
void draw_bare_tree(float x, float y, float height) {
    col_black();
    float tw = height * 0.045f;  // thin trunk

    // Main trunk (slightly curved — lean toward viewer)
    glBegin(GL_QUADS);
    glVertex2f(x - tw*0.6f, y);
    glVertex2f(x + tw*0.6f, y);
    glVertex2f(x + tw*0.3f, y + height);
    glVertex2f(x - tw*0.3f, y + height);
    glEnd();

    // Bare branches — 4 to 6 short angled branches near top
    float bh = height * 0.65f;  // branch height starts here
    // Right branches
    draw_rect_rot(x + tw*3,  y + bh,        tw*0.6f, height*0.18f,  40.0f);
    draw_rect_rot(x + tw*4,  y + bh*1.15f,  tw*0.5f, height*0.12f,  55.0f);
    draw_rect_rot(x + tw*2,  y + bh*1.25f,  tw*0.4f, height*0.10f,  30.0f);
    // Left branches
    draw_rect_rot(x - tw*3,  y + bh,        tw*0.6f, height*0.18f, -40.0f);
    draw_rect_rot(x - tw*4,  y + bh*1.15f,  tw*0.5f, height*0.12f, -55.0f);
    draw_rect_rot(x - tw*2,  y + bh*1.25f,  tw*0.4f, height*0.10f, -30.0f);
}

// Hanging cloth/banner between trees (observed screenshot 1 left side)
void draw_hanging_cloth(float x1, float y1, float x2, float y2) {
    // Rope line
    glColor4f(0.08f, 0.08f, 0.08f, 0.9f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    int segs = 12;
    for (int i = 0; i <= segs; i++) {
        float t  = (float)i / segs;
        float cx = x1 + (x2-x1)*t;
        float cy = y1 + (y2-y1)*t - sinf(t*PI)*30.0f;  // catenary sag
        glVertex2f(cx, cy);
    }
    glEnd();
    glLineWidth(1.0f);

    // Cloth/banner strips hanging down from rope at intervals
    glColor4f(0.05f, 0.05f, 0.06f, 0.8f);
    for (int i = 1; i < segs; i += 3) {
        float t  = (float)i / segs;
        float rx = x1 + (x2-x1)*t;
        float ry = y1 + (y2-y1)*t - sinf(t*PI)*30.0f;
        draw_rect(rx - 5, ry - 35, 10, 35);  // hanging cloth strip
    }
}
```

### 9.3 Mid-ground Stone Pillars (prominently visible in screenshots 1, 3)

```c
// The broken stone pillars visible in the background of screenshots 1 and 3
void draw_bg_stone_pillar(float x, float y, float w, float h) {
    glColor4f(0.05f, 0.06f, 0.08f, 0.75f);
    draw_rect(x, y, w, h);

    // Cap (slightly wider top plate)
    draw_rect(x - 4, y + h - 8, w + 8, 8);

    // Base (slightly wider footing)
    draw_rect(x - 4, y, w + 8, 8);

    // Stone block divisions (horizontal lines every ~40px)
    glColor4f(0.03f, 0.04f, 0.05f, 0.75f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (float by = y + 40; by < y + h; by += 40) {
        glVertex2f(x, by);  glVertex2f(x + w, by);
    }
    glEnd();

    // Two vertical cracks
    glBegin(GL_LINE_STRIP);
    glVertex2f(x + w*0.35f, y + h*0.15f);
    glVertex2f(x + w*0.40f, y + h*0.40f);
    glVertex2f(x + w*0.38f, y + h*0.65f);
    glEnd();
}
```

### 9.4 Pagoda Gate (observed prominently in screenshot 2)

```c
// The large pagoda/temple gate that fills much of screenshot 2
void draw_pagoda_gate(float cx, float y, float scale) {
    col_black();
    float w  = 400 * scale;
    float h  = 300 * scale;
    float pw = 28  * scale;   // pillar width

    // Left pillar
    draw_rect(cx - w*0.5f, y, pw, h * 0.85f);
    // Right pillar
    draw_rect(cx + w*0.5f - pw, y, pw, h * 0.85f);

    // Cross-beam between pillars
    draw_rect(cx - w*0.5f, y + h*0.75f, w, pw);

    // CURVED EAVE ROOF (the distinctive curved pagoda roof seen in screenshot 2)
    // Main roof body
    float roof_y = y + h * 0.75f + pw;
    float roof_w = w * 1.3f;
    // Roof underside — flat
    draw_rect(cx - roof_w*0.5f, roof_y, roof_w, h*0.18f);
    // Roof top — triangle with upward curved eaves
    // Center ridge
    draw_tri(cx - roof_w*0.45f, roof_y + h*0.18f,
             cx + roof_w*0.45f, roof_y + h*0.18f,
             cx,                roof_y + h*0.42f);
    // Eave tips curled up on each side (small upward triangles at roof corners)
    draw_tri(cx - roof_w*0.50f, roof_y + h*0.10f,
             cx - roof_w*0.40f, roof_y + h*0.10f,
             cx - roof_w*0.52f, roof_y + h*0.24f);  // left eave tip
    draw_tri(cx + roof_w*0.40f, roof_y + h*0.10f,
             cx + roof_w*0.50f, roof_y + h*0.10f,
             cx + roof_w*0.52f, roof_y + h*0.24f);  // right eave tip

    // Drip/stalactite texture on roof underside (observed in screenshot 2)
    glColor4f(0.03f, 0.04f, 0.05f, 1.0f);
    for (float dx = cx - roof_w*0.45f + 12; dx < cx + roof_w*0.45f; dx += 16) {
        float drip_h = 8.0f + sinf(dx * 0.3f) * 4.0f;
        draw_tri(dx - 3, roof_y, dx + 3, roof_y, dx, roof_y - drip_h);
    }
}
```

### 9.5 Hanging Red Lanterns (observed in screenshot 2)

```c
// Rope with red lanterns hanging between gate pillars (screenshot 2)
void draw_lantern_string(float x1, float y1, float x2, float y2, int count) {
    // Rope (catenary curve)
    glColor4f(0.06f, 0.04f, 0.02f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float t  = (float)i / 20;
        float lx = x1 + (x2-x1)*t;
        float ly = y1 + (y2-y1)*t - sinf(t*PI)*40.0f;
        glVertex2f(lx, ly);
    }
    glEnd();
    glLineWidth(1.0f);

    // Lanterns evenly spaced along rope
    for (int i = 1; i <= count; i++) {
        float t  = (float)i / (count + 1);
        float lx = x1 + (x2-x1)*t;
        float ly = y1 + (y2-y1)*t - sinf(t*PI)*40.0f;

        // Short string from rope to lantern top
        glColor4f(0.06f, 0.04f, 0.02f, 1.0f);
        draw_rect(lx - 1, ly - 16, 2, 16);

        // Lantern body — rounded rectangle shape
        // Top cap
        col_black();
        draw_rect(lx - 6, ly - 20, 12, 4);
        // Middle glowing body
        glColor4f(0.85f, 0.15f, 0.05f, 1.0f);  // red-orange lantern
        draw_rect(lx - 8, ly - 40, 16, 20);
        // Inner bright center
        glColor4f(1.0f, 0.5f, 0.1f, 0.8f);
        draw_rect(lx - 5, ly - 36, 10, 12);
        // Bottom cap
        col_black();
        draw_rect(lx - 6, ly - 42, 12, 4);
        // Bottom tassel
        draw_rect(lx - 1, ly - 46, 2, 6);

        // Glow halo around lantern
        glColor4f(0.8f, 0.2f, 0.0f, 0.12f);
        draw_circle(lx, ly - 32, 24, 16);
    }
}
```

### 9.6 Bamboo Forest Background (observed in screenshot 4)

```c
// The bamboo forest background from screenshot 4
void draw_bamboo_bg(float cam_x) {
    float ox = -cam_x * 0.25f;

    for (int i = 0; i < 20; i++) {
        float bx = fmodf(i * 65.0f + ox, 1280 + 100) - 50;
        float bh = 350.0f + (i % 5) * 30.0f;
        float bw = 8.0f + (i % 3) * 2.0f;

        // Opacity varies by "depth" — alternate two depth bands
        float alpha = (i % 2 == 0) ? 0.35f : 0.55f;
        glColor4f(0.04f, 0.07f, 0.05f, alpha);

        // Bamboo shaft
        draw_rect(bx - bw/2, 0, bw, bh);

        // Bamboo nodes (horizontal bumps every ~40px)
        glColor4f(0.06f, 0.09f, 0.06f, alpha);
        for (float ny = 35; ny < bh; ny += 40) {
            draw_rect(bx - bw/2 - 2, ny, bw + 4, 4);
        }

        // Leaf clusters at top
        glColor4f(0.04f, 0.07f, 0.04f, alpha * 0.8f);
        for (int l = 0; l < 4; l++) {
            float la = (l * 70.0f + bx * 10.0f) * PI / 180.0f;
            float lx2 = bx + cosf(la) * 22;
            float ly2 = bh + sinf(la) * 10;
            draw_tri(bx, bh, lx2 - 4, ly2, lx2 + 16, ly2 + 6);
        }
    }
}
```

---

## 10. GROUND AND GRASS TUFTS (observed in screenshots)

```c
// The small grass tufts / ground vegetation at base of levels
void draw_grass_tufts(float level_w) {
    col_black();
    // Place tufts every ~30-60px across the ground
    for (float gx = 10; gx < level_w; gx += 28 + fmodf(gx * 7, 32)) {
        float h = 6.0f + fmodf(gx * 3, 8.0f);
        // 3 blades per tuft
        draw_tri(gx - 4, 40, gx,     40, gx - 2, 40 + h);
        draw_tri(gx - 1, 40, gx + 3, 40, gx + 1, 40 + h + 2);
        draw_tri(gx + 2, 40, gx + 6, 40, gx + 4, 40 + h);
    }
}
```

---

## 11. PLAYER CHARACTER — EXACT SPECIFICATION

### 11.1 Visual Description (from screenshots)

From screenshots 1, 2, 3, 4:

- Body is a **pure black silhouette**
- Distinctive **branching/horned headpiece** or wild hair on top of head
  (screenshot 3 and 4 clearly show upward-branching protrusions from the head)
- Wears a **demon face mask** — two red triangular eye marks on the face
- No visible hat in most poses — the "horns/branches" are the distinctive headpiece
- Torso: compact, slightly hunched posture
- Arms: thin, articulated
- Legs: athletic, slightly shorter relative to torso
- **In wall-slide pose** (screenshot 2): body faces wall, one hand flat against surface,
  body slightly angled, legs bent

### 11.2 Player Anatomy Measurements

```
Part              Shape              Size (w x h or radius)   Offset from (cx, cy=feet)
-----------       ----------------   ----------------------   -------------------------
Left foot         rectangle          12 x 4                   cx-13, cy
Right foot        rectangle          12 x 4                   cx+1,  cy
Left lower leg    rectangle          7  x 13                  cx-11, cy+4
Right lower leg   rectangle          7  x 13                  cx+4,  cy+4
Left upper leg    rectangle          9  x 14                  cx-11, cy+17
Right upper leg   rectangle          9  x 14                  cx+3,  cy+17
Hips/waist        rectangle          18 x 8                   cx-9,  cy+29
Torso             rectangle          20 x 16                  cx-10, cy+37
Left shoulder     rectangle          8  x 8                   cx-16, cy+50
Right shoulder    rectangle          8  x 8                   cx+8,  cy+50
Neck              rectangle          7  x 6                   cx-3,  cy+52
Head              circle             radius 10                cx, cy+62
LEFT EYE MARK     red triangle       small                    cx-6, cy+63 to cx-2,cy+63 to cx-4,cy+58
RIGHT EYE MARK    red triangle       small                    cx+2, cy+63 to cx+6,cy+63 to cx+4,cy+58
Horn/branch L     rotated rect       4  x 16                  root cx-4, cy+70, angle -25
Horn/branch R     rotated rect       4  x 14                  root cx+4, cy+70, angle +30
Sub-branch L      rotated rect       3  x 8                   off main branch, angle -50
Sub-branch R      rotated rect       3  x 8                   off main branch, angle +55
Left upper arm    rotated rect       6  x 14                  pivot cx-14, cy+50, angle 15
Left forearm      rotated rect       5  x 12                  pivot cx-17, cy+38, angle 8
Right upper arm   rotated rect       6  x 14                  pivot cx+14, cy+50, angle -15
Right forearm     rotated rect       5  x 12                  pivot cx+17, cy+38, angle -8
```

### 11.3 draw_player_idle(float cx, float cy, int facing_right)

```c
void draw_player_idle(float cx, float cy, int facing_right) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // FEET
    draw_rect(cx-13, cy,   12, 4);   // left foot
    draw_rect(cx+1,  cy,   12, 4);   // right foot

    // LOWER LEGS
    draw_rect(cx-11, cy+4,  7, 13);  // left shin
    draw_rect(cx+4,  cy+4,  7, 13);  // right shin

    // UPPER LEGS
    draw_rect(cx-11, cy+17, 9, 14);  // left thigh
    draw_rect(cx+3,  cy+17, 9, 14);  // right thigh

    // HIPS
    draw_rect(cx-9,  cy+29, 18, 9);

    // TORSO (slightly wider at shoulders, slight trapezoid)
    glBegin(GL_QUADS);
    glVertex2f(cx-9,  cy+37);
    glVertex2f(cx+9,  cy+37);
    glVertex2f(cx+11, cy+53);
    glVertex2f(cx-11, cy+53);
    glEnd();

    // NECK
    draw_rect(cx-3, cy+52, 7, 7);

    // SHOULDER PADS (slightly protruding)
    draw_rect(cx-16, cy+50, 8, 8);   // left
    draw_rect(cx+8,  cy+50, 8, 8);   // right

    // HEAD (circle)
    draw_circle(cx, cy+62, 10, 20);

    // DEMON MASK — two red eye triangles
    col_red(1.0f);
    draw_tri(cx-6, cy+63, cx-2, cy+63, cx-4, cy+58);  // left eye mark
    draw_tri(cx+2, cy+63, cx+6, cy+63, cx+4, cy+58);  // right eye mark
    col_black();

    // HORN / BRANCH HEADPIECE (distinctive feature from screenshots 3 and 4)
    // Main left branch
    glPushMatrix();
    glTranslatef(cx-3, cy+71, 0);
    glRotatef(-25.0f, 0,0,1);
    draw_rect(-2, 0, 4, 16);
    // Sub-branch off left
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(-45.0f, 0,0,1);
    draw_rect(-1, 0, 3, 8);
    glPopMatrix();
    glPopMatrix();

    // Main right branch
    glPushMatrix();
    glTranslatef(cx+3, cy+71, 0);
    glRotatef(30.0f, 0,0,1);
    draw_rect(-2, 0, 4, 14);
    // Sub-branch off right
    glPushMatrix();
    glTranslatef(0, 8, 0);
    glRotatef(50.0f, 0,0,1);
    draw_rect(-1, 0, 3, 7);
    glPopMatrix();
    glPopMatrix();

    // ARMS (idle: relaxed hang)
    draw_rect_rot(cx-14, cy+48, 6, 14,  15.0f);  // left upper arm
    draw_rect_rot(cx-17, cy+36, 5, 12,   8.0f);  // left forearm
    draw_rect_rot(cx+14, cy+48, 6, 14, -15.0f);  // right upper arm
    draw_rect_rot(cx+17, cy+36, 5, 12,  -8.0f);  // right forearm

    glPopMatrix();
}
```

### 11.4 draw_player_run(float cx, float cy, int facing_right, float anim_t)

```c
// anim_t = fmodf(game_time * 6.0f, 1.0f)
void draw_player_run(float cx, float cy, int facing_right, float anim_t) {
    float ph  = anim_t * 2*PI;
    float ls  = sinf(ph)      * 20.0f;   // leg swing degrees
    float as  = sinf(ph+PI)   * 25.0f;   // arm swing (opposite)
    float bob = fabsf(sinf(ph)) * 3.0f;  // body bounce
    cy += bob;

    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // LEGS with hip pivots
    glPushMatrix();
    glTranslatef(cx-4, cy+31, 0);
    glRotatef(-ls, 0,0,1);
    draw_rect(-4, -31, 8,  14);  // thigh
    glTranslatef(0, -22, 0);
    glRotatef(ls*0.6f, 0,0,1);
    draw_rect(-3, -13, 7,  13);  // shin
    draw_rect(-5, -15, 11,  4);  // foot
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx+4, cy+31, 0);
    glRotatef(ls, 0,0,1);
    draw_rect(-4, -31, 8,  14);
    glTranslatef(0, -22, 0);
    glRotatef(-ls*0.6f, 0,0,1);
    draw_rect(-3, -13, 7,  13);
    draw_rect(-8, -15, 11,  4);
    glPopMatrix();

    // TORSO (slight forward lean 10 degrees)
    glPushMatrix();
    glTranslatef(cx, cy+38, 0);
    glRotatef(-10.0f, 0,0,1);
    glBegin(GL_QUADS);
    glVertex2f(-9,  0); glVertex2f( 9,  0);
    glVertex2f(11, 16); glVertex2f(-11, 16);
    glEnd();
    draw_rect(-16, 14, 8, 8);  // left shoulder
    draw_rect(  8, 14, 8, 8);  // right shoulder
    draw_rect( -3, 16, 7, 7);  // neck
    glPopMatrix();

    // HEAD (shifted forward due to lean)
    float hcx = cx - 3.0f;
    float hcy = cy + 62.0f;
    draw_circle(hcx, hcy, 10, 20);
    col_red(1.0f);
    draw_tri(hcx-6,hcy+1, hcx-2,hcy+1, hcx-4,hcy-4);
    draw_tri(hcx+2,hcy+1, hcx+6,hcy+1, hcx+4,hcy-4);
    col_black();

    // HORN BRANCHES (same as idle)
    glPushMatrix(); glTranslatef(hcx-3, hcy+9, 0); glRotatef(-25,0,0,1);
    draw_rect(-2,0,4,14); glPopMatrix();
    glPushMatrix(); glTranslatef(hcx+3, hcy+9, 0); glRotatef(30,0,0,1);
    draw_rect(-2,0,4,12); glPopMatrix();

    // ARMS (swing opposite to legs)
    glPushMatrix();
    glTranslatef(cx-14, cy+52, 0); glRotatef(as, 0,0,1);
    draw_rect(-3,-14,6,14);
    glTranslatef(0,-14,0); glRotatef(-8,0,0,1);
    draw_rect(-3,-12,5,12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx+14, cy+52, 0); glRotatef(-as, 0,0,1);
    draw_rect(-3,-14,6,14);
    glTranslatef(0,-14,0); glRotatef(8,0,0,1);
    draw_rect(-3,-12,5,12);
    glPopMatrix();

    glPopMatrix();
}
```

### 11.5 draw_player_jump(float cx, float cy, int facing_right)

```c
// Legs tucked up, arms raised, body slightly arched
void draw_player_jump(float cx, float cy, int facing_right) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);
    col_black();

    // LEGS tucked upward
    glPushMatrix();
    glTranslatef(cx-5, cy+28, 0); glRotatef(35,0,0,1);
    draw_rect(-4,0,8,14);
    glTranslatef(2,14,0); glRotatef(-80,0,0,1);
    draw_rect(-3,0,7,12); draw_rect(-2,11,10,4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx+5, cy+28, 0); glRotatef(-35,0,0,1);
    draw_rect(-4,0,8,14);
    glTranslatef(-2,14,0); glRotatef(80,0,0,1);
    draw_rect(-3,0,7,12); draw_rect(-8,11,10,4);
    glPopMatrix();

    // TORSO upright
    draw_rect(cx-10, cy+28, 20, 18);
    draw_rect(cx-14, cy+44, 28, 7);
    draw_rect(cx-3,  cy+50, 7,  6);

    // ARMS raised up and out
    draw_rect_rot(cx-18, cy+48, 6, 18,  50.0f);
    draw_rect_rot(cx+18, cy+48, 6, 18, -50.0f);

    // HEAD + BRANCHES
    draw_circle(cx, cy+60, 10, 20);
    col_red(1.0f);
    draw_tri(cx-6,cy+61, cx-2,cy+61, cx-4,cy+56);
    draw_tri(cx+2,cy+61, cx+6,cy+61, cx+4,cy+56);
    col_black();
    glPushMatrix(); glTranslatef(cx-3,cy+69,0); glRotatef(-25,0,0,1); draw_rect(-2,0,4,14); glPopMatrix();
    glPushMatrix(); glTranslatef(cx+3,cy+69,0); glRotatef(30,0,0,1);  draw_rect(-2,0,4,12); glPopMatrix();

    glPopMatrix();
}
```

### 11.6 draw_player_fall(float cx, float cy, int facing_right)

```c
// Arms spread wide, legs extended down
void draw_player_fall(float cx, float cy, int facing_right) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);
    col_black();

    // LEGS straight down
    draw_rect(cx-11, cy,    8, 28);
    draw_rect(cx+3,  cy,    8, 28);
    draw_rect(cx-14, cy-2, 12,  4);
    draw_rect(cx+3,  cy-2, 12,  4);

    // TORSO lean back slightly
    glPushMatrix();
    glTranslatef(cx, cy+38, 0); glRotatef(6,0,0,1);
    draw_rect(-10,0,20,17);
    draw_rect(-14,15,28,7);
    draw_rect(-3,21,7,6);
    glPopMatrix();

    // ARMS wide spread
    draw_rect_rot(cx-20, cy+44, 6, 20,  75.0f);
    draw_rect_rot(cx+20, cy+44, 6, 20, -75.0f);

    // HEAD
    draw_circle(cx+2, cy+60, 10, 20);
    col_red(1.0f);
    draw_tri(cx-4,cy+61, cx,cy+61, cx-2,cy+56);
    draw_tri(cx+4,cy+61, cx+8,cy+61, cx+6,cy+56);
    col_black();
    glPushMatrix(); glTranslatef(cx-1,cy+69,0); glRotatef(-25,0,0,1); draw_rect(-2,0,4,13); glPopMatrix();
    glPushMatrix(); glTranslatef(cx+5,cy+69,0); glRotatef(30,0,0,1);  draw_rect(-2,0,4,11); glPopMatrix();

    glPopMatrix();
}
```

### 11.7 draw_player_wall_slide(float cx, float cy, int wall_side)

```c
// wall_side: +1 = sliding on right wall, -1 = sliding on left wall
// From screenshot 2: body faces wall, hand pressed flat against surface,
// legs slightly bent, body at ~15 degree tilt toward wall
void draw_player_wall_slide(float cx, float cy, int wall_side) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)wall_side, 1.0f, 1.0f);   // face the wall direction
    glTranslatef(-cx, -cy, 0);

    col_black();

    // LEGS bent, feet flat against wall
    glPushMatrix();
    glTranslatef(cx+4, cy+20, 0); glRotatef(-20,0,0,1);
    draw_rect(-4,-20,8,20);
    draw_rect(0,-22,10,4);   // foot pressed against wall (extends right)
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx+4, cy+36, 0); glRotatef(-10,0,0,1);
    draw_rect(-4,-16,8,16);
    glPopMatrix();

    // TORSO tilted toward wall (body leans right = toward wall)
    glPushMatrix();
    glTranslatef(cx, cy+42, 0); glRotatef(15.0f, 0,0,1);
    draw_rect(-9,0,18,18);
    draw_rect(-12,16,24,7);
    draw_rect(-3,22,7,6);
    glPopMatrix();

    // LEFT ARM reaching toward wall (hand flat on surface)
    draw_rect_rot(cx+16, cy+48, 6, 18, -80.0f);  // nearly horizontal toward wall
    draw_rect(cx+22, cy+44, 10, 5);               // hand / palm pressed flat

    // RIGHT ARM behind body
    draw_rect_rot(cx-12, cy+48, 6, 14, -30.0f);

    // HEAD
    float hcx = cx + 4;
    float hcy = cy + 62;
    draw_circle(hcx, hcy, 10, 20);
    col_red(1.0f);
    draw_tri(hcx-4,hcy+1, hcx,hcy+1, hcx-2,hcy-4);
    draw_tri(hcx+2,hcy+1, hcx+6,hcy+1, hcx+4,hcy-4);
    col_black();
    glPushMatrix(); glTranslatef(hcx-2,hcy+9,0); glRotatef(-25,0,0,1); draw_rect(-2,0,4,13); glPopMatrix();
    glPushMatrix(); glTranslatef(hcx+4,hcy+9,0); glRotatef(30,0,0,1);  draw_rect(-2,0,4,11); glPopMatrix();

    glPopMatrix();
}
```

### 11.8 draw_player_attack(float cx, float cy, int facing_right, float anim_t)

```c
void draw_player_attack(float cx, float cy, int facing_right, float anim_t) {
    float swing = anim_t * 130.0f - 35.0f;

    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // STANCE legs (wide base)
    draw_rect(cx-13, cy,     8, 24);
    draw_rect(cx+5,  cy,     8, 24);
    draw_rect(cx-15, cy-2,  12,  4);
    draw_rect(cx+5,  cy-2,  12,  4);

    // TORSO lean forward -18 degrees
    glPushMatrix();
    glTranslatef(cx, cy+36, 0); glRotatef(-18,0,0,1);
    draw_rect(-9,0,18,18);
    draw_rect(-13,16,26,7);
    draw_rect(-3,22,7,6);
    glPopMatrix();

    // LEFT ARM (counterbalance behind)
    draw_rect_rot(cx-15, cy+44, 6, 14, -35.0f);

    // RIGHT ARM swings the sword
    glPushMatrix();
    glTranslatef(cx+12, cy+48, 0);
    glRotatef(-swing, 0,0,1);
    draw_rect(-3,-16,6,16);  // upper arm
    glTranslatef(0,-16,0);
    draw_rect(-3,-12,5,12);  // forearm

    // SWORD
    glTranslatef(0,-13,0);
    col_black();
    draw_rect(-3,-4,6,4);      // grip
    draw_rect(-9,-7,18,3);     // crossguard
    glColor4f(0.14f,0.14f,0.16f,1);
    draw_tri(-2,-7, 2,-7, 0,-50);  // blade
    // Edge highlight
    glColor4f(0.3f,0.3f,0.35f,1);
    draw_tri(1,-7, 2,-7, 0,-50);
    glPopMatrix();

    // SLASH TRAIL
    if (anim_t > 0.05f && anim_t < 0.85f) {
        float ta = 1.0f - fabsf(anim_t - 0.45f) * 2.2f;
        col_white(ta * 0.65f);
        glLineWidth(2.5f);
        for (int i = 0; i < 6; i++) {
            float a  = (-swing + 25.0f + i*14.0f) * PI / 180.0f;
            float r1 = 18.0f, r2 = 55.0f;
            glBegin(GL_LINES);
            glVertex2f(cx+12 + cosf(a)*r1, cy+48 + sinf(a)*r1);
            glVertex2f(cx+12 + cosf(a)*r2, cy+48 + sinf(a)*r2);
            glEnd();
        }
        glLineWidth(1.0f);
        col_black();
    }

    // HEAD
    float hcx = cx - 3, hcy = cy + 61;
    draw_circle(hcx, hcy, 10, 20);
    col_red(1.0f);
    draw_tri(hcx-6,hcy+1, hcx-2,hcy+1, hcx-4,hcy-4);
    draw_tri(hcx+2,hcy+1, hcx+6,hcy+1, hcx+4,hcy-4);
    col_black();
    glPushMatrix(); glTranslatef(hcx-3,hcy+9,0); glRotatef(-25,0,0,1); draw_rect(-2,0,4,14); glPopMatrix();
    glPushMatrix(); glTranslatef(hcx+3,hcy+9,0); glRotatef(30,0,0,1);  draw_rect(-2,0,4,12); glPopMatrix();

    glPopMatrix();
}
```

### 11.9 draw_player_hurt(float cx, float cy, int facing_right)

```c
void draw_player_hurt(float cx, float cy, int facing_right) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);
    col_black();

    // STAGGERED legs
    draw_rect(cx-9, cy,    7, 20);
    draw_rect(cx+3, cy+5,  7, 16);
    draw_rect(cx-13,cy-2, 11,  4);
    draw_rect(cx+2, cy+3, 10,  4);

    // TORSO — hard backward lean
    glPushMatrix();
    glTranslatef(cx, cy+34, 0); glRotatef(22,0,0,1);
    draw_rect(-9,0,18,17);
    draw_rect(-12,15,24,7);
    draw_rect(-3,21,7,6);
    glPopMatrix();

    // ARMS flung outward
    draw_rect_rot(cx-16, cy+46, 6, 18, 105.0f);
    draw_rect_rot(cx+16, cy+46, 6, 18, -85.0f);

    // HEAD thrown back
    float hcx = cx+5, hcy = cy+62;
    draw_circle(hcx, hcy, 10, 20);
    col_red(1.0f);
    draw_tri(hcx-4,hcy+1, hcx,hcy+1, hcx-2,hcy-4);
    draw_tri(hcx+2,hcy+1, hcx+6,hcy+1, hcx+4,hcy-4);
    col_black();
    glPushMatrix(); glTranslatef(hcx,hcy+9,0); glRotatef(-25,0,0,1); draw_rect(-2,0,4,13); glPopMatrix();
    glPushMatrix(); glTranslatef(hcx+6,hcy+9,0); glRotatef(30,0,0,1); draw_rect(-2,0,4,11); glPopMatrix();

    glPopMatrix();
}
```

### 11.10 draw_shuriken(float cx, float cy, float angle)

```c
// angle += 360 * 5 * dt per frame
void draw_shuriken(float cx, float cy, float angle) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0,0,1);

    // The shuriken UI icons visible bottom-left in ALL screenshots
    // Two wing-shapes forming an X — NOT simple triangles
    col_white(1.0f);

    // 4 blade triangles at 90-degree intervals
    float outer = 11.0f, inner = 4.5f;
    for (int i = 0; i < 4; i++) {
        float a  = i * 90.0f * PI / 180.0f;
        float al = (i*90.0f - 28.0f) * PI / 180.0f;
        float ar = (i*90.0f + 28.0f) * PI / 180.0f;
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(al)*inner, sinf(al)*inner);
        glVertex2f(cosf(ar)*inner, sinf(ar)*inner);
        glVertex2f(cosf(a)*outer,  sinf(a)*outer);
        glEnd();
    }
    // Center ring
    glColor4f(0.3f,0.3f,0.35f,1);
    draw_circle(0,0,inner,10);
    glColor4f(0.7f,0.7f,0.75f,1);
    draw_circle(0,0,2,8);

    glPopMatrix();
}
```

---

## 12. ENEMY CHARACTERS — EXACT SPECIFICATION

### 12.1 Patrol Guard / Warrior (observed in screenshots 1, 2, 3, 4)

From screenshots: stands upright, holds a long weapon over one shoulder (scythe or polearm).
The weapon rests on the shoulder — blade/head visible above and behind the shoulder.

```c
void draw_enemy_guard(float cx, float cy, int facing_right, float anim_t) {
    float ph = anim_t * 2*PI;
    float ls = sinf(ph) * 12.0f;

    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // FEET + LEGS
    draw_rect(cx-13, cy,    12, 4);
    draw_rect(cx+1,  cy,    12, 4);
    glPushMatrix();
    glTranslatef(cx-5, cy+27, 0); glRotatef(-ls,0,0,1);
    draw_rect(-5,-27,10,14); // thigh
    glTranslatef(0,-18,0); glRotatef(ls*0.5f,0,0,1);
    draw_rect(-4,-13,8,13); draw_rect(-5,-15,12,4); // shin + foot
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cx+5, cy+27, 0); glRotatef(ls,0,0,1);
    draw_rect(-5,-27,10,14);
    glTranslatef(0,-18,0); glRotatef(-ls*0.5f,0,0,1);
    draw_rect(-4,-13,8,13); draw_rect(-9,-15,12,4);
    glPopMatrix();

    // TORSO (stocky, broader than player)
    glBegin(GL_QUADS);
    glVertex2f(cx-11, cy+27); glVertex2f(cx+11, cy+27);
    glVertex2f(cx+13, cy+45); glVertex2f(cx-13, cy+45);
    glEnd();

    // ARMOR CHEST PLATE (slightly inset rectangle)
    glColor4f(0.06f,0.06f,0.07f,1);
    draw_rect(cx-8, cy+31, 16, 12);
    col_black();

    // PAULDRONS (square shoulder armor)
    draw_rect(cx-18, cy+43, 11, 9);
    draw_rect(cx+7,  cy+43, 11, 9);

    // NECK + HELMET
    draw_rect(cx-4, cy+45, 8,  6);
    draw_circle(cx, cy+55, 12, 20);  // round helmet
    // Helmet crest / ridge
    draw_tri(cx-5, cy+65, cx+5, cy+65, cx, cy+73);
    // Visor slit (dark blue visible = the "eye" of the helmet)
    glColor4f(0.04f, 0.08f, 0.16f, 1.0f);
    draw_rect(cx-8, cy+52, 16, 4);
    col_black();

    // LEFT ARM — casual hang
    draw_rect_rot(cx-16, cy+42, 7, 15, 12.0f);
    draw_rect_rot(cx-18, cy+29, 6, 13, 5.0f);

    // RIGHT ARM — holding scythe/weapon over shoulder
    draw_rect_rot(cx+14, cy+44, 7, 15, -20.0f);

    // SCYTHE WEAPON (resting on right shoulder, blade visible above)
    // The weapon observed in screenshots: long pole, crescent-like head above shoulder
    // Shaft (long diagonal pole)
    glColor4f(0.14f,0.09f,0.04f,1);  // dark wood
    glPushMatrix();
    glTranslatef(cx+20, cy+52, 0);
    glRotatef(-15.0f, 0,0,1);
    draw_rect(-2.5f, -15, 5, 70);    // long pole extending up and down
    // Scythe head (crescent blade at top)
    col_black();
    glTranslatef(0, 55, 0);
    // Outer arc of blade
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 12; i++) {
        float a = (180.0f + i * 15.0f) * PI / 180.0f;
        glVertex2f(cosf(a)*22, sinf(a)*22);
    }
    glEnd();
    // Inner cutout (crescent shape — remove inner circle)
    glColor4f(0.07f,0.16f,0.28f,1);  // sky-matching color = creates crescent illusion
    draw_circle(-8, 8, 14, 14);
    glPopMatrix();

    glPopMatrix();
}
```

### 12.2 Archer Enemy

```c
void draw_enemy_archer(float cx, float cy, int facing_right, int is_shooting) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // LEGS
    draw_rect(cx-10, cy,    7, 20);
    draw_rect(cx+3,  cy,    7, 20);
    draw_rect(cx-13, cy-2, 12, 4);
    draw_rect(cx+2,  cy-2, 12, 4);

    // TORSO
    draw_rect(cx-9, cy+20, 18, 17);
    draw_rect(cx-12,cy+35, 24, 6);
    draw_rect(cx-3, cy+40, 7,  6);

    // HEAD with HOOD (pointed tip)
    draw_circle(cx, cy+50, 10, 20);
    draw_tri(cx-8, cy+58, cx+8, cy+58, cx+5, cy+68);  // hood point

    // Red glowing eye
    glColor4f(0.9f,0.1f,0.1f,1);
    draw_circle(cx+3, cy+51, 2.5f, 8);
    // Eye glow
    glColor4f(0.9f,0.1f,0.1f,0.2f);
    draw_circle(cx+3, cy+51, 6, 10);
    col_black();

    // LEFT ARM extended holding bow
    draw_rect_rot(cx+14, cy+38, 6, 18, -82.0f);

    // BOW (arc)
    glLineWidth(3.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 14; i++) {
        float t  = (float)i/14;
        float a  = (t-0.5f) * 2.4f;
        float bx = cx + 29.0f + sinf(a) * 7.0f;
        float by = cy + 38.0f + (t-0.5f) * 42.0f;
        glVertex2f(bx, by);
    }
    glEnd();
    glLineWidth(1.0f);

    // BOWSTRING
    float pull = is_shooting ? 14.0f : 0.0f;
    glBegin(GL_LINES);
    glVertex2f(cx+29, cy+59); glVertex2f(cx+29+pull, cy+38);
    glVertex2f(cx+29+pull, cy+38); glVertex2f(cx+29, cy+17);
    glEnd();

    // RIGHT ARM pulling back
    draw_rect_rot(cx+10+pull, cy+38, 6, 16, -78.0f);

    // Arrow nocked
    if (is_shooting) {
        glColor4f(0.65f,0.5f,0.2f,1);
        draw_rect(cx+22, cy+36, 20, 3);
        col_black();
        draw_tri(cx+42,cy+35, cx+42,cy+39, cx+48,cy+37);
    }

    glPopMatrix();
}
```

### 12.3 draw_arrow(float cx, float cy, float vx, float vy)

```c
void draw_arrow(float cx, float cy, float vx, float vy) {
    float angle = atan2f(vy, vx) * 180.0f / PI;
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0,0,1);
    glColor4f(0.7f,0.55f,0.2f,1); draw_rect(-16,-1,32,3);
    col_black();
    draw_tri(16,-3, 16,3, 24,0);
    glColor4f(0.6f,0.1f,0.1f,1);
    draw_tri(-16,0,-11,4,-9,0);
    draw_tri(-16,0,-11,-4,-9,0);
    glPopMatrix();
}
```

### 12.4 Heavy Warrior (Level 5+)

```c
void draw_enemy_heavy(float cx, float cy, int facing_right, float anim_t) {
    float ph = anim_t * 2*PI;
    float ls = sinf(ph) * 8.0f;

    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)facing_right, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    col_black();

    // MASSIVE LEGS
    glPushMatrix();
    glTranslatef(cx-7, cy+30, 0); glRotatef(-ls,0,0,1);
    draw_rect(-7,-30,14,30); draw_rect(-9,-32,20,5);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cx+7, cy+30, 0); glRotatef(ls,0,0,1);
    draw_rect(-7,-30,14,30); draw_rect(-11,-32,20,5);
    glPopMatrix();

    // BARREL TORSO
    glBegin(GL_QUADS);
    glVertex2f(cx-14,cy+30); glVertex2f(cx+14,cy+30);
    glVertex2f(cx+17,cy+52); glVertex2f(cx-17,cy+52);
    glEnd();

    // HUGE SHOULDERS
    draw_rect(cx-24,cy+50,15,11);
    draw_rect(cx+9, cy+50,15,11);
    draw_rect(cx-5, cy+54, 10, 8);  // neck

    // LARGE HELMETED HEAD with full face mask
    draw_circle(cx, cy+65, 15, 24);
    glColor4f(0.07f,0.07f,0.08f,1);
    draw_rect(cx-11,cy+61, 22, 3);
    draw_rect(cx-11,cy+66, 22, 3);
    glColor4f(0.9f,0.05f,0.05f,1);
    draw_rect(cx-9, cy+68, 7, 2);
    draw_rect(cx+2, cy+68, 7, 2);
    col_black();

    // ARMS — left heavy hang, right holds cleaver
    draw_rect_rot(cx-22,cy+48,11,22,12.0f);
    draw_rect_rot(cx-24,cy+28,10,20,5.0f);
    draw_rect_rot(cx+22,cy+54,11,24,-28.0f);

    // CLEAVER
    glPushMatrix();
    glTranslatef(cx+34, cy+60, 0); glRotatef(-38,0,0,1);
    glColor4f(0.18f,0.1f,0.04f,1); draw_rect(-3,-22,6,22);  // handle
    col_black();
    glBegin(GL_QUADS);
    glVertex2f(-4,0); glVertex2f(24,0);
    glVertex2f(24,30); glVertex2f(-4,20);
    glEnd();
    glColor4f(0.2f,0.2f,0.22f,1); draw_rect(21,0,3,30);  // edge
    glPopMatrix();

    glPopMatrix();
}
```

---

## 13. TRAPS AND HAZARDS — EXACT SPECIFICATION

### 13.1 Rotating Blade / Circular Saw (observed in screenshots 1 and 3)

The red circular saw in screenshots 1 and 3 is a distinctive red-bladed spinning saw
sitting on or near the ground. It has RED blades (not dark), very visible.

```c
// angle += 200.0f * dt per frame (fast spin)
void draw_blade_trap(float cx, float cy, float angle) {
    // Mount post
    col_black();
    draw_rect(cx-5, cy-20, 10, 20);

    // Blade base circle (dark body)
    glColor4f(0.08f,0.02f,0.02f,1);
    draw_circle(cx, cy, 30, 24);

    // Spinning red blades
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0,0,1);

    for (int i = 0; i < 6; i++) {
        float a  = i * 60.0f * PI / 180.0f;
        float al = (i*60.0f - 22.0f) * PI / 180.0f;
        float ar = (i*60.0f + 22.0f) * PI / 180.0f;
        // Each blade: inner base then outer point
        glColor4f(0.85f, 0.05f, 0.05f, 1.0f);  // BRIGHT RED blades
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(al)*12, sinf(al)*12);
        glVertex2f(cosf(ar)*12, sinf(ar)*12);
        glVertex2f(cosf(a)*30,  sinf(a)*30);
        glEnd();
        // Blade highlight (lighter red tip)
        glColor4f(1.0f, 0.3f, 0.3f, 0.7f);
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(a)*22, sinf(a)*22);
        glVertex2f(cosf(al)*18, sinf(al)*18);
        glVertex2f(cosf(a)*30, sinf(a)*30);
        glEnd();
    }
    // Center hub
    glColor4f(0.15f,0.08f,0.08f,1); draw_circle(0,0,10,14);
    glColor4f(0.3f,0.1f,0.1f,1);    draw_circle(0,0,5,10);
    glPopMatrix();

    // Red glow halo beneath trap
    glColor4f(0.8f,0.0f,0.0f,0.18f);
    draw_circle(cx, cy, 38, 20);
}
```

### 13.2 Red Thorn / Spike Cactus (observed prominently in screenshot 2)

The large red spiky cactus/thorn bush on the lower-left of screenshot 2.
This is NOT just a spike trap — it's a tall, multi-layered spiky plant shape.

```c
void draw_thorn_bush(float cx, float y, float scale) {
    float s = scale;

    // Main dark body (irregular blob shape)
    glColor4f(0.04f,0.02f,0.02f,1);
    draw_circle(cx, y + 40*s, 30*s, 16);
    draw_circle(cx-15*s, y + 25*s, 22*s, 14);
    draw_circle(cx+12*s, y + 28*s, 20*s, 14);
    draw_circle(cx, y + 58*s, 22*s, 14);

    // RED SPIKES radiating outward (this is the key visual from screenshot 2)
    glColor4f(0.85f, 0.06f, 0.06f, 1.0f);
    // Generate spikes around the bush perimeter
    float spike_positions[][3] = {
        // {angle, distance from center, spike_length}
        {0,   30, 18}, {30,  35, 22}, {60,  32, 20}, {90,  36, 24},
        {120, 33, 19}, {150, 30, 17}, {180, 35, 21}, {210, 32, 18},
        {240, 34, 22}, {270, 36, 20}, {300, 30, 18}, {330, 33, 20},
        {15,  38, 16}, {75,  34, 19}, {135, 36, 17}, {195, 32, 21},
        {255, 35, 18}, {315, 30, 22}
    };
    float center_y = y + 40*s;
    for (int i = 0; i < 18; i++) {
        float a  = spike_positions[i][0] * PI / 180.0f;
        float r  = spike_positions[i][1] * s;
        float sl = spike_positions[i][2] * s;
        float bx = cx     + cosf(a) * r;
        float by = center_y + sinf(a) * r;
        float ex = cx     + cosf(a) * (r + sl);
        float ey = center_y + sinf(a) * (r + sl);
        float pw = 3.0f * s;
        // Spike as thin triangle
        float perp = a + PI/2;
        draw_tri(bx + cosf(perp)*pw, by + sinf(perp)*pw,
                 bx - cosf(perp)*pw, by - sinf(perp)*pw,
                 ex, ey);
    }

    // Spike red glow
    glColor4f(0.8f,0.0f,0.0f,0.12f);
    draw_circle(cx, center_y, 55*s, 18);
}
```

### 13.3 Ground Spikes

```c
void draw_spikes_ground(float x, float y, int count) {
    float sw = 14.0f;
    glColor4f(0.05f,0.02f,0.02f,1);
    for (int i = 0; i < count; i++) {
        float sx = x + i*sw;
        draw_rect(sx, y, sw, 5);
        draw_tri(sx+1, y+5, sx+sw-1, y+5, sx+sw/2, y+22);
    }
    glColor4f(0.9f,0.05f,0.05f,0.3f);
    for (int i = 0; i < count; i++) {
        draw_circle(x + i*sw + sw/2, y+22, 4, 8);
    }
}
```

### 13.4 Fire Vent

```c
// active: 1 = fire on, 0 = off
// fire_t = 0..1 animation of flame height
void draw_fire_vent(float cx, float cy, int active, float fire_t) {
    // Vent base
    col_black();
    draw_rect(cx-12, cy, 24, 8);
    draw_rect(cx-8,  cy, 16, 14);

    if (active) {
        float fh = (40.0f + sinf(fire_t * 2*PI) * 12.0f);
        // Outer flame (red)
        glColor4f(0.9f,0.1f,0.0f, 0.85f);
        draw_tri(cx-12, cy+14, cx+12, cy+14, cx, cy+14+fh);
        // Mid flame (orange)
        glColor4f(1.0f,0.5f,0.0f, 0.75f);
        draw_tri(cx-8,  cy+14, cx+8,  cy+14, cx, cy+14+fh*0.75f);
        // Inner flame (yellow)
        glColor4f(1.0f,0.9f,0.1f, 0.6f);
        draw_tri(cx-4,  cy+14, cx+4,  cy+14, cx, cy+14+fh*0.5f);
        // Glow
        glColor4f(0.9f,0.3f,0.0f,0.15f);
        draw_circle(cx, cy+14+fh*0.3f, fh*0.6f, 12);
    }
}
```

---

## 14. PROPS AND ENVIRONMENT OBJECTS

### 14.1 Clay Pots (breakable — observed in screenshots 1, 3, 4)

Screenshot 1 shows two pots side by side on the ground. They are round clay jugs.
Screenshot 3 shows a large pot center-ground. Screenshot 4 shows multiple pots.

```c
// broken: 0 = intact, 1 = just broken (show shards), 2 = gone
void draw_clay_pot(float cx, float cy, int broken, float break_t) {
    if (broken == 2) return;

    if (!broken) {
        col_black();
        // Pot base
        draw_rect(cx-8, cy, 16, 8);
        // Pot body (wide round middle)
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy+20);
        int segs = 16;
        for (int i = 0; i <= segs; i++) {
            float a  = PI + PI*(float)i/segs;  // bottom semicircle
            float rx = 14 + sinf((float)i/segs * PI) * 2;  // slight bulge
            glVertex2f(cx + cosf(a)*rx, cy+20 + sinf(a)*18);
        }
        glEnd();
        // Pot upper (narrows to neck)
        draw_rect(cx-10, cy+20, 20, 14);
        // Neck
        draw_rect(cx-5,  cy+34, 10, 8);
        // Rim
        draw_rect(cx-8,  cy+40, 16, 4);

        // Surface texture lines
        glColor4f(0.04f,0.04f,0.05f,1);
        for (int i = 1; i <= 3; i++) {
            float ty = cy + i*10;
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j <= 10; j++) {
                float a = PI + PI*(float)j/10;
                float rx = 12 + sinf((float)j/10*PI)*2;
                glVertex2f(cx + cosf(a)*rx * (0.85f + i*0.05f), ty + sinf(a)*2);
            }
            glEnd();
        }
    } else {
        // BROKEN: scattered shards exploding outward
        glColor4f(0.06f,0.05f,0.04f,1.0f - break_t);
        float shard_angles[] = {0, 45, 90, 135, 180, 225, 270, 315};
        for (int i = 0; i < 8; i++) {
            float a  = shard_angles[i] * PI / 180.0f;
            float r  = break_t * 40.0f;
            float sx = cx + cosf(a)*r;
            float sy = cy+15 + sinf(a)*r - break_t*20;  // gravity
            float sw = 6 - break_t*4;
            float sh = 8 - break_t*5;
            if (sw > 0 && sh > 0)
                draw_rect_rot(sx, sy, sw, sh, shard_angles[i]);
        }
    }
}
```

### 14.2 Large Stone Grave Markers / Monoliths (observed in screenshot 4 right side)

```c
void draw_stone_marker(float cx, float y, float w, float h) {
    col_black();
    // Main body (slight taper toward top)
    glBegin(GL_QUADS);
    glVertex2f(cx-w/2,   y);
    glVertex2f(cx+w/2,   y);
    glVertex2f(cx+w/2*0.7f, y+h);
    glVertex2f(cx-w/2*0.7f, y+h);
    glEnd();
    // Rounded top
    draw_circle(cx, y+h, w/2*0.7f, 12);
    // Moss/lichen texture
    glColor4f(0.04f,0.06f,0.04f,0.6f);
    for (int i = 0; i < 5; i++) {
        float mx = cx - w*0.3f + i * w*0.15f;
        float my = y + h*0.2f + (i%3) * h*0.2f;
        draw_circle(mx, my, 4+i, 8);
    }
    // Ancient carving lines
    glColor4f(0.03f,0.03f,0.04f,1);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(cx, y+h*0.2f); glVertex2f(cx, y+h*0.7f);
    glVertex2f(cx-w*0.2f, y+h*0.45f); glVertex2f(cx+w*0.2f, y+h*0.45f);
    glEnd();
}
```

### 14.3 Rope Bridge / Fence (observed at bottom-right of screenshot 2)

```c
void draw_rope_fence(float x, float y, float length, int post_count) {
    float spacing = length / (post_count - 1);
    col_black();

    // Fence posts
    for (int i = 0; i < post_count; i++) {
        float px = x + i * spacing;
        draw_rect(px-3, y, 6, 28);
        draw_rect(px-5, y+24, 10, 5);  // post cap
    }

    // Two horizontal rope lines
    glColor4f(0.12f,0.08f,0.04f,1);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= 20; j++) {
        float t  = (float)j/20;
        float rx = x + length*t;
        float ry = y + 18 - sinf(fmodf(t*post_count, 1.0f)*PI) * 4;
        glVertex2f(rx, ry);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= 20; j++) {
        float t  = (float)j/20;
        float rx = x + length*t;
        float ry = y + 10 - sinf(fmodf(t*post_count, 1.0f)*PI) * 3;
        glVertex2f(rx, ry);
    }
    glEnd();
    glLineWidth(1.0f);

    // Vertical rope links between the two horizontal ropes
    glColor4f(0.10f,0.07f,0.03f,1);
    for (int i = 0; i < post_count-1; i++) {
        for (int k = 1; k <= 2; k++) {
            float vx = x + (i + k*0.33f) * spacing;
            glBegin(GL_LINES);
            glVertex2f(vx, y+10); glVertex2f(vx, y+18);
            glEnd();
        }
    }
}
```

### 14.4 Hanging Platform (moss + grass on top — observed in screenshot 2)

```c
void draw_hanging_platform(float cx, float y, float w) {
    // Support chains/ropes from ceiling
    glColor4f(0.08f,0.06f,0.04f,1);
    draw_rect(cx - w*0.35f - 2, y+16, 4, 40);  // left rope
    draw_rect(cx + w*0.35f - 2, y+16, 4, 40);  // right rope

    // Platform body (dark wood)
    glColor4f(0.05f,0.04f,0.02f,1);
    draw_rect(cx-w/2, y, w, 16);

    // Wooden planks (horizontal lines)
    glColor4f(0.08f,0.06f,0.03f,1);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (float py = y+5; py < y+16; py += 5) {
        glVertex2f(cx-w/2, py); glVertex2f(cx+w/2, py);
    }
    glEnd();

    // Moss / grass tufts on top of platform (the dark fluffy top from screenshot 2)
    glColor4f(0.04f,0.06f,0.03f,1);
    for (float gx = cx-w/2+8; gx < cx+w/2-8; gx += 12) {
        float gh = 6 + sinf(gx*0.4f)*3;
        draw_tri(gx-4, y+16, gx+4, y+16, gx,   y+16+gh);
        draw_tri(gx,   y+16, gx+8, y+16, gx+4, y+16+gh-2);
    }
    glLineWidth(1.0f);
}
```

---

## 15. PLATFORM DRAW SPECIFICATIONS (5 THEMES)

### Selection logic

```c
void draw_platform(float x, float y, float w, float h, int theme) {
    switch(theme) {
        case 0: draw_platform_ruins(x,y,w,h);    break;
        case 1: draw_platform_temple(x,y,w,h);   break;
        case 2: draw_platform_village(x,y,w,h);  break;
        case 3: draw_platform_bamboo(x,y,w,h);   break;
        case 4: draw_platform_fortress(x,y,w,h); break;
    }
}
```

### Theme 0 — RUINS (green-black mossy stone with cracks)

```c
void draw_platform_ruins(float x, float y, float w, float h) {
    glColor4f(0.04f,0.06f,0.04f,1); draw_rect(x,y,w,h);
    glColor4f(0.07f,0.10f,0.06f,1); draw_rect(x, y+h-4, w, 4);  // mossy cap
    glColor4f(0.02f,0.03f,0.02f,1);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (float bx=x+32; bx<x+w; bx+=32) { glVertex2f(bx,y); glVertex2f(bx,y+h); }
    glEnd();
    for (float dx=x+18; dx<x+w; dx+=44) draw_rect(dx,y,2,h*0.28f);  // drips
}
```

### Theme 1 — TEMPLE (purple-dark stone, pillar grooves)

```c
void draw_platform_temple(float x, float y, float w, float h) {
    glColor4f(0.05f,0.04f,0.06f,1); draw_rect(x,y,w,h);
    glColor4f(0.09f,0.08f,0.10f,1); draw_rect(x-2,y+h-6,w+4,6);
    glColor4f(0.03f,0.02f,0.04f,1);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float bx=x+24; bx<x+w; bx+=24) { glVertex2f(bx,y); glVertex2f(bx,y+h-6); }
    glEnd();
    glLineWidth(1.0f);
}
```

### Theme 2 — VILLAGE (dark brown wood planks)

```c
void draw_platform_village(float x, float y, float w, float h) {
    glColor4f(0.06f,0.04f,0.02f,1); draw_rect(x,y,w,h);
    glColor4f(0.10f,0.10f,0.10f,1); draw_rect(x,y+h-3,w,3);
    glColor4f(0.08f,0.06f,0.03f,1);
    glBegin(GL_LINES);
    for (float by=y+6; by<y+h; by+=7) { glVertex2f(x,by); glVertex2f(x+w,by); }
    glEnd();
    glColor4f(0.03f,0.02f,0.01f,1); glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float bx=x+48; bx<x+w; bx+=48) { glVertex2f(bx,y); glVertex2f(bx,y+h); }
    glEnd();
    glLineWidth(1.0f);
}
```

### Theme 3 — BAMBOO (dark earth with bamboo stalk texture)

```c
void draw_platform_bamboo(float x, float y, float w, float h) {
    glColor4f(0.04f,0.05f,0.03f,1); draw_rect(x,y,w,h);
    glColor4f(0.06f,0.08f,0.05f,1);
    for (float bx=x+10; bx<x+w; bx+=14) draw_rect(bx,y,4,h);
    glColor4f(0.05f,0.06f,0.04f,1);
    glBegin(GL_LINES);
    for (float bx=x+10; bx<x+w; bx+=14)
        for (float by=y+14; by<y+h; by+=20) { glVertex2f(bx,by); glVertex2f(bx+4,by); }
    glEnd();
}
```

### Theme 4 — FORTRESS (iron black with rivets)

```c
void draw_platform_fortress(float x, float y, float w, float h) {
    glColor4f(0.02f,0.02f,0.03f,1); draw_rect(x,y,w,h);
    glColor4f(0.05f,0.05f,0.06f,1); draw_rect(x,y+h-7,w,7);
    for (float bx=x+8; bx<x+w; bx+=16) draw_circle(bx,y+h-3.5f,2,6);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (float bx=x+20; bx<x+w; bx+=20) { glVertex2f(bx,y); glVertex2f(bx,y+h-7); }
    glEnd();
    glLineWidth(1.0f);
}
```

---

## 16. COLLECTIBLES

### 16.1 Glowing Star (observed in screenshot 4 with green-gold glow and sparks)

```c
// pulse_t = fmodf(game_time*2, 1.0f)
// The star in screenshot 4 emits yellow-green particles/sparks
void draw_star_collectible(float cx, float cy, float pulse_t) {
    float pulse = 1.0f + sinf(pulse_t*2*PI) * 0.15f;
    float outer = 15.0f * pulse;
    float inner = 6.5f  * pulse;

    // Outer glow rings (double halo like in screenshot 4)
    glColor4f(0.8f, 1.0f, 0.1f, 0.08f); draw_circle(cx,cy, outer+18, 16);
    glColor4f(1.0f, 0.9f, 0.1f, 0.18f); draw_circle(cx,cy, outer+8,  16);

    // 5-pointed star
    glColor4f(1.0f, 0.88f, 0.05f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 10; i++) {
        float r = (i%2==0) ? outer : inner;
        float a = (i*36.0f - 90.0f) * PI / 180.0f;
        glVertex2f(cx + cosf(a)*r, cy + sinf(a)*r);
    }
    glEnd();

    // Bright white-yellow center
    glColor4f(1.0f,1.0f,0.8f,1.0f); draw_circle(cx,cy,inner*0.5f,8);

    // Sparkle particles (the green-gold sparks visible in screenshot 4)
    for (int i = 0; i < 6; i++) {
        float sa = (i*60.0f + pulse_t*360.0f) * PI / 180.0f;
        float sr = (outer+10) + sinf(pulse_t*2*PI + i)*5;
        float sx = cx + cosf(sa)*sr;
        float sy = cy + sinf(sa)*sr;
        glColor4f(0.6f,1.0f,0.1f, 0.8f - pulse_t*0.3f);
        draw_circle(sx, sy, 2.5f, 6);
    }
}
```

### 16.2 Gem (blue diamond)

```c
void draw_gem(float cx, float cy) {
    // Glow
    glColor4f(0.2f,0.5f,1.0f,0.2f); draw_circle(cx,cy,14,12);
    // Diamond shape: 4 triangles
    glColor4f(0.3f,0.65f,1.0f,1);
    draw_tri(cx,cy+8,  cx+7,cy+3,  cx+7,cy-4);
    draw_tri(cx,cy-9,  cx+7,cy-4,  cx+7,cy+3);
    draw_tri(cx,cy+8,  cx-7,cy+3,  cx-7,cy-4);
    draw_tri(cx,cy-9,  cx-7,cy-4,  cx-7,cy+3);
    // Highlight
    glColor4f(0.7f,0.9f,1.0f,0.7f);
    draw_tri(cx,cy+8, cx+7,cy+3, cx+2,cy+2);
}
```

---

## 17. HUD — EXACT LAYOUT FROM SCREENSHOTS

### 17.1 Layout Description (from all 4 screenshots)

```
TOP-LEFT AREA:
  - Circular avatar portrait (dark circle with lighter ring, ~76px diameter)
  - Heart icon + number to the right of avatar (e.g. "6")
  - Blue gem icon + number below heart row (e.g. "9999864")
  - Gold coin icon + number below gem row (e.g. "99984034")
  - 3 star icons below the avatar portrait (0-3 filled gold stars)

BOTTOM-LEFT AREA:
  - Two shuriken icons (left-pointing and right-pointing)
  - These are the throw-left / throw-right buttons
  - Each is ~50px, white/silver color, the 4-bladed star shape

BOTTOM-RIGHT AREA:
  - 4 circular skill buttons in a 2x2 grid layout
  - Each button is ~70px diameter with a dark border ring
  - Top-left button: Ninja with hat (summon/skill)
  - Top-right button: Sword attack slash (red motion)
  - Bottom-left button: Dash/speed ninja (yellow motion)
  - Bottom-right button: Jump/explosion burst

TOP-RIGHT:
  - Settings gear icon (white outline)
```

```c
void draw_hud(int health, int max_health, int gems, int gold, int stars,
              float game_time) {
    // ==================== TOP-LEFT HUD ====================

    // AVATAR ring
    glColor4f(0.25f,0.25f,0.30f,1); draw_circle(50, 670, 40, 24);
    col_black();                      draw_circle(50, 670, 33, 24);
    // Mini player silhouette in avatar
    glColor4f(0.35f,0.35f,0.40f,1);
    draw_circle(50, 674, 12, 16);
    draw_rect(42, 658, 16, 14);
    // Mini branches on avatar
    col_black();
    glPushMatrix(); glTranslatef(47,683,0); glRotatef(-25,0,0,1); draw_rect(-1,0,3,9); glPopMatrix();
    glPushMatrix(); glTranslatef(53,683,0); glRotatef(30,0,0,1);  draw_rect(-1,0,3,8); glPopMatrix();
    // Red eye marks on avatar
    col_red(1.0f);
    draw_tri(44,675, 48,675, 46,671);
    draw_tri(52,675, 56,675, 54,671);
    col_black();

    // HEARTS
    float heart_x = 100;
    for (int i = 0; i < max_health; i++) {
        float hx = heart_x + i * 18.0f;
        float hy = 696.0f;
        if (i < health) glColor4f(0.92f,0.12f,0.12f,1);
        else            glColor4f(0.28f,0.10f,0.10f,0.5f);
        draw_circle(hx-3, hy, 5, 10);
        draw_circle(hx+3, hy, 5, 10);
        draw_tri(hx-7,hy-2, hx+7,hy-2, hx, hy-11);
    }

    // GEM ICON + COUNT
    float gix = 98, giy = 678;
    glColor4f(0.25f,0.55f,1.0f,1);
    draw_tri(gix,giy+7,   gix+6,giy+3,  gix+6,giy-3);
    draw_tri(gix,giy-7,   gix+6,giy-3,  gix+6,giy+3);
    draw_tri(gix,giy+7,   gix-6,giy+3,  gix-6,giy-3);
    draw_tri(gix,giy-7,   gix-6,giy-3,  gix-6,giy+3);
    col_white(1.0f);
    char gem_str[24]; sprintf(gem_str, "%d", gems);
    glRasterPos2f(gix+10, giy-6);
    for (char* c = gem_str; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // GOLD COIN ICON + COUNT
    float cix = 98, ciy = 658;
    glColor4f(1.0f,0.78f,0.1f,1); draw_circle(cix, ciy, 7, 12);
    glColor4f(0.8f,0.55f,0.0f,1); draw_circle(cix, ciy, 5, 12);
    col_white(1.0f);
    char gold_str[24]; sprintf(gold_str, "%d", gold);
    glRasterPos2f(cix+10, ciy-6);
    for (char* c = gold_str; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // STARS (0-3 earned this level)
    for (int i = 0; i < 3; i++) {
        float sx = 20.0f + i*22.0f, sy = 636.0f;
        if (i < stars) {
            glColor4f(1.0f,0.88f,0.05f,1);
            glBegin(GL_TRIANGLE_FAN); glVertex2f(sx,sy);
            for (int j=0;j<=10;j++){
                float r=(j%2==0)?9.0f:4.0f, a=(j*36.0f-90)*PI/180.0f;
                glVertex2f(sx+cosf(a)*r, sy+sinf(a)*r);
            } glEnd();
        } else {
            glColor4f(0.30f,0.30f,0.30f,0.5f); draw_circle(sx,sy,9,12);
        }
    }

    // ==================== BOTTOM-LEFT SHURIKENS ====================
    // Left-throw shuriken (faces left)
    draw_shuriken(60, 60, game_time * -180.0f);   // spins slowly in UI
    // Right-throw shuriken (faces right)
    draw_shuriken(130, 60, game_time * -180.0f);

    // ==================== BOTTOM-RIGHT SKILL BUTTONS ====================
    float btn_cx[4] = {1130, 1210, 1130, 1210};
    float btn_cy[4] = {130,  130,   60,   60};

    for (int i = 0; i < 4; i++) {
        // Outer ring
        glColor4f(0.28f,0.28f,0.32f,0.85f); draw_circle(btn_cx[i],btn_cy[i],36,20);
        // Inner fill
        glColor4f(0.12f,0.12f,0.15f,0.90f); draw_circle(btn_cx[i],btn_cy[i],30,20);
    }

    // Button 0 (top-left): Ninja hat icon
    glColor4f(0.55f,0.50f,0.45f,1);
    draw_circle(btn_cx[0], btn_cy[0]+4, 12, 16);
    draw_rect(btn_cx[0]-14, btn_cy[0]+14, 28, 3);
    draw_tri(btn_cx[0]-10,  btn_cy[0]+17, btn_cx[0]+10, btn_cy[0]+17, btn_cx[0], btn_cy[0]+26);

    // Button 1 (top-right): Sword slash icon (red)
    glColor4f(0.8f,0.1f,0.1f,1);
    for (int j=0;j<4;j++){
        float a = (j*45.0f+10.0f)*PI/180.0f;
        draw_rect_rot(btn_cx[1], btn_cy[1], 3, 28, j*45.0f+10.0f);
    }

    // Button 2 (bottom-left): Dash icon (yellow streak + ninja)
    glColor4f(0.9f,0.7f,0.1f,1);
    draw_rect(btn_cx[2]-22, btn_cy[2]-2, 28, 4);
    draw_rect(btn_cx[2]-16, btn_cy[2]+2, 20, 3);
    draw_rect(btn_cx[2]-10, btn_cy[2]+5, 12, 2);

    // Button 3 (bottom-right): Jump burst icon
    glColor4f(0.7f,0.7f,0.75f,1);
    for (int j=0;j<6;j++){
        float a = j*60.0f*PI/180.0f;
        draw_rect_rot(btn_cx[3]+cosf(a)*14, btn_cy[3]+sinf(a)*14, 2, 10, j*60.0f);
    }
    draw_circle(btn_cx[3], btn_cy[3], 7, 10);

    // ==================== TOP-RIGHT SETTINGS GEAR ====================
    glColor4f(0.7f,0.7f,0.75f,0.85f);
    draw_circle(1240, 690, 18, 24);
    col_black();
    draw_circle(1240, 690, 12, 24);
    // Gear teeth
    glColor4f(0.7f,0.7f,0.75f,0.85f);
    for (int i=0;i<8;i++){
        float a = i*45.0f*PI/180.0f;
        draw_rect_rot(1240+cosf(a)*18, 690+sinf(a)*18, 4, 8, i*45.0f);
    }
}
```

---

## 18. ANIMATION STATE MACHINE

```c
typedef enum {
    ANIM_IDLE, ANIM_RUN, ANIM_JUMP, ANIM_FALL,
    ANIM_WALL_SLIDE, ANIM_ATTACK, ANIM_THROW,
    ANIM_DASH, ANIM_HURT, ANIM_DEAD
} AnimState;

typedef struct {
    AnimState state;
    float     timer;
    float     anim_t;
    int       facing;   // +1 right, -1 left
} Animation;

void animation_update(Animation* a, float dt) {
    a->timer += dt;
    switch (a->state) {
        case ANIM_IDLE:       a->anim_t = sinf(a->timer*1.5f)*0.5f+0.5f; break;
        case ANIM_RUN:        a->anim_t = fmodf(a->timer*6.0f, 1.0f); break;
        case ANIM_ATTACK:     a->anim_t = fminf(a->timer/0.25f,1.0f);
                              if(a->timer>=0.25f){a->state=ANIM_IDLE;a->timer=0;} break;
        case ANIM_HURT:       a->anim_t = fminf(a->timer/0.40f,1.0f);
                              if(a->timer>=0.40f){a->state=ANIM_IDLE;a->timer=0;} break;
        default: break;
    }
}

void draw_player_dispatch(Player* p) {
    switch(p->anim.state) {
        case ANIM_IDLE:       draw_player_idle(p->x,p->y,p->anim.facing);             break;
        case ANIM_RUN:        draw_player_run(p->x,p->y,p->anim.facing,p->anim.anim_t); break;
        case ANIM_JUMP:       draw_player_jump(p->x,p->y,p->anim.facing);             break;
        case ANIM_FALL:       draw_player_fall(p->x,p->y,p->anim.facing);             break;
        case ANIM_WALL_SLIDE: draw_player_wall_slide(p->x,p->y,p->anim.facing);       break;
        case ANIM_ATTACK:     draw_player_attack(p->x,p->y,p->anim.facing,p->anim.anim_t); break;
        case ANIM_HURT:       draw_player_hurt(p->x,p->y,p->anim.facing);             break;
        default:              draw_player_idle(p->x,p->y,p->anim.facing);             break;
    }
}
```

---

## 19. PHYSICS

```c
#define PLAYER_W       28.0f
#define PLAYER_H       64.0f
#define GRAVITY      1400.0f
#define JUMP_VY       680.0f
#define DBL_JUMP_VY   600.0f
#define MOVE_SPEED    265.0f
#define DASH_SPEED    540.0f
#define WALL_SLIDE_VY (-180.0f)   // slow fall when wall sliding
#define MAX_FALL_SPD  (-900.0f)

typedef struct {
    float x, y, vx, vy;
    float w, h;
    int on_ground, on_wall;
    int jump_count;   // 0=available, 1=used first, 2=used double
    int health;
    float hurt_timer;
    float attack_timer;
    float dash_timer;
    float dash_cooldown;
    Animation anim;
} Player;

void player_physics(Player* p, float dt) {
    p->vy -= GRAVITY * dt;
    if (p->vy < MAX_FALL_SPD) p->vy = MAX_FALL_SPD;
    if (p->on_wall != 0 && !p->on_ground && p->vy < WALL_SLIDE_VY) {
        p->vy = WALL_SLIDE_VY;
        if (p->anim.state != ANIM_ATTACK) p->anim.state = ANIM_WALL_SLIDE;
    }
    p->x += p->vx * dt;
    p->y += p->vy * dt;
    if (p->hurt_timer > 0)    p->hurt_timer    -= dt;
    if (p->dash_cooldown > 0) p->dash_cooldown -= dt;
}
```

---

## 20. AUDIO SYSTEM

### 20.1 Audio Triggers Table (14+ sounds)

| Sound File               | Trigger Condition                                  |
| ------------------------ | -------------------------------------------------- |
| sfx_jump.wav             | Player presses jump while on_ground == 1           |
| sfx_double_jump.wav      | Player presses jump while jump_count == 1 (in air) |
| sfx_land.wav             | Player transitions from ANIM_FALL to ANIM_IDLE/RUN |
| sfx_wall_slide.wav       | Player enters ANIM_WALL_SLIDE (loop while sliding) |
| sfx_dash.wav             | Player triggers dash                               |
| sfx_attack_slash.wav     | Player presses attack button                       |
| sfx_attack_hit.wav       | Player attack hitbox overlaps with enemy           |
| sfx_shuriken_throw.wav   | Player throws shuriken (left or right)             |
| sfx_shuriken_hit.wav     | Shuriken projectile hits wall or enemy             |
| sfx_player_hurt.wav      | Player health decreases (not dead)                 |
| sfx_player_death.wav     | Player health reaches 0                            |
| sfx_enemy_death.wav      | Any enemy health reaches 0                         |
| sfx_gem_collect.wav      | Player overlaps gem collectible                    |
| sfx_star_collect.wav     | Player overlaps star collectible                   |
| sfx_pot_break.wav        | Player attack or player body hits clay pot         |
| sfx_blade_spin.wav       | Looping while blade trap is on screen (loop)       |
| sfx_fire_vent.wav        | Fire vent activates (on state transition)          |
| sfx_platform_crumble.wav | Crumbling platform begins to fall                  |
| sfx_level_complete.wav   | Player reaches EXIT tile                           |
| sfx_game_over.wav        | Player death with 0 lives remaining                |
| sfx_menu_select.wav      | Cursor moves between menu items                    |
| sfx_menu_confirm.wav     | Player confirms a menu selection                   |
| bgm_forest.wav           | Levels 1-2: loop entire time                       |
| bgm_temple.wav           | Levels 3-4: loop entire time                       |
| bgm_village.wav          | Levels 5-6: loop entire time                       |
| bgm_bamboo.wav           | Levels 7-8: loop entire time                       |
| bgm_fortress.wav         | Levels 9-10: loop entire time                      |

### 20.2 Audio Implementation

```c
// audio.h
typedef enum {
    SFX_JUMP, SFX_DOUBLE_JUMP, SFX_LAND, SFX_WALL_SLIDE, SFX_DASH,
    SFX_ATTACK_SLASH, SFX_ATTACK_HIT, SFX_SHURIKEN_THROW, SFX_SHURIKEN_HIT,
    SFX_PLAYER_HURT, SFX_PLAYER_DEATH, SFX_ENEMY_DEATH,
    SFX_GEM_COLLECT, SFX_STAR_COLLECT, SFX_POT_BREAK,
    SFX_BLADE_SPIN, SFX_FIRE_VENT, SFX_PLATFORM_CRUMBLE,
    SFX_LEVEL_COMPLETE, SFX_GAME_OVER, SFX_MENU_SELECT, SFX_MENU_CONFIRM,
    SFX_COUNT
} SoundEffect;

typedef enum {
    BGM_FOREST, BGM_TEMPLE, BGM_VILLAGE, BGM_BAMBOO, BGM_FORTRESS,
    BGM_COUNT
} BGMTrack;

void audio_init(void);
void audio_play_sfx(SoundEffect sfx);
void audio_play_bgm(BGMTrack track);   // stops current, starts new looping track
void audio_stop_bgm(void);
void audio_stop_sfx(SoundEffect sfx);  // for looping SFX like blade_spin
```

```c
// audio.c — Windows implementation
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")

static const char* sfx_files[SFX_COUNT] = {
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
    "assets/audio/sfx_menu_confirm.wav"
};

static const char* bgm_files[BGM_COUNT] = {
    "assets/audio/bgm_forest.wav",
    "assets/audio/bgm_temple.wav",
    "assets/audio/bgm_village.wav",
    "assets/audio/bgm_bamboo.wav",
    "assets/audio/bgm_fortress.wav"
};

void audio_play_sfx(SoundEffect sfx) {
    PlaySoundA(sfx_files[sfx], NULL, SND_FILENAME | SND_ASYNC);
}

void audio_play_bgm(BGMTrack track) {
    PlaySoundA(bgm_files[track], NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void audio_stop_bgm(void) {
    PlaySoundA(NULL, NULL, 0);
}
#endif

// Linux implementation
#ifndef _WIN32
#include <stdlib.h>
void audio_play_sfx(SoundEffect sfx) {
    char cmd[256];
    sprintf(cmd, "aplay %s &", sfx_files[sfx]);
    system(cmd);
}
void audio_play_bgm(BGMTrack track) {
    char cmd[256];
    sprintf(cmd, "aplay -q --loop %s &", bgm_files[track]);
    system(cmd);
}
#endif
```

### 20.3 BGM Transition Logic

```c
// Call when level changes or game state changes
void set_level_bgm(int level_number) {
    audio_stop_bgm();
    if      (level_number <= 2)  audio_play_bgm(BGM_FOREST);
    else if (level_number <= 4)  audio_play_bgm(BGM_TEMPLE);
    else if (level_number <= 6)  audio_play_bgm(BGM_VILLAGE);
    else if (level_number <= 8)  audio_play_bgm(BGM_BAMBOO);
    else                         audio_play_bgm(BGM_FORTRESS);
}
```

---

## 21. LEVEL FILE FORMAT

```
# level02.txt — Forest Ruins with first enemies
WIDTH        3840
HEIGHT       720
THEME        0            # 0=RUINS 1=TEMPLE 2=VILLAGE 3=BAMBOO 4=FORTRESS
BGM          0            # BGM track index
PLAYER_START 120  100

# Platforms: PLATFORM x y width height
PLATFORM     0    0   3840  40    # main ground
PLATFORM     320  120  160  32
PLATFORM     560  200  128  32
PLATFORM     780  320  200  32
PLATFORM     1060 200  160  32
PLATFORM     1300 360  240  32

# One-way ledge (pass through from below)
ONE_WAY      880  165   96

# Moving platform: MOVING x y w h axis(X|Y) amplitude speed
MOVING       1500 280  128  32  X  90  1.4

# Crumbling: CRUMBLING x y width  (falls after 1.5s of standing)
CRUMBLING    1850 320  128

# Enemies
ENEMY GUARD  420  40   300  600   # patrol between x=300 and x=600
ENEMY ARCHER 1020 40

# Traps
TRAP BLADE   740  40
TRAP SPIKE   640  40  4  UP
TRAP FIRE    1280 40  32  1.8  2.2

# Props
POT          360  40
POT          380  40
POT          1100 40

# Hanging cloth/rope decoration
CLOTH        200  280  340  260   # x1 y1 x2 y2

# Collectibles
GEM          340  160
GEM          580  240
GEM          820  360
STAR         1140 440

# Background props (drawn behind everything)
BG_PILLAR    600  40  40  200
BG_PILLAR    920  40  40  260
BG_TREE      160  0   280
BG_TREE      260  0   240

# Exit portal
EXIT         3720 40
```

---

## 22. LEVEL PROGRESSION

| Level | Theme           | BGM      | New Mechanics                         | Enemy Types        | Difficulty |
| ----- | --------------- | -------- | ------------------------------------- | ------------------ | ---------- |
| 1     | Forest Ruins    | Forest   | Basic run/jump/collect, no enemies    | none               | Easy       |
| 2     | Forest Ruins    | Forest   | Guards, pots, gem collecting          | Guard              | Easy       |
| 3     | Stone Temple    | Temple   | Archers, spike traps                  | Guard, Archer      | Easy       |
| 4     | Stone Temple    | Temple   | Moving platforms, one-way ledges      | Guard, Archer      | Easy-Med   |
| 5     | Night Village   | Village  | Heavy warrior, rotating blade traps   | Guard, Heavy       | Medium     |
| 6     | Night Village   | Village  | Crumbling platforms, thorn bushes     | Guard, Archer, Hvy | Medium     |
| 7     | Bamboo Canyon   | Bamboo   | Fire vents, vertical moving platforms | All types          | Medium     |
| 8     | Bamboo Canyon   | Bamboo   | Blade + moving platform combos        | All types          | Medium     |
| 9     | Shadow Fortress | Fortress | All trap types, multiple heavies      | All types          | Medium     |
| 10    | Shadow Fortress | Fortress | Boss (3-phase heavy warrior)          | Boss + all types   | Medium     |

---

## 23. DRAW ORDER (every frame)

```
1.  draw_sky()
2.  draw_ghost_trees(cam_x)                 -- faint huge trees (0.05x parallax)
3.  draw_ground_atmosphere()                -- fog band
4.  [parallax 0.20] draw_bare_tree() forest or draw_bamboo_bg() bamboo levels
5.  [parallax 0.20] draw_hanging_cloth() where defined
6.  [parallax 0.50] draw_bg_stone_pillar() and draw_bg_pagoda()
7.  glPushMatrix(); glTranslatef(-cam_x, -cam_y, 0);   -- ENTER WORLD SPACE
8.  for each platform: draw_platform() using level theme
9.  for each hanging_platform: draw_hanging_platform()
10. for each rope_fence: draw_rope_fence()
11. for each stone_marker: draw_stone_marker()
12. for each lantern_string: draw_lantern_string()
13. for each TRAP_BLADE: draw_blade_trap() with current rotation angle
14. for each TRAP_SPIKE: draw_spikes_ground()
15. for each TRAP_THORN: draw_thorn_bush()
16. for each TRAP_FIRE: draw_fire_vent()
17. for each POT (intact): draw_clay_pot(cx,cy,0,0)
18. for each GEM: draw_gem()
19. for each STAR: draw_star_collectible() with pulse_t
20. draw_grass_tufts(level_w)
21. for each ENEMY: draw_enemy_guard/archer/heavy()
22. for each ARROW: draw_arrow()
23. for each SHURIKEN: draw_shuriken()
24. draw_player_dispatch(&player)
25. for each PARTICLE: draw particle
26. glPopMatrix();                           -- EXIT WORLD SPACE
27. draw_hud(...)                            -- always on top, no camera transform
```

---

## 24. PARTICLE SYSTEM

```c
#define MAX_PARTICLES 128
typedef struct {
    float x, y, vx, vy;
    float life, max_life;
    float r, g, b, a;
    float size;
    int   active;
} Particle;

Particle particles[MAX_PARTICLES];

void particle_spawn(float x, float y, float vx, float vy,
                    float r, float g, float b, float life, float size) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            particles[i] = (Particle){x,y,vx,vy,life,life,r,g,b,1.0f,size,1};
            return;
        }
    }
}

// Spawn hit sparks (white dots scatter on attack hit)
void spawn_hit_sparks(float cx, float cy) {
    for (int i = 0; i < 8; i++) {
        float a = i * 45.0f * PI / 180.0f;
        float speed = 80 + (i%3)*30;
        particle_spawn(cx,cy, cosf(a)*speed, sinf(a)*speed+60,
                       1,1,0.8f, 0.3f, 3.0f);
    }
}

// Spawn collect burst (gold/green sparkles on star collect)
void spawn_collect_burst(float cx, float cy) {
    for (int i = 0; i < 12; i++) {
        float a = i * 30.0f * PI / 180.0f;
        float speed = 60 + (i%4)*25;
        particle_spawn(cx,cy, cosf(a)*speed, sinf(a)*speed+80,
                       0.8f+0.2f*(i%2), 0.9f, 0.1f*(i%3), 0.6f, 4.0f);
    }
}

void particles_update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        particles[i].x    += particles[i].vx * dt;
        particles[i].y    += particles[i].vy * dt;
        particles[i].vy   -= 200.0f * dt;  // gravity on particles
        particles[i].life -= dt;
        particles[i].a     = particles[i].life / particles[i].max_life;
        if (particles[i].life <= 0) particles[i].active = 0;
    }
}

void particles_draw(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].a);
        draw_circle(particles[i].x, particles[i].y, particles[i].size, 6);
    }
}
```

---

## 25. CAMERA

```c
typedef struct { float x, y; } Camera;

void camera_update(Camera* cam, float px, float py, float level_w, float dt) {
    float tx = px - 1280 * 0.38f;
    float ty = py - 720  * 0.28f;
    cam->x += (tx - cam->x) * 9.0f * dt;
    cam->y += (ty - cam->y) * 7.0f * dt;
    if (cam->x < 0)              cam->x = 0;
    if (cam->x > level_w - 1280) cam->x = level_w - 1280;
    if (cam->y < 0)              cam->y = 0;
}
```

---

## 26. MAIN LOOP

```c
static int    last_ms   = 0;
static Camera camera    = {0,0};
static float  game_time = 0;

void display(void) {
    int   now = glutGet(GLUT_ELAPSED_TIME);
    float dt  = (now - last_ms) / 1000.0f;
    if (dt > 0.05f) dt = 0.05f;
    last_ms   = now;
    game_time += dt;

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (game_state == STATE_PLAYING) {
        player_physics(&player, dt);
        resolve_platforms(&player, platforms, plat_count);
        update_enemies(enemies, enemy_count, &player, dt);
        update_traps(traps, trap_count, dt);
        particles_update(dt);
        animation_update(&player.anim, dt);
        camera_update(&camera, player.x, player.y, level_width, dt);

        // DRAW
        draw_sky();
        draw_ghost_trees(camera.x);
        draw_ground_atmosphere();
        draw_parallax_layer1(camera.x, level_theme);
        draw_parallax_layer2(camera.x, level_theme);

        glPushMatrix();
        glTranslatef(-camera.x, -camera.y, 0);

        draw_all_platforms(level_theme);
        draw_all_props();
        draw_all_traps(game_time);
        draw_all_collectibles(game_time);
        draw_grass_tufts(level_width);
        draw_all_enemies(game_time);
        draw_all_projectiles();
        draw_player_dispatch(&player);
        particles_draw();

        glPopMatrix();

        draw_hud(player.health, MAX_HEALTH, gems, gold, stars, game_time);
    }
    else if (game_state == STATE_MENU)          draw_main_menu(game_time);
    else if (game_state == STATE_PAUSED)        draw_pause_overlay();
    else if (game_state == STATE_GAME_OVER)     draw_game_over_screen();
    else if (game_state == STATE_LEVEL_COMPLETE)draw_level_complete(stars);

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Ninja Shadow");
    glClearColor(0.02f, 0.05f, 0.10f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 720);
    glMatrixMode(GL_MODELVIEW);
    last_ms = glutGet(GLUT_ELAPSED_TIME);
    glutDisplayFunc(display);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutSpecialFunc(special_down);
    glutSpecialUpFunc(special_up);
    audio_init();
    load_level(1);
    set_level_bgm(1);
    glutMainLoop();
    return 0;
}
```

---

## 27. BUILD

```makefile
CC     = gcc
CFLAGS = -Wall -O2 -Iinclude
LIBS   = -lGL -lGLU -lglut -lm
SRC    = $(wildcard src/*.c)

all:
 $(CC) $(SRC) -o ninja_shadow $(CFLAGS) $(LIBS)

windows:
 gcc $(SRC) -o ninja_shadow.exe -Iinclude -Llib -lfreeglut -lopengl32 -lglu32 -lwinmm -lm

clean:
 rm -f ninja_shadow ninja_shadow.exe
```

---

## 28. DEVELOPMENT PHASES

### Phase 1 — Visuals Foundation (Week 1)

- [ ] Window opens, sky gradient renders
- [ ] draw_player_idle() renders ninja with branches, red mask marks, full anatomy
- [ ] draw_bare_tree() renders correctly with branches
- [ ] draw_ghost_trees() faint background parallax
- [ ] draw_platform_ruins() with mossy cap and cracks (NOT a grey rect)
- [ ] Player moves left/right, jumps, gravity works

### Phase 2 — Full Platforming (Week 2)

- [ ] AABB collision with all platform types
- [ ] Jump, double jump, wall slide (draw_player_wall_slide)
- [ ] Camera smooth follow with clamp
- [ ] Level .txt loader parses all tokens
- [ ] All 5 platform themes rendering correctly
- [ ] Run/jump/fall animation switching

### Phase 3 — Enemies and Combat (Week 3)

- [ ] draw_enemy_guard() with scythe renders correctly
- [ ] Guard patrol AI + attack range
- [ ] draw_enemy_archer() with working bow arc
- [ ] Arrow projectile with draw_arrow()
- [ ] Player attack animation + hitbox
- [ ] draw_shuriken() throw in both directions
- [ ] Enemy health / death (spawn hit sparks on death)

### Phase 4 — Traps and Props (Week 4)

- [ ] draw_blade_trap() spinning with red blades (angle += 200\*dt)
- [ ] draw_thorn_bush() with red spikes (from screenshot 2)
- [ ] draw_spikes_ground() instant kill
- [ ] draw_fire_vent() on/off cycle
- [ ] Moving platform oscillation
- [ ] Crumbling platform
- [ ] draw_clay_pot() breakable with shards particle
- [ ] draw_lantern_string() with glowing red lanterns
- [ ] draw_rope_fence()

### Phase 5 — Audio and HUD (Week 5)

- [ ] All 22 SFX loaded and triggered correctly
- [ ] BGM transitions between 5 themes
- [ ] Full HUD with draw_hud() — hearts, gem icon, coin icon, stars, skill buttons
- [ ] draw_star_collectible() with pulse and sparkles
- [ ] Particle system for hits, collects
- [ ] All 10 levels .txt files complete
- [ ] Heavy enemy for levels 5-10

### Phase 6 — Screens and Polish (Week 6)

- [ ] Main menu screen
- [ ] Pause overlay
- [ ] Level complete screen with star animation
- [ ] Game over screen
- [ ] 60 FPS verified
- [ ] All sounds trigger at correct moments
- [ ] Final playtest all 10 levels

---

## 29. QUICK ASSET DOWNLOAD CHECKLIST

Do these in order before starting Phase 5:

```
[ ] Go to https://kenney.nl/assets/impact-sounds
    Download → rename to: sfx_attack_hit.wav, sfx_pot_break.wav, sfx_platform_crumble.wav

[ ] Go to https://kenney.nl/assets/interface-sounds
    Download → rename to: sfx_menu_select.wav, sfx_menu_confirm.wav,
                          sfx_gem_collect.wav, sfx_level_complete.wav

[ ] Go to https://freesound.org (free account)
    Search "sword slash" → sfx_attack_slash.wav
    Search "jump whoosh" → sfx_jump.wav
    Search "ninja shuriken" → sfx_shuriken_throw.wav
    Search "metal clank" → sfx_shuriken_hit.wav
    Search "death grunt" → sfx_player_death.wav, sfx_enemy_death.wav
    Search "hurt grunt" → sfx_player_hurt.wav
    Search "fire burst" → sfx_fire_vent.wav
    Search "stone crumble" → sfx_platform_crumble.wav (or use Kenney one)
    Search "blade spin mechanical" → sfx_blade_spin.wav
    Search "airy double jump" → sfx_double_jump.wav
    Search "landing thud" → sfx_land.wav
    Search "dash swoosh" → sfx_dash.wav
    Search "slide scrape" → sfx_wall_slide.wav
    Search "star collect sparkle" → sfx_star_collect.wav

[ ] Go to https://pixabay.com/music
    Search "japanese ambient loop" → bgm_forest.wav, bgm_temple.wav
    Search "bamboo flute loop" → bgm_bamboo.wav
    Search "dark drums ninja" → bgm_fortress.wav, bgm_village.wav
    Convert all MP3→WAV with: ffmpeg -i input.mp3 output.wav
```

---

## 30. FINAL AGENT CHECKLIST

Before shipping, every item must be verified visually:

**Player:**

- [ ] Ninja has BRANCHING HORNS/ANTLERS growing from head (not a hat cone)
- [ ] Two RED TRIANGLE eye marks on the face circle
- [ ] Upper arm AND forearm are separate draw_rect_rot calls per arm
- [ ] Feet extend beyond shin width (slightly wider toe area)
- [ ] Wall-slide pose shows hand pressed flat against wall surface
- [ ] Attack animation shows a sword with crossguard AND white slash arc trail
- [ ] Shuriken HUD icons at bottom-left are 4-bladed star shapes (not circles)

**Enemies:**

- [ ] Guard holds a long SCYTHE/POLEARM over his shoulder — crescent blade visible at top
- [ ] Guard has round helmet with visor SLIT and crest TRIANGLE on top
- [ ] Guard has square PAULDRON armor on each shoulder
- [ ] Archer has a BOW drawn as a curved arc (GL_LINE_STRIP), not a rectangle
- [ ] Archer has a single GLOWING RED EYE
- [ ] Heavy warrior is visibly LARGER than player (at least 10px taller, much wider)
- [ ] Heavy has full-face mask with RED EYE SLIT bars and a CLEAVER

**Environment:**

- [ ] Background has FAINT GHOST TREES at 5% parallax (barely visible, very transparent)
- [ ] Foreground bare trees have BARE BRANCHES (draw_bare_tree not draw_tree with canopy)
- [ ] Hanging CLOTH STRIPS visible between tree branches on forest levels
- [ ] Stone pillars in background have HORIZONTAL BLOCK LINES and DIAGONAL CRACK
- [ ] Pagoda gate has CURVED EAVE ROOF with drip texture underneath
- [ ] LANTERNS on pagoda level glow RED-ORANGE with halo
- [ ] Thorn bush has visible RED SPIKES radiating outward (from screenshot 2)
- [ ] Rotating blade trap has RED blades (not grey/dark) — BRIGHT RED as in screenshots
- [ ] Clay pots are ROUND JUG shapes, not rectangles
- [ ] Grass tufts appear at ground level

**HUD (exactly matches screenshots):**

- [ ] Avatar circle with mini ninja silhouette inside (with branches and red marks)
- [ ] Hearts use actual HEART shapes (2 circles + downward triangle)
- [ ] GEM icon is a DIAMOND shape in blue
- [ ] Gold COIN icon is a circle (not a square)
- [ ] Stars at top-left below avatar, not at top-center
- [ ] TWO SHURIKEN ICONS at bottom-left (left-facing and right-facing)
- [ ] FOUR SKILL BUTTONS at bottom-right in 2x2 grid with circular border
- [ ] Settings GEAR icon at top-right corner

**Audio:**

- [ ] Jump plays sfx_jump.wav — NEVER silent
- [ ] Double jump plays sfx_double_jump.wav (different from jump)
- [ ] Attack button plays sfx_attack_slash.wav immediately
- [ ] Hitting an enemy plays sfx_attack_hit.wav
- [ ] Star collect plays sfx_star_collect.wav (longer/more celebratory than gem)
- [ ] Level complete plays sfx_level_complete.wav and STOPS the BGM briefly
- [ ] BGM transitions correctly when entering each theme's levels
- [ ] Blade trap loops sfx_blade_spin.wav while on screen

---

## 31. GAMEPLAY FIXES (Applied 2026-04-14)

### 31.1 Auto-jump / Input Bug Fix

- Added `glutIgnoreKeyRepeat(1)` in `main.c` so GLUT does not fire repeated `key_down` events while a key is held. This prevents the character from auto-jumping due to OS key-repeat echoing the space bar.
- In `player_input`, when the player lands on the ground and `jump_available` is restored, `jump_pressed` is also cleared — this requires a fresh key-press before the next jump, preventing immediate re-trigger.

### 31.2 Shuriken Infinite Range

- Player shurikens now have **zero vertical velocity** (no gravity arc) — they fly in a perfectly straight horizontal line.
- Bounds check rewritten: player shurikens are only despawned when they travel past the level's absolute boundaries (`x < -200` or `x > level_w + 200`) rather than when they leave the camera view. This gives true screen-crossing range.
- Enemy shurikens and arrows retain their original arc and camera-bounds despawn.

### 31.3 UI Tree / Ground Separation

- `draw_bare_tree` now extends its trunk from `y = 0` (screen bottom) instead of `y = by`. This ensures that even when the camera scrolls upward during a jump (causing the ground platform to dip below the screen), the background tree trunks remain visually anchored to the bottom edge, preventing the floating-tree gap.

### 31.4 Easier Levels 1–5

| Level | Before                                   | After                                                         |
| ----- | ---------------------------------------- | ------------------------------------------------------------- |
| 1     | 0 enemies, 1 blade trap, tight platforms | 0 enemies, 1 small spike (2 spikes), wide forgiving platforms |
| 2     | 5 enemies (incl. heavy), 5 traps         | 1 guard + 1 archer, 1 spike cluster                           |
| 3     | 4 enemies, 3 traps                       | 2 guards + 1 archer, 1 spike + 1 blade, intro crumbling       |
| 4     | 6 enemies (3 archers), 4 traps           | 2 guards + 1 archer + 1 heavy, 2 spikes + 2 blades            |
| 5     | 6 enemies (3 heavies!), 5 traps          | 3 guards + 1 archer + 1 heavy, 2 spikes + 2 blades            |

Platform spacing is also wider / heights more uniform in levels 1–3 to reduce precision-jump frustration.

### 31.5 Checkpoint System

- New `Checkpoint` struct (`x`, `y`, `triggered`) added to `game.h`.
- `Level` struct gains `checkpoints[MAX_CHECKPOINTS]`, `checkpoint_count`, `respawn_x/y`, and `respawn_active`.
- `update_checkpoints()` runs every frame: if the player walks into a checkpoint's 40 px radius, it is marked triggered and the respawn position is saved.
- On death: if `respawn_active == 1`, the player is **respawned in-place** at the checkpoint (full health, 1 s invincibility, projectiles cleared) without returning to the Game Over screen. If no checkpoint was reached, normal Game Over still triggers.
- `draw_checkpoint()` in `draw_map.c` renders a post + waving flag (grey when inactive, gold + glow when triggered).
- Checkpoints placed in all levels 1–5 (one in L1, one in L2, two in L3–L5).

### 31.6 J / K / L Controls

New key bindings added alongside the existing ones (old keys still work):

| New Key | Old Key   | Action         |
| ------- | --------- | -------------- |
| J       | Z         | Sword attack   |
| K       | X         | Throw shuriken |
| L / E   | E / Enter | Dash           |

`glutIgnoreKeyRepeat(1)` also ensures K/J/L don't auto-repeat when held.

---

## 32. GAMEPLAY FIXES — ROUND 2 (Applied 2026-04-14)

### 32.1 Dash Now Works

Root cause: `vx *= 0.7f` (friction applied every frame when no movement key held) was killing the dash velocity the very next frame after it was set.

Fix:

- When `dash_timer > 0` (active dash window), the movement section is **skipped entirely** and dash velocity is held constant at `DASH_SPEED * facing`.
- `ANIM_DASH` is now excluded from the animation state-machine override at the bottom of `player_input`, so the dash pose stays locked for the full 0.18 s window.

### 32.2 Character No Longer "Jumps" Without Input

Root cause: enemy melee contact was applying `player.vy = 180`, launching the player upward. This was interpreted as the character "jumping by itself."

Fix: enemy melee knockback changed to `player.vy = 0` (horizontal only). Player stays on the ground when hit by melee enemies.

### 32.3 Player Is Now a Dark Silhouette (No Vivid Colors)

Root cause: `draw_player_idle/jump/run` used vivid bronze armor (`0.50, 0.40, 0.20`), glowing golden eyes (`1.0, 0.85, 0.15`), and skin-tone circles — making the character look colorful and inconsistent with the game's dark silhouette style.

Changes:

- Bronze armor → dark steel (`0.17, 0.17, 0.19`)
- Golden eyes → narrow red slits (`0.80, 0.10, 0.10`) — subtle, matches ninja-mask look
- Skin circles → near-black
- Gloves/shin-guards/shoulder pads → near-black
- Horns → near-black

`draw_player_hurt` was also broken (pre-setting red color had no effect since every draw call inside `draw_player_idle` overrides it). Fixed: hurt now draws the idle pose first, then lays a semi-transparent red rectangle over the bounding box as the visible flash.

### 32.4 Background Structures No Longer Float

Root cause: parallax backgrounds (pagoda, stone pillars) were drawn starting at `y = 40` (screen coords). When the camera scrolls up (player jumping), the game-world ground moves below the screen while the background elements stayed at y=40, creating a floating gap.

Fix (in `draw_bg.c`):

- `draw_bg_pagoda`: both gate pillars now start at `y = 0` (screen bottom), extending `y + 200*s` tall.
- `draw_bg_stone_pillar`: pillar body now starts at `y = 0`.
- `draw_bare_tree`: trunk already fixed in round 1 (starts at y=0).

All three background element types now always have geometry reaching the screen bottom, so no gap can appear regardless of camera Y position.

### 32.5 Improved Checkpoint System

New behavior:

- **Touching any trap** (blade, fire, spike) **immediately warps the player to the last triggered checkpoint** instead of deducting health and leaving them in the trap. A 1.5 s hurt-timer invincibility window prevents re-trigger right after respawn.
- **No checkpoint reached**: instead of showing the Game Over screen, the level auto-restarts instantly with `respawn_health - 1` starting hearts (minimum 1). The GAME_OVER state is now unreachable in normal play.
- **Checkpoint triggered**: resets `respawn_health` back to `MAX_HEALTH` — rewarding players who make it to mid-level checkpoints.
- **New level**: `respawn_health` resets to `MAX_HEALTH` on each level advance.

Implementation: `do_respawn()` static function centralises all respawn logic. A `respawn_requested` flag is set by trap collision handlers and processed at the end of `update_game` (after all loops finish) to avoid mutating level data mid-iteration.

---

## Section 33 — Bug-Fix & Visual Upgrade Pass (2026-04-14)

### 33.1 W-Key Jump Not Working

**Root cause:** `key_down` only set `player.jump_pressed = 1` for the space bar (`' '`). The W key was never wired as a jump trigger despite WASD being listed as the movement scheme.

**Fix (game.c):**

- `key_down`: extend condition to `k == ' ' || k == 'w' || k == 'W'`
- `key_up`: same extension so releasing W clears `jump_pressed`

### 33.2 Character Shaking / Auto-Jump on Spawn

**Root cause (two bugs):**

1. **First-frame FALL flicker** — `load_level` reset `player.on_ground = 0` (via `memset`). On frame 0, `player_input` sees `on_ground=0` before `resolve_platforms` runs, forcing the animation to ANIM_FALL for one frame, then snapping to IDLE. Visible as a tiny flicker or "hop".

   Fix: `load_level` now explicitly sets `player.on_ground = 1` after the memset.

2. **Stick-to-ground snap error** — `resolve_platforms` contained a secondary "coyote" snap that ran when the player was exactly on the platform boundary. It compared `p->y + p->h` (player TOP) against `pl->y` (platform BOTTOM) — both the wrong fields. Then it snapped via `p->y = pl->y` (bottom of platform) instead of `pl->y + pl->h` (top of platform), which teleported the player inside the platform, causing collision jitter next frame.

   Fix: corrected the proximity check to compare `p->y` (player bottom) against `pl->y + pl->h` (platform top), and the snap line to `p->y = pl->y + pl->h`.

3. **Stale key state on level load** — if the player pressed Enter/Space to transition from menu → game, those keys stayed in `keys[]`. On the first gameplay frame `dash` (which checks `keys['\r']`) would immediately fire, causing the character to lurch. Fix: `load_level` now calls `memset(keys, 0, ...)` and `memset(special_keys, 0, ...)` before placing the player.

### 33.3 Character Colorful While Jumping (Color Inconsistency Across Poses)

**Root cause:** Different animation pose functions used wildly different color palettes:

- `draw_player_run`: bronze/gold armor (`0.50, 0.40, 0.20`), yellow-glowing eyes (`1.0, 0.85, 0.15`)
- `draw_player_fall`: raw `col_black()` for body, `col_red()` + triangles for eyes
- `draw_player_wall_slide` / `draw_player_attack`: same `col_red` triangle eyes

**Fix (draw_player.c):** All poses now use the same dark-steel color palette as `draw_player_idle`:

- Armor: `(0.17, 0.17, 0.19)` with `(0.22, 0.22, 0.24)` highlight
- Eyes: dark socket `(0.08, 0.07, 0.07)` + red iris `(0.80, 0.10, 0.10)` + black pupil — drawn as `draw_circle` in all poses
- Body: `(0.12–0.14, 0.12–0.14, 0.14–0.16)`
- `draw_player_fall` was rebuilt from scratch with the same level of detail as the idle pose

### 33.4 Cloud Layer Added to All Levels

**New function:** `draw_clouds(float game_time, float cam_x)` in `draw_bg.c`:

- 6 dark blue-grey cloud clusters drawn with overlapping `draw_circle` blobs
- Each cluster has a slow rightward drift + parallax offset at 0.08× camera speed
- Drawn between `draw_ghost_trees` and `draw_birds` in the layered pipeline for correct depth order
- Two call-sites consolidated: the main display callback draws clouds once; `draw_parallax` no longer duplicates them

### 33.5 Improved Birds (Animated Flapping)

**Replaced** the two static triangle-and-line birds with:

- Helper `draw_single_bird()`: body (triangle + circle head), two wing triangles with `sinf`-driven flap amplitude, tail triangle
- 3 large solitary soaring birds at high altitude (slow flap, 0.65–0.80 alpha)
- 1 tight flock of 5 small birds flying together with staggered wing phases
- 1 pair of right-to-left birds in the upper sky

### 33.6 Theme-Specific Backgrounds

**New tree/deco types added to `draw_bg.c`:**

| Function             | Description                                                                          |
| -------------------- | ------------------------------------------------------------------------------------ |
| `draw_pine_tree`     | Three-tiered conical pine — used in Theme 1 (TEMPLE)                                 |
| `draw_willow_tree`   | Round crown + drooping branch lines — used in Theme 2 (VILLAGE)                      |
| `draw_cherry_tree`   | Two-branch silhouette with dark blossom clusters — used in Theme 4 (FORTRESS)        |
| `draw_fortress_wall` | Crenelated wall section with arrow slit and stone lines — used in Theme 4 (FORTRESS) |

**`draw_parallax` now branches per theme (main.c):**

- Theme 0 RUINS: bare trees + cloth
- Theme 1 TEMPLE: pine trees + cloth + pagoda + lanterns
- Theme 2 VILLAGE: alternating willow/bare trees + cloth + pagoda + lanterns
- Theme 3 BAMBOO: bamboo only (unchanged)
- Theme 4 FORTRESS: fortress wall segments + cherry blossom trees

---

## Section 34 — Bug-Fix & Improvement Pass (2026-04-14, Pass 2)

All issues from `buglist.md` (B-06 through B-19) were resolved and further improvements from the Backlog were implemented. A second codebase audit produced `bug_report_2.md` with 17 new findings (3 fixed immediately).

### 34.1 Fixes from Open Bug List

| Bug                                               | Root Cause                                                                                                                 | Fix                                                                                                                                                                           |
| ------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **B-06** Frame-rate-dependent deceleration        | `vx *= 0.7f` applied once per frame regardless of dt                                                                       | Changed to `vx *= powf(0.7f, dt * 60.0f)` — equivalent factor at 60 fps, correct at any rate                                                                                  |
| **B-07** Moving platform doesn't carry player     | `update_moving_platforms` moved platforms but never applied displacement to the player                                     | Added `pvx`/`pvy` fields to `Platform` struct; `update_moving_platforms` stores per-frame displacement; `resolve_platforms` applies it when player is grounded on PLAT_MOVING |
| **B-08** Double-jump after wall jump              | Wall jump set `jump_count = 1`, leaving one aerial jump available                                                          | Wall jump now sets `jump_count = 2`, consuming both jump charges                                                                                                              |
| **B-09** Dash fires on key-repeat                 | Dash checked `keys['e']` (polled), so holding E re-triggered every cooldown                                                | Added `dash_pressed` flag to `Player`; `key_down`/`key_up` set/clear it; player_input consumes and zeroes it on activation                                                    |
| **B-11** Animation timer mis-order                | `animation_update` ran after `player_input`, so new state transitions had a non-zero start timer                           | Moved `animation_update` to run before `player_input` in `update_game`                                                                                                        |
| **B-12** Attack timer mismatch                    | `ANIM_ATTACK` auto-terminated at 0.25 s but `attack_timer` gated new attacks for 0.30 s                                    | Extended ANIM_ATTACK duration to 0.30 s (matching `attack_timer`)                                                                                                             |
| **B-13** Ghost tree wrap modulus                  | Hard-coded 3840 wrap band; fails for levels wider than 3840                                                                | Changed to `level.level_w + 1280` so wrap scales with level width                                                                                                             |
| **B-14** Projectile buffer silently blocks throws | `level.proj_count` never decremented; once 64 projectiles existed, throws were silently ignored                            | Added compaction at end of `update_projectiles`: inactive slots are removed and `proj_count` shrinks                                                                          |
| **B-15** Exit portal offset applied twice         | `draw_world` called `draw_exit_portal(..., exit_y + 32, ...)` and the draw function also applied +32 internally (post-fix) | Removed +32 from `draw_world` call; offset is now applied once inside `draw_exit_portal` via `cy += 32.0f`                                                                    |
| **B-16** Dead `(void)dt;`                         | Leftover from earlier refactor                                                                                             | Removed                                                                                                                                                                       |
| **B-17** Jump/fall poses near-invisible           | Body luminance (0.12–0.17) too close to sky luminance (0.02–0.28)                                                          | Added atmospheric steel-blue glow rect + head circle behind each aerial pose in `draw_player_dispatch`                                                                        |
| **B-18** Mid-air hit gives no feedback            | Enemy melee always cleared `vy = 0`; no particles on hit                                                                   | `vy = 200` applied when hit mid-air; `spawn_hit_sparks` called on every player hit regardless of health                                                                       |
| **B-19** Settings gear overlaps level indicator   | Both drawn near (1200–1240, 690–700)                                                                                       | Level indicator moved to top-center x=590; text updated to "Level N / 10"                                                                                                     |

### 34.2 Improvements Implemented

| Item                                     | Change                                                                                                                                                                                                  |
| ---------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **IMP-04** Theme sky colors              | `draw_sky()` now reads `level.theme` and uses theme-specific gradients: RUINS=dark blue, TEMPLE=deep indigo, VILLAGE=amber dusk, BAMBOO=dark teal, FORTRESS=dark crimson                                |
| **IMP-08/09** No consecutive same themes | Level themes updated: L1 RUINS, L2 TEMPLE, L3 BAMBOO, L4 VILLAGE, L5 FORTRESS, L6 RUINS, L7 TEMPLE, L8 BAMBOO, L9 VILLAGE, L10 FORTRESS — clean 5-theme cycle                                           |
| **IMP-03** Level 10 climax               | L10 replaced with a dedicated FORTRESS climax: narrow platforms (80–100 px wide), 3 heavy enemies, 1 archer + 1 heavy on the final fortress wall, dense trap gauntlet, single mid-level checkpoint only |
| **IMP-06** Enemy variety                 | L6–L9 now have fully hand-crafted layouts with mixed Guard/Archer/Heavy placement; archers and heavies spread across the full level length                                                              |

### 34.3 Critical Bug Found in Audit (Bug Report 2)

| Bug                                                                    | Fix                                                                           |
| ---------------------------------------------------------------------- | ----------------------------------------------------------------------------- |
| **B2-01** `draw_hud` forward declaration missing `int shurikens` param | Forward decl in `game.c:57` updated to match 7-param definition               |
| **B2-02** Exit hitbox misaligned with portal visual position           | `check_exit()` now tests against `portal_y = level.exit_y + 32.0f`            |
| **B2-10** Vertical moving platform carry not applied                   | `resolve_platforms` re-snaps `p->y = carrier->y + carrier->h` when `pvy != 0` |

### 34.4 Remaining Open Items (see bug_report_2.md)

- **B2-07** (High): glColor hurt-flash tint leaks into health-bar draw — cosmetic, no crash
- **B2-04** (Low): `col_black()` used as implicit reset in enemy pose functions — brittle
- **B2-05** (Medium): `ANIM_THROW` enum value unused — dead code
- **B2-06** (Low): Color state leak risk after skill button drawing
- **B2-08** (Low): `pvx`/`pvy` not explicitly zeroed in `add_platform` (safe due to memset in `load_level`)
- **B2-14** (Low): "oo" shuriken display is confusing; should be "INF" or a custom ∞ glyph
- **B2-18** (Info): No frame-rate cap — game spins at uncapped FPS; recommend `glutTimerFunc(16, ...)`

### 34.5 Build Status

```
gcc src/draw_bg.c src/draw_enemy.c src/draw_hud.c src/draw_map.c \
    src/draw_player.c src/game.c src/main.c src/renderer.c \
    -o ninja_shadow -Wall -O2 -lGL -lGLU -lglut -lm
# → Clean build, zero warnings
./tests/test_shuriken  # → ALL TESTS PASSED ✓
```
