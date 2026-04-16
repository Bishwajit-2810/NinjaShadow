#include "game.h"
#include "renderer.h"
#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ── Globals ────────────────────────────────────────────── */
GameState game_state = STATE_MENU;
Player player;
Level level;
Camera camera = {0, 0};
Particle particles[MAX_PARTICLES];
float game_time = 0;
int gems_collected = 0;
int gold_collected = 0;
int stars_collected = 0;
int current_level = 1;
int keys[256];
int special_keys[256];
int respawn_health = MAX_HEALTH; /* Reduces by 1 each time player fails with no checkpoint */

/* Set when a trap injures the player — deferred so we don't mutate level mid-loop */
static int respawn_requested = 0;
/* Set when an enemy melee or arrow hits the player — triggers checkpoint warp */
static int enemy_hit_respawn = 0;

/* ── New globals ────────────────────────────────────────── */
float level_time      = 0;  /* seconds elapsed in this level */
int   enemies_defeated = 0; /* enemies killed this level     */
int   combo_count     = 0;  /* consecutive hit streak        */
float combo_timer     = 0;  /* resets to 2.0 s per hit       */
float penalty_toast_timer = 0.0f; /* counts down while "-1 max heart" toast is visible */

/* Forward declarations from other files */
void draw_player_dispatch(Player *p);
void draw_shuriken(float cx, float cy, float angle);
void draw_enemy_guard(float cx, float cy, int facing, float anim_t);
void draw_enemy_archer(float cx, float cy, int facing, int shooting);
void draw_enemy_heavy(float cx, float cy, int facing, float anim_t);
void draw_arrow(float cx, float cy, float vx, float vy);

void draw_sky(void);
void draw_ghost_trees(float cam_x);
void draw_ground_atmosphere(void);
void draw_bare_tree(float bx, float by, float scale);
void draw_bamboo_bg(float cam_x);
void draw_hanging_cloth(float x1, float y1, float x2, float y2);
void draw_bg_stone_pillar(float cx, float y, float w, float h);
void draw_bg_pagoda(float cx, float y, float scale);
void draw_lantern_string(float x1, float y1, float x2, float y2, int count);
void draw_grass_tufts(float lw);

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

void draw_hud(int health, int max_health, int gems, int gold, int stars, int shurikens, float game_time);
void draw_main_menu(float t);
void draw_pause_overlay(void);
void draw_game_over_screen(void);
void draw_level_complete(int stars);

/* ── Particle system ────────────────────────────────────── */
void particle_spawn(float x, float y, float vx, float vy,
                    float r, float g, float b, float life, float size)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!particles[i].active)
        {
            particles[i].x = x;
            particles[i].y = y;
            particles[i].vx = vx;
            particles[i].vy = vy;
            particles[i].life = life;
            particles[i].max_life = life;
            particles[i].r = r;
            particles[i].g = g;
            particles[i].b = b;
            particles[i].a = 1.0f;
            particles[i].size = size;
            particles[i].active = 1;
            return;
        }
    }
}

void spawn_hit_sparks(float cx, float cy)
{
    for (int i = 0; i < 8; i++)
    {
        float a = i * 45.0f * PI / 180.0f;
        float speed = 80 + (i % 3) * 30;
        particle_spawn(cx, cy, cosf(a) * speed, sinf(a) * speed + 60, 1, 1, 0.8f, 0.3f, 3.0f);
    }
}

void spawn_collect_burst(float cx, float cy)
{
    for (int i = 0; i < 12; i++)
    {
        float a = i * 30.0f * PI / 180.0f, speed = 60 + (i % 4) * 25;
        particle_spawn(cx, cy, cosf(a) * speed, sinf(a) * speed + 80,
                       0.8f + 0.2f * (i % 2), 0.9f, 0.1f * (i % 3), 0.6f, 4.0f);
    }
}

/* I-07: Dust puff when player lands after a fall */
void spawn_landing_dust(float cx, float cy)
{
    for (int i = 0; i < 10; i++)
    {
        float angle = (150.0f + i * 24.0f) * PI / 180.0f; /* spread outward/upward */
        float speed = 40.0f + (i % 3) * 20.0f;
        particle_spawn(cx + (i % 2 ? 6 : -6), cy,
                       cosf(angle) * speed, sinf(angle) * speed,
                       0.55f, 0.50f, 0.44f, 0.45f, 3.5f);
    }
}

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

void particles_draw(void)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!particles[i].active)
            continue;
        glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].a);
        draw_circle(particles[i].x, particles[i].y, particles[i].size, 6);
    }
}

/* ── Camera ─────────────────────────────────────────────── */
void camera_update(Camera *cam, float px, float py, float lw, float dt)
{
    float tx = px - SCREEN_W * 0.38f;
    float ty = py - SCREEN_H * 0.28f;
    /* Improved smoothing: clamp speed to reduce jitter */
    float smooth_x = fminf(fmaxf((tx - cam->x) * 8.0f, -400.0f), 400.0f);
    float smooth_y = fminf(fmaxf((ty - cam->y) * 6.0f, -300.0f), 300.0f);
    cam->x += smooth_x * dt;
    cam->y += smooth_y * dt;
    if (cam->x < 0)
        cam->x = 0;
    if (cam->x > lw - SCREEN_W)
        cam->x = lw - SCREEN_W;
    if (cam->y < 0)
        cam->y = 0;
    if (level.level_h > SCREEN_H && cam->y > level.level_h - SCREEN_H)
        cam->y = level.level_h - SCREEN_H;
}

/* ── Animation update ───────────────────────────────────── */
void animation_update(Animation *a, float dt)
{
    a->timer += dt;
    switch (a->state)
    {
    case ANIM_IDLE:
        a->anim_t = sinf(a->timer * 1.5f) * 0.5f + 0.5f;
        break;
    case ANIM_RUN:
        a->anim_t = fmodf(a->timer * 6.0f, 1.0f);
        break;
    case ANIM_JUMP:
        a->anim_t = 0.0f; /* Static jump pose */
        break;
    case ANIM_FALL:
        a->anim_t = 1.0f; /* Static falling pose */
        break;
    case ANIM_WALL_SLIDE:
        a->anim_t = fmodf(a->timer * 2.0f, 1.0f); /* Slow wave for wall slide */
        break;
    case ANIM_ATTACK:
        /* Match attack_timer (0.3s) so animation and logic end at the same frame */
        a->anim_t = fminf(a->timer / 0.3f, 1.0f);
        if (a->timer >= 0.3f)
        {
            a->state = ANIM_IDLE;
            a->timer = 0;
        }
        break;
    case ANIM_HURT:
        a->anim_t = fminf(a->timer / 0.40f, 1.0f);
        if (a->timer >= 0.40f)
        {
            a->state = ANIM_IDLE;
            a->timer = 0;
        }
        break;
    case ANIM_DASH:
        a->anim_t = fminf(a->timer / 0.18f, 1.0f);
        if (a->timer >= 0.18f)
        {
            a->state = ANIM_IDLE;
            a->timer = 0;
        }
        break;
    default:
        break;
    }
}

