#include "renderer.h"
#include "game.h"
#include <stdio.h>

/* draw_shuriken is shared - forward declare */
void draw_shuriken(float cx, float cy, float angle);

void draw_hud(int health, int max_health, int gems, int gold, int stars, int shurikens, float game_time)
{

    /* ── AVATAR ring ──────────────────────────────────── */
    glColor4f(0.40f, 0.40f, 0.50f, 1);
    draw_circle(50, 670, 40, 24);
    glColor4f(0.10f, 0.10f, 0.14f, 1);
    draw_circle(50, 670, 33, 24);
    /* mini player silhouette */
    glColor4f(0.52f, 0.52f, 0.58f, 1);
    draw_circle(50, 674, 12, 16);
    draw_rect(42, 658, 16, 14);
    col_black();
    glPushMatrix();
    glTranslatef(47, 683, 0);
    glRotatef(-25, 0, 0, 1);
    draw_rect(-1, 0, 3, 9);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(53, 683, 0);
    glRotatef(30, 0, 0, 1);
    draw_rect(-1, 0, 3, 8);
    glPopMatrix();
    col_red(1.0f);
    draw_tri(44, 675, 48, 675, 46, 671);
    draw_tri(52, 675, 56, 675, 54, 671);
    col_black();

    /* ── HEARTS ──────────────────────────────────────── */
    float heart_x = 100;
    for (int i = 0; i < max_health; i++)
    {
        float hx = heart_x + i * 20.0f, hy = 700.0f;
        if (i < health)
            glColor4f(0.92f, 0.12f, 0.12f, 1);
        else
            glColor4f(0.28f, 0.10f, 0.10f, 0.5f);
        draw_circle(hx - 3, hy, 5, 10);
        draw_circle(hx + 3, hy, 5, 10);
        draw_tri(hx - 7, hy - 2, hx + 7, hy - 2, hx, hy - 11);
    }

    /* ── GEM ICON + COUNT ─────────────────────────────── */
    float gix = 100, giy = 680;
    glColor4f(0.25f, 0.55f, 1.0f, 1);
    draw_tri(gix, giy + 7, gix + 6, giy + 3, gix + 6, giy - 3);
    draw_tri(gix, giy - 7, gix + 6, giy - 3, gix + 6, giy + 3);
    draw_tri(gix, giy + 7, gix - 6, giy + 3, gix - 6, giy - 3);
    draw_tri(gix, giy - 7, gix - 6, giy - 3, gix - 6, giy + 3);
    col_white(1.0f);
    char gem_str[24];
    sprintf(gem_str, "%d", gems);
    glRasterPos2f(gix + 10, giy - 6);
    for (char *c = gem_str; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* ── GOLD COIN ICON + COUNT ───────────────────────── */
    float cix = 100, ciy = 660;
    glColor4f(1.0f, 0.78f, 0.1f, 1);
    draw_circle(cix, ciy, 7, 12);
    glColor4f(0.8f, 0.55f, 0.0f, 1);
    draw_circle(cix, ciy, 5, 12);
    col_white(1.0f);
    char gold_str[24];
    sprintf(gold_str, "%d", gold);
    glRasterPos2f(cix + 10, ciy - 6);
    for (char *c = gold_str; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* ── STARS (0-3 earned) ───────────────────────────── */
    for (int i = 0; i < 3; i++)
    {
        float sx = 20.0f + i * 22.0f, sy = 636.0f;
        if (i < stars)
        {
            glColor4f(1.0f, 0.88f, 0.05f, 1);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(sx, sy);
            for (int j = 0; j <= 10; j++)
            {
                float r = (j % 2 == 0) ? 9.0f : 4.0f, a = (j * 36.0f - 90) * PI / 180.0f;
                glVertex2f(sx + cosf(a) * r, sy + sinf(a) * r);
            }
            glEnd();
        }
        else
        {
            glColor4f(0.30f, 0.30f, 0.30f, 0.5f);
            draw_circle(sx, sy, 9, 12);
        }
    }

    /* ── BOTTOM-LEFT SHURIKENS ────────────────────────── */
    /* Left-throw shuriken */
    draw_shuriken(55, 50, game_time * -180.0f);
    /* Right-throw shuriken */
    draw_shuriken(120, 50, game_time * -180.0f);
    /* Shuriken count - show as INF (GLUT bitmap fonts lack the ∞ glyph) */
    col_white(1.0f);
    glRasterPos2f(88, 80);
    const char *inf_str = "INF";
    for (const char *c = inf_str; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* Labels */
    glColor4f(0.6f, 0.6f, 0.65f, 0.8f);
    glRasterPos2f(40, 20);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '<');
    glRasterPos2f(112, 20);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '>');

    /* ── BOTTOM-RIGHT SKILL BUTTONS (2x2 grid) ─────────── */
    float btn_cx[4] = {1130, 1210, 1130, 1210};
    float btn_cy[4] = {130, 130, 60, 60};

    for (int i = 0; i < 4; i++)
    {
        glColor4f(0.42f, 0.42f, 0.50f, 0.90f);
        draw_circle(btn_cx[i], btn_cy[i], 36, 20);
        glColor4f(0.18f, 0.18f, 0.24f, 0.95f);
        draw_circle(btn_cx[i], btn_cy[i], 30, 20);
    }

    /* Button 0: Ninja hat icon */
    glColor4f(0.55f, 0.50f, 0.45f, 1);
    draw_circle(btn_cx[0], btn_cy[0] + 4, 12, 16);
    draw_rect(btn_cx[0] - 14, btn_cy[0] + 14, 28, 3);
    draw_tri(btn_cx[0] - 10, btn_cy[0] + 17, btn_cx[0] + 10, btn_cy[0] + 17, btn_cx[0], btn_cy[0] + 26);

    /* Button 1: Sword slash icon */
    glColor4f(0.8f, 0.1f, 0.1f, 1);
    for (int j = 0; j < 4; j++)
        draw_rect_rot(btn_cx[1], btn_cy[1], 3, 28, j * 45.0f + 10.0f);

    /* Button 2: Dash icon */
    glColor4f(0.9f, 0.7f, 0.1f, 1);
    draw_rect(btn_cx[2] - 22, btn_cy[2] - 2, 28, 4);
    draw_rect(btn_cx[2] - 16, btn_cy[2] + 2, 20, 3);
    draw_rect(btn_cx[2] - 10, btn_cy[2] + 5, 12, 2);

    /* Button 3: Jump burst icon */
    glColor4f(0.7f, 0.7f, 0.75f, 1);
    for (int j = 0; j < 6; j++)
    {
        float a = j * 60.0f * PI / 180.0f;
        draw_rect_rot(btn_cx[3] + cosf(a) * 14, btn_cy[3] + sinf(a) * 14, 2, 10, j * 60.0f);
    }
    draw_circle(btn_cx[3], btn_cy[3], 7, 10);

    /* Reset colour after button loop so any new element added below starts clean */
    glColor4f(1, 1, 1, 1);

    /* ── TOP-RIGHT SETTINGS GEAR ──────────────────────── */
    glColor4f(0.7f, 0.7f, 0.75f, 0.85f);
    draw_circle(1240, 690, 18, 24);
    col_black();
    draw_circle(1240, 690, 12, 24);
    glColor4f(0.7f, 0.7f, 0.75f, 0.85f);
    for (int i = 0; i < 8; i++)
    {
        float a = i * 45.0f * PI / 180.0f;
        draw_rect_rot(1240 + cosf(a) * 18, 690 + sinf(a) * 18, 4, 8, i * 45.0f);
    }
}

/* ── Main menu screen ───────────────────────────────────── */
void draw_main_menu(float t)
{
    /* dark overlay */
    glColor4f(0.02f, 0.04f, 0.08f, 0.9f);
    draw_rect(0, 0, 1280, 720);

    /* title */
    col_white(1.0f);
    glRasterPos2f(530, 440);
    const char *title = "NINJA  SHADOW";
    for (const char *c = title; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    /* subtitle */
    glColor4f(0.6f, 0.6f, 0.7f, 0.8f);
    glRasterPos2f(520, 400);
    const char *sub = "Press ENTER to Start";
    for (const char *c = sub; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* controls */
    glColor4f(0.5f, 0.5f, 0.55f, 0.6f);
    const char *lines[] = {
        "Arrow Keys / WASD - Move",
        "Space / Up - Jump (double jump in air)",
        "Z / X - Attack / Throw Shuriken",
        "Left Shift - Dash",
        "P / Esc - Pause"};
    for (int i = 0; i < 5; i++)
    {
        glRasterPos2f(470, 350 - i * 22.0f);
        for (const char *c = lines[i]; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    /* pulsing start prompt */
    float alpha = 0.5f + sinf(t * 2.5f) * 0.4f;
    glColor4f(1.0f, 0.85f, 0.1f, alpha);
    glRasterPos2f(550, 360);
    const char *go = "[ ENTER ]";
    for (const char *c = go; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

/* ── Pause overlay ──────────────────────────────────────── */
void draw_pause_overlay(void)
{
    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);
    draw_rect(0, 0, 1280, 720);

    col_white(1.0f);
    glRasterPos2f(575, 420);
    const char *p = "PAUSED";
    for (const char *c = p; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    glColor4f(0.7f, 0.7f, 0.8f, 0.9f);
    glRasterPos2f(530, 380);
    const char *r = "Press P or Esc to Resume";
    for (const char *c = r; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

/* ── Game over screen ───────────────────────────────────── */
void draw_game_over_screen(void)
{
    glColor4f(0.04f, 0.0f, 0.0f, 0.85f);
    draw_rect(0, 0, 1280, 720);

    glColor4f(0.9f, 0.1f, 0.1f, 1.0f);
    glRasterPos2f(540, 420);
    const char *go = "GAME  OVER";
    for (const char *c = go; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    glColor4f(0.7f, 0.5f, 0.5f, 0.8f);
    glRasterPos2f(530, 375);
    const char *r = "Press ENTER to Retry";
    for (const char *c = r; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

/* ── Level complete screen ──────────────────────────────── */
void draw_level_complete(int stars)
{
    glColor4f(0.02f, 0.06f, 0.04f, 0.85f);
    draw_rect(0, 0, 1280, 720);

    glColor4f(0.8f, 0.95f, 0.3f, 1.0f);
    glRasterPos2f(490, 450);
    const char *lc = "LEVEL  COMPLETE!";
    for (const char *c = lc; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    /* stars earned */
    for (int i = 0; i < 3; i++)
    {
        float sx = 560.0f + i * 55.0f, sy = 400.0f;
        if (i < stars)
        {
            glColor4f(1.0f, 0.88f, 0.05f, 1);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(sx, sy);
            for (int j = 0; j <= 10; j++)
            {
                float r = (j % 2 == 0) ? 22.0f : 10.0f, a = (j * 36.0f - 90) * PI / 180.0f;
                glVertex2f(sx + cosf(a) * r, sy + sinf(a) * r);
            }
            glEnd();
        }
        else
        {
            glColor4f(0.3f, 0.3f, 0.3f, 0.5f);
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(sx, sy);
            for (int j = 0; j <= 10; j++)
            {
                float r = (j % 2 == 0) ? 22.0f : 10.0f, a = (j * 36.0f - 90) * PI / 180.0f;
                glVertex2f(sx + cosf(a) * r, sy + sinf(a) * r);
            }
            glEnd();
        }
    }

    glColor4f(0.7f, 0.8f, 0.7f, 0.8f);
    glRasterPos2f(530, 350);
    const char *n = "Press ENTER for Next Level";
    for (const char *c = n; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}
