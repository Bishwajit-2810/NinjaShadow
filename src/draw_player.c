#include "renderer.h"
#include "game.h"

/* ── Idle pose - Beautiful Ninja ────────────────────────────── */
void draw_player_idle(float cx, float cy, int facing_right)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    /* ── Feet and base ────────────────────────────────────── */
    glColor4f(0.10f, 0.10f, 0.11f, 1.0f); /* Dark boots */
    draw_rect(cx - 10, cy - 2, 7, 6);
    draw_rect(cx + 3, cy - 2, 7, 6);

    /* Boot soles */
    glColor4f(0.04f, 0.04f, 0.05f, 1.0f);
    draw_rect(cx - 10, cy - 4, 7, 2);
    draw_rect(cx + 3, cy - 4, 7, 2);

    /* ── Legs with armor shading ────────────────────────────– */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f); /* Dark ninja pants */
    draw_rect(cx - 9, cy + 4, 6, 20);
    draw_rect(cx + 3, cy + 4, 6, 20);

    /* Shin guards */
    glColor4f(0.15f, 0.15f, 0.17f, 1.0f);
    draw_rect(cx - 9, cy + 4, 6, 4);
    draw_rect(cx + 3, cy + 4, 6, 4);

    /* Shin lines - detail */
    glColor4f(0.18f, 0.18f, 0.20f, 1.0f);
    draw_rect(cx - 9, cy + 6, 1, 16);
    draw_rect(cx + 3, cy + 6, 1, 16);

    /* ── Torso with armor detail ──────────────────────────– */
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f); /* Dark chest base */
    glBegin(GL_QUADS);
    glVertex2f(cx - 11, cy + 24);
    glVertex2f(cx + 11, cy + 24);
    glVertex2f(cx + 14, cy + 44);
    glVertex2f(cx - 14, cy + 44);
    glEnd();

    /* Chest armor plating - dark steel */
    glColor4f(0.17f, 0.17f, 0.19f, 1.0f);
    draw_rect(cx - 10, cy + 26, 20, 12);

    /* Armor edge highlight */
    glColor4f(0.22f, 0.22f, 0.24f, 0.8f);
    draw_rect(cx - 10, cy + 26, 20, 2);

    /* Middle armor line */
    glColor4f(0.40f, 0.30f, 0.10f, 1.0f);
    draw_rect(cx - 1, cy + 24, 2, 16);

    /* ── Shoulders with pauldrons ─────────────────────────– */
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    draw_circle(cx - 16, cy + 40, 7, 14);
    draw_circle(cx + 16, cy + 40, 7, 14);

    /* Shoulder inner shading */
    glColor4f(0.10f, 0.10f, 0.12f, 1.0f);
    draw_circle(cx - 16, cy + 42, 4, 12);
    draw_circle(cx + 16, cy + 42, 4, 12);

    /* ── Arms with gloves ─────────────────────────────────– */
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f); /* Dark sleeve */
    draw_rect(cx - 20, cy + 34, 6, 14);
    draw_rect(cx + 14, cy + 34, 6, 14);

    /* Gloves - dark */
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect(cx - 20, cy + 34, 6, 4);
    draw_rect(cx + 14, cy + 34, 6, 4);

    /* Glove knuckles detail */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_circle(cx - 17, cy + 34, 2, 6);
    draw_circle(cx + 17, cy + 34, 2, 6);

    /* ── Neck ─────────────────────────────────────────────– */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(cx - 4, cy + 44, 8, 6);

    /* ── Head - detailed and expressive ─────────────────────– */
    draw_circle(cx, cy + 62, 13, 24);

    /* Face (dark) */
    glColor4f(0.08f, 0.07f, 0.07f, 1.0f);
    draw_circle(cx - 3, cy + 61, 2.5f, 8);
    draw_circle(cx + 3, cy + 61, 2.5f, 8);

    /* Eyes - narrow glowing red slits (ninja mask look) */
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(cx - 3, cy + 61, 2.0f, 8);
    draw_circle(cx + 3, cy + 61, 2.0f, 8);

    /* Pupils - intense gaze */
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_circle(cx - 2.5f, cy + 61.5f, 0.8f, 6);
    draw_circle(cx + 3.5f, cy + 61.5f, 0.8f, 6);

    /* Mask/Face wrap - layered */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(cx - 10, cy + 51, 20, 8);

    /* Mask texture */
    glColor4f(0.04f, 0.04f, 0.05f, 1.0f);
    draw_rect(cx - 8, cy + 52, 2, 6);
    draw_rect(cx - 2, cy + 52, 2, 6);
    draw_rect(cx + 4, cy + 52, 2, 6);

    /* Nose above mask */
    glColor4f(0.35f, 0.22f, 0.18f, 1.0f);
    draw_tri(cx - 1.5f, cy + 56, cx + 1.5f, cy + 56, cx, cy + 53);

    /* ── Horns - twin steel blades ──────────────────────────– */
    /* Left horn */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(cx - 5, cy + 74, 0);
    glRotatef(-32, 0, 0, 1);
    draw_rect(-2, 0, 4, 20);
    glColor4f(0.18f, 0.18f, 0.20f, 0.7f);
    draw_rect(-1.5f, 0, 2, 18);
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 14, 0);
    glRotatef(-48, 0, 0, 1);
    draw_rect(-1.5f, 0, 3, 12);
    glPopMatrix();
    glPopMatrix();

    /* Right horn */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(cx + 5, cy + 74, 0);
    glRotatef(36, 0, 0, 1);
    draw_rect(-2, 0, 4, 20);
    glColor4f(0.18f, 0.18f, 0.20f, 0.7f);
    draw_rect(-1.5f, 0, 2, 18);
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 14, 0);
    glRotatef(52, 0, 0, 1);
    draw_rect(-1.5f, 0, 3, 12);
    glPopMatrix();
    glPopMatrix();

    /* ── Cape ─────────────────────────────────────────────– */
    glColor4f(0.08f, 0.08f, 0.10f, 0.75f);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx - 14, cy + 38);
    glVertex2f(cx - 18, cy + 38);
    glVertex2f(cx - 16, cy + 12);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(cx + 14, cy + 38);
    glVertex2f(cx + 18, cy + 38);
    glVertex2f(cx + 16, cy + 12);
    glEnd();

    glPopMatrix();
}