/* ── AABB collision helpers ─────────────────────────────── */
static int aabb_overlap(float ax, float ay, float aw, float ah,
                        float bx, float by, float bw, float bh)
{
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

/* ── Platform collision with improved stability ─────────── */
void resolve_platforms(Player *p)
{
    int count = level.plat_count;
    Platform *plats = level.platforms;

    int prev_on_ground = p->on_ground; /* Remember previous state */
    float prev_vy = p->vy;             /* I-07: needed to detect hard landings */
    p->on_ground = 0;
    p->on_wall = 0;

    float px = p->x - p->w / 2;
    float py = p->y;

    /* First pass: find all collisions and pick best ground */
    float best_ground_y = -1e9f;
    int has_ground = 0;
    int best_ground_idx = -1; /* index of platform providing ground (for carry) */

    for (int i = 0; i < count; i++)
    {
        Platform *pl = &plats[i];
        if (pl->crumbled)
            continue;

        float plx = pl->x, ply = pl->y, plw = pl->w, plh = pl->h;

        /* For moving platforms, use current translated position */
        if (!aabb_overlap(px, py, p->w, p->h, plx, ply, plw, plh))
            continue;

        /* One-way: only collide from above */
        if (pl->type == PLAT_ONE_WAY)
        {
            if (p->vy > 0)
                continue; /* moving up -> pass through */
            if (py + PLAYER_H * 0.7f > ply + plh)
                continue; /* already above -> pass through */
        }

        /* Figure out overlap on each axis */
        float ovl = (px + p->w) - plx;
        float ovr = (plx + plw) - px;
        float ovb = (py + p->h) - ply;
        float ovt = (ply + plh) - py;

        float min_ov = ovl;
        if (ovr < min_ov)
            min_ov = ovr;
        if (ovb < min_ov)
            min_ov = ovb;
        if (ovt < min_ov)
            min_ov = ovt;

        if (min_ov == ovb && p->vy <= 0)
        {
            /* Ground collision */
            if (ply + plh > best_ground_y)
            {
                best_ground_y = ply + plh;
                has_ground = 1;
                best_ground_idx = i;
                if (pl->type == PLAT_CRUMBLING && pl->crumble_timer <= 0)
                    pl->crumble_timer = 0.001f;
            }
        }
        else if (min_ov == ovt && p->vy >= 0)
        {
            /* Ceiling collision */
            p->y = ply - p->h;
            p->vy = 0;
        }
        else if (min_ov == ovl && p->vx > 0)
        {
            /* Wall collision (left side of platform) */
            p->x = plx - p->w / 2;
            p->vx = 0;
            p->on_wall = -1;
        }
        else if (min_ov == ovr && p->vx < 0)
        {
            /* Wall collision (right side of platform) */
            p->x = plx + plw + p->w / 2;
            p->vx = 0;
            p->on_wall = 1;
        }
    }

    /* Apply ground collision if found */
    if (has_ground)
    {
        p->y = best_ground_y;
        p->vy = 0;      /* Clear velocity on landing */
        p->vx *= 0.95f; /* Slight friction on landing */
        p->on_ground = 1;
        p->jump_count = 0;
        /* I-07: Landing dust — only when transitioning from airborne with some speed */
        if (!prev_on_ground && prev_vy < -180.0f)
            spawn_landing_dust(p->x, p->y);
        /* B-07/B2-10: carry player horizontally AND vertically with moving platform */
        if (best_ground_idx >= 0 && plats[best_ground_idx].type == PLAT_MOVING)
        {
            Platform *carrier = &plats[best_ground_idx];
            p->x += carrier->pvx;
            /* Vertical moving platforms: re-snap y so player rides up/down smoothly */
            if (carrier->pvy != 0.0f)
            {
                p->y = carrier->y + carrier->h; /* always sit on top of moving platform */
            }
        }
    }
    else if (prev_on_ground && p->vy <= 0)
    {
        /* Stick to ground: if was on ground and not moving up, snap down to nearest platform */
        /* Prevents 1-frame FALL flicker when player sits exactly on platform edge boundary */
        float px_center = p->x;

        for (int i = 0; i < count; i++)
        {
            Platform *pl = &plats[i];
            if (pl->crumbled)
                continue;

            float plat_top = pl->y + pl->h; /* top edge of platform */
            /* 8 px snap window (B2-17): safe because all platforms have h=32.
               If a platform thinner than 8 px is ever added, reduce this window. */
            if (px_center > pl->x - 4 && px_center < pl->x + pl->w + 4 &&
                p->y >= plat_top - 8 && p->y <= plat_top + 8)
            {
                p->y = plat_top; /* snap player bottom to platform top */
                p->vy = 0;
                p->on_ground = 1;
                break;
            }
        }
    }

    /* Kill if below level */
    if (p->y < -200)
    {
        p->health = 0;
    }
}

/* ── Player physics ─────────────────────────────────────── */
void player_physics(Player *p, float dt)
{
    /* Gravity */
    p->vy -= GRAVITY * dt;
    if (p->vy < MAX_FALL_SPD)
        p->vy = MAX_FALL_SPD;

    /* Wall slide slow fall */
    if (p->on_wall != 0 && !p->on_ground && p->vy < WALL_SLIDE_VY)
    {
        p->vy = WALL_SLIDE_VY;
        if (p->anim.state != ANIM_ATTACK)
            p->anim.state = ANIM_WALL_SLIDE;
    }

    /* Horizontal friction */
    if (p->on_ground && fabsf(p->vx) < 5)
        p->vx = 0;

    /* Dash timer */
    if (p->dash_timer > 0)
    {
        p->dash_timer -= dt;
        if (p->dash_timer <= 0)
        {
            p->dash_timer = 0;
        }
    }
    if (p->dash_cooldown > 0)
        p->dash_cooldown -= dt;

    /* Hurt timer */
    if (p->hurt_timer > 0)
        p->hurt_timer -= dt;

    p->x += p->vx * dt;
    p->y += p->vy * dt;
}

/* ── Input → player action ──────────────────────────────── */
void player_input(Player *p, float dt)
{
    if (p->health <= 0)
        return;
    if (p->anim.state == ANIM_DEAD)
        return;

    int left = keys['a'] || keys['A'] || special_keys[100]; /* left arrow = 100 */
    int right = keys['d'] || keys['D'] || special_keys[102];
    /* jump/dash/shuriken handled via pressed-flags to prevent key-repeat fire */
    int atk  = keys['z'] || keys['Z'] || keys['j'] || keys['J'];

    /* Active dash: hold constant velocity, skip normal movement friction */
    if (p->dash_timer > 0)
    {
        p->vx = DASH_SPEED * p->anim.facing; /* Maintain full dash speed */
        p->anim.state = ANIM_DASH;           /* Keep dash animation locked */
    }
    else if (left && p->anim.state != ANIM_ATTACK)
    {
        p->vx = -MOVE_SPEED;
        p->anim.facing = -1;
        if (p->on_ground)
            p->anim.state = ANIM_RUN;
    }
    else if (right && p->anim.state != ANIM_ATTACK)
    {
        p->vx = MOVE_SPEED;
        p->anim.facing = 1;
        if (p->on_ground)
            p->anim.state = ANIM_RUN;
    }
    else
    {
        /* Frame-rate-independent deceleration: equivalent to 0.7 multiplier at 60 fps */
        p->vx *= powf(0.7f, dt * 60.0f);
        if (p->on_ground && p->anim.state == ANIM_RUN)
            p->anim.state = ANIM_IDLE;
    }

    /* Jump - handle press state and availability */
    if (player.jump_pressed && player.jump_available)
    {
        int jump_triggered = 0;
        if (player.on_ground)
        {
            /* Ground jump */
            player.vy = JUMP_VY;
            player.on_ground = 0;
            player.jump_count = 1;
            player.anim.state = ANIM_JUMP;
            player.jump_available = 0; /* Consume the jump */
            jump_triggered = 1;
            audio_play(SFX_JUMP);
        }
        else if (player.on_wall != 0)
        {
            /* Wall jump — push AWAY from the wall.
               on_wall=-1 means wall is on player's RIGHT → jump left (vx negative).
               on_wall=+1 means wall is on player's LEFT  → jump right (vx positive).
               B-05: was negated (-on_wall), which pushed the wrong direction. */
            player.vy = JUMP_VY * 0.92f;
            player.vx = player.on_wall * MOVE_SPEED * 1.5f;
            player.anim.facing = player.on_wall;
            player.jump_count = 2;
            player.anim.state = ANIM_JUMP;
            player.jump_available = 0;
            jump_triggered = 1;
            audio_play(SFX_JUMP);
        }
        else if (player.jump_count == 1)
        {
            /* Double jump */
            player.vy = DBL_JUMP_VY;
            player.jump_count = 2;
            player.anim.state = ANIM_JUMP;
            player.jump_available = 0;
            jump_triggered = 1;
            audio_play(SFX_DOUBLE_JUMP);
        }
        /* Consume the jump press after it's triggered */
        if (jump_triggered)
            player.jump_pressed = 0;
    }

    /* Restore jump availability when landing on ground */
    if (player.on_ground && !player.jump_available)
    {
        player.jump_available = 1;
        player.jump_count = 0;
        player.jump_pressed = 0; /* Require a fresh key press to jump again */
    }

    /* Shuriken throw — one-shot per key press, infinite ammo */
    if (p->shuriken_pressed)
    {
        if (level.proj_count < MAX_PROJECTILES)
        {
            Projectile *pr = &level.projs[level.proj_count++];
            pr->x = p->x;
            pr->y = p->y + 38;
            pr->vx = p->anim.facing * 500.0f;
            pr->vy = 0;   /* No gravity — perfectly straight line */
            pr->active = 1;
            pr->is_shuriken = 1;
            pr->from_player = 1;
            pr->angle = 0;
            pr->range = 9999.0f;
            audio_play(SFX_SHURIKEN_THROW);
        }
        p->shuriken_pressed = 0;
    }

    /* Dash — one-shot flag prevents key-repeat from firing on every cooldown expiry */
    if (p->dash_pressed && p->dash_cooldown <= 0 && p->dash_timer <= 0)
    {
        p->vx = DASH_SPEED * p->anim.facing;
        p->dash_timer = 0.18f;
        p->dash_cooldown = 0.8f;
        p->anim.state = ANIM_DASH;
        p->dash_pressed = 0; /* Consume: must release and re-press to dash again */
        audio_play(SFX_DASH);
    }

    /* Attack */
    if (atk && p->anim.state != ANIM_ATTACK && p->attack_timer <= 0)
    {
        p->anim.state = ANIM_ATTACK;
        p->anim.timer = 0;
        p->attack_timer = 0.3f;
        audio_play(SFX_ATTACK_SLASH);
    }
    if (p->attack_timer > 0)
        p->attack_timer -= dt;

    /* Update animation state from physics */
    if (p->anim.state != ANIM_ATTACK && p->anim.state != ANIM_HURT &&
        p->anim.state != ANIM_DEAD  && p->anim.state != ANIM_DASH)
    {
        int new_state = p->anim.state;
        if (!p->on_ground)
        {
            if (p->vy > 0)
                new_state = ANIM_JUMP;
            else if (p->on_wall != 0)
                new_state = ANIM_WALL_SLIDE;
            else
                new_state = ANIM_FALL;
        }
        else
        {
            /* Use higher threshold (50) to prevent flickering from friction oscillation */
            if (fabsf(p->vx) > 50)
                new_state = ANIM_RUN;
            else
                new_state = ANIM_IDLE;
        }
        /* Reset timer when state changes to prevent stale animation values */
        if (new_state != p->anim.state)
        {
            p->anim.state = new_state;
            p->anim.timer = 0;
        }
    }
}

/* ── Line-of-sight check (B-01) ─────────────────────────── */
/* Returns 1 if no solid platform blocks the ray from (ax,ay) to (px,py) */
static int has_los(float ax, float ay, float px, float py)
{
    for (int i = 0; i < level.plat_count; i++)
    {
        Platform *pl = &level.platforms[i];
        if (pl->type == PLAT_ONE_WAY || pl->crumbled)
            continue;
        float lx = (ax < px ? ax : px);
        float rx = (ax > px ? ax : px);
        /* X overlap */
        if (pl->x + pl->w <= lx || pl->x >= rx)
            continue;
        /* Sample ray at the platform's center X.
           Guard against mid_x falling outside the ray segment (partial X overlap):
           if t is outside [0,1] the sample point is off the segment — skip. */
        float mid_x = pl->x + pl->w * 0.5f;
        float t = (px == ax) ? 0.5f : (mid_x - ax) / (px - ax);
        if (t < 0.0f || t > 1.0f)
            continue; /* platform only clips the edge; sample is off-segment */
        float ray_y = ay + t * (py - ay);
        if (ray_y >= pl->y && ray_y <= pl->y + pl->h)
            return 0; /* platform blocks LOS */
    }
    return 1;
}

/* ── Enemy update ───────────────────────────────────────── */
void update_enemies(float dt)
{
    for (int i = 0; i < level.enemy_count; i++)
    {
        Enemy *e = &level.enemies[i];

        /* I-08: Death-fade timer — still updated when alive==0 */
        if (!e->alive)
        {
            if (e->death_timer > 0)
                e->death_timer -= dt;
            continue;
        }

        e->anim_t += dt;
        if (e->hurt_flash > 0)
            e->hurt_flash -= dt;
        if (e->hurt_flash < 0)
            e->hurt_flash = 0;
        /* B-03: decay sword hit cooldown */
        if (e->attack_hit_timer > 0)
            e->attack_hit_timer -= dt;

        float dx = player.x - e->x;
        float dy = player.y - e->y;

        /* Archers: restore patrol direction from vx before the movement block.
           e->facing may have been overridden last frame for visual bow-aim; vx is
           the authoritative patrol direction and is never touched by the aim code. */
        if (e->type == ENEMY_ARCHER)
            e->facing = (e->vx >= 0.0f) ? 1 : -1;

        /* Guard/Heavy aggro: charge toward player within 250 px horizontal AND
           100 px vertical — prevents chasing across different platform levels.
           Patrol bounds are enforced so enemies cannot chase off their platform.
           Player must also be within the patrol zone to trigger aggro, preventing
           enemies from jamming against their boundary when player is outside. */
        if (e->type != ENEMY_ARCHER && fabsf(dx) > 20.0f && fabsf(dx) < 250.0f
            && fabsf(dy) < 100.0f
            && player.x >= e->patrol_l && player.x <= e->patrol_r)
        {
            /* Face the player — dead zone prevents flicker right next to the player */
            if (fabsf(dx) > 10.0f)
                e->facing = (dx > 0) ? 1 : -1;
            float spd = (e->type == ENEMY_HEAVY) ? 130.0f : 160.0f;
            e->x += e->facing * spd * dt;
            /* Clamp within patrol bounds so aggroed enemies never leave their platform */
            if (e->x < e->patrol_l) e->x = e->patrol_l;
            if (e->x > e->patrol_r) e->x = e->patrol_r;
        }
        else
        {
            /* Patrol: flip direction only at bounds — no player-position drift.
               Archers always take this path; Guard/Heavy take it when out of aggro range.
               vx stores the persistent patrol direction so the visual-facing override
               for archers (below) cannot corrupt next-frame movement. */
            if (e->x <= e->patrol_l) { e->facing = 1;  e->vx =  1.0f; }
            if (e->x >= e->patrol_r) { e->facing = -1; e->vx = -1.0f; }
            float spd = (e->type == ENEMY_HEAVY) ? 80.0f : 100.0f;
            e->x += e->facing * spd * dt;
            /* Clamp to patrol bounds — prevents overshoot on low-FPS frames */
            if (e->x < e->patrol_l) e->x = e->patrol_l;
            if (e->x > e->patrol_r) e->x = e->patrol_r;
        }

        /* Archers visually face the player (for bow aim) independently of patrol movement */
        if (e->type == ENEMY_ARCHER && fabsf(dx) > 10.0f && fabsf(dx) < 450.0f)
            e->facing = (dx > 0) ? 1 : -1;

        /* Archer: shoot at player with LOS check (B-01) */
        if (e->type == ENEMY_ARCHER)
        {
            e->shoot_timer -= dt;
            if (e->shoot_timer <= 0 && fabsf(dx) < 450)
            {
                /* B-01: only fire if no solid platform blocks the line of sight */
                if (has_los(e->x, e->y + 40, player.x, player.y + 32))
                {
                    if (level.proj_count >= MAX_PROJECTILES)
                    {
                        /* Buffer full — retry soon instead of waiting a full 3 s */
                        e->shoot_timer = 0.5f;
                    }
                    else
                    {
                        e->shoot_timer = 3.0f;
                        Projectile *pr = &level.projs[level.proj_count++];
                        pr->x = e->x;
                        pr->y = e->y + 40;
                        pr->vx = (dx > 0 ? 1 : -1) * 280.0f;
                        {
                            float raw_vy = (player.y + 32 - (e->y + 40)) / fabsf(dx) * 280.0f;
                            if (raw_vy >  120.0f) raw_vy =  120.0f;
                            if (raw_vy < -120.0f) raw_vy = -120.0f;
                            pr->vy = raw_vy;
                        }
                        pr->active = 1;
                        pr->is_shuriken = 0;
                        pr->from_player = 0;
                        pr->range = 0; /* B-10: initialise unused field */
                    }
                }
                else
                {
                    /* No LOS — reset timer quickly so archer checks again soon */
                    e->shoot_timer = 0.5f;
                }
            }
        }

        /* Check if player sword attack hits (B-03: one hit per swing via attack_hit_timer) */
        if (player.anim.state == ANIM_ATTACK && player.attack_timer > 0.15f &&
            e->attack_hit_timer <= 0)
        {
            float reach = 40.0f;
            float ax = player.x + player.anim.facing * 20;
            if (fabsf(ax - e->x) < reach && fabsf(player.y - e->y) < 70
                && (e->x - player.x) * player.anim.facing > -10.0f)
            {
                int dmg = 2;
                e->health -= dmg;
                e->hurt_flash = 0.2f;
                e->attack_hit_timer = 0.3f; /* B-03: lock out re-hit for full swing duration */
                spawn_hit_sparks(e->x, e->y + 40);
                audio_play(SFX_ATTACK_HIT);
                /* Combo counter (I-03) */
                combo_count++;
                combo_timer = 2.0f;
                if (e->health <= 0)
                {
                    e->alive = 0;
                    e->death_timer = 0.5f; /* I-08: start death-fade */
                    enemies_defeated++;
                    spawn_hit_sparks(e->x, e->y + 40);
                    spawn_hit_sparks(e->x + 10, e->y + 30);
                    audio_play(SFX_ENEMY_DEATH);
                }
            }
        }

        /* Enemy melee attacks player — must be facing the player (dx * facing > 0)
           with a small tolerance so standing exactly side-by-side still connects.
           On hit: -1 health and warp to last checkpoint (or level start). */
        if (e->type != ENEMY_ARCHER)
        {
            float mdy = (player.y + 32) - (e->y + 32);
            if (fabsf(dx) < 40 && fabsf(mdy) < 60 && player.hurt_timer <= 0
                && dx * e->facing > -5.0f
                && player.x >= e->patrol_l - 20 && player.x <= e->patrol_r + 20)
            {
                spawn_hit_sparks(player.x, player.y + 32);
                player.hurt_timer = 1.0f; /* Prevent double-hit before warp */
                enemy_hit_respawn = 1;
            }
        }
    }
}

/* ── Projectile update ──────────────────────────────────── */
void update_projectiles(float dt)
{
    for (int i = 0; i < level.proj_count; i++)
    {
        Projectile *pr = &level.projs[i];
        if (!pr->active)
            continue;

        /* Gravity: player shurikens travel straight, arrows and enemy shurikens arc */
        if (!(pr->is_shuriken && pr->from_player))
        {
            float gravity = pr->is_shuriken ? 200.0f : 120.0f;
            pr->vy -= gravity * dt;
        }
        pr->x += pr->vx * dt;
        pr->y += pr->vy * dt;
        pr->angle += (pr->is_shuriken ? 720 : 0) * dt;

        /* Player shurikens: infinite range — only despawn when past level bounds */
        if (pr->is_shuriken && pr->from_player)
        {
            if (pr->x < -200 || pr->x > level.level_w + 200 || pr->y < -200)
                pr->active = 0;
            /* Skip generic out-of-screen check below */
        }
        else
        {
            /* Other projectiles: despawn when off camera */
            if (pr->x < camera.x - 100 || pr->x > camera.x + 1400 || pr->y < -200)
            {
                pr->active = 0;
                continue;
            }
        }

        /* Shuriken hits enemies */
        if (pr->is_shuriken && pr->from_player)
        {
            for (int j = 0; j < level.enemy_count; j++)
            {
                Enemy *e = &level.enemies[j];
                if (!e->alive)
                    continue;
                if (fabsf(pr->x - e->x) < 30 && fabsf(pr->y - e->y - 32) < 50)
                {
                    e->health--;
                    e->hurt_flash = 0.2f;
                    spawn_hit_sparks(e->x, e->y + 32);
                    /* Combo counter (I-03) */
                    combo_count++;
                    combo_timer = 2.0f;
                    if (e->health <= 0)
                    {
                        e->alive = 0;
                        e->death_timer = 0.5f; /* I-08: death fade */
                        enemies_defeated++;
                        spawn_hit_sparks(e->x, e->y + 32);
                        audio_play(SFX_ENEMY_DEATH);
                    }
                    audio_play(SFX_SHURIKEN_HIT);
                    pr->active = 0;
                    break; /* one shuriken can only hit one enemy per frame */
                }
            }
        }

        /* Arrow hits player — -1 health and warp to last checkpoint (or level start) */
        if (!pr->is_shuriken && !pr->from_player)
        {
            if (fabsf(pr->x - player.x) < 20 && fabsf(pr->y - player.y - 32) < 50)
            {
                if (player.hurt_timer <= 0)
                {
                    spawn_hit_sparks(player.x, player.y + 32);
                    player.hurt_timer = 1.0f; /* Prevent double-hit before warp */
                    enemy_hit_respawn = 1;
                }
                pr->active = 0;
            }
        }

        /* Collide with platforms */
        for (int j = 0; j < level.plat_count; j++)
        {
            Platform *pl = &level.platforms[j];
            if (pl->crumbled)
                continue;
            if (pr->x > pl->x && pr->x < pl->x + pl->w && pr->y > pl->y && pr->y < pl->y + pl->h)
            {
                pr->active = 0;
                break;
            }
        }
    }

    /* B-14: Compact inactive slots so the buffer never silently blocks new throws */
    int write = 0;
    for (int i = 0; i < level.proj_count; i++)
    {
        if (level.projs[i].active)
            level.projs[write++] = level.projs[i];
    }
    level.proj_count = write;
}

/* ── Trap update ────────────────────────────────────────── */
void update_traps(float dt)
{
    for (int i = 0; i < level.trap_count; i++)
    {
        Trap *t = &level.traps[i];

        if (t->type == TRAP_BLADE)
        {
            t->angle += 200.0f * dt;
            if (fabsf(player.x - t->x) < 30 && fabsf(player.y + 20 - t->y) < 30)
            {
                if (player.hurt_timer <= 0)
                {
                    spawn_hit_sparks(player.x, player.y + 32);
                    respawn_requested = 1;
                }
            }
        }

        if (t->type == TRAP_FIRE)
        {
            t->fire_t += dt;
            float period = t->fire_on + t->fire_off;
            int on_phase = (fmodf(t->fire_t, period) < t->fire_on);
            if (on_phase != t->active)
                t->active = on_phase;
            if (t->active)
            {
                if (fabsf(player.x - t->x) < 18 && player.y + 64 > t->y && player.y < t->y + 60)
                {
                    if (player.hurt_timer <= 0)
                    {
                        spawn_hit_sparks(player.x, player.y + 32);
                        respawn_requested = 1;
                    }
                }
            }
        }

        if (t->type == TRAP_SPIKE)
        {
            /* Spike group spans from t->x to t->x + count*14; check player center */
            float spike_cx = t->x + t->count * 7.0f;
            if (fabsf(player.x - spike_cx) < t->count * 7.0f + 10.0f &&
                player.y < t->y + 24 && player.y + PLAYER_H > t->y)
            {
                if (player.hurt_timer <= 0)
                {
                    spawn_hit_sparks(player.x, player.y + 10);
                    respawn_requested = 1;
                }
            }
        }

        /* Thorn bush hitbox: scaled with t->scale (default 1.0 → 30 px radius, 80 px tall).
           The bush visually extends from t->y up to t->y+80 (main circle centre at +40,
           top circle centre at +58 radius 22), so the upper bound must be t->y+80, not +40. */
        if (t->type == TRAP_THORN)
        {
            if (fabsf(player.x - t->x) < 30.0f * t->scale &&
                player.y < t->y + 80.0f * t->scale && player.y + PLAYER_H > t->y)
            {
                if (player.hurt_timer <= 0)
                {
                    spawn_hit_sparks(player.x, player.y + 10);
                    respawn_requested = 1;
                }
            }
        }
    }
}

/* ── Collectible update ─────────────────────────────────── */
void update_collectibles(float dt)
{
    for (int i = 0; i < level.coll_count; i++)
    {
        Collectible *c = &level.colls[i];
        if (c->collected)
            continue;
        c->pulse_t += dt;
        if (fabsf(player.x - c->x) < 22 && fabsf(player.y + 32 - c->y) < 28)
        {
            c->collected = 1;
            if (c->is_star)
            {
                stars_collected++;
                spawn_collect_burst(c->x, c->y);
                spawn_collect_burst(c->x + 5, c->y - 5);
                audio_play(SFX_STAR_COLLECT);
            }
            else
            {
                gems_collected += 10;
                spawn_collect_burst(c->x, c->y);
                audio_play(SFX_GEM_COLLECT);
            }
        }
    }
}

/* ── Pot update ─────────────────────────────────────────── */
void update_pots(float dt)
{
    for (int i = 0; i < level.pot_count; i++)
    {
        Pot *pt = &level.pots[i];
        if (!pt->active || pt->broken == 2)
            continue;
        if (pt->broken == 1)
        {
            pt->break_t += dt * 2;
            if (pt->break_t >= 1.0f)
                pt->broken = 2;
            continue;
        }
        /* Attack hits pot — 24 px X radius matches the drawn pot width */
        if (player.anim.state == ANIM_ATTACK && player.attack_timer > 0.15f)
        {
            if (fabsf(player.x - pt->x) < 24 && fabsf(player.y - pt->y) < 40)
            {
                pt->broken = 1;
                pt->break_t = 0;
                gold_collected += 5;
                spawn_hit_sparks(pt->x, pt->y + 22);
                audio_play(SFX_POT_BREAK);
            }
        }
    }
}

/* ── Moving platform update ─────────────────────────────── */
void update_moving_platforms(float dt)
{
    for (int i = 0; i < level.plat_count; i++)
    {
        Platform *pl = &level.platforms[i];
        if (pl->type == PLAT_MOVING)
        {
            float old_x = pl->x, old_y = pl->y;
            pl->phase += pl->speed * dt;
            if (pl->axis == 0)
                pl->x = pl->ox + sinf(pl->phase) * pl->amplitude;
            else
                pl->y = pl->oy + sinf(pl->phase) * pl->amplitude;
            /* Store per-frame displacement for player carry (B-07) */
            pl->pvx = pl->x - old_x;
            pl->pvy = pl->y - old_y;
        }
        else
        {
            pl->pvx = 0.0f;
            pl->pvy = 0.0f;
        }
        if (pl->type == PLAT_CRUMBLING && pl->crumble_timer > 0)
        {
            pl->crumble_timer += dt;
            if (pl->crumble_timer > 1.5f)
                pl->crumbled = 1;
        }
    }
}

/* ── Portal open check ──────────────────────────────────── */
/* Returns 1 when all checkpoints are triggered AND all enemies are dead.
   A level with zero checkpoints / zero enemies satisfies the condition automatically. */
int portal_is_open(void)
{
    for (int i = 0; i < level.checkpoint_count; i++)
        if (!level.checkpoints[i].triggered)
            return 0;
    for (int i = 0; i < level.enemy_count; i++)
        if (level.enemies[i].alive)
            return 0;
    return 1;
}

/* ── Check exit ─────────────────────────────────────────── */
void check_exit(void)
{
    /* Portal is locked until all checkpoints are hit and all enemies are dead */
    if (!portal_is_open())
        return;

    /* B2-02: portal is drawn at exit_y+32 (offset inside draw_exit_portal), test against visual centre */
    float portal_y = level.exit_y + 32.0f;
    if (fabsf(player.x - level.exit_x) < 40 && player.y < portal_y + 80 && player.y + PLAYER_H > portal_y)
    {
        /* Count stars based on collectibles */
        int total_stars = 0;
        for (int i = 0; i < level.coll_count; i++)
            if (level.colls[i].is_star && level.colls[i].collected)
                total_stars++;
        stars_collected = total_stars;
        game_state = STATE_LEVEL_COMPLETE;
        audio_stop_bgm();
        audio_play(SFX_LEVEL_COMPLETE);
    }
}

/* ── Level data builder ─────────────────────────────────── */
static void add_platform(float x, float y, float w, float h, PlatType type)
{
    if (level.plat_count >= MAX_PLATFORMS)
        return;
    Platform *p = &level.platforms[level.plat_count++];
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->type = type;
    p->ox = x;
    p->oy = y;
    p->crumble_timer = 0;
    p->crumbled = 0;
    p->pvx = 0.0f;
    p->pvy = 0.0f;
}
static void add_moving_plat(float x, float y, float w, float h, int axis, float amp, float spd)
{
    if (level.plat_count >= MAX_PLATFORMS)
        return;
    Platform *p = &level.platforms[level.plat_count++];
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->type = PLAT_MOVING;
    p->ox = x;
    p->oy = y;
    p->axis = axis;
    p->amplitude = amp;
    p->speed = spd;
    p->phase = 0;
    p->crumble_timer = 0;
    p->crumbled = 0;
}
static void add_enemy(EnemyType type, float x, float y, float pl, float pr)
{
    if (level.enemy_count >= MAX_ENEMIES)
        return;
    Enemy *e = &level.enemies[level.enemy_count++];
    e->type = type;
    e->x = x;
    e->y = y;
    e->facing = 1;
    e->vx = 1.0f; /* Persistent patrol direction (1=right, -1=left); facing may be
                     overridden for visual aim but vx always tracks movement intent */
    e->patrol_l = pl;
    e->patrol_r = pr;
    e->health = (type == ENEMY_HEAVY) ? 8 : (type == ENEMY_ARCHER) ? 4
                                                                   : 6;
    e->alive = 1;
    e->anim_t = 0;
    e->shoot_timer = 2.0f;
    e->hurt_flash = 0;
    e->attack_hit_timer = 0;
    e->death_timer = 0;
}
static void add_trap_blade(float x, float y)
{
    if (level.trap_count >= MAX_TRAPS)
        return;
    Trap *t = &level.traps[level.trap_count++];
    t->type = TRAP_BLADE;
    t->x = x;
    t->y = y;
    t->angle = 0;
}
static void add_trap_spike(float x, float y, int count)
{
    if (level.trap_count >= MAX_TRAPS)
        return;
    Trap *t = &level.traps[level.trap_count++];
    t->type = TRAP_SPIKE;
    t->x = x;
    t->y = y;
    t->count = count;
}
static void add_trap_thorn(float x, float y)
{
    if (level.trap_count >= MAX_TRAPS)
        return;
    Trap *t = &level.traps[level.trap_count++];
    t->type = TRAP_THORN;
    t->x = x;
    t->y = y;
    t->scale = 1.0f;
}
static void add_trap_fire(float x, float y, float on, float off)
{
    if (level.trap_count >= MAX_TRAPS)
        return;
    Trap *t = &level.traps[level.trap_count++];
    t->type = TRAP_FIRE;
    t->x = x;
    t->y = y;
    t->fire_on = on;
    t->fire_off = off;
    t->fire_t = 0;
    t->active = 0;
}
static void add_pot(float x, float y)
{
    if (level.pot_count >= MAX_POTS)
        return;
    Pot *p = &level.pots[level.pot_count++];
    p->x = x;
    p->y = y;
    p->broken = 0;
    p->break_t = 0;
    p->active = 1;
}
static void add_gem(float x, float y)
{
    if (level.coll_count >= MAX_COLLECTIBLES)
        return;
    Collectible *c = &level.colls[level.coll_count++];
    c->x = x;
    c->y = y;
    c->collected = 0;
    c->is_star = 0;
    c->pulse_t = 0;
}
static void add_star(float x, float y)
{
    if (level.coll_count >= MAX_COLLECTIBLES)
        return;
    Collectible *c = &level.colls[level.coll_count++];
    c->x = x;
    c->y = y;
    c->collected = 0;
    c->is_star = 1;
    c->pulse_t = 0;
}
static void add_bg_pillar(float x, float y, float w, float h)
{
    if (level.bgobj_count >= MAX_BG_OBJS)
        return;
    BgObj *b = &level.bgobjs[level.bgobj_count++];
    b->type = 0;
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
}
static void add_bg_tree(float x, float y, float scale)
{
    if (level.bgobj_count >= MAX_BG_OBJS)
        return;
    BgObj *b = &level.bgobjs[level.bgobj_count++];
    b->type = 1;
    b->x = x;
    b->y = y;
    b->w = scale;
}
static void add_cloth(float x1, float y1, float x2, float y2)
{
    if (level.cloth_count >= MAX_CLOTH)
        return;
    ClothDec *c = &level.cloths[level.cloth_count++];
    c->x1 = x1;
    c->y1 = y1;
    c->x2 = x2;
    c->y2 = y2;
}
static void add_checkpoint(float x, float y)
{
    if (level.checkpoint_count >= MAX_CHECKPOINTS)
        return;
    Checkpoint *ck = &level.checkpoints[level.checkpoint_count++];
    ck->x = x;
    ck->y = y;
    ck->triggered = 0;
}

/* ── Load level ─────────────────────────────────────────── */
void load_level(int num)
{
    memset(&level, 0, sizeof(level));
    gems_collected = 0;
    gold_collected = 0;
    stars_collected = 0;
    enemies_defeated = 0;
    combo_count = 0;
    combo_timer = 0;
    level_time = 0;
    current_level = num;
    camera.x = 0;
    camera.y = 0;

    /* Clear stale key state so keys held during menu/game-over don't fire on first frame */
    memset(keys, 0, sizeof(keys));
    memset(special_keys, 0, sizeof(special_keys));

    /* Reset player */
    memset(&player, 0, sizeof(player));
    player.w = PLAYER_W;
    player.h = PLAYER_H;
    player.health = MAX_HEALTH;
    player.anim.facing = 1;
    player.anim.state = ANIM_IDLE;
    player.shuriken_count = 9999; /* Unlimited */
    player.jump_available = 1;    /* Start with jump available */
    player.jump_pressed = 0;      /* Jump button not pressed */
    player.on_ground = 1;         /* Assume on ground to avoid 1-frame FALL flicker at spawn */

    switch (num)
    {
    case 1:
    {
        /* ── LEVEL 1: Pure tutorial — wide platforms, easy enemies, no dangerous traps ── */
        level.level_w = 3800;
        level.level_h = 720;
        level.theme = 0;
        level.bgm = 0;
        player.x = 120;
        player.y = 80;
        /* Ground */
        add_platform(0, 40, 3800, 40, PLAT_SOLID);
        /* Wide, forgiving platforms — small jumps only */
        add_platform(280, 110, 200, 32, PLAT_SOLID);
        add_platform(560, 160, 200, 32, PLAT_SOLID);
        add_platform(840, 120, 200, 32, PLAT_SOLID);
        add_platform(1120, 200, 200, 32, PLAT_SOLID);
        add_platform(1400, 140, 240, 32, PLAT_SOLID);
        add_platform(1720, 180, 220, 32, PLAT_SOLID);
        add_platform(2020, 120, 220, 32, PLAT_SOLID);
        add_platform(2280, 160, 200, 32, PLAT_SOLID);
        add_platform(2560, 120, 180, 32, PLAT_SOLID);
        add_platform(2820, 160, 200, 32, PLAT_SOLID);
        add_platform(3100, 120, 200, 32, PLAT_SOLID);
        add_platform(3380, 160, 200, 32, PLAT_SOLID);
        /* One gentle moving platform to introduce mechanic */
        add_moving_plat(1560, 200, 160, 32, 0, 50, 0.8f);
        /* Collectibles — easy to grab */
        add_gem(300, 150);
        add_gem(580, 200);
        add_gem(860, 160);
        add_gem(1140, 240);
        add_gem(1440, 180);
        add_gem(1760, 220);
        add_gem(2040, 160);
        add_gem(2300, 200);
        add_gem(2580, 160);
        add_gem(2840, 200);
        add_gem(3120, 160);
        add_star(1120, 280);
        add_star(2020, 200);
        add_star(3100, 200);
        /* Props */
        add_pot(380, 80);
        add_pot(700, 80);
        add_pot(1500, 80);
        add_pot(2400, 80);
        add_pot(3200, 80);
        /* 2 guards — easy enemies, wide patrol zones */
        add_enemy(ENEMY_GUARD, 1900, 80, 1700, 2100);
        add_enemy(ENEMY_GUARD, 3000, 80, 2800, 3300);
        /* Spike clusters */
        add_trap_spike(1050, 80, 2);
        add_trap_spike(2100, 80, 3);
        add_trap_spike(3250, 80, 2);
        /* Bg */
        add_bg_tree(160, 0, 1.0f);
        add_bg_tree(400, 0, 0.85f);
        add_bg_tree(800, 0, 1.1f);
        add_bg_tree(1400, 0, 0.9f);
        add_bg_tree(1900, 0, 1.0f);
        add_bg_tree(2500, 0, 0.95f);
        add_bg_tree(3100, 0, 1.0f);
        add_bg_tree(3600, 0, 0.85f);
        add_bg_pillar(600, 0, 40, 180);
        add_bg_pillar(950, 0, 40, 230);
        add_bg_pillar(1800, 0, 40, 200);
        add_bg_pillar(2600, 0, 40, 210);
        add_bg_pillar(3400, 0, 40, 190);
        add_cloth(180, 220, 320, 200);
        add_cloth(1400, 200, 1600, 190);
        add_cloth(2900, 210, 3100, 200);
        /* Checkpoints */
        add_checkpoint(1200, 80);
        add_checkpoint(2500, 80);
        level.exit_x = 3680;
        level.exit_y = 80;
        break;
    }
    case 2:
    {
        /* ── LEVEL 2: Easy-medium — more guards and archers, wider platforms ── */
        level.level_w = 3800;
        level.level_h = 720;
        level.theme = 1; /* TEMPLE */
        level.bgm = 0;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 3800, 40, PLAT_SOLID);
        add_platform(280, 120, 180, 32, PLAT_SOLID);
        add_platform(540, 180, 180, 32, PLAT_SOLID);
        add_platform(800, 140, 200, 32, PLAT_SOLID);
        add_platform(1080, 220, 200, 32, PLAT_SOLID);
        add_platform(1360, 160, 200, 32, PLAT_SOLID);
        add_platform(1640, 260, 200, 32, PLAT_SOLID);
        add_platform(1920, 180, 220, 32, PLAT_SOLID);
        add_platform(2200, 240, 200, 32, PLAT_SOLID);
        add_platform(2480, 160, 200, 32, PLAT_SOLID);
        add_platform(2760, 200, 200, 32, PLAT_SOLID);
        add_platform(3040, 140, 200, 32, PLAT_SOLID);
        add_platform(3320, 200, 200, 32, PLAT_SOLID);
        add_moving_plat(1500, 200, 160, 32, 0, 60, 1.0f);
        /* 3 guards + 2 archers */
        add_enemy(ENEMY_GUARD, 700, 80, 500, 950);
        add_enemy(ENEMY_GUARD, 1250, 80, 1000, 1450);
        add_enemy(ENEMY_ARCHER, 1800, 80, 1650, 2050);
        add_enemy(ENEMY_GUARD, 2600, 80, 2400, 2850);
        add_enemy(ENEMY_ARCHER, 3150, 80, 2950, 3400);
        /* 3 spike clusters */
        add_trap_spike(480, 80, 3);
        add_trap_spike(1700, 80, 3);
        add_trap_spike(2900, 80, 3);
        /* Collectibles */
        add_gem(300, 160);
        add_gem(560, 220);
        add_gem(820, 180);
        add_gem(1100, 260);
        add_gem(1380, 200);
        add_gem(1660, 300);
        add_gem(1940, 220);
        add_gem(2220, 280);
        add_gem(2500, 200);
        add_gem(2780, 240);
        add_gem(3060, 180);
        add_star(1080, 300);
        add_star(1920, 260);
        add_star(3040, 220);
        /* Props */
        add_pot(380, 80);
        add_pot(750, 80);
        add_pot(1600, 80);
        add_pot(2350, 80);
        add_pot(3150, 80);
        /* Bg */
        add_bg_tree(160, 0, 1.0f);
        add_bg_tree(500, 0, 0.9f);
        add_bg_tree(1000, 0, 1.1f);
        add_bg_tree(1600, 0, 0.85f);
        add_bg_tree(2200, 0, 1.0f);
        add_bg_tree(2800, 0, 0.9f);
        add_bg_tree(3400, 0, 1.05f);
        add_bg_pillar(500, 0, 40, 180);
        add_bg_pillar(1200, 0, 40, 230);
        add_bg_pillar(2100, 0, 40, 200);
        add_bg_pillar(3000, 0, 40, 190);
        add_cloth(180, 220, 320, 200);
        add_cloth(1500, 210, 1700, 200);
        add_cloth(2700, 220, 2900, 210);
        add_checkpoint(1100, 80);
        add_checkpoint(2200, 80);
        add_checkpoint(3100, 80);
        level.exit_x = 3700;
        level.exit_y = 80;
        break;
    }
    case 3:
    {
        /* ── LEVEL 3: Medium — guards + archers, blade + spikes, crumbling platforms ── */
        level.level_w = 4400;
        level.level_h = 720;
        level.theme = 3; /* BAMBOO */
        level.bgm = 1;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4400, 40, PLAT_SOLID);
        add_platform(300, 130, 180, 32, PLAT_SOLID);
        add_platform(560, 200, 180, 32, PLAT_SOLID);
        add_platform(820, 150, 200, 32, PLAT_SOLID);
        add_platform(1100, 240, 200, 32, PLAT_SOLID);
        add_platform(1380, 160, 200, 32, PLAT_SOLID);
        add_platform(1660, 280, 200, 32, PLAT_SOLID);
        add_platform(1940, 180, 200, 32, PLAT_SOLID);
        add_platform(2220, 260, 220, 32, PLAT_SOLID);
        add_platform(2520, 180, 200, 32, PLAT_SOLID);
        add_platform(2800, 260, 220, 32, PLAT_SOLID);
        add_platform(3100, 170, 180, 32, PLAT_SOLID);
        add_platform(3380, 240, 200, 32, PLAT_SOLID);
        add_platform(3660, 160, 200, 32, PLAT_SOLID);
        add_platform(3940, 220, 200, 32, PLAT_SOLID);
        /* Crumbling platform introduction */
        add_platform(1240, 300, 120, 32, PLAT_CRUMBLING);
        add_moving_plat(1800, 220, 140, 32, 0, 70, 1.2f);
        /* 4 guards, 3 archers */
        add_enemy(ENEMY_GUARD, 420, 80, 200, 660);
        add_enemy(ENEMY_ARCHER, 900, 80, 700, 1100);
        add_enemy(ENEMY_GUARD, 1500, 80, 1300, 1750);
        add_enemy(ENEMY_ARCHER, 2100, 80, 1900, 2350);
        add_enemy(ENEMY_GUARD, 2900, 80, 2700, 3150);
        add_enemy(ENEMY_ARCHER, 3500, 80, 3300, 3750);
        add_enemy(ENEMY_GUARD, 4100, 80, 3900, 4300);
        /* Traps */
        add_trap_spike(600, 80, 3);
        add_trap_blade(1200, 80);
        add_trap_spike(2300, 80, 4);
        add_trap_fire(2900, 80, 2.0f, 2.5f);
        add_trap_spike(3600, 80, 3);
        /* Collectibles */
        add_gem(320, 170);
        add_gem(580, 240);
        add_gem(840, 190);
        add_gem(1120, 280);
        add_star(1400, 200);
        add_gem(1680, 320);
        add_gem(1960, 220);
        add_gem(2240, 300);
        add_star(2540, 220);
        add_gem(2820, 300);
        add_gem(3120, 210);
        add_gem(3400, 280);
        add_star(3660, 200);
        add_gem(3960, 260);
        add_pot(450, 80);
        add_pot(1150, 80);
        add_pot(1900, 80);
        add_pot(3000, 80);
        add_pot(3700, 80);
        /* Bg */
        add_bg_tree(200, 0, 1.0f);
        add_bg_tree(600, 0, 0.9f);
        add_bg_tree(1100, 0, 1.1f);
        add_bg_tree(1700, 0, 0.85f);
        add_bg_tree(2300, 0, 1.0f);
        add_bg_tree(2900, 0, 0.95f);
        add_bg_tree(3500, 0, 1.05f);
        add_bg_tree(4100, 0, 0.9f);
        add_bg_pillar(400, 0, 35, 200);
        add_bg_pillar(1000, 0, 35, 220);
        add_bg_pillar(2000, 0, 35, 210);
        add_bg_pillar(3200, 0, 35, 200);
        add_cloth(250, 210, 430, 200);
        add_cloth(1500, 220, 1700, 210);
        add_cloth(3100, 230, 3300, 220);
        add_checkpoint(1050, 80);
        add_checkpoint(2100, 80);
        add_checkpoint(3400, 80);
        level.exit_x = 4300;
        level.exit_y = 80;
        break;
    }
    case 4:
    {
        /* ── LEVEL 4: Medium — heavy enemy intro, blades + spikes, one-way platforms ── */
        level.level_w = 4200;
        level.level_h = 720;
        level.theme = 2; /* VILLAGE */
        level.bgm = 1;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4200, 40, PLAT_SOLID);
        add_platform(260, 130, 180, 32, PLAT_SOLID);
        add_platform(520, 210, 180, 32, PLAT_ONE_WAY);
        add_platform(780, 160, 200, 32, PLAT_SOLID);
        add_platform(1060, 260, 200, 32, PLAT_SOLID);
        add_platform(1340, 180, 200, 32, PLAT_SOLID);
        add_platform(1620, 300, 200, 32, PLAT_CRUMBLING);
        add_platform(1900, 200, 200, 32, PLAT_SOLID);
        add_platform(2180, 280, 220, 32, PLAT_SOLID);
        add_platform(2480, 180, 200, 32, PLAT_ONE_WAY);
        add_platform(2760, 260, 220, 32, PLAT_SOLID);
        add_platform(3020, 180, 180, 32, PLAT_SOLID);
        add_platform(3300, 260, 200, 32, PLAT_SOLID);
        add_platform(3580, 180, 200, 32, PLAT_SOLID);
        add_platform(3860, 240, 200, 32, PLAT_SOLID);
        add_moving_plat(1460, 240, 140, 32, 0, 80, 1.2f);
        add_moving_plat(2620, 220, 140, 32, 1, 70, 1.4f);
        /* 4 guards, 2 archers, 1 heavy */
        add_enemy(ENEMY_GUARD, 380, 80, 200, 600);
        add_enemy(ENEMY_GUARD, 820, 80, 620, 1040);
        add_enemy(ENEMY_ARCHER, 1200, 80, 1000, 1420);
        add_enemy(ENEMY_HEAVY, 1700, 80, 1520, 1920);
        add_enemy(ENEMY_GUARD, 2350, 80, 2150, 2600);
        add_enemy(ENEMY_ARCHER, 2900, 80, 2700, 3100);
        add_enemy(ENEMY_GUARD, 3450, 80, 3250, 3700);
        add_enemy(ENEMY_ARCHER, 4000, 80, 3800, 4150);
        /* 3 blades, 3 spikes, 1 fire */
        add_trap_spike(680, 80, 3);
        add_trap_blade(1400, 80);
        add_trap_spike(2250, 80, 4);
        add_trap_blade(2820, 80);
        add_trap_fire(3060, 80, 2.0f, 2.5f);
        add_trap_spike(3380, 80, 3);
        add_trap_blade(3900, 80);
        /* Collectibles */
        add_gem(280, 170);
        add_gem(540, 250);
        add_gem(800, 200);
        add_gem(1080, 300);
        add_star(1360, 220);
        add_gem(1640, 340);
        add_gem(1920, 240);
        add_gem(2200, 320);
        add_star(2500, 220);
        add_gem(2780, 300);
        add_star(3040, 220);
        add_gem(3320, 300);
        add_gem(3600, 220);
        add_gem(3880, 280);
        add_pot(400, 80);
        add_pot(1100, 80);
        add_pot(2000, 80);
        add_pot(3100, 80);
        add_pot(3900, 80);
        /* Bg */
        add_bg_tree(200, 0, 1.0f);
        add_bg_tree(700, 0, 0.9f);
        add_bg_tree(1300, 0, 1.1f);
        add_bg_tree(1900, 0, 0.85f);
        add_bg_tree(2500, 0, 1.0f);
        add_bg_tree(3100, 0, 0.9f);
        add_bg_tree(3700, 0, 1.05f);
        add_bg_pillar(500, 0, 40, 180);
        add_bg_pillar(1100, 0, 40, 210);
        add_bg_pillar(2000, 0, 40, 200);
        add_bg_pillar(3000, 0, 40, 190);
        add_cloth(300, 220, 500, 210);
        add_cloth(1600, 230, 1800, 220);
        add_cloth(3200, 220, 3400, 210);
        add_checkpoint(1100, 80);
        add_checkpoint(2200, 80);
        add_checkpoint(3400, 80);
        level.exit_x = 4100;
        level.exit_y = 80;
        break;
    }
    case 5:
    {
        /* ── LEVEL 5: Medium — fortress theme, guards + archers + heavies ── */
        level.level_w = 4800;
        level.level_h = 720;
        level.theme = 4; /* FORTRESS */
        level.bgm = 2;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4800, 40, PLAT_SOLID);
        add_platform(280, 130, 180, 32, PLAT_SOLID);
        add_platform(540, 210, 180, 32, PLAT_SOLID);
        add_platform(800, 160, 200, 32, PLAT_SOLID);
        add_platform(1080, 260, 200, 32, PLAT_SOLID);
        add_platform(1360, 180, 200, 32, PLAT_SOLID);
        add_platform(1640, 300, 200, 32, PLAT_CRUMBLING);
        add_platform(1920, 200, 200, 32, PLAT_SOLID);
        add_platform(2200, 280, 220, 32, PLAT_SOLID);
        add_platform(2480, 180, 200, 32, PLAT_SOLID);
        add_platform(2760, 300, 220, 32, PLAT_SOLID);
        add_platform(3040, 200, 200, 32, PLAT_SOLID);
        add_platform(3320, 280, 220, 32, PLAT_SOLID);
        add_platform(3580, 180, 200, 32, PLAT_SOLID);
        add_platform(3860, 260, 200, 32, PLAT_SOLID);
        add_platform(4140, 180, 200, 32, PLAT_SOLID);
        add_platform(4420, 260, 200, 32, PLAT_SOLID);
        add_moving_plat(1200, 240, 140, 32, 0, 90, 1.3f);
        add_moving_plat(2620, 260, 140, 32, 1, 70, 1.5f);
        add_moving_plat(4000, 240, 140, 32, 0, 100, 1.4f);
        /* 4 guards, 3 archers, 2 heavies */
        add_enemy(ENEMY_GUARD, 450, 80, 280, 660);
        add_enemy(ENEMY_ARCHER, 850, 80, 650, 1070);
        add_enemy(ENEMY_HEAVY, 1300, 80, 1100, 1560);
        add_enemy(ENEMY_GUARD, 2000, 80, 1800, 2250);
        add_enemy(ENEMY_ARCHER, 2600, 80, 2400, 2850);
        add_enemy(ENEMY_HEAVY, 3200, 80, 3000, 3460);
        add_enemy(ENEMY_GUARD, 3700, 80, 3500, 3950);
        add_enemy(ENEMY_ARCHER, 4200, 80, 4000, 4450);
        add_enemy(ENEMY_GUARD, 4650, 80, 4450, 4780);
        /* 3 blades, 3 spikes */
        add_trap_spike(650, 80, 3);
        add_trap_blade(1360, 80);
        add_trap_spike(2300, 80, 4);
        add_trap_blade(3100, 80);
        add_trap_spike(3750, 80, 3);
        add_trap_blade(4300, 80);
        /* Collectibles */
        add_gem(300, 170);
        add_gem(560, 250);
        add_gem(820, 200);
        add_gem(1100, 300);
        add_star(1380, 220);
        add_gem(1660, 340);
        add_gem(1940, 240);
        add_gem(2220, 320);
        add_star(2500, 220);
        add_gem(2780, 340);
        add_gem(3060, 240);
        add_star(3340, 320);
        add_gem(3600, 220);
        add_gem(3880, 300);
        add_gem(4160, 220);
        add_gem(4440, 300);
        add_pot(420, 80);
        add_pot(1200, 80);
        add_pot(2400, 80);
        add_pot(3200, 80);
        add_pot(4200, 80);
        /* Bg */
        add_bg_pillar(300, 0, 45, 200);
        add_bg_pillar(800, 0, 45, 220);
        add_bg_pillar(1500, 0, 45, 210);
        add_bg_pillar(2200, 0, 45, 200);
        add_bg_pillar(3000, 0, 45, 220);
        add_bg_pillar(3800, 0, 45, 200);
        add_bg_pillar(4500, 0, 45, 210);
        add_bg_tree(600, 0, 0.9f);
        add_bg_tree(1400, 0, 1.0f);
        add_bg_tree(2200, 0, 0.85f);
        add_bg_tree(3200, 0, 1.0f);
        add_bg_tree(4100, 0, 0.9f);
        add_cloth(400, 220, 600, 210);
        add_cloth(1600, 230, 1800, 220);
        add_cloth(3000, 230, 3200, 220);
        add_cloth(4200, 230, 4400, 220);
        add_checkpoint(1400, 80);
        add_checkpoint(2700, 80);
        add_checkpoint(3800, 80);
        level.exit_x = 4700;
        level.exit_y = 80;
        break;
    }
    case 6:
    {
        /* ── LEVEL 6: RUINS — second cycle, harder density ── */
        int theme6 = 0;
        level.level_w = 4800;
        level.level_h = 720;
        level.theme = theme6;
        level.bgm = theme6;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4800, 40, PLAT_SOLID);
        add_platform(260, 140, 160, 32, PLAT_SOLID);
        add_platform(500, 220, 160, 32, PLAT_SOLID);
        add_platform(760, 160, 180, 32, PLAT_SOLID);
        add_platform(1020, 260, 180, 32, PLAT_SOLID);
        add_platform(1300, 180, 160, 32, PLAT_CRUMBLING);
        add_platform(1540, 300, 160, 32, PLAT_SOLID);
        add_platform(1800, 200, 180, 32, PLAT_SOLID);
        add_platform(2060, 280, 180, 32, PLAT_SOLID);
        add_platform(2340, 180, 160, 32, PLAT_ONE_WAY);
        add_platform(2600, 300, 180, 32, PLAT_SOLID);
        add_platform(2880, 200, 160, 32, PLAT_CRUMBLING);
        add_platform(3140, 280, 180, 32, PLAT_SOLID);
        add_platform(3400, 180, 200, 32, PLAT_SOLID);
        add_platform(3680, 280, 180, 32, PLAT_SOLID);
        add_platform(3960, 180, 200, 32, PLAT_CRUMBLING);
        add_platform(4240, 280, 180, 32, PLAT_SOLID);
        add_moving_plat(1140, 240, 120, 32, 0, 110, 1.5f);
        add_moving_plat(2480, 260, 120, 32, 1, 90, 1.4f);
        add_moving_plat(3800, 240, 120, 32, 0, 100, 1.5f);
        add_enemy(ENEMY_GUARD, 400, 80, 200, 650);
        add_enemy(ENEMY_ARCHER, 850, 80, 650, 1050);
        add_enemy(ENEMY_HEAVY, 1400, 80, 1200, 1650);
        add_enemy(ENEMY_GUARD, 2100, 80, 1900, 2350);
        add_enemy(ENEMY_ARCHER, 2700, 80, 2500, 2950);
        add_enemy(ENEMY_HEAVY, 3250, 80, 3050, 3500);
        add_enemy(ENEMY_GUARD, 3800, 80, 3600, 4050);
        add_enemy(ENEMY_ARCHER, 4350, 80, 4150, 4600);
        add_trap_blade(620, 80);
        add_trap_spike(1050, 80, 5);
        add_trap_fire(1600, 80, 2.0f, 2.5f);
        add_trap_blade(2150, 80);
        add_trap_spike(2650, 80, 6);
        add_trap_blade(3300, 80);
        add_trap_spike(3900, 80, 5);
        add_trap_blade(4450, 80);
        add_trap_thorn(350, 40);
        add_trap_thorn(2400, 40);
        add_trap_thorn(4000, 40);
        add_gem(280, 180);
        add_gem(520, 260);
        add_gem(780, 200);
        add_gem(1040, 300);
        add_star(1320, 220);
        add_gem(1560, 340);
        add_gem(1820, 240);
        add_gem(2080, 320);
        add_star(2360, 220);
        add_gem(2620, 340);
        add_star(3160, 320);
        add_gem(3420, 220);
        add_gem(3700, 320);
        add_gem(3980, 220);
        add_gem(4260, 320);
        add_pot(450, 80);
        add_pot(1180, 80);
        add_pot(2200, 80);
        add_pot(3200, 80);
        add_pot(4100, 80);
        /* Bg */
        add_bg_tree(200, 0, 1.0f);
        add_bg_tree(700, 0, 0.9f);
        add_bg_tree(1300, 0, 1.1f);
        add_bg_tree(2000, 0, 0.85f);
        add_bg_tree(2700, 0, 1.0f);
        add_bg_tree(3400, 0, 0.95f);
        add_bg_tree(4100, 0, 1.05f);
        add_bg_pillar(500, 0, 40, 200);
        add_bg_pillar(1100, 0, 40, 220);
        add_bg_pillar(2200, 0, 40, 200);
        add_bg_pillar(3300, 0, 40, 210);
        add_bg_pillar(4300, 0, 40, 200);
        add_cloth(300, 230, 500, 220);
        add_cloth(1600, 240, 1800, 230);
        add_cloth(3000, 240, 3200, 230);
        add_cloth(4200, 240, 4400, 230);
        add_checkpoint(1300, 80);
        add_checkpoint(2600, 80);
        add_checkpoint(3800, 80);
        level.exit_x = 4700;
        level.exit_y = 80;
        break;
    }
    case 7:
    {
        /* ── LEVEL 7: TEMPLE — second cycle, tighter platforms ── */
        int theme7 = 1;
        level.level_w = 5200;
        level.level_h = 720;
        level.theme = theme7;
        level.bgm = theme7;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 5200, 40, PLAT_SOLID);
        add_platform(240, 160, 140, 32, PLAT_SOLID);
        add_platform(460, 250, 140, 32, PLAT_SOLID);
        add_platform(700, 170, 160, 32, PLAT_CRUMBLING);
        add_platform(940, 280, 160, 32, PLAT_SOLID);
        add_platform(1180, 190, 140, 32, PLAT_ONE_WAY);
        add_platform(1420, 310, 140, 32, PLAT_SOLID);
        add_platform(1660, 210, 160, 32, PLAT_CRUMBLING);
        add_platform(1900, 320, 160, 32, PLAT_SOLID);
        add_platform(2160, 200, 140, 32, PLAT_SOLID);
        add_platform(2400, 310, 140, 32, PLAT_SOLID);
        add_platform(2660, 200, 160, 32, PLAT_ONE_WAY);
        add_platform(2920, 300, 160, 32, PLAT_SOLID);
        add_platform(3180, 190, 160, 32, PLAT_CRUMBLING);
        add_platform(3440, 290, 180, 32, PLAT_SOLID);
        add_platform(3720, 180, 180, 32, PLAT_SOLID);
        add_platform(4000, 290, 160, 32, PLAT_SOLID);
        add_platform(4280, 180, 160, 32, PLAT_CRUMBLING);
        add_platform(4560, 290, 180, 32, PLAT_SOLID);
        add_platform(4820, 180, 180, 32, PLAT_SOLID);
        add_moving_plat(1060, 260, 110, 32, 0, 120, 1.6f);
        add_moving_plat(2280, 280, 110, 32, 1, 100, 1.5f);
        add_moving_plat(3560, 240, 100, 32, 0, 130, 1.8f);
        add_moving_plat(4700, 250, 100, 32, 1, 120, 1.7f);
        add_enemy(ENEMY_GUARD, 300, 80, 150, 520);
        add_enemy(ENEMY_ARCHER, 620, 80, 420, 840);
        add_enemy(ENEMY_GUARD, 980, 80, 780, 1200);
        add_enemy(ENEMY_HEAVY, 1400, 80, 1200, 1650);
        add_enemy(ENEMY_ARCHER, 2050, 80, 1850, 2300);
        add_enemy(ENEMY_GUARD, 2600, 80, 2400, 2850);
        add_enemy(ENEMY_HEAVY, 3150, 80, 2950, 3400);
        add_enemy(ENEMY_ARCHER, 3700, 80, 3500, 3950);
        add_enemy(ENEMY_GUARD, 4200, 80, 4000, 4450);
        add_enemy(ENEMY_HEAVY, 4750, 80, 4550, 4980);
        add_trap_blade(550, 80);
        add_trap_spike(980, 80, 5);
        add_trap_fire(1500, 80, 1.8f, 2.2f);
        add_trap_blade(2050, 80);
        add_trap_spike(2700, 80, 6);
        add_trap_fire(3200, 80, 2.0f, 2.0f);
        add_trap_blade(3750, 80);
        add_trap_spike(4350, 80, 5);
        add_trap_blade(4900, 80);
        add_trap_thorn(320, 40);
        add_trap_thorn(2150, 40);
        add_trap_thorn(4100, 40);
        add_gem(260, 200);
        add_gem(480, 290);
        add_gem(720, 210);
        add_star(960, 320);
        add_gem(1200, 230);
        add_gem(1440, 350);
        add_gem(1680, 250);
        add_star(1920, 360);
        add_gem(2180, 240);
        add_gem(2420, 350);
        add_star(2680, 240);
        add_gem(2940, 340);
        add_gem(3200, 230);
        add_gem(3460, 330);
        add_gem(3740, 220);
        add_gem(4020, 330);
        add_gem(4300, 220);
        add_gem(4580, 330);
        add_pot(420, 80);
        add_pot(1100, 80);
        add_pot(2200, 80);
        add_pot(3300, 80);
        add_pot(4400, 80);
        /* Bg */
        add_bg_pillar(300, 0, 40, 210);
        add_bg_pillar(800, 0, 40, 230);
        add_bg_pillar(1500, 0, 40, 200);
        add_bg_pillar(2300, 0, 40, 220);
        add_bg_pillar(3100, 0, 40, 210);
        add_bg_pillar(3900, 0, 40, 200);
        add_bg_pillar(4700, 0, 40, 220);
        add_bg_tree(600, 0, 0.9f);
        add_bg_tree(1300, 0, 1.0f);
        add_bg_tree(2100, 0, 0.85f);
        add_bg_tree(3000, 0, 1.0f);
        add_bg_tree(4000, 0, 0.9f);
        add_cloth(350, 230, 550, 220);
        add_cloth(1500, 240, 1700, 230);
        add_cloth(2900, 240, 3100, 230);
        add_cloth(4400, 240, 4600, 230);
        add_checkpoint(1200, 80);
        add_checkpoint(2600, 80);
        add_checkpoint(3500, 80);
        add_checkpoint(4600, 80);
        level.exit_x = 5100;
        level.exit_y = 80;
        break;
    }
    case 8:
    {
        /* ── LEVEL 8: BAMBOO — second cycle, elevated platforms ── */
        int theme8 = 3;
        level.level_w = 5200;
        level.level_h = 720;
        level.theme = theme8;
        level.bgm = theme8;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 5200, 40, PLAT_SOLID);
        add_platform(250, 170, 130, 32, PLAT_SOLID);
        add_platform(480, 270, 130, 32, PLAT_ONE_WAY);
        add_platform(720, 190, 150, 32, PLAT_SOLID);
        add_platform(960, 300, 130, 32, PLAT_CRUMBLING);
        add_platform(1200, 200, 130, 32, PLAT_SOLID);
        add_platform(1440, 320, 130, 32, PLAT_SOLID);
        add_platform(1680, 220, 150, 32, PLAT_CRUMBLING);
        add_platform(1920, 340, 130, 32, PLAT_SOLID);
        add_platform(2180, 220, 130, 32, PLAT_SOLID);
        add_platform(2420, 340, 150, 32, PLAT_ONE_WAY);
        add_platform(2680, 220, 130, 32, PLAT_SOLID);
        add_platform(2940, 340, 150, 32, PLAT_CRUMBLING);
        add_platform(3200, 220, 130, 32, PLAT_SOLID);
        add_platform(3460, 340, 150, 32, PLAT_SOLID);
        add_platform(3720, 200, 180, 32, PLAT_SOLID);
        add_platform(4000, 320, 150, 32, PLAT_SOLID);
        add_platform(4260, 200, 150, 32, PLAT_CRUMBLING);
        add_platform(4520, 320, 150, 32, PLAT_ONE_WAY);
        add_platform(4780, 200, 180, 32, PLAT_SOLID);
        add_moving_plat(1080, 270, 100, 32, 0, 130, 1.7f);
        add_moving_plat(2300, 290, 100, 32, 1, 110, 1.6f);
        add_moving_plat(3340, 270, 100, 32, 0, 140, 1.9f);
        add_moving_plat(4400, 280, 100, 32, 1, 120, 1.8f);
        add_enemy(ENEMY_ARCHER, 280, 80, 100, 520);
        add_enemy(ENEMY_GUARD, 600, 80, 400, 840);
        add_enemy(ENEMY_HEAVY, 980, 80, 780, 1240);
        add_enemy(ENEMY_ARCHER, 1560, 80, 1360, 1820);
        add_enemy(ENEMY_GUARD, 2100, 80, 1900, 2360);
        add_enemy(ENEMY_HEAVY, 2660, 80, 2460, 2920);
        add_enemy(ENEMY_ARCHER, 3200, 80, 3000, 3460);
        add_enemy(ENEMY_GUARD, 3750, 80, 3550, 4000);
        add_enemy(ENEMY_HEAVY, 4300, 80, 4100, 4560);
        add_enemy(ENEMY_ARCHER, 4850, 80, 4650, 5000);
        add_trap_spike(500, 80, 5);
        add_trap_blade(750, 80);
        add_trap_fire(1350, 80, 1.5f, 1.8f);
        add_trap_spike(2000, 80, 6);
        add_trap_blade(2450, 80);
        add_trap_fire(3050, 80, 1.8f, 1.8f);
        add_trap_blade(3500, 80);
        add_trap_spike(4100, 80, 6);
        add_trap_fire(4700, 80, 1.5f, 1.8f);
        add_trap_thorn(300, 40);
        add_trap_thorn(2100, 40);
        add_trap_thorn(4000, 40);
        add_gem(270, 210);
        add_gem(500, 310);
        add_gem(740, 230);
        add_star(980, 340);
        add_gem(1220, 240);
        add_gem(1460, 360);
        add_star(1700, 260);
        add_gem(1940, 380);
        add_gem(2200, 260);
        add_star(2440, 380);
        add_gem(2700, 260);
        add_gem(2960, 380);
        add_gem(3220, 260);
        add_gem(3480, 380);
        add_gem(3740, 240);
        add_gem(4020, 360);
        add_gem(4280, 240);
        add_gem(4540, 360);
        add_pot(400, 80);
        add_pot(1100, 80);
        add_pot(2200, 80);
        add_pot(3300, 80);
        add_pot(4400, 80);
        /* Bg */
        add_bg_tree(200, 0, 1.0f);
        add_bg_tree(700, 0, 0.9f);
        add_bg_tree(1300, 0, 1.1f);
        add_bg_tree(2000, 0, 0.85f);
        add_bg_tree(2700, 0, 1.0f);
        add_bg_tree(3400, 0, 0.95f);
        add_bg_tree(4100, 0, 1.05f);
        add_bg_tree(4800, 0, 0.9f);
        add_bg_pillar(500, 0, 35, 200);
        add_bg_pillar(1200, 0, 35, 220);
        add_bg_pillar(2100, 0, 35, 200);
        add_bg_pillar(3100, 0, 35, 210);
        add_bg_pillar(4200, 0, 35, 200);
        add_cloth(300, 220, 480, 210);
        add_cloth(1500, 230, 1700, 220);
        add_cloth(3000, 230, 3200, 220);
        add_cloth(4500, 230, 4700, 220);
        add_checkpoint(1200, 80);
        add_checkpoint(2600, 80);
        add_checkpoint(3600, 80);
        add_checkpoint(4600, 80);
        level.exit_x = 5100;
        level.exit_y = 80;
        break;
    }
    case 9:
    {
        /* ── LEVEL 9: VILLAGE — second cycle, narrow dangerous platforms ── */
        int theme9 = 2;
        level.level_w = 5200;
        level.level_h = 720;
        level.theme = theme9;
        level.bgm = theme9;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 5200, 40, PLAT_SOLID);
        add_platform(240, 180, 120, 32, PLAT_SOLID);
        add_platform(460, 290, 110, 32, PLAT_CRUMBLING);
        add_platform(690, 200, 120, 32, PLAT_SOLID);
        add_platform(920, 320, 110, 32, PLAT_ONE_WAY);
        add_platform(1150, 210, 120, 32, PLAT_CRUMBLING);
        add_platform(1380, 340, 110, 32, PLAT_SOLID);
        add_platform(1610, 230, 120, 32, PLAT_SOLID);
        add_platform(1840, 360, 120, 32, PLAT_CRUMBLING);
        add_platform(2080, 240, 110, 32, PLAT_SOLID);
        add_platform(2310, 360, 120, 32, PLAT_ONE_WAY);
        add_platform(2550, 240, 110, 32, PLAT_SOLID);
        add_platform(2790, 360, 120, 32, PLAT_CRUMBLING);
        add_platform(3030, 240, 120, 32, PLAT_SOLID);
        add_platform(3270, 360, 120, 32, PLAT_SOLID);
        add_platform(3520, 240, 120, 32, PLAT_ONE_WAY);
        add_platform(3760, 180, 180, 32, PLAT_SOLID);
        add_platform(4040, 300, 120, 32, PLAT_SOLID);
        add_platform(4280, 200, 120, 32, PLAT_CRUMBLING);
        add_platform(4520, 320, 120, 32, PLAT_ONE_WAY);
        add_platform(4760, 200, 180, 32, PLAT_SOLID);
        add_moving_plat(800, 280, 100, 32, 0, 140, 1.8f);
        add_moving_plat(2000, 310, 100, 32, 1, 120, 1.7f);
        add_moving_plat(3400, 300, 90, 32, 0, 150, 2.0f);
        add_moving_plat(4650, 270, 90, 32, 1, 130, 1.9f);
        add_enemy(ENEMY_HEAVY, 350, 80, 150, 560);
        add_enemy(ENEMY_ARCHER, 760, 80, 560, 980);
        add_enemy(ENEMY_GUARD, 1220, 80, 1020, 1450);
        add_enemy(ENEMY_HEAVY, 1850, 80, 1650, 2100);
        add_enemy(ENEMY_ARCHER, 2400, 80, 2200, 2650);
        add_enemy(ENEMY_GUARD, 2930, 80, 2730, 3170);
        add_enemy(ENEMY_HEAVY, 3420, 80, 3220, 3680);
        add_enemy(ENEMY_ARCHER, 4000, 80, 3800, 4250);
        add_enemy(ENEMY_GUARD, 4550, 80, 4350, 4800);
        add_trap_fire(500, 80, 1.5f, 1.5f);
        add_trap_blade(720, 80);
        add_trap_spike(1200, 80, 6);
        add_trap_fire(1700, 80, 1.2f, 1.5f);
        add_trap_blade(2200, 80);
        add_trap_spike(2800, 80, 6);
        add_trap_fire(3300, 80, 1.5f, 1.5f);
        add_trap_blade(3900, 80);
        add_trap_spike(4500, 80, 6);
        add_trap_thorn(280, 40);
        add_trap_thorn(2000, 40);
        add_trap_thorn(3500, 40);
        add_trap_thorn(4800, 40);
        add_gem(260, 220);
        add_gem(480, 330);
        add_gem(710, 240);
        add_star(940, 360);
        add_gem(1170, 250);
        add_gem(1400, 380);
        add_star(1630, 270);
        add_gem(1860, 400);
        add_gem(2100, 280);
        add_star(2330, 400);
        add_gem(2570, 280);
        add_gem(2810, 400);
        add_gem(3050, 280);
        add_gem(3290, 400);
        add_gem(3540, 280);
        add_gem(3780, 220);
        add_gem(4060, 340);
        add_gem(4300, 240);
        add_gem(4540, 360);
        add_pot(380, 80);
        add_pot(1100, 80);
        add_pot(2300, 80);
        add_pot(3400, 80);
        add_pot(4500, 80);
        /* Bg */
        add_bg_tree(200, 0, 1.0f);
        add_bg_tree(700, 0, 0.9f);
        add_bg_tree(1300, 0, 1.05f);
        add_bg_tree(2000, 0, 0.85f);
        add_bg_tree(2700, 0, 1.0f);
        add_bg_tree(3400, 0, 0.95f);
        add_bg_tree(4100, 0, 1.0f);
        add_bg_tree(4800, 0, 0.9f);
        add_bg_pillar(500, 0, 40, 190);
        add_bg_pillar(1200, 0, 40, 210);
        add_bg_pillar(2200, 0, 40, 200);
        add_bg_pillar(3300, 0, 40, 210);
        add_bg_pillar(4400, 0, 40, 200);
        add_cloth(300, 230, 500, 220);
        add_cloth(1500, 240, 1700, 230);
        add_cloth(3000, 240, 3200, 230);
        add_cloth(4500, 240, 4700, 230);
        add_checkpoint(1150, 80);
        add_checkpoint(2550, 80);
        add_checkpoint(3600, 80);
        add_checkpoint(4700, 80);
        level.exit_x = 5100;
        level.exit_y = 80;
        break;
    }
    case 10:
    {
        /* ── LEVEL 10: FORTRESS CLIMAX — gauntlet, 3 heavies, dense traps ── */
        level.level_w = 5200;
        level.level_h = 720;
        level.theme = 4; /* FORTRESS */
        level.bgm = 4;
        player.x = 120;
        player.y = 80;

        /* Ground */
        add_platform(0, 40, 5200, 40, PLAT_SOLID);

        /* Fortress battlements — narrow tops */
        add_platform(200, 130, 100, 32, PLAT_SOLID);
        add_platform(380, 220, 80, 32, PLAT_SOLID);
        add_platform(540, 330, 80, 32, PLAT_CRUMBLING);
        add_platform(700, 200, 100, 32, PLAT_SOLID);
        add_platform(880, 330, 80, 32, PLAT_ONE_WAY);
        add_platform(1040, 220, 100, 32, PLAT_SOLID);
        add_platform(1220, 360, 80, 32, PLAT_CRUMBLING);
        add_platform(1400, 250, 100, 32, PLAT_SOLID);
        add_platform(1580, 380, 80, 32, PLAT_SOLID);
        add_platform(1760, 270, 100, 32, PLAT_CRUMBLING);
        add_platform(1940, 400, 80, 32, PLAT_ONE_WAY);
        add_platform(2120, 280, 100, 32, PLAT_SOLID);
        add_platform(2300, 400, 80, 32, PLAT_CRUMBLING);
        add_platform(2480, 280, 100, 32, PLAT_SOLID);
        add_platform(2660, 400, 80, 32, PLAT_ONE_WAY);
        add_platform(2840, 280, 100, 32, PLAT_SOLID);
        add_platform(3020, 400, 80, 32, PLAT_CRUMBLING);
        add_platform(3200, 270, 100, 32, PLAT_SOLID);
        add_platform(3380, 380, 80, 32, PLAT_ONE_WAY);
        add_platform(3560, 270, 100, 32, PLAT_SOLID);
        add_platform(3740, 380, 80, 32, PLAT_CRUMBLING);
        add_platform(3920, 270, 100, 32, PLAT_SOLID);
        add_platform(4100, 380, 80, 32, PLAT_ONE_WAY);
        /* Final fortress wall */
        add_platform(4300, 200, 360, 32, PLAT_SOLID);
        add_platform(4300, 350, 100, 32, PLAT_SOLID);
        add_platform(4520, 350, 100, 32, PLAT_SOLID);
        add_platform(4760, 200, 100, 32, PLAT_SOLID);

        /* Deadly moving platforms */
        add_moving_plat(960, 300, 90, 32, 0, 140, 2.0f);
        add_moving_plat(2000, 360, 90, 32, 1, 130, 1.9f);
        add_moving_plat(3100, 340, 90, 32, 0, 150, 2.2f);
        add_moving_plat(4200, 320, 90, 32, 1, 140, 2.1f);

        /* Enemy gauntlet */
        add_enemy(ENEMY_GUARD, 450, 80, 250, 700);
        add_enemy(ENEMY_ARCHER, 800, 80, 600, 1050);
        add_enemy(ENEMY_HEAVY, 1200, 80, 1000, 1450);
        add_enemy(ENEMY_GUARD, 1800, 80, 1600, 2050);
        add_enemy(ENEMY_ARCHER, 2350, 80, 2150, 2600);
        add_enemy(ENEMY_HEAVY, 2800, 80, 2600, 3050);
        add_enemy(ENEMY_GUARD, 3200, 80, 3000, 3450);
        add_enemy(ENEMY_ARCHER, 3700, 80, 3500, 3950);
        add_enemy(ENEMY_HEAVY, 4380, 232, 4300, 4500);
        add_enemy(ENEMY_HEAVY, 4540, 232, 4400, 4660);
        add_enemy(ENEMY_ARCHER, 4700, 232, 4300, 4900);

        /* Dense trap gauntlet */
        add_trap_blade(500, 80);
        add_trap_spike(900, 80, 6);
        add_trap_fire(1300, 80, 1.5f, 1.5f);
        add_trap_blade(1650, 80);
        add_trap_spike(2100, 80, 7);
        add_trap_fire(2550, 80, 1.2f, 1.2f);
        add_trap_blade(2950, 80);
        add_trap_spike(3300, 80, 6);
        add_trap_fire(3480, 80, 1.5f, 1.0f);
        add_trap_blade(3850, 80);
        add_trap_spike(4150, 80, 6);
        add_trap_thorn(350, 40);
        add_trap_thorn(1500, 40);
        add_trap_thorn(2700, 40);
        add_trap_thorn(3650, 232);
        add_trap_thorn(4450, 232);

        /* Stars and gems */
        add_gem(220, 170);
        add_gem(400, 260);
        add_gem(560, 370);
        add_star(720, 240);
        add_gem(900, 370);
        add_gem(1060, 260);
        add_gem(1240, 400);
        add_star(1420, 290);
        add_gem(1600, 420);
        add_gem(1780, 310);
        add_gem(1960, 440);
        add_star(2140, 320);
        add_gem(2320, 440);
        add_gem(2500, 320);
        add_gem(2680, 440);
        add_gem(2860, 320);
        add_gem(3040, 440);
        add_gem(3220, 310);
        add_gem(3400, 420);
        add_gem(3580, 310);
        add_gem(3760, 420);

        /* Bg */
        add_bg_pillar(300, 0, 50, 210);
        add_bg_pillar(800, 0, 50, 230);
        add_bg_pillar(1500, 0, 50, 200);
        add_bg_pillar(2200, 0, 50, 220);
        add_bg_pillar(3000, 0, 50, 210);
        add_bg_pillar(3800, 0, 50, 220);
        add_bg_pillar(4600, 0, 50, 200);
        add_bg_tree(600, 0, 0.8f);
        add_bg_tree(1800, 0, 0.9f);
        add_bg_tree(3300, 0, 0.85f);
        add_cloth(400, 240, 600, 230);
        add_cloth(1600, 250, 1800, 240);
        add_cloth(3100, 250, 3300, 240);
        add_cloth(4300, 250, 4500, 240);

        add_checkpoint(2120, 80);
        add_checkpoint(3800, 80);

        level.exit_x = 5100;
        level.exit_y = 80;
        break;
    }
    default:
    {
        /* Fallback for any level beyond 10 — cycles back through themes */
        int theme = ((num - 1) % 5);
        level.level_w = 4096;
        level.level_h = 720;
        level.theme = theme;
        level.bgm = theme;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4096, 40, PLAT_SOLID);
        int px2 = 200;
        while (px2 < 3900)
        {
            float gapw = 180 + ((px2 * 7) % 120);
            float plh2 = 80 + ((px2 * 13) % 200);
            float plw2 = 120 + ((px2 * 11) % 80);
            add_platform(px2 + gapw, 40 + (int)plh2, plw2, 32, PLAT_SOLID);
            px2 += (int)gapw + (int)plw2;
        }
        add_moving_plat(1200, 240, 128, 32, 0, 100, 1.5f);
        add_moving_plat(2400, 280, 128, 32, 1, 80, 1.3f);
        add_enemy(ENEMY_GUARD, 400, 80, 200, 700);
        add_enemy(ENEMY_ARCHER, 900, 80, 700, 1100);
        add_enemy(ENEMY_HEAVY, 1500, 80, 1300, 1800);
        add_enemy(ENEMY_GUARD, 2200, 80, 2000, 2500);
        add_enemy(ENEMY_ARCHER, 2800, 80, 2600, 3100);
        add_enemy(ENEMY_HEAVY, 3400, 80, 3200, 3700);
        add_trap_blade(700, 80);
        add_trap_blade(1800, 80);
        add_trap_blade(2900, 80);
        add_trap_spike(1100, 80, 5);
        add_trap_spike(2400, 80, 6);
        add_trap_spike(3500, 80, 5);
        add_trap_fire(500, 80, 2.0f, 2.5f);
        add_trap_fire(1600, 80, 1.8f, 2.2f);
        add_trap_fire(3000, 80, 2.0f, 2.0f);
        add_trap_thorn(350, 40);
        add_trap_thorn(2200, 40);
        add_gem(300, 100);
        add_gem(600, 100);
        add_gem(1000, 100);
        add_star(1400, 100);
        add_star(2600, 100);
        add_star(3600, 100);
        add_pot(450, 80);
        add_pot(1200, 80);
        add_pot(2500, 80);
        add_pot(3300, 80);
        level.exit_x = 4000;
        level.exit_y = 80;
        break;
    }
    }

    /* Start BGM for this level's theme (theme 0–4 maps directly to BGM_FOREST–FORTRESS) */
    audio_play_bgm(level.theme);
}

