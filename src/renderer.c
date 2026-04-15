#include "renderer.h"
#include <stdio.h>

void set_color(float r, float g, float b, float a) { glColor4f(r,g,b,a); }
void col_black(void)   { glColor4f(0.00f, 0.00f, 0.00f, 1.0f); }
void col_red(float a)  { glColor4f(0.80f, 0.05f, 0.05f,    a); }
void col_gold(void)    { glColor4f(1.00f, 0.85f, 0.10f, 1.0f); }
void col_white(float a){ glColor4f(1.0f,  1.0f,  1.0f,    a); }

void draw_circle(float cx, float cy, float r, int segs) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; i++) {
        float a = 2*PI*i/segs;
        glVertex2f(cx + cosf(a)*r, cy + sinf(a)*r);
    }
    glEnd();
}

void draw_rect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x,   y);   glVertex2f(x+w, y);
    glVertex2f(x+w, y+h); glVertex2f(x,   y+h);
    glEnd();
}

void draw_tri(float x1,float y1, float x2,float y2, float x3,float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
    glEnd();
}

void draw_rect_rot(float cx, float cy, float w, float h, float angle) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0, 0, 1);
    draw_rect(-w/2, -h/2, w, h);
    glPopMatrix();
}

void draw_ring(float cx, float cy, float r_in, float r_out, int segs) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; i++) {
        float a = 2*PI*i/segs;
        glVertex2f(cx + cosf(a)*r_in,  cy + sinf(a)*r_in);
        glVertex2f(cx + cosf(a)*r_out, cy + sinf(a)*r_out);
    }
    glEnd();
}

void draw_string(float x, float y, const char* str, void* font) {
    glRasterPos2f(x, y);
    for (const char* c = str; *c; c++)
        glutBitmapCharacter(font, *c);
}
