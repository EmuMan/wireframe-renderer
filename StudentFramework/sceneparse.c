#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "pmath.h"
#include "obj.h"
#include "sceneparse.h"
#include "Transformations.h"
#include "FrameworkInterface.h"
#include "images.h"

char * strParse(char * string, int index)
{
	int word = 0;
	int write = 0;

	//allocate the returned pointer for use in other functions, will be freed in any function it is used
	char * buffer = malloc(sizeof(char) * 16);

	for (int i = 0; 1; i++)
	{
		if (string[i] == '\0')
		{
			break;
		}
		if (string[i] == ' ')
		{
			word++;
			continue;
		}
		if (word == index)
		{
			buffer[write] = string[i];
			write++;
		}
	}

	buffer[write] = '\0';

	return buffer;
}

struct Scene * readScene(char filename[64])
{
	FILE* fp;
	char buffer[128];
	char mode = 'n';
	int index = -1;

	//allocate the scene
	struct Scene * scene = malloc(sizeof(struct Scene));
	
	//open the specified scene file
	fopen_s(&fp, filename, "rt");

	//error if file failed to open
	if (fp == NULL)
	{
		printf("oh no, the level file failed to open correctly");
		exit(-1);
	}

	//set scene's object count to zero, add one for every line starting with m
	scene->objcount = 0;

	while (!feof(fp))
	{
		if (fgets(buffer, 128, fp) != 0)
		{
			if (buffer[0] == 'm')
			{
				scene->objcount++;
			}
		}
	}

	//use scene's object count to allocate the scene's object list
	scene->objects = malloc(sizeof(struct Mesh) * scene->objcount);

	//jump to top of file
	rewind(fp);

	//main parser
	while (!feof(fp))
	{
		if (fgets(buffer, 128, fp) != 0)
		{
			if (buffer[0] == '\n' || buffer[0] == '#')
			{
				//blank lines and comments are skipped
				continue;
			}
			else if (buffer[0] == 'c')
			{
				//camera mode
				mode = 'c';
			}
			else if (buffer[0] == 'm')
			{
				//mesh mode, set the mesh at index to a new mesh, specified by the scene file
				mode = 'm';
				index++;
				char * ptr = strParse(buffer, 1);
				scene->objects[index] = createMesh(ptr);
				ptr = strParse(buffer, 2);
				strcpy_s(scene->objects[index].name, 64, ptr);
				free(ptr);
			}
			else if (buffer[0] == 'p')
			//position
			{
				if (mode == 'c')
				{
					scene->cam.pos.x = fParse(buffer, 1);
					scene->cam.pos.y = fParse(buffer, 2);
					scene->cam.pos.z = fParse(buffer, 3);
				}
				else if (mode == 'm')
				{
					scene->objects[index].loc.x = fParse(buffer, 1);
					scene->objects[index].loc.y = fParse(buffer, 2);
					scene->objects[index].loc.z = fParse(buffer, 3);
				}
				else
				{
					printf("Error parsing scene file: attempted to set the position of a non-mesh/camera");
					exit(-1);
				}
			}
			else if (buffer[0] == 'r')
			//rotation
			{
				if (mode == 'c')
				{
					scene->cam.facing.x = fParse(buffer, 1);
					scene->cam.facing.y = fParse(buffer, 2);
					scene->cam.facing.z = fParse(buffer, 3);
				}
				else if (mode == 'm')
				{
					scene->objects[index].rot.x = fParse(buffer, 1);
					scene->objects[index].rot.y = fParse(buffer, 2);
					scene->objects[index].rot.z = fParse(buffer, 3);
				}
				else
				{
					printf("Error parsing scene file: attempted to set the rotation of a non-mesh/camera");
					exit(-1);
				}
			}
			else if (buffer[0] == 'u')
			//up vector
			{
				if (mode == 'c')
				{
					scene->cam.up.x = fParse(buffer, 1);
					scene->cam.up.y = fParse(buffer, 2);
					scene->cam.up.z = fParse(buffer, 3);
				}
				else
				{
					printf("Error parsing scene file: attempted to set the up-vector of a non-camera");
					exit(-1);
				}
			}
			else if (buffer[0] == 'd')
			{
				if (mode == 'c')
				{
					scene->cam.focalLength = fParse(buffer, 1);
				}
				else
				{
					printf("Error parsing scene file: attempted to set the focal distance of a non-camera");
					exit(-1);
				}
			}
			else if (buffer[0] == 's')
			{
				if (mode == 'm')
				{
					scene->objects[index].scale.x = fParse(buffer, 1);
					scene->objects[index].scale.y = fParse(buffer, 2);
					scene->objects[index].scale.z = fParse(buffer, 3);
				}
				else
				{
					printf("Error parsing scene file: attempted to set the scale of a non-mesh");
					exit(-1);
				}
			}
			else if (buffer[0] == 'b')
			{
				char * tempPtr = strParse(buffer, 1);
				scene->bg = LoadBitmapFile(tempPtr, &(scene->bgInfo));
			}
			else
			{
				printf("Error parsing scene file: unsupported line start");
				exit(-1);
			}
		}
	}
	
	fclose(fp);

	for (int i = 0; i < scene->objcount; i++)
	{
		scene->objects[i].center.x = 0.0f;
		scene->objects[i].center.y = 0.0f;
		scene->objects[i].center.z = 0.0f;
	}

	scene->cam.mat = alloc2d(4, 4);

	return scene;
}