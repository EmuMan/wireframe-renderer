#include "LineRasterizer.h"
#include "FrameworkInterface.h"
#include <math.h>


void DrawLineDDA(float X1, float Y1, float X2, float Y2, Color rgb)
{
	// Find the change in x
	float diffX = X1 - X2;
	// Find the change in y
	float diffY = Y1 - Y2;
	
	// if the change in x and the change in y is less than 1,
	if (fabs(diffX) < 1.0 && fabs(diffY) < 1.0)
	{
		// Draw a single pixel at the location.
		int dotX = round(X1);
		int dotY = round(Y1);
		PF_SetPixel(dotX, dotY, rgb.Red, rgb.Green, rgb.Blue);
	}
	
	// if the x doesn't change, call the draw a veritcal line function
	if (fabs(diffX) < 1.0)
	{
		DrawVerticalLine(X1, Y1, Y2, rgb);
	}
	else
	// if the y doesn't change, call the draw a horizontal line function
	if (fabs(diffY) < 1.0)
	{
		DrawHorizontalLine(X1, X2, Y1, rgb);
	}
	else
	{
		// otherwise find out if the slope is large or small
		float slope = diffY / diffX;
		// if the slope greater than 1
		if (fabs(slope) > 1.0)
		{
			DrawLineDDALargeSlope(X1, Y1, X2, Y2, rgb);
		}
		else // otherwise 
		{
			DrawLineDDASmallSlope(X1, Y1, X2, Y2, rgb);
		}
	}

}

void DrawVerticalLine(float X, float Y1, float Y2, Color rgb)
{
	//move to integers (Round)
	int x = round(X);
	int y1 = round(Y1);
	int y2 = round(Y2);
	
	//check to see if we are walking up or down the line
	int increment;
	if (Y1 < Y2) increment = 1;
	else increment = -1;
	
	// while the start and end points aren't the same, set the pixel and move on
	int y = y1;
	while (y != y2)
	{
		PF_SetPixel(x, y, rgb.Red, rgb.Green, rgb.Blue);
		y += increment;
	}
	
	//set the pixel where Y1 == Y2
	PF_SetPixel(x, y, rgb.Red, rgb.Green, rgb.Blue);
}
void DrawHorizontalLine(float X1, float X2, float Y, Color rgb)
{
	//move to integers (Round)
	int y = round(Y);
	int x1 = round(X1);
	int x2 = round(X2);
	
	//check to see if we are walking up or down the line
	int increment;
	if (X1 < X2) increment = 1;
	else increment = -1;
	
	// while the start and end points aren't the same, set the pixel and move on
	int x = x1;
	while (x != x2)
	{
		PF_SetPixel(x, y, rgb.Red, rgb.Green, rgb.Blue);
		x += increment;
	}
	
	//set the pixel where X1 == X2
		PF_SetPixel(x, y, rgb.Red, rgb.Green, rgb.Blue);
}
void DrawLineDDALargeSlope(float X1, float Y1, float X2, float Y2, Color rgb)
{
	// Find inverse slope
	float diffX = X1 - X2;
	float diffY = Y1 - Y2;
	float slope = diffY / diffX;
	float invslope = 1.0 / slope;
	
	// round the start and end points for y
	float y1 = round(Y1);
	float y2 = round(Y2);
	
	// find what direction to increment the y
	int increment = 1;
	if (y1 > y2)
	{
		increment = -1;
		invslope *= -1;
	}
	
	// While we haven't walked from the start to the end,
	float x = X1;
	int y = y1;
	while (y != (int)y2)

	{
		// Set the pixel you're stepping on,
		PF_SetPixel(round(x + (y1 - Y1) * invslope), y, rgb.Red, rgb.Green, rgb.Blue);
		// increment the y, and calculate the new x
		x += invslope;
		y += increment;
	}

	// After the we've finished looping, draw the last point.
	PF_SetPixel(round(x), y, rgb.Red, rgb.Green, rgb.Blue);
  
}
void DrawLineDDASmallSlope(float X1, float Y1, float X2, float Y2, Color rgb)
{
	// Find the slope
	float diffX = X1 - X2;
	float diffY = Y1 - Y2;
	float slope = diffY / diffX;
  
  	// round the start and end points for x
	float x1 = round(X1);
	float x2 = round(X2);
	
	// find what direction to increment the x
	int increment = 1;
	if (x1 > x2)
	{
		increment = -1;
		slope *= -1;
	}
	// While we haven't walked from the start to the end,
	int x = x1;
	float y = Y1;
	while (x != (int)x2)
	{
		// Set the pixel you're stepping on,
		PF_SetPixel(x, round(y + (x1 - X1) * slope), rgb.Red, rgb.Green, rgb.Blue);
		// increment the x, and calculate the new y
		x += increment;
		y += slope;
	}
		
	// Set the last pixel after the loop is over.
	PF_SetPixel(x, round(y), rgb.Red, rgb.Green, rgb.Blue);
}