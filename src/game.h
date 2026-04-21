#ifndef GAME_H
#define GAME_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265f
#define SCREEN_W 1280
#define SCREEN_H 720

/* ── Physics constants ──────────────────────────────────── */
#define PLAYER_W 28.0f
#define PLAYER_H 64.0f
#define GRAVITY 1180.0f
#define JUMP_VY 500.0f
#define DBL_JUMP_VY 440.0f
#define MOVE_SPEED 220.0f
#define DASH_SPEED 460.0f
#define WALL_SLIDE_VY (-180.0f)
#define MAX_FALL_SPD (-900.0f)
#define MAX_HEALTH 6
#define SHURIKEN_PER_LEVEL 10
#define ENEMY_DAMAGE_BAR_MAX 100.0f
#define ENEMY_DAMAGE_PER_HIT 25.0f

/* ── Game states ────────────────────────────────────────── */
typedef enum
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_LEVEL_COMPLETE,
    STATE_SETTINGS
} GameState;

/* ── Animation states ───────────────────────────────────── */
typedef enum
{
    ANIM_IDLE,
    ANIM_RUN,
    ANIM_JUMP,
    ANIM_FALL,
    ANIM_WALL_SLIDE,
    ANIM_ATTACK,
    ANIM_DASH,
    ANIM_HURT,
    ANIM_DEAD
} AnimState;

typedef struct
{
    AnimState state;
    float timer;
    float anim_t;
    int facing; /* +1 right, -1 left */
} Animation;

/* ── Player ─────────────────────────────────────────────── */
typedef struct
{
    float x, y, vx, vy;
    float w, h;
    int on_ground, on_wall;
    int jump_count;
    int health;
    float hurt_timer;
    float attack_timer;
    float dash_timer;
    float dash_cooldown;
    int shuriken_count;
    int shuriken_pressed; /* Track if shuriken throw button is pressed */
    int jump_pressed;     /* Track if jump button is currently pressed */
    int jump_available;   /* Can we jump now (released since last jump) */
    int dash_pressed;     /* One-shot flag: pressed since last release (prevents key-repeat) */
    Animation anim;
} Player;

/* ── Platform ───────────────────────────────────────────── */
typedef enum
{
    PLAT_SOLID,
    PLAT_ONE_WAY,
    PLAT_MOVING,
    PLAT_CRUMBLING
} PlatType;

typedef struct
{
    float x, y, w, h;
    PlatType type;
    /* moving */
    int axis; /* 0=X 1=Y */
    float amplitude, speed, phase;
    float ox, oy; /* origin */
    /* crumbling */
    float crumble_timer;
    int crumbled;
    /* per-frame displacement in px (not px/s) — computed each frame for PLAT_MOVING,
       used to carry the player; multiply by dt would be a ~60× error */
    float pvx, pvy;
} Platform;

/* ── Enemy ──────────────────────────────────────────────── */
typedef enum
{
    ENEMY_GUARD,
    ENEMY_ARCHER,
    ENEMY_HEAVY
} EnemyType;

typedef struct
{
    EnemyType type;
    float x, y, vx;
    float patrol_l, patrol_r;
    int health;
    int facing;
    float anim_t;
    float shoot_timer;
    int alive;
    float hurt_flash;
    float attack_hit_timer; /* B-03: cooldown so a single sword swing can't multi-hit */
    float death_timer;      /* I-08: counts down from 0.5 for death-fade animation */
} Enemy;

/* ── Projectile ─────────────────────────────────────────── */
typedef struct
{
    float x, y, vx, vy;
    int active;
    int is_shuriken; /* 1=shuriken, 0=arrow */
    float angle;
    int from_player;
    float range; /* max distance shuriken can travel */
} Projectile;

/* ── Trap ───────────────────────────────────────────────── */
typedef enum
{
    TRAP_BLADE,
    TRAP_SPIKE,
    TRAP_THORN,
    TRAP_FIRE
} TrapType;

