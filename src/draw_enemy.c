#include "renderer.h"
#include "game.h"

/* ── Guard enemy ────────────────────────────────────────── */
void draw_enemy_guard(float cx, float cy, int facing_right, float anim_t) {
    float ph = anim_t * 2*PI;
    float ls = sinf(ph) * 14.0f;

    col_black(); /* explicit initial colour — not inherited from caller */
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glScalef(facing_right?1.0f:-1.0f,1.0f,1.0f);
    glTranslatef(-cx,-cy,0);

    /* LEGS */
    glPushMatrix(); glTranslatef(cx-5,cy+27,0); glRotatef(-ls,0,0,1);
    draw_rect(-5,-27,10,14);
    glTranslatef(0,-18,0); glRotatef(ls*0.5f,0,0,1);
    draw_rect(-4,-13,8,13); draw_rect(-5,-15,12,4);
    glPopMatrix();

    glPushMatrix(); glTranslatef(cx+5,cy+27,0); glRotatef(ls,0,0,1);
    draw_rect(-5,-27,10,14);
    glTranslatef(0,-18,0); glRotatef(-ls*0.5f,0,0,1);
    draw_rect(-4,-13,8,13); draw_rect(-9,-15,12,4);
    glPopMatrix();

    /* TORSO */
    glBegin(GL_QUADS);
    glVertex2f(cx-11,cy+27); glVertex2f(cx+11,cy+27);
    glVertex2f(cx+13,cy+45); glVertex2f(cx-13,cy+45);
    glEnd();

    /* Chest plate */
    glColor4f(0.06f,0.06f,0.07f,1);
    draw_rect(cx-8,cy+31,16,12);
    col_black();

    /* Pauldrons */
    draw_rect(cx-18,cy+43,11,9);
    draw_rect(cx+7, cy+43,11,9);
    draw_rect(cx-4, cy+45, 8, 6);

    /* Helmet */
    draw_circle(cx,cy+55,12,20);
    draw_tri(cx-5,cy+65,cx+5,cy+65,cx,cy+73);
    glColor4f(0.04f,0.08f,0.16f,1.0f);
    draw_rect(cx-8,cy+52,16,4);
    col_black();

    /* Arms */
    draw_rect_rot(cx-16,cy+42,7,15, 12.0f);
    draw_rect_rot(cx-18,cy+29,6,13,  5.0f);
    draw_rect_rot(cx+14,cy+44,7,15,-20.0f);

    /* Scythe */
    glColor4f(0.14f,0.09f,0.04f,1);
    glPushMatrix();
    glTranslatef(cx+20,cy+52,0); glRotatef(-15.0f,0,0,1);
    draw_rect(-2.5f,-15,5,70);
    col_black();
    glTranslatef(0,55,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0,0);
    for (int i=0;i<=12;i++) {
        float a=(180.0f+i*15.0f)*PI/180.0f;
        glVertex2f(cosf(a)*22,sinf(a)*22);
    }
    glEnd();
    glColor4f(0.07f,0.16f,0.28f,1);
    draw_circle(-8,8,14,14);
    glPopMatrix();

    glPopMatrix();
}

/* ── Archer enemy ───────────────────────────────────────── */
void draw_enemy_archer(float cx, float cy, int facing_right, int is_shooting) {
    col_black(); /* explicit initial colour — not inherited from caller */
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glScalef(facing_right?1.0f:-1.0f,1.0f,1.0f);
    glTranslatef(-cx,-cy,0);

    draw_rect(cx-10,cy,   7,20); draw_rect(cx+3, cy,   7,20);
    draw_rect(cx-13,cy-2,12, 4); draw_rect(cx+2, cy-2,12, 4);

    draw_rect(cx-9,cy+20,18,17);
    draw_rect(cx-12,cy+35,24,6);
    draw_rect(cx-3, cy+40, 7,6);

    draw_circle(cx,cy+50,10,20);
    draw_tri(cx-8,cy+58,cx+8,cy+58,cx+5,cy+68);

    glColor4f(0.9f,0.1f,0.1f,1);
    draw_circle(cx+3,cy+51,2.5f,8);
    glColor4f(0.9f,0.1f,0.1f,0.2f);
    draw_circle(cx+3,cy+51,6,10);
    col_black();

    draw_rect_rot(cx+14,cy+38,6,18,-82.0f);

    /* Bow arc */
    glLineWidth(3.5f);
    glBegin(GL_LINE_STRIP);
    for (int i=0;i<=14;i++) {
        float t=(float)i/14;
        float a=(t-0.5f)*2.4f;
        float bx=cx+29.0f+sinf(a)*7.0f;
        float by=cy+38.0f+(t-0.5f)*42.0f;
        glVertex2f(bx,by);
    }
    glEnd();
    glLineWidth(1.0f);

    float pull = is_shooting ? 14.0f : 0.0f;
    glBegin(GL_LINES);
    glVertex2f(cx+29,cy+59); glVertex2f(cx+29+pull,cy+38);
    glVertex2f(cx+29+pull,cy+38); glVertex2f(cx+29,cy+17);
    glEnd();

    draw_rect_rot(cx+10+pull,cy+38,6,16,-78.0f);

    if (is_shooting) {
        glColor4f(0.65f,0.5f,0.2f,1);
        draw_rect(cx+22,cy+36,20,3);
        col_black();
        draw_tri(cx+42,cy+35,cx+42,cy+39,cx+48,cy+37);
    }

    glPopMatrix();
}

