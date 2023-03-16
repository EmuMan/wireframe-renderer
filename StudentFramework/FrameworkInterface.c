#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include "FrameworkInterface.h"
#include "InputManager.h"
#include "LineRasterizer.h"
#include "pmath.h"
#include "Transformations.h"
#include "obj.h"
#include "pfmath.h"
#include "noise.h"
#include "sceneparse.h"
#include "images.h"

extern int winID;

//coseo.frerichs@digipen.edu

struct Point gOrigin = { 0.0f, 0.0f, 0.0f }; //setting the global origin

static float gMouseSens = 0.3f; //setting the mouse sensitivity

static int gCrosshairSize = 3; //setting the crosshair size

static HWND gWinHandle; //declaring a window handle

struct Scene * gScene1; //declaring a random scene

struct Scene * currentScene; //declaring the variable that holds the pointer to the current scene

void Init(void)
{
	//define gScene1 using the scene-1 scene file
	gScene1 = readScene("./data/scenes/scene-1.scn");
	
	currentScene = gScene1;

	//hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);
}

void render()
{
	if (IsKeyPressed('v'))
	{
		//set the window handle to the foreground window
		gWinHandle = GetForegroundWindow();
	}

	//define a bunch of crap used for moving the camera
	float xScale = 0.0f;
	float yScale = 0.0f;
	float zScale = 0.0f;

	float fVel = 0.0f;
	float sVel = 0.0f;

	float rVel = 0.0f;

	float scalef = 0.0f;
	
	//wasd checks
	if (IsKeyPressed('w'))
		fVel = 0.05f;
	if (IsKeyPressed('a'))
		sVel = -0.05f;
	if (IsKeyPressed('s'))
		fVel = -0.05f;
	if (IsKeyPressed('d'))
		sVel = 0.05f;
	if (IsKeyPressed(VK_ESCAPE))
		exit(0);

	//make a copy of the camera facing vector so you can apply velocity without hecking up view
	struct Vector move = currentScene->cam.facing;
	move.x *= fVel;
	move.y *= fVel;
	move.z *= fVel;

	//move the camera forwards/backwards
	translate(&(currentScene->cam.pos), move);

	//create a right vector for side to side moving and up and down rotation
	struct Vector right = crossp(currentScene->cam.facing, currentScene->cam.up);
	
	struct Vector rightT = right;
	rightT.x *= sVel;
	rightT.y *= sVel;
	rightT.z *= sVel;

	//move the camera left/right
	translate(&(currentScene->cam.pos), rightT);
	
	if (GetForegroundWindow() == gWinHandle) //if the previously defined window (pressing v) is the active one
	{
		//set cursor to center
		glutWarpPointer(WIDTH / 2, HEIGHT / 2);

		//rotate along the y axis (side to side)
		rotateV(&(currentScene->cam.facing), 0, -1 * (float)(mouseX - WIDTH / 2) / (200 / gMouseSens), 0);
		//rotate along the relative x axis (up and down)
		rotateMV(&(currentScene->cam.facing), right, -1 * (float)(mouseY - HEIGHT / 2) / (200 / gMouseSens));
	}

	camMatrix(currentScene->cam);

	//define basic colors because why not
	Color Black = { 0, 0, 0 };
	Color Grey = { 127,127,127 };
	Color White = { 255, 255, 255 };
	Color Red = { 255, 0, 0 };
	Color Yellow = { 255, 255, 0 };
	Color Green = { 0, 255, 0 };
	Color Cyan = { 0, 255, 255 };
	Color Blue = { 0, 0, 255 };
	Color Magenta = { 255, 0, 255 };

	//set the screen background to the scene's specified background image
	imageFillScreen(currentScene->bg, &(currentScene->bgInfo));

	//draw the scene objects
	drawScene(currentScene, White);

	//draw crosshair
	DrawLineDDA(WIDTH / 2 - gCrosshairSize, HEIGHT / 2, WIDTH / 2 + gCrosshairSize, HEIGHT / 2, Red);
	DrawLineDDA(WIDTH / 2, HEIGHT / 2 - gCrosshairSize, WIDTH / 2, HEIGHT / 2 + gCrosshairSize, Red);
}

void PF_SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	//self explanatory, sets the pixel
	FrameBuffer_SetPixel(&GlobalFrameBuffer, x, y, r, g, b);
}

