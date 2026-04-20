# Ninja Shadow Concept Match (Lab 1 to Lab 6)

## Purpose

This file maps what was taught in Lab 1 to Lab 6 with what is implemented in Ninja Shadow, and lists improvements to make the project look even closer to class style.

## Overall Conclusion

Your project already follows the core class concepts. The trees and environment are built from basic OpenGL primitives, loops, transformations, and timer-based animation, which directly match the lab progression.

## Lab-to-Project Match

### Lab 1: Basic primitives and pattern drawing

- Taught in lab:
  - Drawing with GL_QUADS and GL_LINES
  - Vertex placement with glVertex2f
  - Repetition with nested loops (chess pattern)
- Lab evidence:
  - Lab_01/chess.cpp
  - Lab_01/cube.cpp
- Used in Ninja Shadow:
  - Primitive helpers: draw_rect, draw_tri, draw_circle in src/renderer.c
  - Tree trunks and body parts with GL_QUADS in src/draw_bg.c
  - Repeated background objects using for-loops in src/main.c and src/draw_bg.c
- Match status: Strong match

### Lab 2: DDA and pixel/point logic

- Taught in lab:
  - DDA line stepping using dx, dy, steps, increments
  - Point plotting using GL_POINTS
- Lab evidence:
  - Lab_02/dda.cpp
  - Lab_02/house.cpp
- Used in Ninja Shadow (conceptually):
  - Incremental stepping and continuous update logic in particles and motion updates in src/game.c
  - Repeated generated geometry from loops (cloud circles, grass tufts, branch groups) in src/draw_bg.c
- Match status: Concept match (not literal DDA function)

### Lab 3: Bresenham style line logic and circle generation

- Taught in lab:
  - Bresenham line algorithm (integer decision variable)
  - Circle drawing using angle loop and trig
- Lab evidence:
  - Lab_03/BH.cpp
  - Lab_03/circle.cpp
- Used in Ninja Shadow:
  - Circle generation with segmented triangle fan in src/renderer.c and src/draw_bg.c
  - Line-based details and branch silhouettes in trees and hanging decor in src/draw_bg.c
- Match status: Strong match for circle/line construction, partial for Bresenham-specific integer method

### Lab 4: Transformations and keyboard movement

- Taught in lab:
  - Translation with glTranslatef
  - Keyboard control with WASD and glutPostRedisplay
- Lab evidence:
  - Lab_04/a.cpp
- Used in Ninja Shadow:
  - Object-space transforms with glPushMatrix/glTranslatef/glRotatef in src/renderer.c and animated draw modules
  - Input-driven state changes from keyboard in src/main.c and game updates in src/game.c
  - Camera movement and world translation logic in src/game.c and src/main.c
- Match status: Strong match

### Lab 5: Timer updates, motion, boundary checks, audio idea

- Taught in lab:
  - Periodic updates with glutTimerFunc
  - Velocity-based movement and bounce/bounds
  - Audio integration idea in movement_audio
- Lab evidence:
  - Lab_05/movement_audio.cpp
  - Lab_05/movement.cpp
- Used in Ninja Shadow:
  - Frame/timer update loop and time-based animation in src/main.c and src/game.c
  - Velocity, gravity, collision, and constraints in src/game.c
  - Music and SFX module in src/audio.c
- Match status: Strong match

### Lab 6: Full scene composition from reusable draw functions

- Taught in lab:
  - Building landscapes with modular functions (cloud, hill, bird, pine, grass)
  - Layering and visual storytelling from basic shapes
- Lab evidence:
  - Lab_06/a.cpp
- Used in Ninja Shadow:
  - The same style of decomposition into draw functions in src/draw_bg.c, src/draw_enemy.c, src/draw_player.c, src/draw_hud.c
  - Scene layering (sky, stars, trees, platforms, enemies, HUD) in src/main.c
- Match status: Very strong match

---

## Character Build Match (Proper References)

This section gives direct references for how the **player character** is built from Lab 1 to Lab 6 concepts.

### Lab 1 -> Primitive construction of character body

- Lab reference:
  - `Lab_01/chess.cpp` (`display_black`, `display_white`): direct `GL_QUADS` + `glVertex2f` placement.
- Character reference:
  - `ninja_shadow/src/draw_player.c` (`draw_player_idle`, `draw_player_run`, `draw_player_jump`): body parts (boots, legs, torso, arms, mask) are composed from quads/triangles/circles.
  - `ninja_shadow/src/renderer.c` (`draw_rect`, `draw_tri`, `draw_circle`): primitive helper layer used by player drawing.
