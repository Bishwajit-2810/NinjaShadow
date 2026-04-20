#include "renderer.h"
#include "game.h"
#include "audio.h"
#include <stdio.h>

/* ── Forward declarations of all draw functions ─────────── */
void draw_sky(void);
void draw_ghost_trees(float cam_x);
void draw_ground_atmosphere(void);
void draw_bare_tree(float bx, float by, float scale);
void draw_pine_tree(float bx, float by, float scale);
void draw_willow_tree(float bx, float by, float scale);
void draw_cherry_tree(float bx, float by, float scale);
void draw_fortress_wall(float bx, float by, float scale);
void draw_bamboo_bg(float cam_x);
void draw_hanging_cloth(float x1, float y1, float x2, float y2);
void draw_bg_stone_pillar(float cx, float y, float w, float h);
void draw_bg_pagoda(float cx, float y, float scale);
void draw_lantern_string(float x1, float y1, float x2, float y2, int count);
void draw_grass_tufts(float lw);
void draw_moon(float cx, float cy, float phase, int scene_type);
void draw_night_stars(float game_time);
void draw_birds(float game_time, float cam_x);
void draw_clouds(float game_time, float cam_x);

void draw_platform(Platform *p, int theme);
void draw_blade_trap(float cx, float cy, float angle);
void draw_thorn_bush(float cx, float y, float scale);
void draw_spikes_ground(float x, float y, int count);
void draw_fire_vent(float cx, float cy, int active, float fire_t);
void draw_clay_pot(float cx, float cy, int broken, float break_t);
void draw_stone_marker(float cx, float y, float w, float h);
void draw_rope_fence(float x, float y, float length, int post_count);
void draw_gem(float cx, float cy, float pulse_t);
void draw_star_collectible(float cx, float cy, float pulse_t);
void draw_exit_portal(float cx, float cy, float t, int open);

void draw_player_dispatch(Player *p);
void draw_shuriken(float cx, float cy, float angle);

void draw_enemy_guard(float cx, float cy, int facing, float anim_t);
void draw_enemy_archer(float cx, float cy, int facing, int shooting);
void draw_enemy_heavy(float cx, float cy, int facing, float anim_t);
void draw_arrow(float cx, float cy, float vx, float vy);

void draw_hud(int health, int max_health, int gems, int gold, int stars, int shurikens, float game_time);
void draw_main_menu(float t);
void draw_pause_overlay(void);
void draw_game_over_screen(void);
void draw_level_complete(int stars);
void draw_settings_overlay(void);

void particles_draw(void);
void key_down(unsigned char k, int x, int y);
void key_up(unsigned char k, int x, int y);
void special_down(int k, int x, int y);
void special_up(int k, int x, int y);
void update_game(float dt);

/* ── Mouse click handler ────────────────────────────────── */
static void mouse_click(int button, int state, int mx, int my)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;
    /* GLUT y=0 is at top; flip to match our coordinate system (y=0 at bottom) */
    float wx = (float)mx;
    float wy = (float)(SCREEN_H - my);

    /* Settings gear is drawn at (1240, 690) with radius 18 */
    float dx = wx - 1240.0f, dy = wy - 690.0f;
    if (dx * dx + dy * dy < 22.0f * 22.0f) /* slightly generous hit zone */
    {
        if (game_state == STATE_PLAYING || game_state == STATE_PAUSED)
            game_state = STATE_SETTINGS;
        else if (game_state == STATE_SETTINGS)
            game_state = STATE_PLAYING;
    }
}

/* ── Timing ─────────────────────────────────────────────── */
static int last_ms = 0;
static float sim_accum = 0.0f;

/* ── ~60 fps frame timer (B2-18) ───────────────────────── */
static void timer_callback(int val)
{
    (void)val;
    glutPostRedisplay();
    glutTimerFunc(16, timer_callback, 0); /* 16 ms ≈ 62.5 fps */
}

