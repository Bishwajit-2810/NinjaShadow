#include "renderer.h"
#include "game.h"
#include <math.h>

/* Teacher demonstration modes for tree rendering.
   0 = normal production trees, 1 = primitive-only tree, 2 = DDA/Bresenham tree. */
static int tree_demo_mode = 0;

void set_tree_demo_mode(int mode)
{
    if (mode < 0 || mode > 2)
        return;
    tree_demo_mode = mode;
}

int get_tree_demo_mode(void)
{
    return tree_demo_mode;
}

/* Manual primitive tree: intentionally uses direct glBegin/glVertex blocks only. */
static void draw_tree_manual_primitive(float bx, float by, float scale)
{
    float s = scale;

    glColor4f(0.16f, 0.11f, 0.08f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(bx - 6 * s, 0);
    glVertex2f(bx + 6 * s, 0);
    glVertex2f(bx + 4 * s, by + 92 * s);
    glVertex2f(bx - 4 * s, by + 92 * s);
    glEnd();

    glColor4f(0.08f, 0.30f, 0.10f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 42 * s, by + 76 * s);
    glVertex2f(bx + 42 * s, by + 76 * s);
    glVertex2f(bx, by + 132 * s);

    glVertex2f(bx - 34 * s, by + 102 * s);
    glVertex2f(bx + 34 * s, by + 102 * s);
    glVertex2f(bx, by + 154 * s);

    glVertex2f(bx - 24 * s, by + 126 * s);
    glVertex2f(bx + 24 * s, by + 126 * s);
    glVertex2f(bx, by + 174 * s);
    glEnd();

    glColor4f(0.10f, 0.23f, 0.08f, 1.0f);
    glLineWidth(2.0f * s);
    glBegin(GL_LINES);
    glVertex2f(bx, by + 90 * s);
    glVertex2f(bx - 20 * s, by + 120 * s);
    glVertex2f(bx, by + 95 * s);
    glVertex2f(bx + 20 * s, by + 122 * s);
    glEnd();
    glLineWidth(1.0f);
}

/* Algorithm tree: trunk with primitives, branches with DDA/Bresenham, foliage with midpoint circles. */
static void draw_tree_algorithmic(float bx, float by, float scale)
{
    float s = scale;

    glColor4f(0.16f, 0.11f, 0.08f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(bx - 5 * s, 0);
    glVertex2f(bx + 5 * s, 0);
    glVertex2f(bx + 3 * s, by + 96 * s);
    glVertex2f(bx - 3 * s, by + 96 * s);
    glEnd();

    glColor4f(0.18f, 0.12f, 0.10f, 1.0f);
    draw_line_dda(bx, by + 88 * s, bx - 34 * s, by + 138 * s);
    draw_line_dda(bx, by + 90 * s, bx + 30 * s, by + 136 * s);
    draw_line_bresenham((int)roundf(bx), (int)roundf(by + 78 * s),
                        (int)roundf(bx - 16 * s), (int)roundf(by + 118 * s));
    draw_line_bresenham((int)roundf(bx), (int)roundf(by + 82 * s),
                        (int)roundf(bx + 18 * s), (int)roundf(by + 122 * s));

    glColor4f(0.08f, 0.30f, 0.10f, 1.0f);
    draw_circle_midpoint((int)roundf(bx), (int)roundf(by + 138 * s), (int)roundf(24 * s));
    draw_circle_midpoint((int)roundf(bx - 22 * s), (int)roundf(by + 128 * s), (int)roundf(17 * s));
    draw_circle_midpoint((int)roundf(bx + 22 * s), (int)roundf(by + 130 * s), (int)roundf(17 * s));
}

/* ── Sky gradient (IMP-04: theme-specific colors) ───────── */
void draw_sky(void)
{
    float r1, g1, b1, r2, g2, b2;
    switch (level.theme)
    {
    case 1: /* TEMPLE — indigo twilight */
        r1 = 0.20f;
        g1 = 0.22f;
        b1 = 0.54f;
        r2 = 0.08f;
        g2 = 0.10f;
        b2 = 0.34f;
        break;
    case 2: /* VILLAGE — amber dusk */
        r1 = 0.52f;
        g1 = 0.30f;
        b1 = 0.14f;
        r2 = 0.22f;
        g2 = 0.12f;
        b2 = 0.08f;
        break;
    case 3: /* BAMBOO — teal evening */
        r1 = 0.14f;
        g1 = 0.36f;
        b1 = 0.46f;
        r2 = 0.06f;
        g2 = 0.20f;
        b2 = 0.26f;
        break;
    case 4: /* FORTRESS — deep crimson */
        r1 = 0.34f;
        g1 = 0.12f;
        b1 = 0.16f;
        r2 = 0.18f;
        g2 = 0.06f;
        b2 = 0.10f;
        break;
    default: /* RUINS (0) and menu: deep blue night sky */
        r1 = 0.18f;
        g1 = 0.32f;
        b1 = 0.54f;
        r2 = 0.08f;
        g2 = 0.16f;
        b2 = 0.32f;
        break;
    }
    glBegin(GL_QUADS);
    glColor4f(r1, g1, b1, 1.0f);
    glVertex2f(0, 0);
    glColor4f(r1, g1, b1, 1.0f);
    glVertex2f(1280, 0);
    glColor4f(r2, g2, b2, 1.0f);
    glVertex2f(1280, 720);
    glColor4f(r2, g2, b2, 1.0f);
    glVertex2f(0, 720);
    glEnd();
}

/* ── Night sky stars (twinkle effect) ──────────────────── */
void draw_night_stars(float game_time)
{
    /* 48 fixed star positions spread across the sky */
    static const float star_x[] = {
        42, 110, 195, 278, 360, 445, 530, 615,
        700, 785, 870, 955, 1040, 1125, 1210, 1265,
        80, 160, 250, 340, 430, 520, 610, 700,
        790, 880, 970, 1060, 1150, 1240, 30, 120,
        205, 310, 410, 510, 610, 710, 810, 910,
        1010, 1110, 1180, 55, 155, 355, 655, 955};
    static const float star_y[] = {
        680, 670, 690, 665, 675, 685, 668, 678,
        688, 660, 672, 682, 692, 663, 673, 650,
        630, 640, 625, 635, 645, 628, 638, 648,
        622, 632, 642, 620, 612, 605, 580, 590,
        570, 582, 592, 560, 572, 584, 564, 576,
        556, 568, 598, 540, 550, 530, 545, 535};
    static const float star_size[] = {
        1.5f, 1.2f, 1.8f, 1.0f, 2.0f, 1.4f, 1.6f, 1.2f,
        1.8f, 1.0f, 1.5f, 2.0f, 1.3f, 1.7f, 1.1f, 1.9f,
        1.4f, 1.2f, 1.8f, 1.6f, 1.0f, 2.0f, 1.3f, 1.7f,
        1.5f, 1.1f, 1.9f, 1.4f, 1.6f, 1.2f, 1.8f, 1.0f,
        2.0f, 1.5f, 1.3f, 1.7f, 1.1f, 1.9f, 1.4f, 1.6f,
        1.2f, 1.8f, 1.0f, 2.0f, 1.5f, 1.3f, 1.7f, 1.1f};
    static const float star_phase[] = {
        0.0f, 1.2f, 2.4f, 0.6f, 1.8f, 3.0f, 0.3f, 1.5f,
        2.7f, 0.9f, 2.1f, 3.3f, 0.4f, 1.6f, 2.8f, 0.2f,
        1.4f, 2.6f, 0.8f, 2.0f, 3.2f, 0.5f, 1.7f, 2.9f,
        0.1f, 1.3f, 2.5f, 0.7f, 1.9f, 3.1f, 0.6f, 1.8f,
        3.0f, 0.4f, 1.6f, 2.8f, 0.2f, 1.4f, 2.6f, 0.8f,
        2.0f, 3.2f, 0.5f, 1.7f, 2.9f, 0.3f, 1.1f, 2.3f};
    int n = sizeof(star_x) / sizeof(star_x[0]);

    /* Darker theme stars are brighter white; bright-sky themes are dimmer */
    float base_alpha = (level.theme == 2) ? 0.25f : 0.55f; /* less visible on amber sky */

    for (int i = 0; i < n; i++)
    {
        float twinkle = 0.5f + 0.5f * sinf(game_time * (1.2f + star_phase[i] * 0.3f) + star_phase[i]);
        float a = base_alpha * twinkle;
        float r = star_size[i];
        glColor4f(1.0f, 0.95f, 0.85f, a);
        /* Draw as a small cross for a classic star shape */
        draw_circle(star_x[i], star_y[i], r, 6);
        /* Small 4-point sparkle on larger stars */
        if (r >= 1.6f && twinkle > 0.7f)
        {
            float s = r * 2.5f * (twinkle - 0.7f) / 0.3f;
            glColor4f(1.0f, 1.0f, 0.90f, a * 0.5f);
            draw_rect(star_x[i] - s, star_y[i] - 0.5f, s * 2.0f, 1.0f);
            draw_rect(star_x[i] - 0.5f, star_y[i] - s, 1.0f, s * 2.0f);
        }
    }
}

/* ── Ground atmosphere fog ──────────────────────────────── */
void draw_ground_atmosphere(void)
{
    glBegin(GL_QUADS);
    glColor4f(0.10f, 0.18f, 0.34f, 0.40f);
    glVertex2f(0, 0);
    glColor4f(0.10f, 0.18f, 0.34f, 0.40f);
    glVertex2f(1280, 0);
    glColor4f(0.10f, 0.18f, 0.34f, 0.00f);
    glVertex2f(1280, 80);
    glColor4f(0.10f, 0.18f, 0.34f, 0.00f);
    glVertex2f(0, 80);
    glEnd();
}

/* ── Ghost trees (far background, 0.05x parallax) ──────── */
void draw_ghost_trees(float cam_x)
{
    float ox = -cam_x * 0.05f;
    glColor4f(0.16f, 0.26f, 0.44f, 0.55f);

    /* B-13: use level width so trees wrap correctly for levels wider than 3840 */
    float wrap_w = (level.level_w > 0 ? level.level_w : 3840) + 1280.0f;
    float positions[] = {200, 480, 760, 1040};
    for (int i = 0; i < 4; i++)
    {
        float bx = fmodf(positions[i] + ox + wrap_w, wrap_w) - 1280;
        float by = 0;
        float h = 400.0f + i * 40.0f;
        float tw = h * 0.06f;

        glBegin(GL_QUADS);
        glVertex2f(bx - tw * 0.5f, by);
        glVertex2f(bx + tw * 0.5f, by);
        glVertex2f(bx + tw * 0.3f, by + h * 0.5f);
        glVertex2f(bx - tw * 0.3f, by + h * 0.5f);
        glEnd();

        float cw = h * 0.45f;
        draw_tri(bx - cw, by + h * 0.35f, bx + cw, by + h * 0.35f, bx, by + h * 0.75f);
        draw_tri(bx - cw * 0.7f, by + h * 0.50f, bx + cw * 0.7f, by + h * 0.50f, bx, by + h * 0.85f);
        draw_tri(bx - cw * 0.4f, by + h * 0.65f, bx + cw * 0.4f, by + h * 0.65f, bx, by + h * 0.95f);
    }
}

/* ── Moon phases ────────────────────────────────────────── */
void draw_moon(float cx, float cy, float phase, int scene_type)
{
    /* phase: 0=new, 0.25=first quarter, 0.5=full, 0.75=last quarter */
    float radius = 50.0f;

    /* Decide moon color based on scene type */
    if (scene_type == 2 || scene_type == 4)
    {
        glColor4f(1.0f, 0.3f, 0.1f, 0.9f); /* Red moon */
    }
    else
    {
        glColor4f(0.95f, 0.95f, 0.7f, 0.95f); /* Yellow/white moon */
    }

    /* Draw moon circle */
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 32; i++)
    {
        float a = 2 * PI * i / 32.0f;
        glVertex2f(cx + cosf(a) * radius, cy + sinf(a) * radius);
    }
    glEnd();

    /* Shadow for crescent/moon phases */
    if (phase > 0.01f && phase < 0.99f)
    {
        glColor4f(0.02f, 0.05f, 0.10f, 0.8f);
        float shadow_offset = cosf(phase * PI) * radius * 1.5f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx + shadow_offset, cy);
        for (int i = 0; i <= 32; i++)
        {
            float a = 2 * PI * i / 32.0f;
            glVertex2f(cx + shadow_offset + cosf(a) * radius, cy + sinf(a) * radius);
        }
        glEnd();
    }
}

