#include "renderer.h"
#include "game.h"
#include <math.h>

/* ── Platform drawing (themed) ──────────────────────────── */
void draw_platform(Platform* p, int theme) {
    float x=p->x, y=p->y, w=p->w, h=p->h;

    if (p->crumbled) return;

    switch (theme) {
    case 0: /* RUINS — mossy stone */
        /* main body */
        glColor4f(0.32f,0.28f,0.26f,1);
        draw_rect(x,y,w,h);
        /* stone block lines */
        glColor4f(0.20f,0.18f,0.17f,1);
        for (float bx=x+30; bx<x+w; bx+=35)
            draw_rect(bx,y,2,h);
        /* mossy green cap */
        glColor4f(0.18f,0.36f,0.12f,0.85f);
        draw_rect(x,y+h-5,w,5);
        for (float mx=x+8; mx<x+w; mx+=22)
            draw_circle(mx,y+h-1,4+sinf(mx)*2,8);
        /* diagonal crack */
        if (w>60) {
            glColor4f(0.14f,0.12f,0.12f,1);
            glLineWidth(1.0f);
            glBegin(GL_LINES);
            glVertex2f(x+w*0.3f,y+h);
            glVertex2f(x+w*0.45f,y);
            glEnd();
        }
        break;

    case 1: /* TEMPLE — stone slabs */
        glColor4f(0.28f,0.24f,0.34f,1);
        draw_rect(x,y,w,h);
        glColor4f(0.18f,0.15f,0.22f,1);
        for (float bx=x+40; bx<x+w; bx+=45) draw_rect(bx,y,3,h);
        glColor4f(0.34f,0.28f,0.38f,1); draw_rect(x,y+h-4,w,4);
        break;

    case 2: /* VILLAGE — wood planks */
        glColor4f(0.42f,0.26f,0.14f,1);
        draw_rect(x,y,w,h);
        glColor4f(0.28f,0.17f,0.09f,1);
        for (float wy=y+4; wy<y+h; wy+=6) draw_rect(x,wy,w,1);
        glColor4f(0.50f,0.32f,0.16f,1); draw_rect(x,y+h-5,w,5);
        /* grass/moss on top */
        glColor4f(0.18f,0.36f,0.10f,0.7f);
        for (float gx=x+5; gx<x+w; gx+=14) draw_circle(gx,y+h,3,6);
        break;

    case 3: /* BAMBOO — woven wood */
        glColor4f(0.38f,0.32f,0.16f,1);
        draw_rect(x,y,w,h);
        glColor4f(0.50f,0.42f,0.20f,1);
        for (float bx=x+12; bx<x+w; bx+=14) draw_rect(bx,y,3,h);
        break;

    case 4: /* FORTRESS — dark metal */
        glColor4f(0.24f,0.24f,0.30f,1);
        draw_rect(x,y,w,h);
        glColor4f(0.32f,0.32f,0.40f,1); draw_rect(x,y+h-6,w,6);
        glColor4f(0.16f,0.16f,0.20f,1);
        for (float bx=x+20; bx<x+w; bx+=22) draw_rect(bx,y,2,h);
        break;
    }

    /* crumbling crack overlay */
    if (p->type == PLAT_CRUMBLING && p->crumble_timer > 0) {
        float t = p->crumble_timer;
        glColor4f(0.9f,0.3f,0.0f,t*0.4f);
        draw_rect(x,y,w,h);
    }
}

/* ── Blade trap ─────────────────────────────────────────── */
void draw_blade_trap(float cx, float cy, float angle) {
    glColor4f(0.22f, 0.18f, 0.16f, 1);
    draw_rect(cx-5,cy-20,10,20);

    glColor4f(0.28f,0.08f,0.08f,1);
    draw_circle(cx,cy,30,24);

    glPushMatrix();
    glTranslatef(cx,cy,0);
    glRotatef(angle,0,0,1);

    for (int i=0;i<6;i++) {
        float a  = i*60.0f*PI/180.0f;
        float al = (i*60.0f-22.0f)*PI/180.0f;
        float ar = (i*60.0f+22.0f)*PI/180.0f;
        glColor4f(0.85f,0.05f,0.05f,1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(al)*12,sinf(al)*12);
        glVertex2f(cosf(ar)*12,sinf(ar)*12);
        glVertex2f(cosf(a)*30, sinf(a)*30);
        glEnd();
        glColor4f(1.0f,0.3f,0.3f,0.7f);
        glBegin(GL_TRIANGLES);
        glVertex2f(cosf(a)*22,  sinf(a)*22);
        glVertex2f(cosf(al)*18, sinf(al)*18);
        glVertex2f(cosf(a)*30,  sinf(a)*30);
        glEnd();
    }
    glColor4f(0.42f,0.20f,0.20f,1); draw_circle(0,0,10,14);
    glColor4f(0.60f,0.25f,0.25f,1); draw_circle(0,0,5,10);
    glPopMatrix();

    glColor4f(0.8f,0.0f,0.0f,0.18f);
    draw_circle(cx,cy,38,20);
}