/* ── Enemy-hit warp: teleport to checkpoint (or start) without full-health restore ── */
static void do_enemy_hit_respawn(void)
{
    if (level.respawn_active)
    {
        player.x = level.respawn_x;
        player.y = level.respawn_y + 10;
    }
    else
    {
        /* No checkpoint reached yet — return to level start */
        player.x = 120;
        player.y = 80;
    }
    player.vx = 0;
    player.vy = 0;
    player.hurt_timer = 1.5f;
    player.anim.state = ANIM_HURT;
    player.anim.timer = 0;
    player.jump_pressed = 0;
    player.jump_available = 1;
    player.jump_count = 0;
    /* Clear stray projectiles so arrows don't re-hit on arrival */
    for (int i = 0; i < level.proj_count; i++)
        level.projs[i].active = 0;
    level.proj_count = 0;
    /* Reset crumbled platforms so no route is permanently blocked */
    for (int i = 0; i < level.plat_count; i++)
    {
        if (level.platforms[i].type == PLAT_CRUMBLING)
        {
            level.platforms[i].crumbled = 0;
            level.platforms[i].crumble_timer = 0;
        }
    }
}

/* ── Respawn / restart ──────────────────────────────────── */

/* ── Checkpoint update ──────────────────────────────────── */
void update_checkpoints(void)
{
    for (int i = 0; i < level.checkpoint_count; i++)
    {
        Checkpoint *ck = &level.checkpoints[i];
        if (ck->triggered)
            continue;
        if (fabsf(player.x - ck->x) < 40 && player.y < ck->y + 80 && player.y + PLAYER_H > ck->y)
        {
            ck->triggered = 1;
            level.respawn_x = ck->x;
            level.respawn_y = ck->y;
            level.respawn_active = 1;
            respawn_health = MAX_HEALTH; /* Reset penalty — player earned this checkpoint */
        }
    }
}

