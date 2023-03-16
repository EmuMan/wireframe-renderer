#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "images.h"
#include "LineRasterizer.h"

struct Color *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr; //our file pointer
	BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
	struct Color *bitmapImage;  //store image data
	unsigned char *charList; //stores temporary character list
	unsigned int i = 0;  //image index counter
	unsigned int imageSize;

							//open filename in read binary mode
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL)
		return NULL;

	//read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	//verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.bfType != 0x4D42)
	{
		fclose(filePtr);
		return NULL;
	}

	//read the bitmap info header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); // small edit. forgot to add the closing bracket at sizeof

																   //move file point to the begging of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	if (bitmapInfoHeader->biSizeImage == 0)
		imageSize = bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight * 3;
	else
		imageSize = bitmapInfoHeader->biSizeImage;

	//allocate enough memory for the temporary character list
	charList = (unsigned char *)malloc(sizeof(unsigned char) * imageSize);

	//allocate enough memory for the bitmap image data
	bitmapImage = (struct Color *)malloc(sizeof(struct Color) * imageSize / 3);

	//verify memory allocation
	if (!charList)
	{
		free(charList);
		fclose(filePtr);
		return NULL;
	}
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//read in the bitmap image data
	fread(charList, imageSize, 1, filePtr);
	
	//make sure bitmap image data was read
	if (charList == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	//swap the r and b values to get RGB
	/*
	for (i = 0; i < imageSize; i += 3)
	{
		tempRGB = charList[i];
		charList[i] = charList[i + 2];
		charList[i + 2] = tempRGB;
	}
	*/

	for (i = 0; i < imageSize / 3; i += 1)
	{
		bitmapImage[i].Red = charList[i * 3 + 2];
		bitmapImage[i].Green = charList[i * 3 + 1];
		bitmapImage[i].Blue = charList[i * 3 + 0];
	}

	//close file, free the character list, and return bitmap image data
	fclose(filePtr);
	return bitmapImage;
}