/* ── Thorn bush ─────────────────────────────────────────── */
void draw_thorn_bush(float cx, float y, float scale) {
    float s=scale;
    glColor4f(0.16f,0.22f,0.08f,1);
    draw_circle(cx, y+40*s, 30*s,16);
    draw_circle(cx-15*s,y+25*s,22*s,14);
    draw_circle(cx+12*s,y+28*s,20*s,14);
    draw_circle(cx, y+58*s, 22*s,14);

    glColor4f(0.85f,0.06f,0.06f,1.0f);
    float sp[][3] = {
        {0,30,18},{30,35,22},{60,32,20},{90,36,24},
        {120,33,19},{150,30,17},{180,35,21},{210,32,18},
        {240,34,22},{270,36,20},{300,30,18},{330,33,20},
        {15,38,16},{75,34,19},{135,36,17},{195,32,21},
        {255,35,18},{315,30,22}
    };
    float cy2 = y+40*s;
    for (int i=0;i<18;i++) {
        float a = sp[i][0]*PI/180.0f;
        float r = sp[i][1]*s;
        float sl= sp[i][2]*s;
        float bx=cx+cosf(a)*r, by=cy2+sinf(a)*r;
        float ex=cx+cosf(a)*(r+sl), ey=cy2+sinf(a)*(r+sl);
        float pw=3.0f*s, perp=a+PI/2;
        draw_tri(bx+cosf(perp)*pw,by+sinf(perp)*pw,
                 bx-cosf(perp)*pw,by-sinf(perp)*pw,
                 ex,ey);
    }
    glColor4f(0.8f,0.0f,0.0f,0.12f);
    draw_circle(cx,cy2,55*s,18);
}

/* ── Ground spikes ──────────────────────────────────────── */
void draw_spikes_ground(float x, float y, int count) {
    float sw=14.0f;
    glColor4f(0.30f,0.18f,0.16f,1);
    for (int i=0;i<count;i++) {
        float sx=x+i*sw;
        draw_rect(sx,y,sw,5);
        draw_tri(sx+1,y+5,sx+sw-1,y+5,sx+sw/2,y+22);
    }
    glColor4f(0.9f,0.05f,0.05f,0.3f);
    for (int i=0;i<count;i++)
        draw_circle(x+i*sw+sw/2,y+22,4,8);
}

/* ── Fire vent ──────────────────────────────────────────── */
void draw_fire_vent(float cx, float cy, int active, float fire_t) {
    glColor4f(0.24f,0.20f,0.16f,1);
    draw_rect(cx-12,cy,24,8);
    draw_rect(cx-8, cy,16,14);

    if (active) {
        float fh = 40.0f + sinf(fire_t*2*PI)*12.0f;
        glColor4f(0.9f,0.1f,0.0f,0.85f);
        draw_tri(cx-12,cy+14,cx+12,cy+14,cx,cy+14+fh);
        glColor4f(1.0f,0.5f,0.0f,0.75f);
        draw_tri(cx-8, cy+14,cx+8, cy+14,cx,cy+14+fh*0.75f);
        glColor4f(1.0f,0.9f,0.1f,0.6f);
        draw_tri(cx-4, cy+14,cx+4, cy+14,cx,cy+14+fh*0.5f);
        glColor4f(0.9f,0.3f,0.0f,0.15f);
        draw_circle(cx,cy+14+fh*0.3f,fh*0.6f,12);
    }
}