/* ── Main update ────────────────────────────────────────── */
void update_game(float dt)
{
    if (game_state != STATE_PLAYING)
        return;

    level_time += dt;
    if (penalty_toast_timer > 0) penalty_toast_timer -= dt;

    /* I-03: Combo timer decay */
    if (combo_timer > 0)
    {
        combo_timer -= dt;
        if (combo_timer <= 0)
        {
            combo_timer = 0;
            combo_count = 0;
        }
    }

    /* animation_update runs first so state transitions inside player_input get a clean timer=0 */
    animation_update(&player.anim, dt);
    player_input(&player, dt);
    player_physics(&player, dt);
    update_moving_platforms(dt);
    resolve_platforms(&player);
    update_enemies(dt);
    update_projectiles(dt);
    update_traps(dt);
    update_collectibles(dt);
    update_pots(dt);
    update_checkpoints();
    particles_update(dt);
    camera_update(&camera, player.x, player.y, level.level_w, dt);
    check_exit();

    /* Trap contact: -1 health; game over when health hits 0 */
    if (respawn_requested)
    {
        respawn_requested = 0;
        player.health--;
        if (player.health <= 0)
        {
            player.anim.state = ANIM_DEAD;
            game_state = STATE_GAME_OVER;
            audio_stop_bgm();
            audio_play(SFX_PLAYER_DEATH);
        }
        else
        {
            do_enemy_hit_respawn(); /* Teleport to checkpoint, keep current health */
            audio_play(SFX_PLAYER_HURT);
        }
        return;
    }

    /* Enemy/arrow hit: -1 health; game over when health hits 0 */
    if (enemy_hit_respawn)
    {
        enemy_hit_respawn = 0;
        player.health--;
        if (player.health <= 0)
        {
            player.anim.state = ANIM_DEAD;
            game_state = STATE_GAME_OVER;
            audio_stop_bgm();
            audio_play(SFX_PLAYER_DEATH);
        }
        else
        {
            do_enemy_hit_respawn(); /* Teleport to checkpoint, keep current health */
            audio_play(SFX_PLAYER_HURT);
        }
        return;
    }

    /* Death from fall or other sources */
    if (player.health <= 0 && player.anim.state != ANIM_DEAD)
    {
        player.anim.state = ANIM_DEAD;
        game_state = STATE_GAME_OVER;
        audio_stop_bgm();
        audio_play(SFX_PLAYER_DEATH);
    }
}