/* ── Parallax background draw ───────────────────────────── */
static void draw_parallax(float cam_x, int theme)
{
    float ox1 = -cam_x * 0.20f;
    float ox2 = -cam_x * 0.50f;

    if (theme == 3)
    {
        /* BAMBOO: bamboo forest, no other trees */
        draw_bamboo_bg(cam_x);
    }
    else if (theme == 4)
    {
        /* FORTRESS: fortress walls + cherry blossom trees */
        for (int i = 0; i < 4; i++)
        {
            float bx = fmodf(i * 340.0f + ox1 + 3000, 2400) - 100;
            float scale = 0.6f + (i % 2) * 0.2f;
            draw_fortress_wall(bx, 40, scale);
        }
        /* A few bare trees for atmosphere */
        for (int i = 0; i < 3; i++)
        {
            float bx = fmodf(i * 380.0f + 180.0f + ox1 + 3000, 2400) - 100;
            draw_cherry_tree(bx, 40, 0.7f + (i % 2) * 0.15f);
        }
    }
    else if (theme == 1)
    {
        /* TEMPLE: pine trees + pagoda */
        for (int i = 0; i < 4; i++)
        {
            float bx = fmodf(i * 240.0f + ox1 + 3000, 2200) - 100;
            float scale = 0.75f + (i % 3) * 0.15f;
            draw_pine_tree(bx, 40, scale);
        }
        for (int i = 0; i < level.cloth_count; i++)
        {
            ClothDec *c = &level.cloths[i];
            draw_hanging_cloth(c->x1 + ox1, c->y1, c->x2 + ox1, c->y2);
        }
    }
    else if (theme == 2)
    {
        /* VILLAGE: willow trees + hanging cloth */
        for (int i = 0; i < 4; i++)
        {
            float bx = fmodf(i * 260.0f + ox1 + 3000, 2200) - 100;
            float scale = 0.65f + (i % 3) * 0.15f;
            if (i % 2 == 0)
                draw_willow_tree(bx, 40, scale);
            else
                draw_bare_tree(bx, 40, scale);
        }
        for (int i = 0; i < level.cloth_count; i++)
        {
            ClothDec *c = &level.cloths[i];
            draw_hanging_cloth(c->x1 + ox1, c->y1, c->x2 + ox1, c->y2);
        }
    }
    else
    {
        /* RUINS (theme 0): bare trees (default) */
        for (int i = 0; i < 6; i++)
        {
            float bx = fmodf(i * 220.0f + ox1 + 3000, 2200) - 100;
            float scale = 0.7f + (i % 3) * 0.15f;
            draw_bare_tree(bx, 40, scale);
        }
        /* Cloth strips */
        for (int i = 0; i < level.cloth_count; i++)
        {
            ClothDec *c = &level.cloths[i];
            draw_hanging_cloth(c->x1 + ox1, c->y1, c->x2 + ox1, c->y2);
        }
    }

    /* Layer 2: bg stone pillars at 0.50x (all themes except bamboo/fortress) */
    if (theme != 3 && theme != 4)
    {
        for (int i = 0; i < level.bgobj_count; i++)
        {
            BgObj *b = &level.bgobjs[i];
            float bx = b->x + ox2;
            if (b->type == 0)
                draw_bg_stone_pillar(bx, b->y, b->w, b->h);
        }
    }

    /* Pagoda gate for temple and village */
    if (theme == 2 || theme == 1)
    {
        draw_bg_pagoda(800 + ox2, 40, 1.0f);
        draw_lantern_string(600 + ox2, 260, 1000 + ox2, 260, 5);
    }
}