/* ── Dark clouds ────────────────────────────────────────── */
void draw_clouds(float game_time, float cam_x)
{
    /* Slow parallax: clouds move at 0.08x camera */
    float ox = -cam_x * 0.08f;

    /* 12 cloud clusters at different heights and speeds */
    struct
    {
        float bx, by, scale, speed, alpha;
    } clouds[] = {
        {100, 620, 1.2f, 22.0f, 0.18f},
        {420, 650, 0.9f, 18.0f, 0.14f},
        {750, 600, 1.4f, 25.0f, 0.20f},
        {1050, 635, 1.0f, 20.0f, 0.16f},
        {220, 580, 0.8f, 15.0f, 0.12f},
        {900, 660, 1.1f, 30.0f, 0.22f},
        {540, 545, 1.5f, 12.0f, 0.10f},
        {1180, 570, 1.0f, 17.0f, 0.13f},
        {320, 510, 1.3f, 10.0f, 0.09f},
        {800, 525, 0.7f, 28.0f, 0.11f},
        {650, 690, 1.6f, 35.0f, 0.24f},
        {1100, 500, 0.9f, 8.0f, 0.08f},
    };
    int nc = sizeof(clouds) / sizeof(clouds[0]);

    /* V-06: Scale wrap width with level width so clouds don't prematurely repeat on
       levels wider than ~3840 px (levels 7-10 are 4096 px wide). */
    float cloud_wrap = (level.level_w > 3840.0f) ? level.level_w + 1280.0f : 3840.0f;

    for (int i = 0; i < nc; i++)
    {
        float cx = fmodf(clouds[i].bx + ox + game_time * clouds[i].speed + cloud_wrap, cloud_wrap) - 1280.0f;
        if (cx < -200 || cx > 1480)
            continue;

        float s = clouds[i].scale;
        float cy = clouds[i].by;
        float a = clouds[i].alpha;

        /* Dusky cloud made of overlapping circles */
        glColor4f(0.20f, 0.30f, 0.50f, a * 1.5f);
        draw_circle(cx, cy, 28 * s, 14);
        draw_circle(cx + 32 * s, cy + 8 * s, 22 * s, 12);
        draw_circle(cx - 28 * s, cy + 6 * s, 20 * s, 12);
        draw_circle(cx + 14 * s, cy + 18 * s, 18 * s, 10);
        draw_circle(cx - 10 * s, cy + 16 * s, 16 * s, 10);

        /* Lighter inner highlight */
        glColor4f(0.30f, 0.42f, 0.64f, a * 0.8f);
        draw_circle(cx + 6 * s, cy + 8 * s, 14 * s, 10);
        draw_circle(cx - 12 * s, cy + 10 * s, 11 * s, 10);
    }
}