- Match statement:
  - Same primitive-assembly method as Lab 1, scaled into a full character model.

### Lab 2 -> DDA / incremental step concept in engine utilities

- Lab reference:
  - `Lab_02/dda.cpp` (`dda`): stepwise incremental update from `(x1,y1)` to `(x2,y2)`.
- Character-related project reference:
  - `ninja_shadow/src/renderer.c` (`draw_line_dda`): explicit DDA implementation kept in renderer toolkit.
  - `ninja_shadow/src/game.c` (`particles_update`): per-step incremental updates (`x += vx*dt`, `y += vy*dt`) for motion effects around player actions.
- Match statement:
  - Lab 2 incremental logic appears both as direct DDA utility and as frame-by-frame update logic used by character effects.

### Lab 3 -> Bresenham/circle algorithms used by rendering toolkit

- Lab reference:
  - `Lab_03/car.cpp` (`bresenhamLine`, `bresenhamCircle`)
  - `Lab_03/circle.cpp` / `Lab_03/circle_.cpp` (circle generation)
- Character-related project reference:
  - `ninja_shadow/src/renderer.c` (`draw_line_bresenham`, `draw_circle_midpoint`, `draw_circle`): same algorithm family available in engine draw layer.
  - `ninja_shadow/src/draw_player.c`: head/eyes/joints use circle-based construction repeatedly.
- Match statement:
  - Character visuals rely on the same circle/line algorithm ideas taught in Lab 3.

### Lab 4 -> Transformations for facing and limb motion

- Lab reference:
  - `Lab_04/a.cpp` (`glTranslatef`), `Lab_04/c.cpp` (`glRotatef`), `Lab_04/b.cpp` (`glScalef`)
- Character reference:
  - `ninja_shadow/src/draw_player.c`:
    - facing flip with `glScalef(facing_right ? 1.0f : -1.0f, ...)`
    - local limb/horn rotations with `glPushMatrix`, `glTranslatef`, `glRotatef`
    - pose-local transforms for run/attack/dash variants.
- Match statement:
  - Lab 4 transform pipeline is directly used to animate and orient the character.

### Lab 5 -> Input + timer-based animation + action SFX

- Lab reference:
  - `Lab_05/movement.cpp`, `Lab_05/box_movement.cpp` (keyboard + timer motion)
  - `Lab_05/movement_audio.cpp` (motion/audio concept)
- Character reference:
  - `ninja_shadow/src/game.c` (`key_down`, `key_up`, `special_down`, `special_up`): input capture for move/jump/dash/attack/shuriken.
  - `ninja_shadow/src/game.c` (`player_input`, `player_physics`, `animation_update`): player state and timing logic.
  - `ninja_shadow/src/audio.c` (`audio_play`, `audio_play_bgm`): jump/dash/attack/hurt sound triggering.
  - `ninja_shadow/src/main.c` (`timer_callback`, fixed-step update in `display`): timer-driven continuous gameplay loop.
- Match statement:
  - Lab 5 control/update/audio pattern is expanded into full character gameplay behavior.

### Lab 6 -> Full modular scene + character integration

- Lab reference:
  - `Lab_06/a.cpp` (modular draw functions building one complete visual scene)
- Character reference:
  - `ninja_shadow/src/main.c` (`draw_world`, `display`): player rendered within layered world pipeline.
  - `ninja_shadow/src/draw_player.c` (`draw_player_dispatch`): state-based pose selection (`ANIM_IDLE`, `ANIM_RUN`, `ANIM_JUMP`, `ANIM_ATTACK`, `ANIM_DASH`, etc.).
  - `ninja_shadow/src/game.c` (`player_input`, `resolve_platforms`, animation state selection): gameplay state drives final pose.
- Match statement:
  - Same Lab 6 philosophy: modular functions combined into one coherent animated scene, with character as a central module.

### Character Runtime Flow (Reference Trace)

1. Input capture:
   - `ninja_shadow/src/game.c` -> `key_down`, `key_up`, `special_down`, `special_up`
2. Player logic and physics:
   - `ninja_shadow/src/game.c` -> `player_input`, `player_physics`, `resolve_platforms`
3. Animation state update:
   - `ninja_shadow/src/game.c` -> `animation_update`