/* ── Run pose - Enhanced with better colors ──────────────────– */
void draw_player_run(float cx, float cy, int facing_right, float anim_t)
{
    float ph = anim_t * 2 * PI;
    /* Offset phases by PI/2 so the legs are ALWAYS separated (never both at center) */
    float back_swing  = sinf(ph)          * 26.0f;
    float front_swing = sinf(ph + PI)     * 26.0f; /* exactly opposite back leg */
    float arm_swing   = sinf(ph + PI)     * 32.0f;
    float body_bob    = fabsf(sinf(ph))   *  5.0f;
    float lean        = sinf(ph)          *  8.0f;
    float head_tilt   = sinf(ph)          *  3.0f;
    cy += body_bob;

    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    /* ── Back leg ─────────────────────────────────────────– */
    glPushMatrix();
    glTranslatef(cx - 5, cy + 35, 0);  /* slightly more separated from center */
    glRotatef(-back_swing, 0, 0, 1);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(-4, -35, 8, 16);

    glColor4f(0.28f, 0.28f, 0.30f, 1.0f);
    draw_rect(-4, -35, 8, 4);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    glTranslatef(0, -26, 0);
    glRotatef(back_swing * 0.7f, 0, 0, 1);
    draw_rect(-3, -14, 7, 14);
    /* Symmetric foot: centered on shin axis */
    draw_rect(-6, -16, 12, 5);
    glPopMatrix();

    /* ── Forward leg ──────────────────────────────────────– */
    glPushMatrix();
    glTranslatef(cx + 5, cy + 35, 0);  /* matching separation on the other side */
    glRotatef(-front_swing, 0, 0, 1);  /* front_swing = -back_swing so always opposite */

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(-4, -35, 8, 16);

    glColor4f(0.28f, 0.28f, 0.30f, 1.0f);
    draw_rect(-4, -35, 8, 4);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    glTranslatef(0, -26, 0);
    glRotatef(front_swing * 0.7f, 0, 0, 1);
    draw_rect(-3, -14, 7, 14);
    /* Same symmetric foot shape as back leg */
    draw_rect(-6, -16, 12, 5);
    glPopMatrix();

    /* ── Dynamic torso ────────────────────────────────────– */
    glPushMatrix();
    glTranslatef(cx, cy + 40, 0);
    glRotatef(lean - 12.0f, 0, 0, 1);

    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-10, 0);
    glVertex2f(10, 0);
    glVertex2f(12, 18);
    glVertex2f(-12, 18);
    glEnd();

    /* Dark steel armor — consistent with idle/jump */
    glColor4f(0.17f, 0.17f, 0.19f, 1.0f);
    draw_rect(-9, 2, 18, 10);
    glColor4f(0.22f, 0.22f, 0.24f, 0.8f);
    draw_rect(-9, 2, 18, 2);
    /* Center line detail */
    glColor4f(0.40f, 0.30f, 0.10f, 1.0f);
    draw_rect(-1, 2, 2, 14);

    glPopMatrix();

    /* ── Back arm with swing ──────────────────────────────– */
    glPushMatrix();
    glTranslatef(cx - 16, cy + 36, 0);
    glRotatef(-arm_swing, 0, 0, 1);

    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect(-4, 0, 6, 16);

    glColor4f(0.40f, 0.40f, 0.42f, 1.0f);
    draw_rect(-4, 0, 6, 4);
    glPopMatrix();

    /* ── Forward arm with swing ───────────────────────────– */
    glPushMatrix();
    glTranslatef(cx + 16, cy + 36, 0);
    glRotatef(arm_swing, 0, 0, 1);

    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect(-4, 0, 6, 16);

    glColor4f(0.40f, 0.40f, 0.42f, 1.0f);
    draw_rect(-4, 0, 6, 4);
    glPopMatrix();

    /* ── Head with animation ──────────────────────────────– */
    float hcx = cx + head_tilt - 3.0f, hcy = cy + 64.0f;
    draw_circle(hcx, hcy, 11, 22);

    /* Face */
    glColor4f(0.08f, 0.07f, 0.07f, 1.0f);
    draw_circle(hcx - 3, hcy - 2, 2.5f, 8);
    draw_circle(hcx + 3, hcy - 2, 2.5f, 8);

    /* Eyes — red slits, consistent with idle */
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(hcx - 3, hcy - 2, 2.0f, 8);
    draw_circle(hcx + 3, hcy - 2, 2.0f, 8);

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_circle(hcx - 2.5f, hcy - 1.5f, 0.9f, 6);
    draw_circle(hcx + 3.5f, hcy - 1.5f, 0.9f, 6);

    /* Mask */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(hcx - 10, hcy - 6, 20, 8);

    /* Horns mirroring movement */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(hcx - 3, hcy + 9, 0);
    glRotatef(-25 + sinf(ph) * 4.0f, 0, 0, 1);
    draw_rect(-2, 0, 4, 15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(hcx + 3, hcy + 9, 0);
    glRotatef(28 + sinf(ph + PI) * 4.0f, 0, 0, 1);
    draw_rect(-2, 0, 4, 15);
    glPopMatrix();

    glPopMatrix();
}

