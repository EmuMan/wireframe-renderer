#include <stdio.h>
#include <stdlib.h>
#include "pmath.h"
#include "obj.h"

float fParse(char string[], int index)
{
	int word = 0;
	int write = 0;
	char buffer[16];

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

	return (float)atof(buffer);
}

int iParse(char string[], int index)
{
	int word = 0;
	int write = 0;
	char buffer[16];

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

	return atoi(buffer);
}

void readObj(char filename[64], struct Mesh * m)
{
	FILE* fp;
	char buffer[128];
	int vertcount = 0;
	int edgecount = 0;

	fopen_s(&fp, filename, "rt");

	if (fp == NULL)
	{
		printf("oh no, the file failed to open correctly");
		return;
	}

	while (!feof(fp))
	{
		if (fgets(buffer, 128, fp) != 0)
		{
			if (buffer[0] == 'v')
			{
				m->points[vertcount].x = fParse(buffer, 1);
				m->points[vertcount].y = fParse(buffer, 2);
				m->points[vertcount].z = fParse(buffer, 3);
				vertcount++;
			}
			else if (buffer[0] == 'f' || buffer[0] == 'l') {
				m->edges[edgecount * 2] = iParse(buffer, 1) - 1;
				m->edges[edgecount * 2 + 1] = iParse(buffer, 2) - 1;
				edgecount++;
			}
		}
	}

	fclose(fp);
}

struct Mesh createMesh(char filename[])
{
	struct Mesh result;

	FILE* fp;
	char buffer[128];
	int vertcount = 0;
	int edgecount = 0;

	fopen_s(&fp, filename, "rt");

	if (fp == NULL)
	{
		printf("oh no, the file failed to open correctly");
		exit(-1);
	}

	while (!feof(fp))
	{
		if (fgets(buffer, 128, fp) != 0)
		{
			if (buffer[0] == 'v')
			{
				vertcount++;
			}
			else if (buffer[0] == 'f' || buffer[0] == 'l') {
				edgecount++;
			}
		}
	}

	fclose(fp);

	result.vertcount = vertcount;
	result.edgecount = edgecount;

	result.points = malloc(sizeof(struct Point) * vertcount);
	result.edges = malloc(sizeof(int) * edgecount * 2);

	readObj(filename, &result);

	return result;
};