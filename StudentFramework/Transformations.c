#include "pmath.h"
#include "Transformations.h"
#include <math.h>
#include "obj.h"

struct Vector multv(float((*m)[3][3]), struct Vector v)
{
	struct Vector result;
	result.x = (*m)[0][0] * v.x + (*m)[0][1] * v.y + (*m)[0][2] * v.z;
	result.y = (*m)[1][0] * v.x + (*m)[1][1] * v.y + (*m)[1][2] * v.z;
	result.z = (*m)[2][0] * v.x + (*m)[2][1] * v.y + (*m)[2][2] * v.z;

	return result;
}

struct Point multp(float((*m)[4][4]), struct Point p)
{
	struct Point result;
	result.x = (*m)[0][0] * p.x + (*m)[0][1] * p.y + (*m)[0][2] * p.z + (*m)[0][3];
	result.y = (*m)[1][0] * p.x + (*m)[1][1] * p.y + (*m)[1][2] * p.z + (*m)[1][3];
	result.z = (*m)[2][0] * p.x + (*m)[2][1] * p.y + (*m)[2][2] * p.z + (*m)[2][3];

	return result;
}

void multm(float((*m1)[4][4]), float((*m2)[4][4]), float((*mr)[4][4]))
{
	//multiplies two matrices
	int r;
	int c;
	int i;

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			(*mr)[r][c] = 0;
			for (i = 0; i < 4; i++)
			{
				(*mr)[r][c] += (*m1)[r][i] * (*m2)[i][c];
			}
		}
	}
}

void scale(struct Point * p, struct Point center, float x, float y, float z)
{
	//scales a point using a vector

	//make copy of p
	struct Point pc = *p;

	//turn the center into a vector for use in translate funcs (o2c = origin to center)
	struct Vector o2c;
	o2c.x = center.x;
	o2c.y = center.y;
	o2c.z = center.z;

	//move stuff
	reverseTranslate(&pc, o2c);
	
	//scale
	/*
	pc.x *= x;
	pc.y *= y;
	pc.z *= z;
	*/
	float smat[4][4];
	smat[0][0] = x;
	smat[0][1] = 0;
	smat[0][2] = 0;
	smat[0][3] = 0;
	smat[1][0] = 0;
	smat[1][1] = y;
	smat[1][2] = 0;
	smat[1][3] = 0;
	smat[2][0] = 0;
	smat[2][1] = 0;
	smat[2][2] = z;
	smat[2][3] = 0;
	smat[3][0] = 0;
	smat[3][1] = 0;
	smat[3][2] = 0;
	smat[3][3] = 1;

	pc = multp(&smat, pc);

	//move stuff back
	translate(&pc, o2c);

	*p = pc;
}

