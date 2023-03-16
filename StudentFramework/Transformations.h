struct Vector multv(float((*m)[3][3]), struct Vector v);
struct Point multp(float((*m)[4][4]), struct Point p);
void multm(float((*m1)[4][4]), float((*m2)[4][4]), float((*mr)[4][4]));

void scale(struct Point * p, struct Point center, float x, float y, float z);
void rotate(struct Point * p, struct Point center, float x, float y, float z);
void translate(struct Point * p, struct Vector v);
void reverseTranslate(struct Point * p, struct Vector v);
void translateMesh(struct Mesh * mesh, struct Vector v);
void rotateShape(struct Point points[], int verts, struct Point center, float x, float y, float z);
void scaleShape(struct Point points[], int verts, struct Point center, float x, float y, float z);