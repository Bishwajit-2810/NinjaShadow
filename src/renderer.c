#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

void set_color(float r, float g, float b, float a) { glColor4f(r, g, b, a); }
void col_black(void) { glColor4f(0.00f, 0.00f, 0.00f, 1.0f); }
void col_red(float a) { glColor4f(0.80f, 0.05f, 0.05f, a); }
void col_gold(void) { glColor4f(1.00f, 0.85f, 0.10f, 1.0f); }
void col_white(float a) { glColor4f(1.0f, 1.0f, 1.0f, a); }

void draw_circle(float cx, float cy, float r, int segs)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; i++)
    {
        float a = 2 * PI * i / segs;
        glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r);
    }
    glEnd();
}

void draw_circle_midpoint(int cx, int cy, int r)
{
    int x = 0;
    int y = (r < 0) ? 0 : r;
    int p = 1 - y;

    glBegin(GL_POINTS);
    while (x <= y)
    {
        glVertex2i(cx + x, cy + y);
        glVertex2i(cx - x, cy + y);
        glVertex2i(cx + x, cy - y);
        glVertex2i(cx - x, cy - y);
        glVertex2i(cx + y, cy + x);
        glVertex2i(cx - y, cy + x);
        glVertex2i(cx + y, cy - x);
        glVertex2i(cx - y, cy - x);

        x++;
        if (p < 0)
        {
            p += 2 * x + 1;
        }
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
    glEnd();
}

void draw_rect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void draw_tri(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void draw_line_dda(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    int steps = (int)fmaxf(fabsf(dx), fabsf(dy));

    glBegin(GL_POINTS);
    if (steps <= 0)
    {
        glVertex2f(roundf(x1), roundf(y1));
        glEnd();
        return;
    }

    float x_inc = dx / steps;
    float y_inc = dy / steps;
    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++)
    {
        glVertex2f(roundf(x), roundf(y));
        x += x_inc;
        y += y_inc;
    }
    glEnd();
}

void draw_line_bresenham(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int sx = (x1 < x2) ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
}

void draw_rect_rot(float cx, float cy, float w, float h, float angle)
{
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angle, 0, 0, 1);
    draw_rect(-w / 2, -h / 2, w, h);
    glPopMatrix();
}

void draw_ring(float cx, float cy, float r_in, float r_out, int segs)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; i++)
    {
        float a = 2 * PI * i / segs;
        glVertex2f(cx + cosf(a) * r_in, cy + sinf(a) * r_in);
        glVertex2f(cx + cosf(a) * r_out, cy + sinf(a) * r_out);
    }
    glEnd();
}

void draw_string(float x, float y, const char *str, void *font)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c; c++)
        glutBitmapCharacter(font, *c);
}