void FillScreen(unsigned char r, unsigned char g, unsigned char b)
{
	//simply moves through all the pixels and sets each one to the given color
	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			PF_SetPixel(w, h, r, g, b);

		}
	}
}


void perlinFillScreen(void)
{
	//really slow, try to avoid using.
	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			unsigned char value = (unsigned char)round(perlin2d(w, h, 0.1, 4) * 256);

			PF_SetPixel(w, h, value, value, value);
		}
	}
}

void imageFillScreen(struct Color * imageData, BITMAPINFOHEADER *bitmapInfoHeader)
{
	//puts a loaded .bmp file onto the screen
	for (long w = 0; w < bitmapInfoHeader->biWidth; w++)
	{
		for (long h = 0; h < bitmapInfoHeader->biHeight; h++)
		{
			PF_SetPixel(w, bitmapInfoHeader->biHeight - h, //h reversed because of bitmap formatting, blah blah blah
				//imageData is NOT a 2d array, so you have to use this fancy conversion to get the value at column w row h
				imageData[(h * bitmapInfoHeader->biWidth + w)].Red,
				imageData[(h * bitmapInfoHeader->biWidth + w)].Green,
				imageData[(h * bitmapInfoHeader->biWidth + w)].Blue
				);
		}
	}
}

void drawMesh(struct Mesh * m, struct Camera cam, struct Color rgb)
{
	//draws a mesh onto the screen

	//allocate space for a copy of the mesh for transformations
	struct Point * mCopy = malloc(sizeof(struct Point) * m->vertcount);

	struct Vector transV; //vector for passing into transformations

	for (int i = 0; i < m->vertcount; i++)
	{
		//create copy of the current point
		mCopy[i] = m->points[i];

		//define values for the translation vector
		transV.x = m->loc.x;
		transV.y = m->loc.y;
		transV.z = m->loc.z;

		/*
		scale, rotate, and translate the mesh around the center of the world,
		which at this point is also the center of the model because no
		transformations have been applied.
		*/
		scale(&(mCopy[i]), gOrigin, m->scale.x, m->scale.y, m->scale.z);
		rotate(&(mCopy[i]), gOrigin, m->rot.x, m->rot.y, m->rot.z);
		translate(&(mCopy[i]), transV);

		//send the newly modified point (still copy) through the 3d pipeline
		mCopy[i] = toCamera(cam, mCopy[i]);
		mCopy[i] = Perspective(mCopy[i], cam.focalLength);
		mCopy[i] = ToScreen(mCopy[i]);
	}

	for (int i = 0; i < m->edgecount; i++)
	{
		/*only draw if both points are in front of the camera plane (decided
		at the top of the Perspective function)*/
		if (mCopy[m->edges[i * 2]].z < (-1 * cam.focalLength) + 0.00001f &&
			mCopy[m->edges[i*2+1]].z < (-1 * cam.focalLength) + 0.00001f)
		{
			/*
			remember, mCopy is an array that holds the transformed points,
			and m is a pointer to the original mesh.  the array does not hold
			information like which vertices are joined in an edge, but the
			original is not transformed, so a combination must be used.
			*/
			DrawLineDDA(mCopy[m->edges[i * 2]].x,
						mCopy[m->edges[i * 2]].y,
						mCopy[m->edges[i * 2 + 1]].x,
						mCopy[m->edges[i * 2 + 1]].y, rgb);
		}
	}

	//free the array copy to prevent a bad case of memory leakage
	free(mCopy);
}

void drawScene(struct Scene * s, struct Color rgb)
{
	//simply goes through every mesh in the scene and passes it through drawMesh
	//(along with scene camera and supplied color)
	for (int i = 0; i < s->objcount; i++)
	{
		drawMesh(&(s->objects[i]), s->cam, rgb);
	}
}

float ** alloc2d(int r, int c)
{
	//this function allocates a 2d array, used for matrix stuff

	//allocate space for r number of float pointer pointers, ie one pointer pointer per row
	float ** result = malloc(sizeof(float *) * r);

	for (int i = 0; i < r; i++)
	{
		//for each row, allocate c float pointers
		result[i] = malloc(sizeof(float) * c);
	}

	return result;
}

void dealloc2d(float **array, int r)
{
	//deallocates an array allocated by alloc2d
	for (int i = 0; i < r; i++)
	{
		//deallocate all of the float pointers
		free(array[i]);
	}

	//deallocate the actual float pointer pointer
	free(array);
}