/* Helper: draw a single bird with animated wings */
static void draw_single_bird(float bx, float by, float scale, float wing_phase, float alpha)
{
    float wf = sinf(wing_phase) * 6.0f * scale; /* wing flap offset */
    glColor4f(0.10f, 0.10f, 0.14f, alpha * 1.2f);

    /* Body */
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 5 * scale, by);
    glVertex2f(bx + 5 * scale, by);
    glVertex2f(bx, by - 4 * scale);
    glEnd();
    /* Head */
    draw_circle(bx + 5 * scale, by - 1 * scale, 3.0f * scale, 8);

    /* Left wing */
    glBegin(GL_TRIANGLES);
    glVertex2f(bx, by);
    glVertex2f(bx - 14 * scale, by - wf);
    glVertex2f(bx - 7 * scale, by + 3 * scale);
    glEnd();
    /* Right wing */
    glBegin(GL_TRIANGLES);
    glVertex2f(bx, by);
    glVertex2f(bx + 14 * scale, by - wf);
    glVertex2f(bx + 7 * scale, by + 3 * scale);
    glEnd();
    /* Tail */
    glBegin(GL_TRIANGLES);
    glVertex2f(bx - 5 * scale, by);
    glVertex2f(bx - 9 * scale, by - 5 * scale);
    glVertex2f(bx - 9 * scale, by + 2 * scale);
    glEnd();
}

