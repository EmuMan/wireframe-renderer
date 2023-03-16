#ifndef LINERASTERIZER_H
#define LINERASTERIZER_H

typedef struct Color
{
  unsigned char Red;
  unsigned char Green;
  unsigned char Blue;

}Color;

//interface call
void DrawLineDDA(float X1, float Y1, float X2, float Y2, Color rgb);

//helper functions go here
void DrawVerticalLine(float X, float Y1, float Y2, Color rgb);
void DrawHorizontalLine(float X1, float X2, float Y, Color rgb);
void DrawLineDDALargeSlope(float X1, float Y1, float X2, float Y2, Color rgb);
void DrawLineDDASmallSlope(float X1, float Y1, float X2, float Y2, Color rgb);


#endif