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

    /* ── ENEMY DAMAGE BAR (enemy-only) ───────────────── */
    {
        float bar_x = 100.0f;
        float bar_y = 676.0f;
        float bar_w = 150.0f;
        float bar_h = 8.0f;
        float t = enemy_damage_bar / ENEMY_DAMAGE_BAR_MAX;
        if (t < 0.0f)
            t = 0.0f;
        if (t > 1.0f)
            t = 1.0f;

        glColor4f(0.16f, 0.10f, 0.10f, 0.85f);
        draw_rect(bar_x, bar_y, bar_w, bar_h);

        if (enemy_damage_bar_flash > 0.0f)
            glColor4f(1.0f, 0.30f, 0.20f, 0.95f);
        else
            glColor4f(0.95f, 0.55f, 0.20f, 0.90f);
        draw_rect(bar_x, bar_y, bar_w * t, bar_h);

        glColor4f(0.80f, 0.80f, 0.85f, 0.75f);
        glRasterPos2f(bar_x, bar_y - 12.0f);
        {
            const char *lbl = "Enemy Damage Bar";
            for (const char *c = lbl; *c; c++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
        }
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
    draw_shuriken(55, 50, game_time * -180.0f);
    draw_shuriken(120, 50, game_time * -180.0f);
    /* Shuriken count — unlimited, show INF */
    col_white(1.0f);
    glRasterPos2f(88, 80);
    {
        const char *inf_str = "INF";
        for (const char *c = inf_str; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    /* Labels */
    glColor4f(0.6f, 0.6f, 0.65f, 0.8f);
    glRasterPos2f(40, 20);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '<');
    glRasterPos2f(112, 20);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '>');

    /* ── BOTTOM-RIGHT SKILL BUTTONS (2x2 grid) ─────────── */
    float btn_cx[4] = {1130, 1210, 1130, 1210};
    float btn_cy[4] = {130, 130, 60, 60};

    /* Determine which buttons are active from player state */
    int btn_active[4] = {0, 0, 0, 0};
    /* Button 0: Shuriken — lit while shuriken is in flight */
    for (int i = 0; i < level.proj_count; i++)
        if (level.projs[i].active && level.projs[i].is_shuriken && level.projs[i].from_player)
        {
            btn_active[0] = 1;
            break;
        }
    /* Button 1: Sword attack */
    btn_active[1] = (player.anim.state == ANIM_ATTACK);
    /* Button 2: Dash */
    btn_active[2] = (player.dash_timer > 0);
    /* Button 3: Jump / in air */
    btn_active[3] = (!player.on_ground && player.vy > 0);

    for (int i = 0; i < 4; i++)
    {
        if (btn_active[i])
        {
            /* Active glow: bright outer ring */
            glColor4f(1.0f, 0.85f, 0.15f, 0.80f);
            draw_circle(btn_cx[i], btn_cy[i], 38, 20);
            glColor4f(0.60f, 0.50f, 0.10f, 0.60f);
            draw_circle(btn_cx[i], btn_cy[i], 34, 20);
            glColor4f(0.26f, 0.22f, 0.08f, 0.95f);
            draw_circle(btn_cx[i], btn_cy[i], 30, 20);
        }
        else
        {
            glColor4f(0.42f, 0.42f, 0.50f, 0.90f);
            draw_circle(btn_cx[i], btn_cy[i], 36, 20);
            glColor4f(0.18f, 0.18f, 0.24f, 0.95f);
            draw_circle(btn_cx[i], btn_cy[i], 30, 20);
        }
    }

    /* Button 0: Shuriken (star) icon */
    {
        float a0 = btn_active[0] ? 1.0f : 0.65f;
        glColor4f(0.85f, 0.75f, 0.20f, a0);
        /* Small 8-pointed star */
        for (int j = 0; j < 4; j++)
            draw_rect_rot(btn_cx[0], btn_cy[0], 4, 22, j * 45.0f);
        draw_circle(btn_cx[0], btn_cy[0], 6, 12);
    }

    /* Button 1: Sword slash icon */
    {
        float a1 = btn_active[1] ? 1.0f : 0.80f;
        glColor4f(0.8f, 0.1f, 0.1f, a1);
        for (int j = 0; j < 4; j++)
            draw_rect_rot(btn_cx[1], btn_cy[1], 3, 28, j * 45.0f + 10.0f);
        if (btn_active[1])
        {
            glColor4f(1.0f, 0.6f, 0.1f, 0.7f);
            draw_circle(btn_cx[1], btn_cy[1], 16, 16);
        }
    }

    /* Button 2: Dash icon */
    {
        float a2 = btn_active[2] ? 1.0f : 0.80f;
        glColor4f(0.9f, 0.7f, 0.1f, a2);
        draw_rect(btn_cx[2] - 22, btn_cy[2] - 2, 28, 4);
        draw_rect(btn_cx[2] - 16, btn_cy[2] + 2, 20, 3);
        draw_rect(btn_cx[2] - 10, btn_cy[2] + 5, 12, 2);
        if (btn_active[2])
        {
            /* Speed lines */
            glColor4f(1.0f, 0.9f, 0.3f, 0.6f);
            for (int j = 0; j < 3; j++)
                draw_rect(btn_cx[2] - 22, btn_cy[2] - 5 + j * 6, 10 + j * 4, 2);
        }
    }

    /* Button 3: Jump burst icon */
    {
        float a3 = btn_active[3] ? 1.0f : 0.70f;
        glColor4f(0.7f, 0.7f, 0.75f, a3);
        for (int j = 0; j < 6; j++)
        {
            float a = j * 60.0f * PI / 180.0f;
            draw_rect_rot(btn_cx[3] + cosf(a) * 14, btn_cy[3] + sinf(a) * 14, 2, 10, j * 60.0f);
        }
        draw_circle(btn_cx[3], btn_cy[3], 7, 10);
        if (btn_active[3])
        {
            glColor4f(0.5f, 0.8f, 1.0f, 0.5f);
            draw_circle(btn_cx[3], btn_cy[3], 20, 16);
        }
    }

    /* Button labels below each button */
    glColor4f(0.55f, 0.55f, 0.60f, 0.70f);
    const char *btn_labels[] = {"SHURKN", "SWORD", "DASH", "JUMP"};
    float lbl_offsets[] = {-14, -10, -10, -10};
    for (int i = 0; i < 4; i++)
    {
        glRasterPos2f(btn_cx[i] + lbl_offsets[i], btn_cy[i] - 44);
        for (const char *c = btn_labels[i]; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
    }

    /* Reset colour after button loop so any new element added below starts clean */
    glColor4f(1, 1, 1, 1);

    /* ── I-05: LEVEL PROGRESS BAR (top, centred) ─────── */
    {
        float bar_w = 400.0f, bar_h = 6.0f;
        float bar_x = (1280.0f - bar_w) * 0.5f;
        float bar_y = 712.0f; /* just below top edge */
        float progress = (level.level_w > 0)
                             ? (player.x / level.level_w)
                             : 0.0f;
        if (progress > 1.0f)
            progress = 1.0f;
        /* Background track */
        glColor4f(0.25f, 0.25f, 0.30f, 0.70f);
        draw_rect(bar_x, bar_y, bar_w, bar_h);
        /* Filled portion */
        glColor4f(0.55f, 0.85f, 0.35f, 0.85f);
        draw_rect(bar_x, bar_y, bar_w * progress, bar_h);
        /* Small player marker */
        glColor4f(1.0f, 0.90f, 0.20f, 0.95f);
        draw_circle(bar_x + bar_w * progress, bar_y + bar_h * 0.5f, 4.0f, 8);
    }

    /* ── HEALTH-PENALTY TOAST ────────────────────────── */
    if (penalty_toast_timer > 0)
    {
        float alpha = penalty_toast_timer > 0.5f ? 1.0f : penalty_toast_timer * 2.0f;
        glColor4f(1.0f, 0.25f, 0.25f, alpha);
        glRasterPos2f(520.0f, 630.0f);
        const char *msg = "Restarting... (-1 max heart)";
        for (const char *c = msg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    /* ── I-03: COMBO COUNTER ─────────────────────────── */
    if (combo_count >= 2)
    {
        char combo_str[32];
        sprintf(combo_str, "x%d COMBO!", combo_count);
        /* Fade out as timer approaches 0 */
        float alpha = combo_timer / 2.0f;
        if (alpha > 1.0f)
            alpha = 1.0f;
        /* Pulsing scale effect using combo_timer */
        float pulse = 1.0f + 0.1f * sinf(combo_timer * 15.0f);
        (void)pulse; /* used for visual cue via colour intensity */
        glColor4f(1.0f, 0.80f * alpha + 0.20f, 0.10f, alpha);
        glRasterPos2f(560.0f, 670.0f);
        for (const char *c = combo_str; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    /* ── TOP-RIGHT LEVEL TIMER ───────────────────────── */
    {
        int mins = (int)(level_time / 60);
        int secs = (int)level_time % 60;
        char timer_buf[16];
        sprintf(timer_buf, "%d:%02d", mins, secs);
        glColor4f(0.70f, 0.70f, 0.80f, 0.75f);
        glRasterPos2f(1180.0f, 700.0f);
        for (const char *c = timer_buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

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
    glRasterPos2f(575, 460);
    const char *p = "PAUSED";
    for (const char *c = p; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    glColor4f(0.7f, 0.7f, 0.8f, 0.9f);
    glRasterPos2f(530, 420);
    const char *r = "Press P or Esc to Resume";
    for (const char *c = r; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* I-09: Level stats panel */
    glColor4f(0.20f, 0.22f, 0.30f, 0.80f);
    draw_rect(440, 290, 400, 110);
    glColor4f(0.40f, 0.44f, 0.60f, 0.50f);
    draw_rect(440, 400, 400, 2); /* top border */
    draw_rect(440, 290, 400, 2); /* bottom border */

    glColor4f(0.70f, 0.75f, 0.85f, 0.85f);
    /* Enemies defeated */
    {
        char buf[48];
        sprintf(buf, "Enemies Defeated:  %d", enemies_defeated);
        glRasterPos2f(460, 380);
        for (const char *c = buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    /* Stars collected this level */
    {
        int star_count = 0;
        for (int i = 0; i < level.coll_count; i++)
            if (level.colls[i].is_star && level.colls[i].collected)
                star_count++;
        char buf[48];
        sprintf(buf, "Stars Collected:   %d / 3", star_count);
        glRasterPos2f(460, 358);
        for (const char *c = buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    /* Time elapsed in level */
    {
        int mins = (int)(level_time / 60);
        int secs = (int)level_time % 60;
        char buf[48];
        sprintf(buf, "Time:              %d:%02d", mins, secs);
        glRasterPos2f(460, 336);
        for (const char *c = buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    /* Gems collected */
    {
        char buf[48];
        sprintf(buf, "Gems:              %d", gems_collected);
        glRasterPos2f(460, 314);
        for (const char *c = buf; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
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
    const char *r = "Press ENTER to Restart";
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

/* ── Settings overlay ───────────────────────────────────── */
void draw_settings_overlay(void)
{
    /* Dark panel background */
    glColor4f(0.04f, 0.06f, 0.12f, 0.92f);
    draw_rect(0, 0, 1280, 720);

    /* Panel border */
    glColor4f(0.40f, 0.50f, 0.70f, 0.60f);
    draw_rect(230, 100, 820, 520);
    glColor4f(0.06f, 0.09f, 0.18f, 1.0f);
    draw_rect(234, 104, 812, 512);

    /* Title */
    glColor4f(0.85f, 0.88f, 1.0f, 1.0f);
    glRasterPos2f(560, 590);
    const char *title = "SETTINGS";
    for (const char *c = title; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    /* Divider */
    glColor4f(0.35f, 0.45f, 0.65f, 0.5f);
    draw_rect(260, 572, 760, 2);

    /* Controls section header */
    glColor4f(0.70f, 0.80f, 1.0f, 0.9f);
    glRasterPos2f(280, 548);
    const char *ch = "Controls";
    for (const char *c = ch; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* Control bindings */
    glColor4f(0.75f, 0.80f, 0.85f, 0.85f);
    const char *bindings[][2] = {
        {"Move Left / Right", "Arrow Keys  /  A D"},
        {"Jump  (double jump)", "Space  /  W  /  Up Arrow"},
        {"Sword Attack", "Z  /  J"},
        {"Throw Shuriken", "X  /  K"},
        {"Dash", "E  /  L  /  Enter"},
        {"Pause", "P  /  Esc"},
        {"Settings", "Tab  (this screen)"},
    };
    int nb = sizeof(bindings) / sizeof(bindings[0]);
    for (int i = 0; i < nb; i++)
    {
        float row_y = 512.0f - i * 32.0f;
        /* Action label */
        glColor4f(0.70f, 0.75f, 0.80f, 0.80f);
        glRasterPos2f(280, row_y);
        for (const char *c = bindings[i][0]; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        /* Separator dot */
        glColor4f(0.45f, 0.55f, 0.70f, 0.60f);
        glRasterPos2f(510, row_y);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ':');
        /* Key binding */
        glColor4f(0.90f, 0.92f, 1.0f, 0.90f);
        glRasterPos2f(530, row_y);
        for (const char *c = bindings[i][1]; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    /* Close hint */
    float alpha = 0.5f + sinf((float)glutGet(GLUT_ELAPSED_TIME) * 0.002f) * 0.35f;
    glColor4f(0.80f, 0.88f, 1.0f, alpha);
    glRasterPos2f(510, 130);
    const char *close = "Press Tab or Esc to close";
    for (const char *c = close; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* Gear icon in top-right corner of panel */
    glColor4f(0.50f, 0.60f, 0.80f, 0.70f);
    draw_circle(1010, 590, 16, 24);
    glColor4f(0.06f, 0.09f, 0.18f, 1.0f);
    draw_circle(1010, 590, 10, 24);
    glColor4f(0.50f, 0.60f, 0.80f, 0.70f);
    for (int i = 0; i < 8; i++)
    {
        float a = i * 45.0f * 3.14159f / 180.0f;
        draw_rect_rot(1010 + cosf(a) * 16, 590 + sinf(a) * 16, 3, 7, i * 45.0f);
    }
}