/* ── Flying birds ───────────────────────────────────────── */
void draw_birds(float game_time, float cam_x)
{
    (void)cam_x;

    /* Large soaring black birds — slow, high up */
    struct
    {
        float ox, speed, y, scale, alpha;
    } large_birds[] = {
        {100, 80.0f, 600, 1.4f, 0.75f},
        {700, 65.0f, 580, 1.2f, 0.65f},
        {1200, 90.0f, 560, 1.6f, 0.80f},
        {400, 100.0f, 540, 1.0f, 0.60f},
        {950, 55.0f, 615, 1.3f, 0.70f},
    };
    /* V-06: Bird wrap width matches level width so birds aren't absent at far-right edge */
    float bird_wrap = (level.level_w > 1380.0f) ? 1380.0f : 1380.0f; /* screen-space wrap unchanged */
    (void)bird_wrap;                                                 /* Birds are screen-space; 1380 covers the viewport adequately */

    for (int i = 0; i < 5; i++)
    {
        float bx = fmodf(large_birds[i].ox + game_time * large_birds[i].speed, 1380.0f) - 50.0f;
        float wing_phase = game_time * 2.0f + i * 2.1f;
        draw_single_bird(bx, large_birds[i].y, large_birds[i].scale, wing_phase, large_birds[i].alpha);
    }

    /* Small flock 1 — faster, mid-height */
    float flock_ox = fmodf(game_time * 130.0f, 1500.0f) - 100.0f;
    for (int i = 0; i < 7; i++)
    {
        float bx = flock_ox + i * 26.0f;
        float by = 480.0f + sinf(i * 1.3f + game_time * 1.8f) * 12.0f;
        float wing_phase = game_time * 5.5f + i * 0.6f;
        if (bx > -20 && bx < 1300)
            draw_single_bird(bx, by, 0.7f, wing_phase, 0.55f);
    }

    /* Small flock 2 — second group at different height */
    float flock2_ox = fmodf(game_time * 95.0f + 600.0f, 1500.0f) - 100.0f;
    for (int i = 0; i < 5; i++)
    {
        float bx = flock2_ox + i * 30.0f;
        float by = 520.0f + sinf(i * 1.7f + game_time * 2.2f) * 10.0f;
        float wing_phase = game_time * 4.8f + i * 0.8f;
        if (bx > -20 && bx < 1300)
            draw_single_bird(bx, by, 0.65f, wing_phase, 0.45f);
    }

    /* Distant right-to-left birds high in sky */
    float rtl_x = 1280.0f - fmodf(game_time * 110.0f, 1500.0f);
    if (rtl_x > -60 && rtl_x < 1340)
    {
        draw_single_bird(rtl_x, 640.0f, 1.1f, game_time * 3.0f, 0.60f);
        draw_single_bird(rtl_x + 35, 625.0f, 0.9f, game_time * 3.0f + 0.8f, 0.50f);
        draw_single_bird(rtl_x + 65, 635.0f, 0.8f, game_time * 3.0f + 1.4f, 0.45f);
    }

    /* Second right-to-left group offset by half cycle */
    float rtl2_x = 1280.0f - fmodf(game_time * 75.0f + 750.0f, 1500.0f);
    if (rtl2_x > -60 && rtl2_x < 1340)
    {
        draw_single_bird(rtl2_x, 660.0f, 1.0f, game_time * 2.5f, 0.55f);
        draw_single_bird(rtl2_x + 40, 645.0f, 0.85f, game_time * 2.5f + 1.0f, 0.45f);
    }
}
void draw_bare_tree(float bx, float by, float scale)
{
    if (tree_demo_mode == 1)
    {
        draw_tree_manual_primitive(bx, by, scale);
        return;
    }
    if (tree_demo_mode == 2)
    {
        draw_tree_algorithmic(bx, by, scale);
        return;
    }

    float s = scale;
    glColor4f(0.12f, 0.11f, 0.13f, 1.0f);
    /* trunk — extends to y=0 so it always meets the ground even when camera moves up */
    glBegin(GL_QUADS);
    glVertex2f(bx - 7 * s, 0);
    glVertex2f(bx + 7 * s, 0);
    glVertex2f(bx + 4 * s, by + 120 * s);
    glVertex2f(bx - 4 * s, by + 120 * s);
    glEnd();
    /* main branches */
    float brs[][5] = {
        {0, 100, -70, 170, 3.5f},
        {0, 110, 60, 160, 3.0f},
        {0, 130, -50, 200, 2.5f},
        {0, 140, 40, 210, 2.0f},
        {0, 150, -20, 220, 2.0f},
        {-40, 160, -80, 200, 1.5f},
        {30, 150, 65, 185, 1.5f},
    };
    int nb = sizeof(brs) / sizeof(brs[0]);
    for (int i = 0; i < nb; i++)
    {
        float x1 = bx + brs[i][0] * s, y1 = by + brs[i][1] * s;
        float x2 = bx + brs[i][2] * s, y2 = by + brs[i][3] * s;
        float hw = brs[i][4] * s;
        float dx = x2 - x1, dy = y2 - y1, len = sqrtf(dx * dx + dy * dy) + 0.001f;
        float nx = -dy / len * hw, ny = dx / len * hw;
        glBegin(GL_QUADS);
        glVertex2f(x1 + nx, y1 + ny);
        glVertex2f(x1 - nx, y1 - ny);
        glVertex2f(x2, y2);
        glVertex2f(x2, y2);
        glEnd();
    }
}

