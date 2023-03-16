struct Mesh
{
	int vertcount;
	int edgecount;

	struct Point * points;
	int * edges;

	char name[64];

	struct Point loc;
	struct Point rot;
	struct Point scale;

	struct Point center;
};

void readObj(char filename[64], struct Mesh * m);
struct Mesh createMesh(char filename[]);
float fParse(char string[], int index);
int iParse(char string[], int index);