/* ── Key callbacks ──────────────────────────────────────── */
void key_down(unsigned char k, int x, int y)
{
    (void)x;
    (void)y;
    keys[(int)k] = 1;

    if (game_state == STATE_MENU)
    {
        if (k == '\r' || k == '\n')
        {
            audio_play(SFX_MENU_CONFIRM);
            game_state = STATE_PLAYING;
            load_level(1);
        }
        return;
    }
    if (game_state == STATE_GAME_OVER)
    {
        if (k == '\r' || k == '\n')
        {
            audio_play(SFX_MENU_CONFIRM);
            respawn_health = MAX_HEALTH;
            game_state = STATE_PLAYING;
            load_level(current_level);
        }
        return;
    }
    if (game_state == STATE_LEVEL_COMPLETE)
    {
        if (k == '\r' || k == '\n')
        {
            audio_play(SFX_MENU_CONFIRM);
            current_level++;
            if (current_level > 10)
                current_level = 1;
            respawn_health = MAX_HEALTH; /* Fresh level — reset health penalty */
            game_state = STATE_PLAYING;
            load_level(current_level);
        }
        return;
    }

    if (k == 'p' || k == 'P' || k == 27)
    {
        if (game_state == STATE_PLAYING)
            game_state = STATE_PAUSED;
        else if (game_state == STATE_PAUSED)
            game_state = STATE_PLAYING;
        else if (game_state == STATE_SETTINGS)
            game_state = STATE_PLAYING; /* Esc closes settings */
    }

    /* Tab toggles settings overlay from any in-game state */
    if (k == '\t')
    {
        if (game_state == STATE_PLAYING || game_state == STATE_PAUSED)
            game_state = STATE_SETTINGS;
        else if (game_state == STATE_SETTINGS)
            game_state = STATE_PLAYING;
        return;
    }

    /* Jump input - just track that jump button is pressed */
    if ((k == ' ' || k == 'w' || k == 'W') && game_state == STATE_PLAYING)
    {
        player.jump_pressed = 1;
    }

    /* Shuriken throw (X or K) - just track button press (will throw in update) */
    if ((k == 'x' || k == 'X' || k == 'k' || k == 'K') && game_state == STATE_PLAYING)
    {
        player.shuriken_pressed = 1;
    }

    /* Dash — '\r' (Enter) is also bound here. Menu states above return early before reaching
       this branch, so there is no collision between menu-confirm and dash (B2-15). */
    if ((k == 'e' || k == 'E' || k == 'l' || k == 'L' || k == '\r') && game_state == STATE_PLAYING)
    {
        player.dash_pressed = 1;
    }
}