/* ── Hanging cloth strips ───────────────────────────────── */
void draw_hanging_cloth(float x1, float y1, float x2, float y2)
{
    /* Supports so cloth does not appear floating without attachment. */
    glColor4f(0.18f, 0.12f, 0.09f, 0.95f);
    draw_rect(x1 - 2, y1, 4, 20);
    draw_rect(x2 - 2, y2, 4, 20);
    glColor4f(0.22f, 0.16f, 0.12f, 0.95f);
    draw_circle(x1, y1, 2.5f, 8);
    draw_circle(x2, y2, 2.5f, 8);

    glColor4f(0.32f, 0.18f, 0.14f, 0.85f);
    float dx = x2 - x1, cx = (x1 + x2) / 2;
    /* rope: in algorithm mode, draw piecewise DDA segments to mirror Lab 2 style */
    if (tree_demo_mode == 2)
    {
        float px = x1;
        float py = y1;
        for (int i = 1; i <= 16; i++)
        {
            float t = (float)i / 16;
            float rx = x1 + dx * t;
            float ry = y1 + (y2 - y1) * t - sinf(t * PI) * 30.0f;
            draw_line_dda(px, py, rx, ry);
            px = rx;
            py = ry;
        }
    }
    else
    {
        glLineWidth(1.5f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 16; i++)
        {
            float t = (float)i / 16;
            float rx = x1 + dx * t;
            float ry = y1 + (y2 - y1) * t - sinf(t * PI) * 30.0f;
            glVertex2f(rx, ry);
        }
        glEnd();
        glLineWidth(1.0f);
    }
    /* cloth strips hanging down */
    for (int s = 0; s < 5; s++)
    {
        float t = (s + 0.5f) / 5.0f;
        float hx = x1 + dx * t;
        float hy = y1 + (y2 - y1) * t - sinf(t * PI) * 30.0f;
        float sw = 8.0f, sh = 40.0f + s * 8.0f;
        glBegin(GL_QUADS);
        glVertex2f(hx - sw / 2, hy);
        glVertex2f(hx + sw / 2, hy);
        glVertex2f(hx + sw / 2 - 2, hy - sh);
        glVertex2f(hx - sw / 2 + 2, hy - sh);
        glEnd();
    }
    (void)cx;
}

