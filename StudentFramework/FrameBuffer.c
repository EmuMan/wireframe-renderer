#include "FrameBuffer.h"
#include "FrameworkInterface.h"
int Round(double input)
{
	int FlooredInput = input;

	if (input - FlooredInput > 0.5)
	{
		return FlooredInput + 1;
	}
	else
	{
		return FlooredInput;
	}
}

BOOLEAN float_IsEqual(float left, float right)
{
	//if difference is within the epsilon range

	return ((left - right) < EPSILON) &&
		((left - right) > -EPSILON) ? TRUE : FALSE;
}
BOOLEAN double_IsEqual(double left, double right)
{
	return ((left - right) < EPSILON) &&
		((left - right) > -EPSILON) ? TRUE : FALSE;
}


//Clears the frame buffer to the given (r, g, b) color
void FrameBuffer_Clear(FrameBuffer * fb, unsigned char r, unsigned char g, unsigned char b)
{
	int i, j;

	for (i = 0; i < fb->width; ++i)
	{
		for (j = 0; j < fb->height; ++j)
		{
			fb->buffer[i * 3 + j * fb->width * 3] = r;
			fb->buffer[i * 3 + j * fb->width * 3 + 1] = g;
			fb->buffer[i * 3 + j * fb->width * 3 + 2] = b;
		}
	}
}

//Initializes the frame buffer
void FrameBuffer_Init(FrameBuffer * fb, unsigned int w, unsigned int h)
{
	fb->width = w;
	fb->height = h;
	fb->buffer = (unsigned char *)malloc(fb->width * fb->height * 3);


	FrameBuffer_Clear(fb, 255, 255, 255);
}

//Set the pixel's color at (x,y)
void FrameBuffer_SetPixel(FrameBuffer * fb, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	int _y = fb->height - y;

	if (x < 0 || x >= fb->width || _y < 0 || _y >= fb->height)
		return;

	fb->buffer[x * 3 + _y * fb->width * 3] = r;
	fb->buffer[x * 3 + _y * fb->width * 3 + 1] = g;
	fb->buffer[x * 3 + _y * fb->width * 3 + 2] = b;
}

//Stores the color of the given (x,y) pixel in r, g & b
void FrameBuffer_GetPixel(FrameBuffer * fb, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	int _y = fb->height - y;

	if (x < 0 || x >= fb->width || _y < 0 || _y >= fb->height)
		return;

	r = fb->buffer[x * 3 + _y * fb->width * 3];
	g = fb->buffer[x * 3 + _y * fb->width * 3 + 1];
	b = fb->buffer[x * 3 + _y * fb->width * 3 + 2];
}

void FrameBuffer_Destroy(FrameBuffer * fb)
{
	free(fb->buffer);
}

/////

