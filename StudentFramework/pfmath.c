#include "pfmath.h"
#include "pmath.h"
#include <math.h>

struct rMat I;
int iSetup = 0;


struct rMat getRotateMatrix(struct Vector axis, float degrees)
{
	if (!iSetup)
	{
		iSetup = !iSetup;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i != j)
					I.m[i][j] = 0.0f;
				else
					I.m[i][j] = 1.0f;
			}
		}
	}
	float l = Length(axis);
	if (l != 1.0f)
	{
		axis.x = axis.x / l;
		axis.y = axis.y / l;
		axis.z = axis.z / l;
	}

	struct rMat result = I;
	struct rMat intermedSin;
	struct rMat intermedCos;
	struct rMat intermedFinal;

	float rads = (degrees / 180.0f) * 3.14159f;
	float sinval = (float)(sin(rads));
	float cosval = (float)(cos(rads));

	intermedSin.m[0][0] = 0;
	intermedSin.m[1][1] = 0;
	intermedSin.m[2][2] = 0;
	intermedSin.m[3][3] = 0;
	intermedSin.m[0][1] = axis.z * -1.0f * sinval;
	intermedSin.m[0][2] = axis.y * sinval;	
	intermedSin.m[0][3] = 0.0f;
	intermedSin.m[1][0] = axis.z * sinval;
	intermedSin.m[1][2] = axis.x * -1.0f * sinval;
	intermedSin.m[1][3] = 0.0f;
	intermedSin.m[2][0] = axis.y * -1.0f * sinval;
	intermedSin.m[2][1] = axis.x * sinval;
	intermedSin.m[2][3] = 0.0f;
	intermedSin.m[3][0] = 0.0f;
	intermedSin.m[3][1] = 0.0f;
	intermedSin.m[3][2] = 0.0f;
	intermedSin.m[3][3] = 1.0f;

	intermedCos = I;
	intermedFinal = I;
	for (int i = 0; i < 3; ++i)
	{
		float v1;

		switch (i)
		{
		case 0:
			v1 = axis.x;
			break;
		case 1:
			v1 = axis.y;
			break;
		case 2:
			v1 = axis.z;
			break;
		}
		for (int j = 0; j < 3; ++j)
		{

			float v2;
			switch (j)
			{
			case 0:
				v2 = axis.x;
				break;
			case 1:
				v2 = axis.y;
				break;
			case 2:
				v2 = axis.z;
				break;
			}
			intermedFinal.m[i][j] = v1*v2;
		}
	}
	intermedFinal;

	for (int i = 0; i < 3; ++i)
	{
		float v1;

		switch (i)
		{
		case 0:
			v1 = axis.x;
			break;
		case 1:
			v1 = axis.y;
			break;
		case 2:
			v1 = axis.z;
			break;
		}
		for (int j = 0; j < 3; ++j)
		{

			float v2;
			switch (j)
			{
			case 0:
				v2 = axis.x;
				break;
			case 1:
				v2 = axis.y;
				break;
			case 2:
				v2 = axis.z;
				break;
			}

			intermedCos.m[i][j] = (I.m[i][j] - intermedFinal.m[i][j])*cosval;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			result.m[i][j] = intermedSin.m[i][j] + intermedCos.m[i][j] + intermedFinal.m[i][j];
		}
	}


	return result;
}