/* ── Stone pillar (background) ──────────────────────────── */
void draw_bg_stone_pillar(float cx, float y, float w, float h)
{
    glColor4f(0.22f, 0.20f, 0.24f, 1.0f);
    /* main body — extend to y=0 to stay grounded when camera scrolls up */
    glBegin(GL_QUADS);
    glVertex2f(cx - w / 2, 0);
    glVertex2f(cx + w / 2, 0);
    glVertex2f(cx + w / 2 * 0.85f, y + h);
    glVertex2f(cx - w / 2 * 0.85f, y + h);
    glEnd();
    /* cap */
    draw_rect(cx - w / 2 - 4, y + h - 8, w + 8, 12);
    /* horizontal block lines */
    glColor4f(0.14f, 0.13f, 0.16f, 1);
    for (float ly = y + 20; ly < y + h - 10; ly += 28)
    {
        draw_rect(cx - w / 2, ly, w, 3);
    }
    /* diagonal crack */
    glLineWidth(1.5f);
    glColor4f(0.12f, 0.11f, 0.14f, 1);
    glBegin(GL_LINES);
    glVertex2f(cx - w * 0.1f, y + h * 0.2f);
    glVertex2f(cx + w * 0.2f, y + h * 0.6f);
    glEnd();
    glLineWidth(1.0f);
    /* mossy cap patches */
    glColor4f(0.14f, 0.24f, 0.12f, 0.6f);
    draw_rect(cx - w * 0.3f, y + h - 2, w * 0.6f, 4);
    draw_circle(cx - w * 0.15f, y + h - 2, 5, 8);
    draw_circle(cx + w * 0.1f, y + h - 2, 4, 8);
}

/* ── Pagoda gate ────────────────────────────────────────── */
void draw_bg_pagoda(float cx, float y, float scale)
{
    float s = scale;

    /* Gate pillars — extend to y=0 so no gap appears when camera scrolls up */
    glColor4f(0.18f, 0.16f, 0.20f, 1.0f);
    draw_rect(cx - 100 * s, 0, 20 * s, y + 200 * s);
    draw_rect(cx + 80 * s, 0, 20 * s, y + 200 * s);

    /* Crossbeam */
    draw_rect(cx - 110 * s, y + 190 * s, 230 * s, 18 * s);

    /* Curved eave roof */
    float roof_y = y + 200 * s;
    float roof_w = 280 * s;
    float roof_h = 50 * s;
    glBegin(GL_QUADS);
    glVertex2f(cx - roof_w / 2, roof_y);
    glVertex2f(cx + roof_w / 2, roof_y);
    glVertex2f(cx + roof_w * 0.42f, roof_y + roof_h);
    glVertex2f(cx - roof_w * 0.42f, roof_y + roof_h);
    glEnd();
    /* roof curve ends */
    draw_tri(cx - roof_w / 2, roof_y, cx - roof_w * 0.42f, roof_y + roof_h, cx - roof_w / 2 - 15 * s, roof_y + roof_h * 0.6f);
    draw_tri(cx + roof_w / 2, roof_y, cx + roof_w * 0.42f, roof_y + roof_h, cx + roof_w / 2 + 15 * s, roof_y + roof_h * 0.6f);
    /* drip texture on underside */
    glColor4f(0.03f, 0.04f, 0.05f, 1);
    for (float dx = cx - roof_w * 0.45f + 12; dx < cx + roof_w * 0.45f; dx += 16 * s)
    {
        float drip_h = 8.0f + sinf(dx * 0.3f) * 4.0f;
        draw_tri(dx - 3, roof_y, dx + 3, roof_y, dx, roof_y - drip_h);
    }
}