/* ── Draw all world objects ─────────────────────────────── */
static void draw_world(void)
{
    int theme = level.theme;

    /* Platforms */
    for (int i = 0; i < level.plat_count; i++)
        draw_platform(&level.platforms[i], theme);

    /* Rope fences (hard-coded for village theme) */
    if (theme == 2)
        draw_rope_fence(200, 40, 300, 5);

    /* Traps */
    for (int i = 0; i < level.trap_count; i++)
    {
        Trap *t = &level.traps[i];
        switch (t->type)
        {
        case TRAP_BLADE:
            draw_blade_trap(t->x, t->y, t->angle);
            break;
        case TRAP_SPIKE:
            draw_spikes_ground(t->x, t->y, t->count);
            break;
        case TRAP_THORN:
            draw_thorn_bush(t->x, t->y, t->scale);
            break;
        case TRAP_FIRE:
            draw_fire_vent(t->x, t->y, t->active, t->fire_t);
            break;
        }
    }

    /* Pots */
    for (int i = 0; i < level.pot_count; i++)
    {
        Pot *pt = &level.pots[i];
        if (pt->active)
            draw_clay_pot(pt->x, pt->y, pt->broken, pt->break_t);
    }

    /* Collectibles */
    for (int i = 0; i < level.coll_count; i++)
    {
        Collectible *c = &level.colls[i];
        if (c->collected)
            continue;
        if (c->is_star)
            draw_star_collectible(c->x, c->y, c->pulse_t);
        else
            draw_gem(c->x, c->y, c->pulse_t);
    }

    /* Grass tufts */
    draw_grass_tufts(level.level_w);

    /* Checkpoints */
    for (int i = 0; i < level.checkpoint_count; i++)
    {
        Checkpoint *ck = &level.checkpoints[i];
        draw_checkpoint(ck->x, ck->y, ck->triggered, game_time);
    }

    /* Exit portal — colour changes based on completion conditions */
    draw_exit_portal(level.exit_x, level.exit_y, game_time, portal_is_open());

    /* Enemies */
    for (int i = 0; i < level.enemy_count; i++)
    {
        Enemy *e = &level.enemies[i];

        /* I-08: Render dead enemies during death_timer for a fade-out effect */
        if (!e->alive)
        {
            if (e->death_timer <= 0)
                continue;
            /* Dissolve: three dark circles at body regions fade out over 0.5 s */
            float fade_a = e->death_timer / 0.5f; /* 1→0 */
            float r = (e->type == ENEMY_HEAVY) ? 22.0f : 18.0f;
            glColor4f(0.05f, 0.03f, 0.06f, fade_a * 0.85f);
            draw_circle(e->x, e->y + 14, r, 14);        /* lower body */
            draw_circle(e->x, e->y + 40, r * 1.1f, 16); /* torso */
            draw_circle(e->x, e->y + 62, r * 0.8f, 12); /* head */
            continue;
        }

        switch (e->type)
        {
        case ENEMY_GUARD:
            draw_enemy_guard(e->x, e->y, e->facing, e->anim_t);
            break;
        case ENEMY_ARCHER:
            draw_enemy_archer(e->x, e->y, e->facing, e->shoot_timer < 0.5f);
            break;
        case ENEMY_HEAVY:
            draw_enemy_heavy(e->x, e->y, e->facing, e->anim_t);
            break;
        }
        /* Reset colour state before health bar so no tint leaks through */
        glColor4f(1, 1, 1, 1);

        /* Health bar above enemy */
        if (e->health > 0)
        {
            int max_hp = (e->type == ENEMY_HEAVY) ? 12 : (e->type == ENEMY_ARCHER) ? 4
                                                                                   : 6;
            float bar_w = 40.0f;
            float bar_h = 4.0f;
            float bx = e->x - bar_w / 2;
            float by = e->y + 90;
            glColor4f(0.3f, 0, 0, 0.8f);
            draw_rect(bx, by, bar_w, bar_h);
            glColor4f(0.9f, 0.1f, 0.1f, 1);
            draw_rect(bx, by, bar_w * (float)e->health / max_hp, bar_h);
        }
    }

    /* Projectiles */
    for (int i = 0; i < level.proj_count; i++)
    {
        Projectile *pr = &level.projs[i];
        if (!pr->active)
            continue;
        if (pr->is_shuriken)
            draw_shuriken(pr->x, pr->y, pr->angle);
        else
            draw_arrow(pr->x, pr->y, pr->vx, pr->vy);
    }

    /* Player — I-01: skip every other frame during post-hit invincibility window */
    int draw_player = 1;
    if (player.hurt_timer > 0 && player.anim.state != ANIM_HURT && player.anim.state != ANIM_DEAD)
    {
        /* Blink at ~10 Hz: hide player on alternating frames to show invincibility */
        draw_player = (fmodf(game_time * 10.0f, 1.0f) > 0.4f);
    }
    if (draw_player)
        draw_player_dispatch(&player);

    /* Particles */
    particles_draw();
}