4. Pose dispatch and draw:
   - `ninja_shadow/src/draw_player.c` -> `draw_player_dispatch` -> pose functions (`draw_player_idle/run/jump/...`)
5. Frame presentation loop:
   - `ninja_shadow/src/main.c` -> `display` + `timer_callback`

Direct viva line:

"My character pipeline follows Lab progression: primitives (Lab 1), incremental/algorithmic rendering ideas (Lab 2-3), matrix transformations (Lab 4), keyboard and timer-driven state updates with audio feedback (Lab 5), and modular full-scene integration (Lab 6)."

---

## Tree Construction Match (Direct Answer for Teacher)

### How trees are made in Ninja Shadow

- Tree variants are split into reusable functions:
  - draw_bare_tree
  - draw_pine_tree
  - draw_willow_tree
  - draw_cherry_tree
- Location:
  - src/draw_bg.c

### What class concepts are used in these tree functions

- Primitive composition:
  - Trunk from quads
  - Leaves/crowns from triangles and circles
- Loop-based construction:
  - Branch sets and decorative segments through for-loops
- Scale and placement reuse:
  - Same function reused with scale and position parameters
- Scene integration:
  - Trees are repeated with wrapping/parallax offsets using camera-dependent formulas

### Statement you can use in viva

"I built trees using only class concepts: primitive shapes, loop-based construction, transformation through position and scale, and scene layering with camera movement. I reused modular draw functions exactly like our lab progression from basic drawing to full scene composition."

---

## Cross-Reference Code Samples (Lab vs Ninja Shadow)

### Sample Pair 1: Tree from basic primitives

Lab sample path: `Lab_06/a.cpp` (`drawPine`)

```cpp
void drawPine(float x, float y, float scale)
{
  glColor3f(0.36f, 0.20f, 0.10f);
  glBegin(GL_QUADS);
  glVertex2f(x - 5.0f * scale, y);
  glVertex2f(x + 5.0f * scale, y);
  glVertex2f(x + 5.0f * scale, y + 22.0f * scale);
  glVertex2f(x - 5.0f * scale, y + 22.0f * scale);
  glEnd();

  glColor3f(0.05f, 0.42f, 0.12f);
  glBegin(GL_TRIANGLES);
  glVertex2f(x - 20.0f * scale, y + 14.0f * scale);
  glVertex2f(x, y + 42.0f * scale);
  glVertex2f(x + 20.0f * scale, y + 14.0f * scale);
  glEnd();
}
```

Project sample path: `ninja_shadow/src/draw_bg.c` (`draw_pine_tree`)

```c
void draw_pine_tree(float bx, float by, float scale)
{
  float s = scale;
  glColor4f(0.10f, 0.14f, 0.10f, 1.0f);
  glBegin(GL_QUADS);
  glVertex2f(bx - 4 * s, 0);
  glVertex2f(bx + 4 * s, 0);
  glVertex2f(bx + 3 * s, by + 80 * s);
  glVertex2f(bx - 3 * s, by + 80 * s);
  glEnd();

  float tiers[][3] = {
    { 80, 140,  60 },
    { 115, 165, 45 },
    { 145, 195, 30 },
  };
  for (int i = 0; i < 3; i++) {
    float base = by + tiers[i][0] * s;
    float top  = by + tiers[i][1] * s;
    float hw   = tiers[i][2] * s;
    draw_tri(bx - hw, base, bx + hw, base, bx, top);
  }
}
```

Cross-reference note:

- Same teaching concept: trunk from quad + leaves from triangles.
- Your project extends the lab idea with layered tiers and reusable scale parameters.

### Sample Pair 2: Primitive helper style (vertex-level drawing)

Lab sample path: `Lab_01/chess.cpp` (`display_black`)

```cpp
void display_black(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);
  glEnd();
}
```

Project sample path: `ninja_shadow/src/renderer.c` (`draw_rect`)

```c
void draw_rect(float x, float y, float w, float h) {
  glBegin(GL_QUADS);
  glVertex2f(x,   y);   glVertex2f(x+w, y);
  glVertex2f(x+w, y+h); glVertex2f(x,   y+h);
  glEnd();
}
```

Cross-reference note:

- Same base method: explicit `glBegin` + `glVertex` primitive construction.
- Your project wraps this in helpers to keep scene files cleaner, but the core drawing style is exactly the same.

### Sample Pair 3: Incremental step logic (Lab DDA style)