/* ── Clay pot ───────────────────────────────────────────── */
void draw_clay_pot(float cx, float cy, int broken, float break_t) {
    if (broken==2) return;
    if (!broken) {
        glColor4f(0.52f,0.30f,0.18f,1);
        draw_rect(cx-8,cy,16,8);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx,cy+20);
        for (int i=0;i<=16;i++) {
            float a=PI+PI*(float)i/16;
            float rx=14+sinf((float)i/16*PI)*2;
            glVertex2f(cx+cosf(a)*rx,cy+20+sinf(a)*18);
        }
        glEnd();
        draw_rect(cx-10,cy+20,20,14);
        draw_rect(cx-5, cy+34,10, 8);
        draw_rect(cx-8, cy+40,16, 4);
        glColor4f(0.32f,0.18f,0.10f,1);
        for (int i=1;i<=3;i++) {
            float ty=cy+i*10;
            glBegin(GL_LINE_STRIP);
            for (int j=0;j<=10;j++) {
                float a=PI+PI*(float)j/10;
                float rx=12+sinf((float)j/10*PI)*2;
                glVertex2f(cx+cosf(a)*rx*(0.85f+i*0.05f),ty+sinf(a)*2);
            }
            glEnd();
        }
    } else {
        glColor4f(0.42f,0.24f,0.14f,1.0f-break_t);
        float sa[] = {0,45,90,135,180,225,270,315};
        for (int i=0;i<8;i++) {
            float a=sa[i]*PI/180.0f, r=break_t*40.0f;
            float sx=cx+cosf(a)*r, sy=cy+15+sinf(a)*r-break_t*20;
            float sw=6-break_t*4, sh=8-break_t*5;
            if (sw>0&&sh>0) draw_rect_rot(sx,sy,sw,sh,sa[i]);
        }
    }
}

/* ── Stone grave marker ─────────────────────────────────── */
void draw_stone_marker(float cx, float y, float w, float h) {
    glColor4f(0.30f,0.28f,0.34f,1);
    glBegin(GL_QUADS);
    glVertex2f(cx-w/2,y);
    glVertex2f(cx+w/2,y);
    glVertex2f(cx+w/2*0.7f,y+h);
    glVertex2f(cx-w/2*0.7f,y+h);
    glEnd();
    draw_circle(cx,y+h,w/2*0.7f,12);
    glColor4f(0.16f,0.32f,0.12f,0.7f);
    for (int i=0;i<5;i++) {
        float mx=cx-w*0.3f+i*w*0.15f;
        float my=y+h*0.2f+(i%3)*h*0.2f;
        draw_circle(mx,my,4+i,8);
    }
    glColor4f(0.16f,0.14f,0.18f,1);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex2f(cx,y+h*0.2f); glVertex2f(cx,y+h*0.7f);
    glVertex2f(cx-w*0.2f,y+h*0.45f); glVertex2f(cx+w*0.2f,y+h*0.45f);
    glEnd();
}

/* ── Rope fence ─────────────────────────────────────────── */
void draw_rope_fence(float x, float y, float length, int post_count) {
    float spacing = length / (post_count-1);
    glColor4f(0.32f,0.22f,0.12f,1);
    for (int i=0;i<post_count;i++) {
        float px=x+i*spacing;
        draw_rect(px-3,y,6,28);
        draw_rect(px-5,y+24,10,5);
    }
    glColor4f(0.44f,0.32f,0.16f,1);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for (int j=0;j<=20;j++) {
        float t=(float)j/20;
        float rx=x+length*t;
        float ry=y+18-sinf(fmodf(t*post_count,1.0f)*PI)*4;
        glVertex2f(rx,ry);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int j=0;j<=20;j++) {
        float t=(float)j/20;
        float rx=x+length*t;
        float ry=y+8-sinf(fmodf(t*post_count,1.0f)*PI)*3;
        glVertex2f(rx,ry);
    }
    glEnd();
    glLineWidth(1.0f);
}

/* ── Gem collectible ────────────────────────────────────── */
void draw_gem(float cx, float cy, float pulse_t) {
    float sc = 1.0f + sinf(pulse_t*3)*0.08f;
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glScalef(sc,sc,1);

    glColor4f(0.25f,0.55f,1.0f,1);
    draw_tri(0,10,   8,3,  8,-3);
    draw_tri(0,-10,  8,-3, 8,3);
    draw_tri(0,10,  -8,3, -8,-3);
    draw_tri(0,-10, -8,-3,-8,3);
    glColor4f(0.5f,0.75f,1.0f,0.5f);
    draw_tri(0,8,6,2,6,-2);

    /* glow */
    glColor4f(0.3f,0.6f,1.0f,0.15f);
    draw_circle(0,0,18,12);

    glPopMatrix();
}