/* ── Jump pose - Powerful with better colors ───────────────────– */
void draw_player_jump(float cx, float cy, int facing_right)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    /* Back leg - pulled up */
    glPushMatrix();
    glTranslatef(cx - 5, cy + 28, 0);
    glRotatef(35, 0, 0, 1);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(-4, 0, 8, 14);

    glColor4f(0.28f, 0.28f, 0.30f, 1.0f);
    draw_rect(-4, 0, 8, 3);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    glTranslatef(2, 14, 0);
    glRotatef(-80, 0, 0, 1);
    draw_rect(-3, 0, 7, 12);
    draw_rect(-2, 11, 10, 4);
    glPopMatrix();

    /* Forward leg - stretched */
    glPushMatrix();
    glTranslatef(cx + 5, cy + 28, 0);
    glRotatef(-35, 0, 0, 1);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(-4, 0, 8, 14);

    glColor4f(0.28f, 0.28f, 0.30f, 1.0f);
    draw_rect(-4, 0, 8, 3);

    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    glTranslatef(-2, 14, 0);
    glRotatef(80, 0, 0, 1);
    draw_rect(-3, 0, 7, 12);
    draw_rect(-8, 11, 10, 4);
    glPopMatrix();

    /* Torso */
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    draw_rect(cx - 10, cy + 28, 20, 18);

    /* Chest armor during jump - dark steel */
    glColor4f(0.17f, 0.17f, 0.19f, 1.0f);
    draw_rect(cx - 8, cy + 32, 16, 10);
    glColor4f(0.22f, 0.22f, 0.24f, 0.6f);
    draw_rect(cx - 8, cy + 32, 16, 2);

    /* Shoulder armor */
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    draw_rect(cx - 14, cy + 44, 28, 7);
    draw_rect(cx - 3, cy + 50, 7, 6);

    /* Arms extended upward for balance */
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect_rot(cx - 18, cy + 48, 6, 18, 50.0f);
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect_rot(cx - 18, cy + 48, 6, 3, 50.0f);

    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect_rot(cx + 18, cy + 48, 6, 18, -50.0f);
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect_rot(cx + 18, cy + 48, 6, 3, -50.0f);

    /* Head */
    draw_circle(cx, cy + 60, 10, 20);

    /* Face with intensity */
    glColor4f(0.45f, 0.30f, 0.25f, 1.0f);
    draw_circle(cx - 3, cy + 59, 1.5f, 6);
    draw_circle(cx + 3, cy + 59, 1.5f, 6);

    /* Eyes wide - red slits */
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(cx - 3, cy + 59, 1.3f, 6);
    draw_circle(cx + 3, cy + 59, 1.3f, 6);

    /* Mask - determined */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(cx - 8, cy + 54, 16, 6);

    /* Horns - extended upward */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(cx - 3, cy + 69, 0);
    glRotatef(-35, 0, 0, 1);
    draw_rect(-2, 0, 4, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx + 3, cy + 69, 0);
    glRotatef(35, 0, 0, 1);
    draw_rect(-2, 0, 4, 16);
    glPopMatrix();

    glPopMatrix();
}