/* ── Arrow ──────────────────────────────────────────────── */
void draw_arrow(float cx, float cy, float vx, float vy) {
    float angle = atan2f(vy, vx) * 180.0f / PI;
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glRotatef(angle,0,0,1);
    glColor4f(0.7f,0.55f,0.2f,1); draw_rect(-16,-1,32,3);
    col_black();
    draw_tri(16,-3,16,3,24,0);
    glColor4f(0.6f,0.1f,0.1f,1);
    draw_tri(-16,0,-11,4,-9,0);
    draw_tri(-16,0,-11,-4,-9,0);
    glPopMatrix();
}

/* ── Heavy warrior ──────────────────────────────────────── */
void draw_enemy_heavy(float cx, float cy, int facing_right, float anim_t) {
    float ph = anim_t * 2*PI;
    float ls = sinf(ph) * 8.0f;

    col_black(); /* explicit initial colour — not inherited from caller */
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glScalef(facing_right?1.0f:-1.0f,1.0f,1.0f);
    glTranslatef(-cx,-cy,0);

    glPushMatrix(); glTranslatef(cx-7,cy+30,0); glRotatef(-ls,0,0,1);
    draw_rect(-7,-30,14,30); draw_rect(-9,-32,20,5);
    glPopMatrix();
    glPushMatrix(); glTranslatef(cx+7,cy+30,0); glRotatef(ls,0,0,1);
    draw_rect(-7,-30,14,30); draw_rect(-11,-32,20,5);
    glPopMatrix();

    glBegin(GL_QUADS);
    glVertex2f(cx-14,cy+30); glVertex2f(cx+14,cy+30);
    glVertex2f(cx+17,cy+52); glVertex2f(cx-17,cy+52);
    glEnd();

    draw_rect(cx-24,cy+50,15,11);
    draw_rect(cx+9, cy+50,15,11);
    draw_rect(cx-5, cy+54,10, 8);

    draw_circle(cx,cy+65,15,24);
    glColor4f(0.07f,0.07f,0.08f,1);
    draw_rect(cx-11,cy+61,22,3);
    draw_rect(cx-11,cy+66,22,3);
    glColor4f(0.9f,0.05f,0.05f,1);
    draw_rect(cx-9,cy+68,7,2);
    draw_rect(cx+2,cy+68,7,2);
    col_black();

    draw_rect_rot(cx-22,cy+48,11,22, 12.0f);
    draw_rect_rot(cx-24,cy+28,10,20,  5.0f);
    draw_rect_rot(cx+22,cy+54,11,24,-28.0f);

    /* Cleaver */
    glPushMatrix();
    glTranslatef(cx+34,cy+60,0); glRotatef(-38,0,0,1);
    glColor4f(0.18f,0.1f,0.04f,1); draw_rect(-3,-22,6,22);
    col_black();
    glBegin(GL_QUADS);
    glVertex2f(-4,0); glVertex2f(24,0);
    glVertex2f(24,30); glVertex2f(-4,20);
    glEnd();
    glColor4f(0.2f,0.2f,0.22f,1); draw_rect(21,0,3,30);
    glPopMatrix();

    glPopMatrix();
}