Lab sample path: `Lab_02/dda.cpp` (`dda`)

```cpp
void dda(float x1, float y1, float x2, float y2)
{
  float dx = x2 - x1;
  float dy = y2 - y1;
  int steps = max(abs(dx), abs(dy));
  float x_inc = dx / steps;
  float y_inc = dy / steps;

  float x = x1;
  float y = y1;

  glBegin(GL_POINTS);
  for (int i = 0; i <= steps; i++)
  {
    glVertex2f(round(x), round(y));
    x = x + x_inc;
    y = y + y_inc;
  }
  glEnd();
}
```

Project sample path: `ninja_shadow/src/game.c` (`particles_update`)

```c
void particles_update(float dt)
{
  for (int i = 0; i < MAX_PARTICLES; i++)
  {
    if (!particles[i].active)
      continue;
    particles[i].x += particles[i].vx * dt;
    particles[i].y += particles[i].vy * dt;
    particles[i].vy -= 200.0f * dt;
    particles[i].life -= dt;
    particles[i].a = particles[i].life / particles[i].max_life;
    if (particles[i].life <= 0)
      particles[i].active = 0;
  }
}
```

Cross-reference note:

- Same incremental-update concept: repeated per-step position updates in a loop.
- Lab applies it to plotted line points, project applies it to particles and motion state.

### Sample Pair 4: Circle and line algorithm style

Lab sample path: `Lab_03/circle.cpp` (`circle`)

```cpp
void circle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy)
{
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (int i = 0; i <= 100; i++)
  {
    float angle = 2.0f * 3.1416f * i / 100;
    float x = rx * cosf(angle);
    float y = ry * sinf(angle);
    glVertex2f(cx + x, cy + y);
  }
  glEnd();
}
```

Project sample path: `ninja_shadow/src/renderer.c` (`draw_circle`)

```c
void draw_circle(float cx, float cy, float r, int segs) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (int i = 0; i <= segs; i++) {
    float a = 2*PI*i/segs;
    glVertex2f(cx + cosf(a)*r, cy + sinf(a)*r);
  }
  glEnd();
}
```

Cross-reference note:

- Same Lab 3 circle-generation idea using angle loop + trigonometry.
- This project function is reused across trees, moon, clouds, particles, and effects.

### Sample Pair 5: Transform and keyboard control

Lab sample path: `Lab_04/a.cpp` (`display`, `keyboard`)

```cpp
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(movex, movey, 0);
  rec(X1, Y1, X2, Y2);
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'd': movex = movex + tx; break;
  case 'a': movex = movex - tx; break;
  case 's': movey = movey - ty; break;
  case 'w': movey = movey + ty; break;
  }
  glutPostRedisplay();
}
```

Project sample path: `ninja_shadow/src/renderer.c` (`draw_rect_rot`) and `ninja_shadow/src/game.c` (`key_down`)

```c
void draw_rect_rot(float cx, float cy, float w, float h, float angle) {
  glPushMatrix();
  glTranslatef(cx, cy, 0);
  glRotatef(angle, 0, 0, 1);
  draw_rect(-w/2, -h/2, w, h);
  glPopMatrix();
}
```

```c
void key_down(unsigned char k, int x, int y)
{
  (void)x;
  (void)y;
  keys[(int)k] = 1;

  if ((k == ' ' || k == 'w' || k == 'W') && game_state == STATE_PLAYING)
    player.jump_pressed = 1;
}
```

Cross-reference note:

- Same Lab 4 transformation pipeline (`glTranslatef`) and keyboard-driven state changes.
- Project scales this up from simple rectangle movement to full character action input.

### Sample Pair 6: Timer update loop and audio integration

Lab sample path: `Lab_05/movement_audio.cpp` (`update`)

```cpp
void update(int value)
{
  X1 += dx;
  X2 += dx;
  Y1 += dy;
  Y2 += dy;

  if (X2 >= 100 || X1 <= -100)
    dx = -dx;
  if (Y2 >= 100 || Y1 <= -100)
    dy = -dy;

  glutPostRedisplay();
  glutTimerFunc(16, update, 0);
}
```

Project sample path: `ninja_shadow/src/main.c` (`timer_callback`) and `ninja_shadow/src/audio.c` (`audio_play_bgm`)

```c
static void timer_callback(int val)
{
  (void)val;
  glutPostRedisplay();
  glutTimerFunc(16, timer_callback, 0);
}
```