/* ── Fall pose ──────────────────────────────────────────── */
void draw_player_fall(float cx, float cy, int facing_right)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    /* Legs spread wide */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_rect(cx - 11, cy, 8, 28);
    draw_rect(cx + 3, cy, 8, 28);
    /* Shin guards */
    glColor4f(0.28f, 0.28f, 0.30f, 1.0f);
    draw_rect(cx - 11, cy, 8, 4);
    draw_rect(cx + 3, cy, 8, 4);
    /* Boots */
    glColor4f(0.10f, 0.10f, 0.11f, 1.0f);
    draw_rect(cx - 14, cy - 2, 12, 4);
    draw_rect(cx + 3, cy - 2, 12, 4);

    /* Torso slightly angled */
    glPushMatrix();
    glTranslatef(cx, cy + 38, 0);
    glRotatef(6, 0, 0, 1);
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    draw_rect(-10, 0, 20, 17);
    /* Chest armor */
    glColor4f(0.17f, 0.17f, 0.19f, 1.0f);
    draw_rect(-8, 2, 16, 10);
    glColor4f(0.22f, 0.22f, 0.24f, 0.7f);
    draw_rect(-8, 2, 16, 2);
    /* Shoulders */
    glColor4f(0.14f, 0.14f, 0.16f, 1.0f);
    draw_rect(-14, 15, 28, 7);
    draw_rect(-3, 21, 7, 6);
    glPopMatrix();

    /* Arms wide — bracing for landing */
    glColor4f(0.16f, 0.16f, 0.18f, 1.0f);
    draw_rect_rot(cx - 20, cy + 44, 6, 20, 75.0f);
    draw_rect_rot(cx + 20, cy + 44, 6, 20, -75.0f);

    /* Head */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_circle(cx + 2, cy + 60, 10, 20);

    /* Eyes — wide red slits (falling/alarmed) */
    glColor4f(0.08f, 0.07f, 0.07f, 1.0f);
    draw_circle(cx - 2, cy + 59, 2.5f, 8);
    draw_circle(cx + 4, cy + 59, 2.5f, 8);
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(cx - 2, cy + 59, 2.0f, 8);
    draw_circle(cx + 4, cy + 59, 2.0f, 8);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_circle(cx - 1.5f, cy + 59.5f, 0.8f, 6);
    draw_circle(cx + 4.5f, cy + 59.5f, 0.8f, 6);

    /* Mask */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(cx - 9, cy + 54, 18, 6);

    /* Horns */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(cx - 1, cy + 69, 0);
    glRotatef(-25, 0, 0, 1);
    draw_rect(-2, 0, 4, 13);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cx + 5, cy + 69, 0);
    glRotatef(30, 0, 0, 1);
    draw_rect(-2, 0, 4, 11);
    glPopMatrix();

    glPopMatrix();
}

