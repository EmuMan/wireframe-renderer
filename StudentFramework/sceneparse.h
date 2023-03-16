struct Scene
{
	struct Mesh * objects;
	int objcount;
	struct Camera cam;
	struct Color * bg;
	BITMAPINFOHEADER bgInfo;
};

struct Scene * readScene(char filename[64]);