typedef struct
{
    TrapType type;
    float x, y;
    int count;   /* spike count */
    float angle; /* blade angle */
    int active;  /* fire on/off */
    float fire_t;
    float fire_on, fire_off;
    float scale;
} Trap;

/* ── Prop ───────────────────────────────────────────────── */
typedef struct
{
    float x, y;
    int broken;
    float break_t;
    int active;
} Pot;

/* ── Collectible ────────────────────────────────────────── */
typedef struct
{
    float x, y;
    int collected;
    int is_star;
    float pulse_t;
} Collectible;

/* ── Background object ──────────────────────────────────── */
typedef struct
{
    int type; /* 0=pillar 1=tree */
    float x, y, w, h;
} BgObj;

/* ── Cloth/rope decoration ──────────────────────────────── */
typedef struct
{
    float x1, y1, x2, y2;
} ClothDec;

/* ── Checkpoint ─────────────────────────────────────────── */
#define MAX_CHECKPOINTS 8
typedef struct
{
    float x, y;
    int triggered;
} Checkpoint;

/* ── Level ──────────────────────────────────────────────── */
#define MAX_PLATFORMS 64
#define MAX_ENEMIES 32
#define MAX_PROJECTILES 64
#define MAX_TRAPS 32
#define MAX_POTS 32
#define MAX_COLLECTIBLES 64
#define MAX_BG_OBJS 32
#define MAX_CLOTH 16

typedef struct
{
    float level_w, level_h;
    int theme; /* 0=RUINS 1=TEMPLE 2=VILLAGE 3=BAMBOO 4=FORTRESS */
    int bgm;

    Platform platforms[MAX_PLATFORMS];
    int plat_count;
    Enemy enemies[MAX_ENEMIES];
    int enemy_count;
    Projectile projs[MAX_PROJECTILES];
    int proj_count;
    Trap traps[MAX_TRAPS];
    int trap_count;
    Pot pots[MAX_POTS];
    int pot_count;
    Collectible colls[MAX_COLLECTIBLES];
    int coll_count;
    BgObj bgobjs[MAX_BG_OBJS];
    int bgobj_count;
    ClothDec cloths[MAX_CLOTH];
    int cloth_count;

    float exit_x, exit_y;

    /* Checkpoint system */
    Checkpoint checkpoints[MAX_CHECKPOINTS];
    int checkpoint_count;
    float respawn_x, respawn_y; /* Position of last triggered checkpoint */
    int respawn_active;         /* 1 = player has a checkpoint to respawn at */
} Level;

/* ── Particle ───────────────────────────────────────────── */
#define MAX_PARTICLES 256
typedef struct
{
    float x, y, vx, vy;
    float life, max_life;
    float r, g, b, a;
    float size;
    int active;
} Particle;

/* ── Camera ─────────────────────────────────────────────── */
typedef struct
{
    float x, y;
} Camera;

/* ── Globals (defined in game.c) ───────────────────────── */
extern GameState game_state;
extern Player player;
extern Level level;
extern Camera camera;
extern Particle particles[MAX_PARTICLES];
extern float game_time;
extern float level_time; /* A-02/I-09: seconds elapsed in current level */
extern int gems_collected;
extern int gold_collected;
extern int stars_collected;
extern int current_level;
extern int keys[256];
extern int special_keys[256];
extern int respawn_health;           /* Starting health when no checkpoint was reached */
extern int enemies_defeated;         /* I-09: count of enemies killed this level */
extern int combo_count;              /* I-03: consecutive hit counter */
extern float combo_timer;            /* I-03: resets to 2.0 on each hit, then counts down */
extern float penalty_toast_timer;    /* counts down while "-1 max heart" toast is visible */
extern float enemy_damage_bar;       /* Enemy-only damage meter; depletes before losing one heart */
extern float enemy_damage_bar_flash; /* Brief flash amount for HUD feedback when enemy hits */

/* ── Function declarations ──────────────────────────────── */
void game_init(void);
void load_level(int num);
void update_game(float dt);
int portal_is_open(void); /* 1 when all checkpoints triggered + all enemies dead */

#endif /* GAME_H */
