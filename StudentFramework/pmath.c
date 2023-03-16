#include "pmath.h"
#include "FrameworkInterface.h"
#include "Transformations.h"
#include "pfmath.h"
#include <math.h>

void printm(float(*m)[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int o = 0; o < 4; o++)
		{
			printf("%f ", (*m)[i][o]);
		}
		printf("\n");
	}
}

float dotp(struct Vector V1, struct Vector V2)
{
	return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
}

struct Vector crossp(struct Vector V1, struct Vector V2)
{
	struct Vector result;

	result.x = ((V1.y) * (V2.z)) - ((V2.y) * (V1.z));
	result.y = ((V1.z) * (V2.x)) - ((V2.z) * (V1.x));
	result.z = ((V1.x) * (V2.y)) - ((V2.x) * (V1.y));

	return result;
}

//projects the projectee on to the projector
struct Vector proj(struct Vector projector, struct Vector projectee)
{
	float mod = dotp(projector, projectee) / dotp(projector, projector);
	struct Vector result;

	result.x = projector.x * mod;
	result.y = projector.y * mod;
	result.z = projector.z * mod;

	return result;
}

float Length(struct Vector v)
{
	return sqrt(dotp(v, v));
}

struct Point Perspective(struct Point P, float focalDistance)
{
	/*
	1. Get the line from the plane in the x, y directions, to the point.
	   A = struct Point {x = 0, y = 0, z = -1 * focalDistance}

	2. Vector PA = P - A

	2.5 Vector N - {0, 0, -1}

	project PA onto the normal of the plane
	3. Vector Np = proj(N, PA)


	4. Vector CP = Make a vector with x, y, and z equal to P's x, y, and z

	5. Get the length of CP using our helper function, store it in a float named L
	6. GEt the length of Np using our helper function
	7. float t = focalDistance * CP/(focalDistance + Length(NP))
	^the length of the 'perspective projection' vector

	8. Vector result;
	set result equal to the CP vector, divided by its length, and multiplied by t.

	9. return result
	*/

	if (P.z >= -1 * focalDistance)
	{
		struct Point result;
		result.x = P.x;
		result.y = P.y;
		result.z = 1;
	}
	struct Point A;
	A.x = 0.0f;
	A.y = 0.0f;
	A.z = -1.0f * focalDistance;

	struct Vector PA;
	PA.x = P.x - A.x;
	PA.y = P.y - A.y;
	PA.z = P.z - A.z;

	struct Vector N = { 0, 0, -1 };

	struct Vector NP = proj(N, PA);

	struct Vector CP;
	CP.x = P.x;
	CP.y = P.y;
	CP.z = P.z;

	float L = Length(CP);
	
	float t = focalDistance * L / (focalDistance + Length(NP));

	struct Point result;
	result.x = (CP.x / L) * t;
	result.y = (CP.y / L) * t;
	result.z = (CP.z / L) * t;

	return result;
}

struct Point ToScreen(struct Point p)
{
	// Get screen width  (type WIDTH)
	// Get screen height (type HEIGHT)
	// make a point that is equal to p, that you will modify and return 
	struct Point p1 = p;

	// modify point:
	// invert y (multiply y value by -1)
	p1.y *= -1;
	// add 1 to x value, add 1 to y value 
	p1.x += 1;
	p1.y += 1;
	// scale by 0.5f in both x and y directions
	p1.x *= 0.5f;
	p1.y *= 0.5f;
	// scale by width on the x axis
	//p1.x = p1.x * HEIGHT + (WIDTH - HEIGHT) / 2;
	p1.x *= WIDTH;
	// scale by height on the y axis
	p1.y = p1.y * WIDTH - (WIDTH - HEIGHT) / 2;
	//p1.y *= HEIGHT;

	// return modified point
	return p1;

}