/* ── Main display callback ──────────────────────────────── */
void display(void)
{
    const float fixed_dt = 1.0f / 60.0f;
    int now = glutGet(GLUT_ELAPSED_TIME);
    float frame_dt = (now - last_ms) / 1000.0f;
    if (frame_dt < 0.0f)
        frame_dt = 0.0f;
    if (frame_dt > 0.10f)
        frame_dt = 0.10f;
    last_ms = now;

    /* Visual time always advances smoothly, even outside gameplay. */
    game_time += frame_dt;
    /* A-02: Wrap at a large multiple of 2π — sinf/cosf stay precise, no discontinuity */
    if (game_time > 2.0f * PI * 1000.0f)
        game_time -= 2.0f * PI * 1000.0f;

    /* Fixed-step simulation prevents speed spikes and inconsistent hit windows. */
    if (game_state == STATE_PLAYING)
    {
        sim_accum += frame_dt;
        if (sim_accum > 0.20f)
            sim_accum = 0.20f;
        while (sim_accum >= fixed_dt)
        {
            update_game(fixed_dt);
            sim_accum -= fixed_dt;
        }
    }
    else
    {
        sim_accum = 0.0f;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    switch (game_state)
    {
    case STATE_PLAYING:
    case STATE_PAUSED:
    {
        /* -- Background layers (no world transform) -- */
        draw_sky();
        /* Twinkling night stars (drawn before moon so moon sits in front) */
        draw_night_stars(game_time);
        /* Draw moon based on cycle (changes every 3 minutes of gameplay) */
        float moon_phase = fmodf(game_time / 180.0f, 1.0f); /* Full cycle every 3 min */
        draw_moon(1100.0f, 600.0f, moon_phase, level.theme);
        /* Far ghost trees */
        draw_ghost_trees(camera.x);
        /* Clouds in mid-sky */
        draw_clouds(game_time, camera.x);
        /* Flying birds above the cloud layer */
        draw_birds(game_time, camera.x);
        draw_ground_atmosphere();
        draw_parallax(camera.x, level.theme);

        /* -- World space -- */
        glPushMatrix();
        glTranslatef(-camera.x, -camera.y, 0);
        draw_world();
        glPopMatrix();

        draw_hud(player.health, MAX_HEALTH, gems_collected, gold_collected,
                 stars_collected, player.shuriken_count, game_time);

        /* Level indicator — centered at top so it doesn't overlap the settings gear (B-19) */
        glColor4f(0.7f, 0.7f, 0.8f, 0.7f);
        char lvl_str[32];
        sprintf(lvl_str, "Level %d / 10", current_level);
        glRasterPos2f(590, 700);
        for (char *c = lvl_str; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

        if (game_state == STATE_PAUSED)
            draw_pause_overlay();
        break;
    }
    case STATE_MENU:
        draw_sky();
        draw_night_stars(game_time);
        draw_ghost_trees(0);
        draw_main_menu(game_time);
        break;
    case STATE_GAME_OVER:
        draw_sky();
        draw_game_over_screen();
        break;
    case STATE_LEVEL_COMPLETE:
        draw_sky();
        draw_level_complete(stars_collected);
        break;
    case STATE_SETTINGS:
        /* Re-draw the game world underneath then overlay settings panel */
        draw_sky();
        draw_night_stars(game_time);
        draw_ghost_trees(camera.x);
        draw_settings_overlay();
        break;
    }

    glutSwapBuffers();
    /* Do NOT call glutPostRedisplay() here — timer_callback drives the loop at ~60 fps */
}

/* ── Entry point ────────────────────────────────────────── */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutCreateWindow("Ninja Shadow");

    glClearColor(0.08f, 0.14f, 0.24f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_W, 0, SCREEN_H);
    glMatrixMode(GL_MODELVIEW);

    last_ms = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutTimerFunc(16, timer_callback, 0); /* start the ~60 fps timer loop */
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutSpecialFunc(special_down);
    glutSpecialUpFunc(special_up);
    glutMouseFunc(mouse_click);
    glutIgnoreKeyRepeat(1); /* Prevent auto-repeat from causing repeated jumps/throws */

    audio_init(); /* silent no-op if OpenAL unavailable or files not yet placed */
    game_init();

    printf("==============================================\n");
    printf("  NINJA SHADOW — 2D OpenGL Platformer\n");
    printf("==============================================\n");
    printf("  Controls:\n");
    printf("  Arrow Keys / WASD   - Move\n");
    printf("  Space / Up Arrow    - Jump (double jump)\n");
    printf("  Z / J               - Sword Attack\n");
    printf("  X / K               - Throw Shuriken\n");
    printf("  E / L / Enter       - Dash\n");
    printf("  P / Esc             - Pause\n");
    printf("==============================================\n");

    glutMainLoop();
    audio_cleanup();
    return 0;
}
