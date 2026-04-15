#include "game.h"
#include "renderer.h"
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
void draw_exit_portal(float cx, float cy, float t);

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
        }
        else if (player.on_wall != 0)
        {
            /* Wall jump — consume both jump charges to prevent immediate aerial double-jump */
            player.vy = JUMP_VY * 0.92f;
            player.vx = -player.on_wall * MOVE_SPEED * 1.5f;
            player.anim.facing = -player.on_wall;
            player.jump_count = 2;
            player.anim.state = ANIM_JUMP;
            player.jump_available = 0;
            jump_triggered = 1;
        }
        else if (player.jump_count == 1)
        {
            /* Double jump */
            player.vy = DBL_JUMP_VY;
            player.jump_count = 2;
            player.anim.state = ANIM_JUMP;
            player.jump_available = 0;
            jump_triggered = 1;
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

    /* Shuriken throw — one-shot per key press, infinite range, straight line */
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
    }

    /* Attack */
    if (atk && p->anim.state != ANIM_ATTACK && p->attack_timer <= 0)
    {
        p->anim.state = ANIM_ATTACK;
        p->anim.timer = 0;
        p->attack_timer = 0.3f;
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

/* ── Enemy update ───────────────────────────────────────── */
void update_enemies(float dt)
{
    for (int i = 0; i < level.enemy_count; i++)
    {
        Enemy *e = &level.enemies[i];
        if (!e->alive)
            continue;

        e->anim_t += dt;
        e->hurt_flash -= dt;
        if (e->hurt_flash < 0)
            e->hurt_flash = 0;

        /* Patrol */
        if (e->x <= e->patrol_l)
            e->facing = 1;
        if (e->x >= e->patrol_r)
            e->facing = -1;
        float spd = (e->type == ENEMY_HEAVY) ? 80.0f : 100.0f;
        e->x += e->facing * spd * dt;

        /* Archer: shoot at player */
        if (e->type == ENEMY_ARCHER)
        {
            e->shoot_timer -= dt;
            float dx = player.x - e->x;
            if (e->shoot_timer <= 0 && fabsf(dx) < 450)
            {
                e->shoot_timer = 3.0f;
                /* Spawn arrow projectile */
                if (level.proj_count < MAX_PROJECTILES)
                {
                    Projectile *pr = &level.projs[level.proj_count++];
                    pr->x = e->x;
                    pr->y = e->y + 40;
                    pr->vx = (dx > 0 ? 1 : -1) * 280.0f;
                    pr->vy = 20;
                    pr->active = 1;
                    pr->is_shuriken = 0;
                    pr->from_player = 0;
                }
            }
        }

        /* Check if player attack hits */
        if (player.anim.state == ANIM_ATTACK && player.attack_timer > 0.15f)
        {
            float reach = 55.0f;
            float ax = player.x + player.anim.facing * 20;
            if (fabsf(ax - e->x) < reach && fabsf(player.y - e->y) < 70)
            {
                int dmg = (e->type == ENEMY_HEAVY) ? 1 : 2;
                e->health -= dmg;
                e->hurt_flash = 0.2f;
                spawn_hit_sparks(e->x, e->y + 40);
                if (e->health <= 0)
                {
                    e->alive = 0;
                    spawn_hit_sparks(e->x, e->y + 40);
                    spawn_hit_sparks(e->x + 10, e->y + 30);
                }
            }
        }

        /* Enemy attacks player */
        if (e->type != ENEMY_ARCHER)
        {
            float dx = player.x - e->x;
            float dy = (player.y + 32) - (e->y + 32);
            if (fabsf(dx) < 40 && fabsf(dy) < 60 && player.hurt_timer <= 0)
            {
                player.health--;
                player.hurt_timer = 1.0f;
                /* B-18: apply upward vy when hit mid-air so knockback is always readable */
                if (!player.on_ground)
                    player.vy = 200.0f;
                else
                    player.vy = 0;
                player.vx = -e->facing * 220.0f;
                player.anim.state = ANIM_HURT;
                player.anim.timer = 0;
                /* B-18: spawn hit sparks at player so every hit has visible feedback */
                spawn_hit_sparks(player.x, player.y + 32);
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
                    if (e->health <= 0)
                    {
                        e->alive = 0;
                        spawn_hit_sparks(e->x, e->y + 32);
                    }
                    pr->active = 0;
                }
            }
        }

        /* Arrow hits player */
        if (!pr->is_shuriken && !pr->from_player)
        {
            if (fabsf(pr->x - player.x) < 20 && fabsf(pr->y - player.y - 32) < 50)
            {
                if (player.hurt_timer <= 0)
                {
                    player.health--;
                    player.hurt_timer = 1.0f;
                    player.anim.state = ANIM_HURT;
                    player.anim.timer = 0;
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
            if (fabsf(player.x - t->x) < 32 && fabsf(player.y + 16 - t->y) < 32)
            {
                if (player.hurt_timer <= 0 && !respawn_requested)
                {
                    respawn_requested = 1; /* Warp to checkpoint immediately */
                    player.hurt_timer = 1.5f; /* Prevent re-trigger after respawn */
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
                    if (player.hurt_timer <= 0 && !respawn_requested)
                    {
                        respawn_requested = 1;
                        player.hurt_timer = 1.5f;
                    }
                }
            }
        }

        if (t->type == TRAP_SPIKE)
        {
            if (fabsf(player.x - t->x - t->count * 7) < t->count * 7 + 10 &&
                player.y < t->y + 24 && player.y + PLAYER_H > t->y)
            {
                if (player.hurt_timer <= 0 && !respawn_requested)
                {
                    respawn_requested = 1;
                    player.hurt_timer = 1.5f;
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
            }
            else
            {
                gems_collected += 10;
                spawn_collect_burst(c->x, c->y);
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
        /* Attack hits pot */
        if (player.anim.state == ANIM_ATTACK && player.attack_timer > 0.15f)
        {
            if (fabsf(player.x - pt->x) < 55 && fabsf(player.y - pt->y) < 60)
            {
                pt->broken = 1;
                pt->break_t = 0;
                gold_collected += 5;
                spawn_hit_sparks(pt->x, pt->y + 22);
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

/* ── Check exit ─────────────────────────────────────────── */
void check_exit(void)
{
    /* B2-02: portal is drawn at exit_y+32 (offset inside draw_exit_portal), test against visual centre */
    float portal_y = level.exit_y + 32.0f;
    /* The 80 px vertical window is intentionally generous to prevent frustrating near-misses.
       All current portals are placed away from ceilings, so the oversized hitbox is safe.
       If a level ever places the portal near a ceiling, tighten this to ~40 px. */
    if (fabsf(player.x - level.exit_x) < 40 && player.y < portal_y + 80 && player.y + PLAYER_H > portal_y)
    {
        /* Count stars based on collectibles */
        int total_stars = 0;
        for (int i = 0; i < level.coll_count; i++)
            if (level.colls[i].is_star && level.colls[i].collected)
                total_stars++;
        stars_collected = total_stars;
        game_state = STATE_LEVEL_COMPLETE;
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
    e->patrol_l = pl;
    e->patrol_r = pr;
    e->health = (type == ENEMY_HEAVY) ? 12 : (type == ENEMY_ARCHER) ? 4
                                                                    : 6;
    e->alive = 1;
    e->anim_t = 0;
    e->shoot_timer = 2.0f;
    e->hurt_flash = 0;
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
    player.shuriken_count = 9999; /* Unlimited ammo */
    player.jump_available = 1;    /* Start with jump available */
    player.jump_pressed = 0;      /* Jump button not pressed */
    player.on_ground = 1;         /* Assume on ground to avoid 1-frame FALL flicker at spawn */

    switch (num)
    {
    case 1:
    {
        /* ── LEVEL 1: Pure tutorial — no enemies, wide platforms, no dangerous traps ── */
        level.level_w = 2560;
        level.level_h = 720;
        level.theme = 0;
        level.bgm = 0;
        player.x = 120;
        player.y = 80;
        /* Ground */
        add_platform(0, 40, 2560, 40, PLAT_SOLID);
        /* Wide, forgiving platforms — small jumps only */
        add_platform(280, 110, 200, 32, PLAT_SOLID);
        add_platform(560, 160, 200, 32, PLAT_SOLID);
        add_platform(840, 120, 200, 32, PLAT_SOLID);
        add_platform(1120, 200, 200, 32, PLAT_SOLID);
        add_platform(1400, 140, 240, 32, PLAT_SOLID);
        add_platform(1720, 180, 220, 32, PLAT_SOLID);
        add_platform(2020, 120, 220, 32, PLAT_SOLID);
        add_platform(2280, 160, 200, 32, PLAT_SOLID);
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
        add_star(1120, 280);
        add_star(2020, 200);
        /* Props */
        add_pot(380, 80);
        add_pot(700, 80);
        add_pot(1500, 80);
        /* No blade traps — introduce only a single small spike cluster near the end */
        add_trap_spike(2100, 80, 2);
        /* Bg */
        add_bg_tree(160, 0, 1.0f);
        add_bg_tree(260, 0, 0.85f);
        add_bg_tree(1200, 0, 1.1f);
        add_bg_pillar(600, 0, 40, 180);
        add_bg_pillar(950, 0, 40, 230);
        add_cloth(180, 220, 320, 200);
        /* Checkpoints — mid-level so dying doesn't restart from scratch */
        add_checkpoint(1200, 80);
        level.exit_x = 2480;
        level.exit_y = 80;
        break;
    }
    case 2:
    {
        /* ── LEVEL 2: Meet first enemy, 1 spike trap, wider platforms ── */
        level.level_w = 2560;
        level.level_h = 720;
        level.theme = 1; /* TEMPLE — no consecutive RUINS (IMP-08) */
        level.bgm = 0;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 2560, 40, PLAT_SOLID);
        add_platform(280, 120, 180, 32, PLAT_SOLID);
        add_platform(540, 180, 180, 32, PLAT_SOLID);
        add_platform(800, 140, 200, 32, PLAT_SOLID);
        add_platform(1080, 220, 200, 32, PLAT_SOLID);
        add_platform(1360, 160, 200, 32, PLAT_SOLID);
        add_platform(1640, 260, 200, 32, PLAT_SOLID);
        add_platform(1920, 180, 220, 32, PLAT_SOLID);
        add_platform(2200, 240, 200, 32, PLAT_SOLID);
        add_moving_plat(1500, 200, 160, 32, 0, 60, 1.0f);
        /* 1 guard enemy — intro combat */
        add_enemy(ENEMY_GUARD, 900, 80, 750, 1100);
        /* 1 archer far away so player can choose to dodge or use shuriken */
        add_enemy(ENEMY_ARCHER, 1800, 80, 1650, 2000);
        /* Only 1 spike cluster, easy to avoid */
        add_trap_spike(680, 80, 3);
        /* Collectibles */
        add_gem(300, 160);
        add_gem(560, 220);
        add_gem(820, 180);
        add_gem(1100, 260);
        add_gem(1380, 200);
        add_gem(1660, 300);
        add_gem(1940, 220);
        add_gem(2220, 280);
        add_star(1080, 300);
        add_star(1920, 260);
        /* Props */
        add_pot(380, 80);
        add_pot(750, 80);
        add_pot(1600, 80);
        /* Bg */
        add_bg_tree(160, 40, 1.0f);
        add_bg_tree(800, 40, 0.9f);
        add_bg_tree(1600, 40, 1.1f);
        add_bg_pillar(500, 40, 40, 180);
        add_bg_pillar(1200, 40, 40, 230);
        add_cloth(180, 220, 320, 200);
        add_checkpoint(1100, 80);
        level.exit_x = 2440;
        level.exit_y = 80;
        break;
    }
    case 3:
    {
        /* ── LEVEL 3: 2 guards + 1 archer, 1 blade + 1 spike, intro crumbling platform ── */
        level.level_w = 3200;
        level.level_h = 720;
        level.theme = 3; /* BAMBOO — cycle: RUINS→TEMPLE→BAMBOO (IMP-08) */
        level.bgm = 1;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 3200, 40, PLAT_SOLID);
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
        /* Gentle crumbling platform introduction */
        add_platform(1240, 300, 120, 32, PLAT_CRUMBLING);
        add_moving_plat(1800, 220, 140, 32, 0, 70, 1.2f);
        /* 2 guards, 1 archer */
        add_enemy(ENEMY_GUARD, 650, 80, 480, 880);
        add_enemy(ENEMY_ARCHER, 1450, 80, 1280, 1650);
        add_enemy(ENEMY_GUARD, 2300, 80, 2100, 2550);
        /* 1 spike (small), 1 blade (mid-level) */
        add_trap_spike(780, 80, 3);
        add_trap_blade(1700, 80);
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
        add_star(2820, 380);
        add_pot(450, 80);
        add_pot(1150, 80);
        add_pot(1900, 80);
        add_checkpoint(1200, 80);
        add_checkpoint(2200, 80);
        level.exit_x = 3120;
        level.exit_y = 80;
        break;
    }
    case 4:
    {
        /* ── LEVEL 4: Introduce heavy enemy, 2 blades + 2 spikes, one-way platforms ── */
        level.level_w = 3200;
        level.level_h = 720;
        level.theme = 2; /* VILLAGE — cycle: ...BAMBOO→VILLAGE (IMP-08) */
        level.bgm = 1;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 3200, 40, PLAT_SOLID);
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
        add_moving_plat(1460, 240, 140, 32, 0, 80, 1.2f);
        add_moving_plat(2620, 220, 140, 32, 1, 70, 1.4f);
        /* 2 guards, 1 archer, 1 heavy (introduced mid-level) */
        add_enemy(ENEMY_GUARD, 560, 80, 380, 780);
        add_enemy(ENEMY_ARCHER, 1200, 80, 1000, 1420);
        add_enemy(ENEMY_HEAVY, 1700, 80, 1520, 1920);
        add_enemy(ENEMY_GUARD, 2350, 80, 2150, 2600);
        add_enemy(ENEMY_ARCHER, 2900, 80, 2700, 3100);
        /* 2 blades, 2 spikes — clearly spaced so player can anticipate */
        add_trap_spike(680, 80, 3);
        add_trap_blade(1400, 80);
        add_trap_spike(2250, 80, 4);
        add_trap_blade(2820, 80);
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
        add_pot(400, 80);
        add_pot(1100, 80);
        add_pot(2000, 80);
        add_checkpoint(1100, 80);
        add_checkpoint(2200, 80);
        level.exit_x = 3120;
        level.exit_y = 80;
        break;
    }
    case 5:
    {
        /* ── LEVEL 5: Fortress theme, 3 guards + 1 archer + 1 heavy, 2 blades + 2 spikes ── */
        level.level_w = 3840;
        level.level_h = 720;
        level.theme = 4; /* FORTRESS — cycle: ...VILLAGE→FORTRESS (IMP-08) */
        level.bgm = 2;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 3840, 40, PLAT_SOLID);
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
        add_moving_plat(1200, 240, 140, 32, 0, 90, 1.3f);
        add_moving_plat(2620, 260, 140, 32, 1, 70, 1.5f);
        /* 3 guards, 1 archer, 1 heavy — spread across the level */
        add_enemy(ENEMY_GUARD, 450, 80, 280, 660);
        add_enemy(ENEMY_ARCHER, 1100, 80, 900, 1320);
        add_enemy(ENEMY_HEAVY, 1700, 80, 1500, 1950);
        add_enemy(ENEMY_GUARD, 2350, 80, 2150, 2600);
        add_enemy(ENEMY_GUARD, 3100, 80, 2900, 3350);
        /* 2 blades, 2 spikes */
        add_trap_spike(650, 80, 3);
        add_trap_blade(1360, 80);
        add_trap_spike(2300, 80, 4);
        add_trap_blade(3100, 80);
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
        add_pot(420, 80);
        add_pot(1200, 80);
        add_pot(2400, 80);
        add_pot(3200, 80);
        add_checkpoint(1400, 80);
        add_checkpoint(2700, 80);
        level.exit_x = 3760;
        level.exit_y = 80;
        break;
    }
    case 6:
    {
        /* ── LEVEL 6: RUINS — second cycle, harder density ── */
        /* Theme: RUINS (0) — cycle: FORTRESS→RUINS (IMP-08: no consecutive same) */
        int theme6 = 0;
        level.level_w = 3840;
        level.level_h = 720;
        level.theme = theme6;
        level.bgm = theme6;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 3840, 40, PLAT_SOLID);
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
        add_moving_plat(1140, 240, 120, 32, 0, 110, 1.5f);
        add_moving_plat(2480, 260, 120, 32, 1, 90, 1.4f);
        add_enemy(ENEMY_GUARD, 400, 80, 200, 650);
        add_enemy(ENEMY_ARCHER, 850, 80, 650, 1050);
        add_enemy(ENEMY_HEAVY, 1400, 80, 1200, 1650);
        add_enemy(ENEMY_GUARD, 2100, 80, 1900, 2350);
        add_enemy(ENEMY_ARCHER, 2700, 80, 2500, 2950);
        add_enemy(ENEMY_HEAVY, 3250, 80, 3050, 3500);
        add_trap_blade(620, 80);
        add_trap_spike(1050, 80, 5);
        add_trap_fire(1600, 80, 2.0f, 2.5f);
        add_trap_blade(2150, 80);
        add_trap_spike(2650, 80, 6);
        add_trap_blade(3300, 80);
        add_trap_thorn(350, 40);
        add_trap_thorn(2400, 40);
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
        add_pot(450, 80);
        add_pot(1180, 80);
        add_pot(2200, 80);
        add_pot(3200, 80);
        add_checkpoint(1300, 80);
        add_checkpoint(2600, 80);
        level.exit_x = 3760;
        level.exit_y = 80;
        break;
    }
    case 7:
    {
        /* ── LEVEL 7: TEMPLE — second cycle, tighter platforms ── */
        int theme7 = 1;
        level.level_w = 4096;
        level.level_h = 720;
        level.theme = theme7;
        level.bgm = theme7;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4096, 40, PLAT_SOLID);
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
        add_moving_plat(1060, 260, 110, 32, 0, 120, 1.6f);
        add_moving_plat(2280, 280, 110, 32, 1, 100, 1.5f);
        add_moving_plat(3560, 240, 100, 32, 0, 130, 1.8f);
        add_enemy(ENEMY_GUARD, 380, 80, 200, 620);
        add_enemy(ENEMY_ARCHER, 800, 80, 600, 1000);
        add_enemy(ENEMY_HEAVY, 1300, 80, 1100, 1550);
        add_enemy(ENEMY_GUARD, 2000, 80, 1800, 2250);
        add_enemy(ENEMY_ARCHER, 2550, 80, 2350, 2800);
        add_enemy(ENEMY_HEAVY, 3100, 80, 2900, 3350);
        add_enemy(ENEMY_GUARD, 3650, 80, 3450, 3900);
        add_trap_blade(550, 80);
        add_trap_spike(980, 80, 5);
        add_trap_fire(1500, 80, 1.8f, 2.2f);
        add_trap_blade(2050, 80);
        add_trap_spike(2700, 80, 6);
        add_trap_fire(3200, 80, 2.0f, 2.0f);
        add_trap_blade(3750, 80);
        add_trap_thorn(320, 40);
        add_trap_thorn(2150, 40);
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
        add_pot(420, 80);
        add_pot(1100, 80);
        add_pot(2200, 80);
        add_pot(3300, 80);
        add_checkpoint(1200, 80);
        add_checkpoint(2600, 80);
        add_checkpoint(3500, 80);
        level.exit_x = 4000;
        level.exit_y = 80;
        break;
    }
    case 8:
    {
        /* ── LEVEL 8: BAMBOO — second cycle, elevated platforms ── */
        int theme8 = 3;
        level.level_w = 4096;
        level.level_h = 720;
        level.theme = theme8;
        level.bgm = theme8;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4096, 40, PLAT_SOLID);
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
        add_moving_plat(1080, 270, 100, 32, 0, 130, 1.7f);
        add_moving_plat(2300, 290, 100, 32, 1, 110, 1.6f);
        add_moving_plat(3340, 270, 100, 32, 0, 140, 1.9f);
        add_enemy(ENEMY_ARCHER, 350, 80, 150, 600);
        add_enemy(ENEMY_GUARD, 780, 80, 580, 1020);
        add_enemy(ENEMY_HEAVY, 1280, 80, 1080, 1540);
        add_enemy(ENEMY_ARCHER, 1980, 80, 1780, 2240);
        add_enemy(ENEMY_GUARD, 2550, 80, 2350, 2810);
        add_enemy(ENEMY_HEAVY, 3100, 80, 2900, 3360);
        add_enemy(ENEMY_ARCHER, 3650, 80, 3450, 3900);
        add_trap_spike(500, 80, 5);
        add_trap_blade(750, 80);
        add_trap_fire(1350, 80, 1.5f, 1.8f);
        add_trap_spike(2000, 80, 6);
        add_trap_blade(2450, 80);
        add_trap_fire(3050, 80, 1.8f, 1.8f);
        add_trap_blade(3500, 80);
        add_trap_thorn(300, 40);
        add_trap_thorn(2100, 40);
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
        add_pot(400, 80);
        add_pot(1100, 80);
        add_pot(2200, 80);
        add_pot(3300, 80);
        add_checkpoint(1200, 80);
        add_checkpoint(2600, 80);
        add_checkpoint(3600, 80);
        level.exit_x = 4000;
        level.exit_y = 80;
        break;
    }
    case 9:
    {
        /* ── LEVEL 9: VILLAGE — second cycle, narrow dangerous platforms ── */
        int theme9 = 2;
        level.level_w = 4096;
        level.level_h = 720;
        level.theme = theme9;
        level.bgm = theme9;
        player.x = 120;
        player.y = 80;
        add_platform(0, 40, 4096, 40, PLAT_SOLID);
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
        add_moving_plat(800, 280, 100, 32, 0, 140, 1.8f);
        add_moving_plat(2000, 310, 100, 32, 1, 120, 1.7f);
        add_moving_plat(3400, 300, 90, 32, 0, 150, 2.0f);
        add_enemy(ENEMY_HEAVY, 350, 80, 150, 560);
        add_enemy(ENEMY_ARCHER, 760, 80, 560, 980);
        add_enemy(ENEMY_GUARD, 1220, 80, 1020, 1450);
        add_enemy(ENEMY_HEAVY, 1850, 80, 1650, 2100);
        add_enemy(ENEMY_ARCHER, 2400, 80, 2200, 2650);
        add_enemy(ENEMY_GUARD, 2930, 80, 2730, 3170);
        add_enemy(ENEMY_HEAVY, 3420, 80, 3220, 3680);
        add_trap_fire(500, 80, 1.5f, 1.5f);
        add_trap_blade(720, 80);
        add_trap_spike(1200, 80, 6);
        add_trap_fire(1700, 80, 1.2f, 1.5f);
        add_trap_blade(2200, 80);
        add_trap_spike(2800, 80, 6);
        add_trap_fire(3300, 80, 1.5f, 1.5f);
        add_trap_thorn(280, 40);
        add_trap_thorn(2000, 40);
        add_trap_thorn(3500, 40);
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
        add_pot(380, 80);
        add_pot(1100, 80);
        add_pot(2300, 80);
        add_pot(3400, 80);
        add_checkpoint(1150, 80);
        add_checkpoint(2550, 80);
        add_checkpoint(3600, 80);
        level.exit_x = 4000;
        level.exit_y = 80;
        break;
    }
    case 10:
    {
        /* ── LEVEL 10: FORTRESS CLIMAX — boss-like, no early checkpoints, max challenge ── */
        /* IMP-03: Distinct mega-fortress layout — vertical stacks, 3 heavies, scarce checkpoints */
        level.level_w = 4096;
        level.level_h = 720;
        level.theme = 4; /* FORTRESS */
        level.bgm = 4;
        player.x = 120;
        player.y = 80;

        /* Ground */
        add_platform(0, 40, 4096, 40, PLAT_SOLID);

        /* Fortress battlements — tall vertical stacks with narrow tops */
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
        /* Final approach — wider fortress wall to fight boss */
        add_platform(3560, 200, 360, 32, PLAT_SOLID);
        add_platform(3560, 350, 100, 32, PLAT_SOLID);
        add_platform(3780, 350, 100, 32, PLAT_SOLID);

        /* Deadly moving platforms — no handrails */
        add_moving_plat(960, 300, 90, 32, 0, 140, 2.0f);
        add_moving_plat(2000, 360, 90, 32, 1, 130, 1.9f);
        add_moving_plat(3100, 340, 90, 32, 0, 150, 2.2f);

        /* Enemy gauntlet — 3 heavies guard the gate, archers on high ground */
        add_enemy(ENEMY_GUARD, 450, 80, 250, 700);
        add_enemy(ENEMY_ARCHER, 800, 80, 600, 1050);
        add_enemy(ENEMY_HEAVY, 1200, 80, 1000, 1450);     /* First heavy — mid fortress */
        add_enemy(ENEMY_GUARD, 1800, 80, 1600, 2050);
        add_enemy(ENEMY_ARCHER, 2350, 80, 2150, 2600);
        add_enemy(ENEMY_HEAVY, 2800, 80, 2600, 3050);     /* Second heavy — deeper */
        add_enemy(ENEMY_GUARD, 3200, 80, 3000, 3450);
        add_enemy(ENEMY_HEAVY, 3700, 232, 3560, 3920);    /* Final heavy — stands on fortress wall */
        add_enemy(ENEMY_ARCHER, 3850, 232, 3560, 3920);   /* Archer on wall with final heavy */

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
        add_trap_thorn(350, 40);
        add_trap_thorn(1500, 40);
        add_trap_thorn(2700, 40);
        add_trap_thorn(3600, 232); /* thorn on fortress wall */

        /* Stars and gems hidden in dangerous spots */
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

        /* Only one checkpoint — halfway — to keep pressure on */
        add_checkpoint(2120, 80);

        level.exit_x = 4000;
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
}

/* ── Respawn / restart ──────────────────────────────────── */
static void do_respawn(void)
{
    if (level.respawn_active)
    {
        /* Checkpoint respawn: full health, 1.5 s invincibility */
        player.x = level.respawn_x;
        player.y = level.respawn_y + 10;
        player.vx = 0;
        player.vy = 0;
        player.health = MAX_HEALTH;
        player.hurt_timer = 1.5f;
        player.anim.state = ANIM_IDLE;
        player.anim.timer = 0;
        player.jump_pressed = 0;
        player.jump_available = 1;
        player.jump_count = 0;
        /* Clear stray projectiles */
        for (int i = 0; i < level.proj_count; i++)
            level.projs[i].active = 0;
        level.proj_count = 0;
    }
    else
    {
        /* No checkpoint: auto-restart level and reduce starting health by 1 */
        if (respawn_health > 1)
            respawn_health--;
        int saved = respawn_health;
        load_level(current_level);
        player.health = saved;
        /* game_state remains STATE_PLAYING — no game-over screen */
    }
}

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

    /* Trap contact: warp to checkpoint (or auto-restart with health penalty) */
    if (respawn_requested)
    {
        respawn_requested = 0;
        do_respawn();
        return; /* Skip death check — respawn handled everything */
    }

    /* Death from enemy/arrow/fall */
    if (player.health <= 0 && player.anim.state != ANIM_DEAD)
    {
        player.anim.state = ANIM_DEAD;
        do_respawn(); /* Checkpoint or auto-restart — never shows game-over screen */
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
            game_state = STATE_PLAYING;
            load_level(1);
        }
        return;
    }
    if (game_state == STATE_GAME_OVER)
    {
        if (k == '\r' || k == '\n')
        {
            game_state = STATE_PLAYING;
            load_level(current_level);
        }
        return;
    }
    if (game_state == STATE_LEVEL_COMPLETE)
    {
        if (k == '\r' || k == '\n')
        {
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