void camMatrix(struct Camera cam)
{
	float tmat[4][4];
	float hmmmat[4][4];
	float fmat[4][4];

	//normalize facing vector
	float l = Length(cam.facing);

	struct Vector nFacing = cam.facing;
	nFacing.x = nFacing.x / l;
	nFacing.y = nFacing.y / l;
	nFacing.z = nFacing.z / l;

	//create right and relative up vectors
	struct Vector right = crossp(cam.facing, cam.up);
	struct Vector rUp = crossp(right, cam.facing);

	//normalize right and relative up vectors
	struct Vector nRight = right;
	struct Vector nrUp = rUp;

	l = Length(right);
	nRight.x /= l;
	nRight.y /= l;
	nRight.z /= l;

	l = Length(rUp);
	nrUp.x /= l;
	nrUp.y /= l;
	nrUp.z /= l;

	//create the rotation and translation matrices
	hmmmat[0][0] = nRight.x;
	hmmmat[0][1] = nRight.y;
	hmmmat[0][2] = nRight.z;
	hmmmat[0][3] = 0;
	hmmmat[1][0] = nrUp.x;
	hmmmat[1][1] = nrUp.y;
	hmmmat[1][2] = nrUp.z;
	hmmmat[1][3] = 0;
	hmmmat[2][0] = -1 * nFacing.x;
	hmmmat[2][1] = -1 * nFacing.y;
	hmmmat[2][2] = -1 * nFacing.z;
	hmmmat[2][3] = 0;
	hmmmat[3][0] = 0;
	hmmmat[3][1] = 0;
	hmmmat[3][2] = 0;
	hmmmat[3][3] = 0;

	tmat[0][0] = 1;
	tmat[0][1] = 0;
	tmat[0][2] = 0;
	tmat[0][3] = -1 * cam.pos.x;
	tmat[1][0] = 0;
	tmat[1][1] = 1;
	tmat[1][2] = 0;
	tmat[1][3] = -1 * cam.pos.y;
	tmat[2][0] = 0;
	tmat[2][1] = 0;
	tmat[2][2] = 1;
	tmat[2][3] = -1 * cam.pos.z;
	tmat[3][0] = 0;
	tmat[3][1] = 0;
	tmat[3][2] = 0;
	tmat[3][3] = 0;

	//multiply the matrices into fmat
	multm(&hmmmat, &tmat, &fmat);

	//convert fmat into a float**
	for (int i = 0; i < 4; i++)
	{
		for (int o = 0; o < 4; o++)
		{
			cam.mat[i][o] = fmat[i][o];
		}
	}
}

struct Point toCamera(struct Camera cam, struct Point p)
{
	float fmat[4][4];

	//convert float** into (float*)[4][4]
	for (int i = 0; i < 4; i++)
	{
		for (int o = 0; o < 4; o++)
		{
			fmat[i][o] = cam.mat[i][o];
		}
	}

	//return the matrix applied to the point
	return multp(&fmat, p);
}

struct Point centerPoint(struct Point m[], int num)
{
	struct Point result;

	float sumX = 0.0f;
	float sumY = 0.0f;
	float sumZ = 0.0f;

	for (int i = 0; i < num; i++)
	{
		sumX += m[i].x;
		sumY += m[i].y;
		sumZ += m[i].z;
	}

	result.x = sumX / num;
	result.y = sumY / num;
	result.z = sumZ / num;

	return result;
}

void rotateV(struct Vector *v, float x, float y, float z)
{
	struct Point zero = { 0.0f, 0.0f, 0.0f };
	struct Point temp;

	temp.x = v->x;
	temp.y = v->y;
	temp.z = v->z;

	rotate(&temp, zero, x, y, z);

	v->x = temp.x;
	v->y = temp.y;
	v->z = temp.z;
}

void rotateMV(struct Vector *v, struct Vector axis, float rad)
{
	struct Point temp;
	temp.x = v->x;
	temp.y = v->y;
	temp.z = v->z;

	struct rMat mat = getRotateMatrix(axis, (rad / 3.141592f) * 180);

	temp = multp(&mat.m, temp);
	//printm(&mat.m);

	v->x = temp.x;
	v->y = temp.y;
	v->z = temp.z;
}