/* ── Wall-slide pose ────────────────────────────────────── */
void draw_player_wall_slide(float cx, float cy, int wall_side)
{
    col_black(); /* explicit initial colour — not inherited from caller */
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef((float)wall_side, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    glPushMatrix();
    glTranslatef(cx + 4, cy + 20, 0);
    glRotatef(-20, 0, 0, 1);
    draw_rect(-4, -20, 8, 20);
    draw_rect(0, -22, 10, 4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx + 4, cy + 36, 0);
    glRotatef(-10, 0, 0, 1);
    draw_rect(-4, -16, 8, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx, cy + 42, 0);
    glRotatef(15.0f, 0, 0, 1);
    draw_rect(-9, 0, 18, 18);
    draw_rect(-12, 16, 24, 7);
    draw_rect(-3, 22, 7, 6);
    glPopMatrix();

    draw_rect_rot(cx + 16, cy + 48, 6, 18, -80.0f);
    draw_rect(cx + 22, cy + 44, 10, 5);
    draw_rect_rot(cx - 12, cy + 48, 6, 14, -30.0f);

    float hcx = cx + 4, hcy = cy + 62;
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_circle(hcx, hcy, 10, 20);
    /* Eyes — red slits */
    glColor4f(0.08f, 0.07f, 0.07f, 1.0f);
    draw_circle(hcx - 3, hcy - 1, 2.5f, 8);
    draw_circle(hcx + 3, hcy - 1, 2.5f, 8);
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(hcx - 3, hcy - 1, 2.0f, 8);
    draw_circle(hcx + 3, hcy - 1, 2.0f, 8);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_circle(hcx - 2.5f, hcy - 0.5f, 0.8f, 6);
    draw_circle(hcx + 3.5f, hcy - 0.5f, 0.8f, 6);
    /* Mask */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(hcx - 9, hcy - 6, 18, 7);
    /* Horns */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(hcx - 2, hcy + 9, 0);
    glRotatef(-25, 0, 0, 1);
    draw_rect(-2, 0, 4, 13);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(hcx + 4, hcy + 9, 0);
    glRotatef(30, 0, 0, 1);
    draw_rect(-2, 0, 4, 11);
    glPopMatrix();

    glPopMatrix();
}

/* ── Attack pose ────────────────────────────────────────── */
void draw_player_attack(float cx, float cy, int facing_right, float anim_t)
{
    float swing = anim_t * 130.0f - 35.0f;

    col_black(); /* explicit initial colour — not inherited from caller */
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);

    draw_rect(cx - 12, cy, 10, 26);
    draw_rect(cx + 2, cy, 10, 26);
    draw_rect(cx - 14, cy - 2, 12, 4);
    draw_rect(cx + 2, cy - 2, 12, 4);
    draw_rect(cx - 9, cy + 26, 18, 12);

    glBegin(GL_QUADS);
    glVertex2f(cx - 9, cy + 38);
    glVertex2f(cx + 9, cy + 38);
    glVertex2f(cx + 11, cy + 54);
    glVertex2f(cx - 11, cy + 54);
    glEnd();
    draw_rect(cx - 3, cy + 53, 7, 6);
    draw_rect(cx - 16, cy + 51, 8, 8);
    draw_rect(cx + 8, cy + 51, 8, 8);

    /* LEFT arm (non-sword side) */
    draw_rect_rot(cx - 16, cy + 50, 6, 14, -20.0f);

    /* RIGHT arm swinging sword */
    glPushMatrix();
    glTranslatef(cx + 13, cy + 52, 0);
    glRotatef(swing, 0, 0, 1);
    draw_rect(-3, -14, 6, 14);
    glTranslatef(0, -14, 0);
    draw_rect(-2.5f, -12, 5, 12);
    /* Sword */
    glColor4f(0.35f, 0.35f, 0.40f, 1);
    draw_rect(-2, -30, 4, 30);
    /* Crossguard */
    col_black();
    draw_rect(-7, -11, 14, 4);
    glPopMatrix();

    /* Slash arc trail */
    glColor4f(1.0f, 1.0f, 1.0f, 0.55f - anim_t * 0.5f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 12; i++)
    {
        float t = (float)i / 12;
        float a = (swing - 80.0f + t * 80.0f) * PI / 180.0f;
        float r = 38.0f;
        glVertex2f(cx + 13 + cosf(a) * r, cy + 52 + sinf(a) * r);
    }
    glEnd();
    glLineWidth(1.0f);

    /* Head */
    glColor4f(0.12f, 0.12f, 0.14f, 1.0f);
    draw_circle(cx, cy + 63, 10, 20);
    /* Eyes — red slits, focused on attack */
    glColor4f(0.08f, 0.07f, 0.07f, 1.0f);
    draw_circle(cx - 3, cy + 62, 2.5f, 8);
    draw_circle(cx + 3, cy + 62, 2.5f, 8);
    glColor4f(0.80f, 0.10f, 0.10f, 1.0f);
    draw_circle(cx - 3, cy + 62, 2.0f, 8);
    draw_circle(cx + 3, cy + 62, 2.0f, 8);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_circle(cx - 2.5f, cy + 62.5f, 0.8f, 6);
    draw_circle(cx + 3.5f, cy + 62.5f, 0.8f, 6);
    /* Mask */
    glColor4f(0.06f, 0.06f, 0.08f, 0.95f);
    draw_rect(cx - 8, cy + 57, 16, 6);
    /* Horns */
    glColor4f(0.13f, 0.13f, 0.15f, 1.0f);
    glPushMatrix();
    glTranslatef(cx - 3, cy + 72, 0);
    glRotatef(-25, 0, 0, 1);
    draw_rect(-2, 0, 4, 14);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cx + 3, cy + 72, 0);
    glRotatef(30, 0, 0, 1);
    draw_rect(-2, 0, 4, 12);
    glPopMatrix();

    glPopMatrix();
}

