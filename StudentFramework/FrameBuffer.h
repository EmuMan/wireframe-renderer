#pragma once

#include "windows.h"
#include "glut.h"
#include "math.h"
#include "stdio.h"



typedef struct FrameBuffer
{
	unsigned char * buffer;
	int width;
	int height;
} FrameBuffer;

FrameBuffer GlobalFrameBuffer;

void FrameBuffer_Clear(FrameBuffer * fb, unsigned char r, unsigned char g, unsigned char b);
void FrameBuffer_Init(FrameBuffer * fb, unsigned int w, unsigned int h);
void FrameBuffer_SetPixel(FrameBuffer * fb, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void FrameBuffer_GetPixel(FrameBuffer * fb, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void FrameBuffer_Destroy(FrameBuffer * fb);

void PF_SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