```c
void audio_play_bgm(int bgm_id)
{
  if (!audio_ok) return;
  if (bgm_id < 0 || bgm_id >= BGM_COUNT) return;
  /* ... binds selected bgm buffer and plays looping music ... */
}
```

Cross-reference note:

- Same Lab 5 continuous timer-update pattern with `glutTimerFunc`.
- Audio is integrated as a module in the project (same concept as movement+audio lab, cleaner architecture).

---

## Recent Bug Fixes Implemented

### 1) High game speed / inconsistent timing fixed

- Root issue:
  - Variable frame-step updates caused simulation bursts and inconsistent combat windows on fluctuating frame times.
- Fix applied:
  - Switched gameplay simulation to fixed-step 60 Hz update loop with accumulator.
- File reference:
  - `ninja_shadow/src/main.c` (`display`, `sim_accum`, `fixed_dt`)

Code reference:

```c
const float fixed_dt = 1.0f / 60.0f;
sim_accum += frame_dt;
while (sim_accum >= fixed_dt)
{
    update_game(fixed_dt);
    sim_accum -= fixed_dt;
}
```

### 2) Hanging decoration without support fixed

- Root issue:
  - Cloth and lantern decorations appeared to float with no visible attachment points.
- Fix applied:
  - Added endpoint support posts (draw_rect) and metal anchor circles at both ends of every hanging cloth.
- File reference:
  - `ninja_shadow/src/draw_bg.c` (`draw_hanging_cloth`)

Code reference:

```c
/* Support posts and anchors so cloth does not appear floating */
draw_rect(x1 - 2, y1, 4, 20);
draw_rect(x2 - 2, y2, 4, 20);
draw_circle(x1, y1, 2.5f, 8);
draw_circle(x2, y2, 2.5f, 8);
```

### 3) Enemy engagement and sword hit registration improved

- Root issue:
  - Aggro and melee hit checks were too strict: enemy facing was only updated inside the aggro block, and the melee attack was gated by a patrol-zone check.
- Fix applied:
  - Relaxed aggro gating; widened sword hit reach to 56 px; moved facing update outside the aggro block; removed patrol-zone restriction from melee attack.
- File reference:
  - `ninja_shadow/src/game.c` (`update_enemies`)

Code reference:

```c
/* Facing always updated at combat range */
if (e->type != ENEMY_ARCHER && fabsf(dx) < 280.0f && fabsf(dy) < 120.0f && fabsf(dx) > 5.0f)
    e->facing = (dx > 0) ? 1 : -1;

/* Sword hit — reach 56 px from sword tip 26 px ahead of player */
float reach = 56.0f;
float ax = player.x + player.anim.facing * 26.0f;
if (fabsf(ax - e->x) < reach && fabsf(ay - ey) < 86.0f) { /* hit */ }

/* Melee — patrol-zone gate removed */
if (fabsf(dx) < 40 && fabsf(mdy) < 60 && player.hurt_timer <= 0 && dx * e->facing > -5.0f)
```

### 4) Character auto-jumping to upper block when moving left/right fixed

- Root issue:
  - Two collision paths caused unintended vertical snaps during horizontal movement.
  - **Path A (main collision):** The ground-collision branch used `p->vy <= 0`. When walking horizontally (vy = 0) into a tall platform, the minimum-overlap axis flipped to the vertical (ovb), snapping the player upward to the platform top.
  - **Path B (stick-to-ground):** The edge-snap code allowed the player centre to be up to 4 px outside the platform's x-span before snapping, teleporting the player onto a slightly elevated adjacent platform.
- Fix applied:
  - Changed ground-collision condition from `p->vy <= 0` to `p->vy < 0` so it fires only during an actual fall.
  - Removed the ±4 px x-overhang from stick-to-ground; player centre must be strictly within the platform's x-span. Tightened vertical window from ±8 px to ±6 px.
- File reference:
  - `ninja_shadow/src/game.c` (`resolve_platforms`, ground collision and stick-to-ground blocks)

Code reference:

```c
/* Before: fired even while walking (vy == 0) */
if (min_ov == ovb && p->vy <= 0)

/* After: only fires while actually falling */
if (min_ov == ovb && p->vy < 0)

/* Stick-to-ground: before (±4 px x-overhang, ±8 px y-window) */
if (px_center > pl->x - 4 && px_center < pl->x + pl->w + 4 &&
    p->y >= plat_top - 8 && p->y <= plat_top + 8)

/* Stick-to-ground: after (strict x-bounds, ±6 px y-window) */
if (px_center >= pl->x && px_center <= pl->x + pl->w &&
    p->y >= plat_top - 6 && p->y <= plat_top + 6)
```