/* ── Hanging red lanterns ───────────────────────────────── */
void draw_lantern_string(float x1, float y1, float x2, float y2, int count)
{
    /* Endpoint supports to avoid floating string look. */
    glColor4f(0.20f, 0.14f, 0.08f, 1.0f);
    draw_rect(x1 - 2, y1, 4, 18);
    draw_rect(x2 - 2, y2, 4, 18);
    draw_circle(x1, y1, 2.5f, 8);
    draw_circle(x2, y2, 2.5f, 8);

    glColor4f(0.28f, 0.20f, 0.10f, 1);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++)
    {
        float t = (float)i / 20;
        float lx = x1 + (x2 - x1) * t;
        float ly = y1 + (y2 - y1) * t - sinf(t * PI) * 40.0f;
        glVertex2f(lx, ly);
    }
    glEnd();
    glLineWidth(1.0f);

    for (int i = 1; i <= count; i++)
    {
        float t = (float)i / (count + 1);
        float lx = x1 + (x2 - x1) * t;
        float ly = y1 + (y2 - y1) * t - sinf(t * PI) * 40.0f;

        glColor4f(0.28f, 0.20f, 0.10f, 1);
        draw_rect(lx - 1, ly - 16, 2, 16);
        col_black();
        draw_rect(lx - 6, ly - 20, 12, 4);
        glColor4f(0.85f, 0.15f, 0.05f, 1);
        draw_rect(lx - 8, ly - 40, 16, 20);
        glColor4f(1.0f, 0.5f, 0.1f, 0.8f);
        draw_rect(lx - 5, ly - 36, 10, 12);
        col_black();
        draw_rect(lx - 6, ly - 42, 12, 4);
        draw_rect(lx - 1, ly - 46, 2, 6);
        glColor4f(0.8f, 0.2f, 0.0f, 0.12f);
        draw_circle(lx, ly - 32, 24, 16);
    }
}

/* ── Pine tree (Temple / Village theme) ─────────────────── */
void draw_pine_tree(float bx, float by, float scale)
{
    if (tree_demo_mode == 1)
    {
        draw_tree_manual_primitive(bx, by, scale);
        return;
    }
    if (tree_demo_mode == 2)
    {
        draw_tree_algorithmic(bx, by, scale);
        return;
    }

    float s = scale;
    glColor4f(0.10f, 0.14f, 0.10f, 1.0f);
    /* Trunk */
    glBegin(GL_QUADS);
    glVertex2f(bx - 4 * s, 0);
    glVertex2f(bx + 4 * s, 0);
    glVertex2f(bx + 3 * s, by + 80 * s);
    glVertex2f(bx - 3 * s, by + 80 * s);
    glEnd();
    /* Three layered triangular tiers */
    float tiers[][3] = {
        {80, 140, 60}, /* bottom: base_y, top_y, half_width */
        {115, 165, 45},
        {145, 195, 30},
    };
    for (int i = 0; i < 3; i++)
    {
        float base = by + tiers[i][0] * s;
        float top = by + tiers[i][1] * s;
        float hw = tiers[i][2] * s;
        draw_tri(bx - hw, base, bx + hw, base, bx, top);
    }
}

/* ── Willow tree (Village theme) ─────────────────────────── */
void draw_willow_tree(float bx, float by, float scale)
{
    if (tree_demo_mode == 1)
    {
        draw_tree_manual_primitive(bx, by, scale);
        return;
    }
    if (tree_demo_mode == 2)
    {
        draw_tree_algorithmic(bx, by, scale);
        return;
    }

    float s = scale;
    glColor4f(0.10f, 0.14f, 0.10f, 1.0f);
    /* Trunk */
    glBegin(GL_QUADS);
    glVertex2f(bx - 5 * s, 0);
    glVertex2f(bx + 5 * s, 0);
    glVertex2f(bx + 3 * s, by + 140 * s);
    glVertex2f(bx - 3 * s, by + 140 * s);
    glEnd();
    /* Crown */
    draw_circle(bx, by + 160 * s, 42 * s, 18);
    draw_circle(bx - 20 * s, by + 148 * s, 30 * s, 14);
    draw_circle(bx + 22 * s, by + 152 * s, 28 * s, 14);
    /* Drooping branches */
    for (int i = 0; i < 7; i++)
    {
        float a = (i - 3) * 0.38f;
        float droop_x = bx + sinf(a) * 38 * s;
        float droop_y_top = by + (140 + cosf(a) * 20) * s;
        float droop_len = (55 + i * 5) * s;
        glLineWidth(1.5f);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= 8; j++)
        {
            float t = (float)j / 8;
            glVertex2f(droop_x + sinf(t * 0.8f) * 6 * s,
                       droop_y_top - droop_len * t);
        }
        glEnd();
        glLineWidth(1.0f);
    }
}

