struct vertex;

struct face;

typedef struct s_half_edge {
	struct s_half_edge *prev, *next, *twin;
	struct vertex *origin;
	struct face *face; //face on the left
} half_edge;

typedef struct vertex{
	int x,y;
	half_edge *rep;
} vertex;

typedef struct face{
	half_edge *rep;
} face;

typedef struct s_structInsertVertex{
	half_edge *edge;
	vertex *new;
} structInsertVertex;

face *createFace();

vertex *createVertex(int x, int y);

vertex **initVertexList(int size);

void changeVertexListSize(vertex ***list, int newSize);

void catVertexList(vertex ***dest, int destSize, vertex ***src, int srcSize);

void sortVertexListX(vertex **list, int size);

void sortVertexListY(vertex **list, int size);

void createPolygon(int num, vertex **listCCW, face *in, face *out);

void insertVertex(vertex *v, half_edge *he);

void insertVertexKeep(vertex *v, half_edge *he);
//keepFace on the left
void insertEdge(vertex *src, vertex *dest, face *keepFace);

void insertEdgeUpdateFace(vertex *src, vertex *dest, face *keepFace);

void printVertexList(vertex **list, int size);

void printDCEL(vertex **list, int size, int mult);

int vertexConnected(vertex* vertex1, vertex* vertex2);

half_edge *getConnectedEdge(vertex* vertex1, vertex* vertex2);

half_edge *getUpEdge(vertex* vertex1);

half_edge *getDownEdge(vertex* vertex1);

half_edge *getRightEdge(vertex* vertex1);

half_edge *getLeftEdge(vertex* vertex1);

