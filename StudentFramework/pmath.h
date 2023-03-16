struct Point
{
	float x;
	float y;
	float z;
};

struct Vector
{
	float x;
	float y;
	float z;
};

struct Camera
{
	struct Point pos;
	struct Vector facing;
	struct Vector up;
	float focalLength;
	float** mat;
};

float dotp(struct Vector V1, struct Vector V2);
struct Vector crossp(struct Vector V1, struct Vector V2);
struct Vector proj(struct Vector projector, struct Vector projectee);
float Length(struct Vector v);
struct Point Perspective(struct Point P, float focalDistance);
struct Point ToScreen(struct Point p);
void camMatrix(struct Camera gCam);
struct Point toCamera(struct Camera gCam, struct Point p);

struct Point centerPoint(struct Point m[], int num);
void rotateV(struct Vector *v, float x, float y, float z);
void rotateMV(struct Vector *v, struct Vector axis, float rad);