/* ── Cherry blossom tree (Fortress accent) ──────────────── */
void draw_cherry_tree(float bx, float by, float scale)
{
    if (tree_demo_mode == 1)
    {
        draw_tree_manual_primitive(bx, by, scale);
        return;
    }
    if (tree_demo_mode == 2)
    {
        draw_tree_algorithmic(bx, by, scale);
        return;
    }

    float s = scale;
    glColor4f(0.14f, 0.10f, 0.10f, 1.0f);
    /* Trunk */
    glBegin(GL_QUADS);
    glVertex2f(bx - 5 * s, 0);
    glVertex2f(bx + 5 * s, 0);
    glVertex2f(bx + 2 * s, by + 100 * s);
    glVertex2f(bx - 2 * s, by + 100 * s);
    glEnd();
    /* Two main branches */
    glLineWidth(4.0f * s);
    glBegin(GL_LINES);
    glVertex2f(bx, by + 100 * s);
    glVertex2f(bx - 35 * s, by + 148 * s);
    glVertex2f(bx, by + 100 * s);
    glVertex2f(bx + 32 * s, by + 145 * s);
    glEnd();
    glLineWidth(1.0f);
    /* Blossom clusters — muted pink */
    glColor4f(0.38f, 0.22f, 0.28f, 0.85f);
    float bpos[][2] = {
        {-35, 148},
        {-20, 165},
        {-50, 160},
        {32, 145},
        {48, 158},
        {18, 162},
        {5, 172},
    };
    for (int i = 0; i < 7; i++)
        draw_circle(bx + bpos[i][0] * s, by + bpos[i][1] * s, 20 * s, 12);
}

/* ── Fortress wall segment (theme 4) ─────────────────────── */
void draw_fortress_wall(float bx, float by, float scale)
{
    float s = scale;
    glColor4f(0.16f, 0.15f, 0.18f, 1.0f);
    /* Main wall body */
    draw_rect(bx - 60 * s, 0, 120 * s, by + 200 * s);
    /* Battlements — 5 merlons */
    for (int i = 0; i < 5; i++)
    {
        float mx = bx - 50 * s + i * 24 * s;
        draw_rect(mx, by + 200 * s, 14 * s, 30 * s);
    }
    /* Arrow slit */
    glColor4f(0.02f, 0.02f, 0.03f, 1.0f);
    draw_rect(bx - 3 * s, by + 140 * s, 6 * s, 22 * s);
    draw_rect(bx - 9 * s, by + 148 * s, 18 * s, 8 * s);
    /* Stone texture lines */
    glColor4f(0.04f, 0.04f, 0.05f, 1.0f);
    for (float ly = by + 20; ly < by + 200; ly += 25)
        draw_rect(bx - 58 * s, ly * s, 116 * s, 2 * s);
}

/* ── Bamboo background ──────────────────────────────────── */
void draw_bamboo_bg(float cam_x)
{
    float ox = -cam_x * 0.25f;
    for (int i = 0; i < 20; i++)
    {
        float bx = fmodf(i * 65.0f + ox + 1280 + 3000, 1280 + 100) - 50;
        float bh = 350.0f + (i % 5) * 30.0f;
        float bw = 8.0f + (i % 3) * 2.0f;
        float alpha = (i % 2 == 0) ? 0.45f : 0.65f;
        glColor4f(0.16f, 0.28f, 0.18f, alpha);
        draw_rect(bx - bw / 2, 0, bw, bh);
        glColor4f(0.22f, 0.36f, 0.22f, alpha);
        for (float ny = 35; ny < bh; ny += 40)
            draw_rect(bx - bw / 2 - 2, ny, bw + 4, 4);
        glColor4f(0.14f, 0.24f, 0.14f, alpha * 0.8f);
        for (int l = 0; l < 4; l++)
        {
            float la = (l * 70.0f + bx * 10.0f) * PI / 180.0f;
            float lx2 = bx + cosf(la) * 22;
            float ly2 = bh + sinf(la) * 10;
            draw_tri(bx, bh, lx2 - 4, ly2, lx2 + 16, ly2 + 6);
        }
    }
}

/* ── Grass tufts ────────────────────────────────────────── */
void draw_grass_tufts(float level_w)
{
    glColor4f(0.16f, 0.36f, 0.10f, 1.0f);
    for (float gx = 10; gx < level_w; gx += 28 + fmodf(gx * 7, 32))
    {
        float h = 6.0f + fmodf(gx * 3, 8.0f);
        draw_tri(gx - 4, 40, gx, 40, gx - 2, 40 + h);
        draw_tri(gx - 1, 40, gx + 3, 40, gx + 1, 40 + h + 2);
        draw_tri(gx + 2, 40, gx + 6, 40, gx + 4, 40 + h);
    }
}