void rotate(struct Point * p, struct Point center, float x, float y, float z)
{
	//rotates a point around the origin

	//turn the center into a vector for use in translate funcs (o2c = origin to center)
	struct Vector o2c;
	o2c.x = center.x;
	o2c.y = center.y;
	o2c.z = center.z;

	//set center as origin
	reverseTranslate(p, o2c);

	//define matrices
	float xrmat[4][4];
	xrmat[0][0] = 1;
	xrmat[0][1] = 0;
	xrmat[0][2] = 0;
	xrmat[0][3] = 0;
	xrmat[1][0] = 0;
	xrmat[1][1] = (float)cos(x);
	xrmat[1][2] = -1 * (float)sin(x);
	xrmat[1][3] = 0;
	xrmat[2][0] = 0;
	xrmat[2][1] = (float)sin(x);
	xrmat[2][2] = (float)cos(x);
	xrmat[2][3] = 0;
	xrmat[3][0] = 0;
	xrmat[3][1] = 0;
	xrmat[3][2] = 0;
	xrmat[3][3] = 1;

	float yrmat[4][4];
	yrmat[0][0] = (float)cos(y);
	yrmat[0][1] = 0;
	yrmat[0][2] = (float)sin(y);
	yrmat[0][3] = 0;
	yrmat[1][0] = 0;
	yrmat[1][1] = 1;
	yrmat[1][2] = 0;
	yrmat[1][3] = 0;
	yrmat[2][0] = -1 * (float)sin(y);
	yrmat[2][1] = 0;
	yrmat[2][2] = (float)cos(y);
	yrmat[2][3] = 0;
	yrmat[3][0] = 0;
	yrmat[3][1] = 0;
	yrmat[3][2] = 0;
	yrmat[3][3] = 1;

	float zrmat[4][4];
	zrmat[0][0] = (float)cos(z);
	zrmat[0][1] = -1 * (float)sin(z);
	zrmat[0][2] = 0;
	zrmat[0][3] = 0;
	zrmat[1][0] = (float)sin(z);
	zrmat[1][1] = (float)cos(z);
	zrmat[1][2] = 0;
	zrmat[1][3] = 0;
	zrmat[2][0] = 0;
	zrmat[2][1] = 0;
	zrmat[2][2] = 1;
	zrmat[2][3] = 0;
	zrmat[3][0] = 0;
	zrmat[3][1] = 0;
	zrmat[3][2] = 0;
	zrmat[3][3] = 1;

	float xyrmat[4][4];
	float rmat[4][4];

	multm(&xrmat, &yrmat, &xyrmat);
	multm(&xyrmat, &zrmat, &rmat);

	//apply the matrix
	*p = multp(&rmat, *p);

	//move the thing back
	translate(p, o2c);
}

void translate(struct Point * p, struct Vector v)
{
	//moves a point using a vector

	/*
	p->x += v.x;
	p->y += v.y;
	p->z += v.z;
	*/

	float tmat[4][4];
	tmat[0][0] = 1;
	tmat[0][1] = 0;
	tmat[0][2] = 0;
	tmat[0][3] = v.x;
	tmat[1][0] = 0;
	tmat[1][1] = 1;
	tmat[1][2] = 0;
	tmat[1][3] = v.y;
	tmat[2][0] = 0;
	tmat[2][1] = 0;
	tmat[2][2] = 1;
	tmat[2][3] = v.z;
	tmat[3][0] = 0;
	tmat[3][1] = 0;
	tmat[3][2] = 0;
	tmat[3][3] = 1;

	*p = multp(&tmat, *p);
}

void reverseTranslate(struct Point * p, struct Vector v)
{
	//moves a point backwards along a vector

	/*
	p->x -= v.x;
	p->y -= v.y;
	p->z -= v.z;
	*/

	float tmat[4][4];
	tmat[0][0] = 1;
	tmat[0][1] = 0;
	tmat[0][2] = 0;
	tmat[0][3] = -1 * v.x;
	tmat[1][0] = 0;
	tmat[1][1] = 1;
	tmat[1][2] = 0;
	tmat[1][3] = -1 * v.y;
	tmat[2][0] = 0;
	tmat[2][1] = 0;
	tmat[2][2] = 1;
	tmat[2][3] = -1 * v.z;
	tmat[3][0] = 0;
	tmat[3][1] = 0;
	tmat[3][2] = 0;
	tmat[3][3] = 1;

	*p = multp(&tmat, *p);
}

void translateMesh(struct Mesh * mesh, struct Vector v)
{
	for (int i = 0; i < mesh->vertcount; i++)
	{
		mesh->points[i].x += v.x;
		mesh->points[i].y += v.y;
		mesh->points[i].z += v.z;
	}
}

void rotateShape(struct Point points[], int verts, struct Point center, float x, float y, float z)
{
	for (int i = 0; i < verts; i++)
	{
		rotate(&points[i], center, x, y, z);
	}
}

void scaleShape(struct Point points[], int verts, struct Point center, float x, float y, float z)
{
	for (int i = 0; i < verts; i++)
	{
		scale(&points[i], center, x, y, z);
	}
}