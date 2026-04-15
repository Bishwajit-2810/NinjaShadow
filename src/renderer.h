#ifndef RENDERER_H
#define RENDERER_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <math.h>
#define PI 3.14159265f

void set_color(float r, float g, float b, float a);
void col_black(void);
void col_red(float a);
void col_gold(void);
void col_white(float a);

void draw_circle(float cx, float cy, float r, int segs);
void draw_rect(float x, float y, float w, float h);
void draw_tri(float x1, float y1, float x2, float y2, float x3, float y3);
void draw_rect_rot(float cx, float cy, float w, float h, float angle);
void draw_ring(float cx, float cy, float r_in, float r_out, int segs);
void draw_string(float x, float y, const char* str, void* font);

/* draw_map.c extras */
void draw_checkpoint(float cx, float y, int triggered, float t);

/* draw_bg.c tree/deco variants */
void draw_pine_tree(float bx, float by, float scale);
void draw_willow_tree(float bx, float by, float scale);
void draw_cherry_tree(float bx, float by, float scale);
void draw_fortress_wall(float bx, float by, float scale);
void draw_clouds(float game_time, float cam_x);

#endif
