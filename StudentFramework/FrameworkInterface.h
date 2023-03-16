#pragma once
#include "FrameBuffer.h"
#ifndef RENDERER_H
#define RENDERER_H

#define WIDTH 640
#define HEIGHT 480

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

#define EPSILON 0.0001f

#define TRUE 1
#define FALSE 0
#define BOOLEAN int

typedef unsigned char      u8;
typedef char               s8;
typedef unsigned int       u32;
typedef int                s32;
typedef unsigned long long u64;
typedef long long          s64;
typedef float              f32;
typedef double             f64;

BOOLEAN float_IsEqual(float left, float right);
BOOLEAN double_IsEqual(double left, double right);
int Round(double input);

void Init(void);
void render(void);

void PF_SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
void FillScreen(unsigned char r, unsigned char g, unsigned char b);
void perlinFillScreen(void);
void imageFillScreen(struct Color * imageData, BITMAPINFOHEADER *bitmapInfoHeader);
void drawMesh(struct Mesh * m, struct Camera cam, struct Color rgb);
void drawScene(struct Scene * s, struct Color rgb);
float ** alloc2d(int r, int c);

#endif