---

## Teacher-Style Improvements Implemented

All requested improvements are now implemented in code without changing core game rules.

### 1) Explicit DDA function added and used

- Implemented in:
  - `ninja_shadow/src/renderer.c` -> `draw_line_dda(...)`
  - Declaration in `ninja_shadow/src/renderer.h`
- Applied in scene:
  - `ninja_shadow/src/draw_bg.c` -> algorithm tree branches
  - `ninja_shadow/src/draw_bg.c` -> hanging rope path in algorithm mode

Code reference:

```c
void draw_line_dda(float x1, float y1, float x2, float y2)
{
  float dx = x2 - x1;
  float dy = y2 - y1;
  int steps = (int)fmaxf(fabsf(dx), fabsf(dy));
  /* point-by-point incremental rasterization */
}
```

### 2) Explicit Bresenham function added and used

- Implemented in:
  - `ninja_shadow/src/renderer.c` -> `draw_line_bresenham(...)`
  - Declaration in `ninja_shadow/src/renderer.h`
- Applied in scene:
  - `ninja_shadow/src/draw_bg.c` -> algorithm tree branch outlines

Code reference:

```c
void draw_line_bresenham(int x1, int y1, int x2, int y2)
{
  int dx = abs(x2 - x1), dy = -abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
  int err = dx + dy;
  /* integer decision updates */
}
```

### 3) Midpoint circle option added beside trig-circle

- Existing production circle (trig):
  - `ninja_shadow/src/renderer.c` -> `draw_circle(...)`
- New midpoint circle:
  - `ninja_shadow/src/renderer.c` -> `draw_circle_midpoint(...)`
- Applied in scene:
  - `ninja_shadow/src/draw_bg.c` -> algorithm tree foliage circles

Code reference:

```c
void draw_circle_midpoint(int cx, int cy, int r)
{
  int x = 0, y = r;
  int p = 1 - y;
  /* 8-way symmetry plotting */
}
```

### 4) Manual primitive tree variant added (no helper wrappers)

- Implemented in:
  - `ninja_shadow/src/draw_bg.c` -> `draw_tree_manual_primitive(...)`
- Characteristics:
  - Direct `glBegin/glVertex` usage for trunk and foliage
  - Useful for teacher-style demonstration of raw primitive composition

Code reference:

```c
static void draw_tree_manual_primitive(float bx, float by, float scale)
{
  glBegin(GL_QUADS); /* trunk */
  /* ... */
  glEnd();
  glBegin(GL_TRIANGLES); /* foliage */
  /* ... */
  glEnd();
}
```

### 5) Concept mapping documentation updated

- This report updated with:
  - full Lab 1-6 matches
  - cross-reference code samples
  - implemented improvement references

### 6) Demo mode toggle added (concept-by-concept showcase)

- API added:
  - `ninja_shadow/src/draw_bg.c` -> `set_tree_demo_mode(...)`, `get_tree_demo_mode(...)`
  - declarations in `ninja_shadow/src/renderer.h`
- Input toggle added:
  - `ninja_shadow/src/game.c` (`key_down`)
- Controls:
  - Key `1`: Normal production trees
  - Key `2`: Primitive-only tree mode
  - Key `3`: Algorithm tree mode (DDA + Bresenham + midpoint circle)

Code reference:

```c
if (k == '1') set_tree_demo_mode(0);
else if (k == '2') set_tree_demo_mode(1);
else if (k == '3') set_tree_demo_mode(2);
```

Note:

- Default mode remains normal (`1`), so gameplay behavior is preserved.
- The toggle only changes tree/decor rendering style for demonstration.

---

## Quick Viva Checklist

- Explain trunk and foliage are made from basic primitives.
- Explain loops generate multiple branches/objects.
- Explain scale/position parameters reuse same function.
- Explain camera parallax as transformation over time.
- Mention Lab 2 and Lab 3 algorithms are conceptually present and can be added explicitly if required by rubric.

## Final Assessment

Ninja Shadow is not outside class scope. It is an advanced composition of the same fundamentals taught in Lab 1 to Lab 6, especially for environment and tree building.
