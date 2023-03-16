#include "pmath.h"
#include "FrameworkInterface.h"
#include <math.h>

struct Vector multv(float((*m)[3][3]), struct Vector v)
{
struct Vector result;
result.x = (*m)[0][0] * v.x + (*m)[0][1] * v.y + (*m)[0][2] * v.z;
result.y = (*m)[1][0] * v.x + (*m)[1][1] * v.y + (*m)[1][2] * v.z;
result.z = (*m)[2][0] * v.x + (*m)[2][1] * v.y + (*m)[2][2] * v.z;

return result;
}

struct Point multp(float((*m)[3][3]), struct Point p)
{
struct Point result;
result.x = (*m)[0][0] * p.x + (*m)[0][1] * p.y + (*m)[0][2] * p.z;
result.y = (*m)[1][0] * p.x + (*m)[1][1] * p.y + (*m)[1][2] * p.z;
result.z = (*m)[2][0] * p.x + (*m)[2][1] * p.y + (*m)[2][2] * p.z;

return result;
}
struct Point translatePoint(struct Point* p, struct Point* c)
{
	struct Point result;
	result = *p;
	result.x -= c->x;
	result.y -= c->y;
	result.z -= c->z;

	return result;
}
#define THETA 0.052359f

struct Point translateBack(struct Point *p, struct Point* c)
{
	struct Point result;
	result = *p;
	result.x += c->x;
	result.y += c->y;
	result.z += c->z;

	return result;
}

void rotateTriangle(struct Point * p1, struct Point * p2, struct Point * p3)
{
	// find center 
	struct Point center;
	center.x = (p1->x + p2->x + p3->x) / 3.0f;
	center.y = (p1->y + p2->y + p3->y) / 3.0f;
	center.z = (p1->z + p2->z + p3->z) / 3.0f;

	// we're gonna rotate around the center 
	struct Point t1 = translatePoint(p1, &center);
	struct Point t2 = translatePoint(p2, &center);
	struct Point t3 = translatePoint(p3, &center);

	// now we need to rotate around the center
	// we'll do so perpi
	float rmat[3][3];
	rmat[0][0] = cos(THETA);
	rmat[0][1] = 0;
	rmat[0][2] = sin(THETA);
	rmat[1][0] = 0;
	rmat[1][1] = 1;
	rmat[1][2] = 0;
	rmat[2][0] = -1 * sin(THETA);
	rmat[2][1] = 0;
	rmat[2][2] = cos(THETA);

	t1 = multp(&rmat, t1);
	t2 = multp(&rmat, t2);
	t3 = multp(&rmat, t3);

	*p1 = translateBack(&t1, &center);
	*p2 = translateBack(&t2, &center);
	*p3 = translateBack(&t3, &center);
}