/* ── Hurt pose ──────────────────────────────────────────── */
void draw_player_hurt(float cx, float cy, int facing_right)
{
    draw_player_idle(cx, cy, facing_right);
}

/* ── Shuriken ───────────────────────────────────────────── */
void draw_shuriken(float cx, float cy, float angle)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0, 0, 1);
    col_white(1.0f);
    for (int i = 0; i < 4; i++)
    {
        float a = i * 90.0f * PI / 180.0f;
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(a) * 4, sinf(a) * 4);
        glVertex2f(cosf(a + 0.4f) * 4, sinf(a + 0.4f) * 4);
        glVertex2f(cosf(a + PI / 4) * 16, sinf(a + PI / 4) * 16);
        glEnd();
    }
    glColor4f(0.4f, 0.4f, 0.44f, 1);
    draw_circle(0, 0, 4, 8);
    glPopMatrix();
}

/* ── Dash pose ──────────────────────────────────────────── */
void draw_player_dash(float cx, float cy, int facing_right)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glScalef(facing_right ? 1.0f : -1.0f, 1.0f, 1.0f);
    glTranslatef(-cx, -cy, 0);
    col_black();

    /* Stretched horizontal pose */
    draw_rect(cx - 12, cy + 28, 24, 12); /* torso */
    draw_rect(cx - 16, cy + 42, 30, 8);  /* legs */
    draw_rect(cx - 18, cy + 36, 8, 8);   /* left arm */
    draw_rect(cx + 10, cy + 36, 8, 8);   /* right arm */
    draw_circle(cx, cy + 53, 9, 16);     /* head */

    glPopMatrix();
}

/* ── Draw player dispatch ───────────────────────────────── */
void draw_player_dispatch(Player *p)
{
    switch (p->anim.state)
    {
    case ANIM_IDLE:
        draw_player_idle(p->x, p->y, p->anim.facing);
        break;
    case ANIM_RUN:
        draw_player_run(p->x, p->y, p->anim.facing, p->anim.anim_t);
        break;
    case ANIM_JUMP:
        /* aerial visibility: soft head glow only — no bounding rect */
        glColor4f(0.22f, 0.30f, 0.48f, 0.35f);
        draw_circle(p->x, p->y + 61.0f, 14, 16); /* head rim */
        draw_player_jump(p->x, p->y, p->anim.facing);
        break;
    case ANIM_FALL:
        glColor4f(0.18f, 0.25f, 0.42f, 0.35f);
        draw_circle(p->x + 2, p->y + 61.0f, 14, 16);
        draw_player_fall(p->x, p->y, p->anim.facing);
        break;
    case ANIM_WALL_SLIDE:
        draw_player_wall_slide(p->x, p->y, p->anim.facing);
        break;
    case ANIM_ATTACK:
        draw_player_attack(p->x, p->y, p->anim.facing, p->anim.anim_t);
        break;
    case ANIM_DASH:
        draw_player_dash(p->x, p->y, p->anim.facing);
        break;
    case ANIM_HURT:
        draw_player_hurt(p->x, p->y, p->anim.facing);
        break;
    case ANIM_DEAD:
        /* draw fallen player */
        glPushMatrix();
        glTranslatef(p->x + 14, p->y + 14, 0);
        glRotatef(90, 0, 0, 1);
        glTranslatef(-(p->x + 14), -(p->y + 14), 0);
        draw_player_idle(p->x, p->y, p->anim.facing);
        glPopMatrix();
        break;
    default:
        draw_player_idle(p->x, p->y, p->anim.facing);
        break;
    }
}