void key_up(unsigned char k, int x, int y)
{
    (void)x;
    (void)y;
    keys[(int)k] = 0;

    /* Clear jump button state */
    if (k == ' ' || k == 'w' || k == 'W')
    {
        player.jump_pressed = 0;
    }

    /* Clear shuriken button state */
    if (k == 'x' || k == 'X' || k == 'k' || k == 'K')
    {
        player.shuriken_pressed = 0;
    }

    /* Clear dash pressed flag on key release (B-09) */
    if (k == 'e' || k == 'E' || k == 'l' || k == 'L' || k == '\r')
    {
        player.dash_pressed = 0;
    }
}

void special_down(int k, int x, int y)
{
    (void)x;
    (void)y;
    if (k < 256)
        special_keys[k] = 1;

    /* Up arrow jump input */
    if (k == GLUT_KEY_UP && game_state == STATE_PLAYING)
    {
        player.jump_pressed = 1;
    }
}

void special_up(int k, int x, int y)
{
    (void)x;
    (void)y;
    if (k < 256)
        special_keys[k] = 0;

    /* Clear jump input when up arrow released */
    if (k == GLUT_KEY_UP)
    {
        player.jump_pressed = 0;
    }
}

void game_init(void)
{
    memset(keys, 0, sizeof(keys));
    memset(special_keys, 0, sizeof(special_keys));
    memset(particles, 0, sizeof(particles));
    game_state = STATE_MENU;
}