/* ── Star collectible ───────────────────────────────────── */
void draw_star_collectible(float cx, float cy, float pulse_t) {
    float sc = 1.0f + sinf(pulse_t*2)*0.12f;
    glPushMatrix();
    glTranslatef(cx,cy+8,0);
    glScalef(sc,sc,1);
    glRotatef(sinf(pulse_t)*5,0,0,1);

    /* Gold star body */
    glColor4f(1.0f,0.88f,0.05f,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0,0);
    for (int i=0;i<=10;i++) {
        float r=(i%2==0)?16.0f:7.0f;
        float a=(i*36.0f-90)*PI/180.0f;
        glVertex2f(cosf(a)*r,sinf(a)*r);
    }
    glEnd();

    /* bright center */
    glColor4f(1.0f,1.0f,0.6f,0.9f);
    draw_circle(0,0,6,10);

    /* sparkle particles around */
    glColor4f(1.0f,0.9f,0.2f,0.6f);
    for (int i=0;i<6;i++) {
        float a=(i*60.0f+pulse_t*120)*PI/180.0f;
        float r=20+sinf(pulse_t*4+i)*5;
        draw_circle(cosf(a)*r,sinf(a)*r,2,6);
    }
    /* glow */
    glColor4f(1.0f,0.8f,0.0f,0.18f);
    draw_circle(0,0,26,14);

    glPopMatrix();
}

/* ── Checkpoint shrine ──────────────────────────────────── */
void draw_checkpoint(float cx, float y, int triggered, float t)
{
    /* Post */
    glColor4f(0.38f, 0.28f, 0.18f, 1.0f);
    draw_rect(cx - 4, y, 8, 52);

    /* Flag/banner */
    if (triggered)
    {
        /* Bright golden flag when activated */
        glColor4f(0.95f, 0.80f, 0.10f, 1.0f);
    }
    else
    {
        /* Dim grey flag when not yet reached */
        glColor4f(0.35f, 0.35f, 0.38f, 0.8f);
    }
    /* Waving flag shape */
    float wave = triggered ? sinf(t * 3.0f) * 4.0f : 0.0f;
    glBegin(GL_TRIANGLES);
    glVertex2f(cx + 4, y + 52);
    glVertex2f(cx + 28, y + 48 + wave);
    glVertex2f(cx + 4, y + 38);
    glEnd();

    /* Glow pulse when triggered */
    if (triggered)
    {
        glColor4f(1.0f, 0.9f, 0.2f, 0.25f + sinf(t * 4.0f) * 0.1f);
        draw_circle(cx, y + 45, 20, 16);
    }
}

/* ── Exit portal ────────────────────────────────────────── */
/* open=1: all conditions met → bright cyan/blue (passable)
   open=0: locked         → dim red/orange (blocked)         */
void draw_exit_portal(float cx, float cy, float t, int open) {
    /* +32 offset applied here so callers pass raw ground-y from level data */
    cy += 32.0f;

    float pulse = sinf(t * 2.0f) * 0.1f;

    if (open) {
        /* Active — cyan/blue */
        glColor4f(0.4f, 0.8f, 1.0f, 0.35f + pulse);
        draw_ring(cx, cy, 24, 32, 20);
        glColor4f(0.2f, 0.6f, 0.9f, 0.25f);
        draw_ring(cx, cy, 16, 24, 16);

        glPushMatrix();
        glTranslatef(cx, cy, 0);
        glRotatef(t * 60, 0, 0, 1);
        for (int i = 0; i < 8; i++) {
            float a = i * 45.0f * PI / 180.0f;
            glColor4f(0.5f, 0.9f, 1.0f, 0.5f);
            draw_rect_rot(cosf(a) * 28, sinf(a) * 28, 3, 10, i * 45.0f);
        }
        glPopMatrix();

        /* glow center */
        glColor4f(0.3f, 0.7f, 1.0f, 0.4f);
        draw_circle(cx, cy, 18, 16);
        glColor4f(0.7f, 0.95f, 1.0f, 0.7f);
        draw_circle(cx, cy, 8, 12);
    } else {
        /* Locked — dark red/orange, slow pulse, no spinning shards */
        glColor4f(0.8f, 0.2f, 0.1f, 0.25f + pulse * 0.5f);
        draw_ring(cx, cy, 24, 32, 20);
        glColor4f(0.6f, 0.15f, 0.05f, 0.18f);
        draw_ring(cx, cy, 16, 24, 16);

        /* Slow-spinning crossed bars to signal "blocked" */
        glPushMatrix();
        glTranslatef(cx, cy, 0);
        glRotatef(t * 20, 0, 0, 1);
        for (int i = 0; i < 4; i++) {
            float a = i * 90.0f * PI / 180.0f;
            glColor4f(0.9f, 0.3f, 0.1f, 0.45f);
            draw_rect_rot(cosf(a) * 20, sinf(a) * 20, 4, 12, i * 90.0f);
        }
        glPopMatrix();

        /* dim red center */
        glColor4f(0.7f, 0.15f, 0.05f, 0.35f);
        draw_circle(cx, cy, 18, 16);
        glColor4f(0.95f, 0.4f, 0.1f, 0.5f);
        draw_circle(cx, cy, 8, 12);
    }
}
