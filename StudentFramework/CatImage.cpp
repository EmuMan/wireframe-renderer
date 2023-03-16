#include "CatImage.h"
#include "LineRasterizer.h"

void DrawCatImage(void)
{
  Color black;
  Color red;
  Color blue;
  
  black.Red = 0;
  black.Green = 0;
  black.Blue = 0;
  
  red.Red = 255;
  red.Green = 0;
  red.Blue = 0;

  blue.Red = 0;
  blue.Green = 0;
  blue.Blue = 255;

  //nose
  DrawLineDDA(320,240, 350, 300, red);
  DrawLineDDA(350,300, 290, 300, red);
  DrawLineDDA(290,300, 320, 240, red);

  //head
  DrawLineDDA(150, 100, 500, 100, black);
  DrawLineDDA(500, 100, 550, 130, black);
  DrawLineDDA(550, 130, 320, 480, black);
  DrawLineDDA(550, 130, 320, 480, black);
  DrawLineDDA(320, 480, 100, 130,  black);
  DrawLineDDA(100, 130, 150, 100, black);

  //mouth
  DrawLineDDA(350, 380, 290, 380, black);
  DrawLineDDA(290, 380, 260, 350, black);
  DrawLineDDA(350, 380, 380, 350, black);

  //right eye
  DrawLineDDA(380, 150, 400, 200, blue);
  DrawLineDDA(400, 200, 380, 250, blue);
  DrawLineDDA(380, 250, 360, 200, blue);
  DrawLineDDA(360, 200, 380, 150, blue);

  //left eye
  DrawLineDDA(270, 150, 250, 200,  blue);
  DrawLineDDA(250, 200, 270, 250,  blue);
  DrawLineDDA(270, 250, 290, 200,  blue);
  DrawLineDDA(290, 200, 270, 150,  blue);
 
  //right ear
  DrawLineDDA(525, 115, 550, 70 , black);
  DrawLineDDA(550, 70, 590, 70, black);
  DrawLineDDA(590, 70, 590, 30,  black);
  DrawLineDDA(590, 30, 550, 30,  black);
  DrawLineDDA(550, 30, 550, 70,  black);


  //left ear
  DrawLineDDA(120, 120 ,100,  70 , black);
  DrawLineDDA(100, 70, 60, 70, black);
  DrawLineDDA(60, 70, 60, 30, black);
  DrawLineDDA(60, 30, 100, 30, black);
  DrawLineDDA(100, 30, 100, 70, black);
  
  
}