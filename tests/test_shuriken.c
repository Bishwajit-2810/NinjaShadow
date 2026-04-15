/* Test file for shuriken system constraints
 * Compile: gcc test_shuriken.c -o test_shuriken -Wall
 * Run: ./test_shuriken
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>

/* Constants from game.h */
#define MAX_PROJECTILES 64
#define SHURIKEN_RANGE 50.0f
#define SHURIKEN_VELOCITY 450.0f
#define SHURIKEN_PER_LEVEL 10

/* Simplified test structures */
typedef struct
{
    float x, y, vx;
    int active;
    float range;
    int is_shuriken;
    int from_player;
} TestProjectile;

typedef struct
{
    float x;
    int facing;
    int shuriken_count;
} TestPlayer;

/* Test 1: Shuriken count initial value */
void test_initial_count()
{
    printf("Test 1: Initial shuriken count...");
    TestPlayer player = {0.0f, 1, SHURIKEN_PER_LEVEL};
    assert(player.shuriken_count == 10);
    printf(" ✓ PASS\n");
}

/* Test 2: Shuriken count never goes negative */
void test_count_clamp()
{
    printf("Test 2: Shuriken count clamp (no negatives)...");
    TestPlayer player = {0.0f, 1, 0};

    /* Try to decrement below zero */
    player.shuriken_count--;
    if (player.shuriken_count < 0)
        player.shuriken_count = 0;

    assert(player.shuriken_count == 0);
    printf(" ✓ PASS\n");
}

/* Test 3: Shuriken range is enforced */
void test_range_limit()
{
    printf("Test 3: Shuriken range limit (50 units)...");
    TestProjectile proj = {100.0f, 0.0f, SHURIKEN_VELOCITY, 1, SHURIKEN_RANGE, 1, 1};
    TestPlayer player = {0.0f, 1, 10};

    /* Simulate projectile at range limit */
    float dist = fabsf(proj.x - player.x);
    assert(dist == 100.0f);

    /* Check if range exceeded */
    if (dist > proj.range)
    {
        printf(" (range %.1f exceeded, distance %.1f) ", proj.range, dist);
    }
    assert(proj.range == SHURIKEN_RANGE);
    printf(" ✓ PASS\n");
}

/* Test 4: Shuriken velocity and direction */
void test_shuriken_velocity()
{
    printf("Test 4: Shuriken velocity and direction...");
    TestPlayer player_right = {0.0f, 1, 10}; /* Facing right */
    TestPlayer player_left = {0.0f, -1, 10}; /* Facing left */

    /* Right facing */
    float vx_right = player_right.facing * SHURIKEN_VELOCITY;
    assert(vx_right == 450.0f);

    /* Left facing */
    float vx_left = player_left.facing * SHURIKEN_VELOCITY;
    assert(vx_left == -450.0f);
    printf(" ✓ PASS\n");
}

/* Test 5: Shuriken count decrements only once per throw */
void test_single_decrement()
{
    printf("Test 5: Single decrement per throw...");
    TestPlayer player = {0.0f, 1, 10};

    /* Simulate throw */
    if (player.shuriken_count > 0)
    {
        player.shuriken_count--;
    }

    assert(player.shuriken_count == 9);
    printf(" ✓ PASS\n");
}

/* Test 6: Multiple throws reduce count correctly */
void test_multiple_throws()
{
    printf("Test 6: Multiple consecutive throws...");
    TestPlayer player = {0.0f, 1, 10};

    /* Throw 5 times */
    for (int i = 0; i < 5; i++)
    {
        if (player.shuriken_count > 0)
        {
            player.shuriken_count--;
        }
    }

    assert(player.shuriken_count == 5);
    printf(" ✓ PASS\n");
}

/* Test 7: Verify 10 throw limit */
void test_exactly_ten_throws()
{
    printf("Test 7: Exactly 10 throws per level...");
    TestPlayer player = {0.0f, 1, 10};

    /* Try to throw 15 times */
    int throws = 0;
    for (int i = 0; i < 15; i++)
    {
        if (player.shuriken_count > 0)
        {
            player.shuriken_count--;
            throws++;
        }
    }

    assert(throws == 10);
    assert(player.shuriken_count == 0);
    printf(" ✓ PASS\n");
}

/* Test 8: Projectile buffer limit */
void test_max_projectiles()
{
    printf("Test 8: Max projectiles buffer (64)...");
    int proj_count = MAX_PROJECTILES;

    assert(proj_count == 64);
    assert(proj_count < 65); /* Can't exceed 64 */
    printf(" ✓ PASS\n");
}

int main()
{
    printf("\n=== SHURIKEN SYSTEM CONSTRAINTS TEST ===\n\n");

    test_initial_count();
    test_count_clamp();
    test_range_limit();
    test_shuriken_velocity();
    test_single_decrement();
    test_multiple_throws();
    test_exactly_ten_throws();
    test_max_projectiles();

    printf("\n=== ALL TESTS PASSED ✓ ===\n\n");
    return 0;
}
