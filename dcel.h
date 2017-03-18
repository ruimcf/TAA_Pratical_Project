
typedef struct s_half_edge {
	struct s_half_edge *prev, *next, *twin;
	vertex *origin;
	face *face; //face on the left
} half_edge;

typedef struct vertex{
	int x,y;
	half_edge *rep;
} vertex;

typedef struct face{
	half_edge *rep;
} face;

vertex createVertex(int x, int y);

vertex **initVertexList(int size);

half_edge **createPolygon(int num, vertex *listCCW, face *in, face *out);

void printVertex(vertex **list, int size);//todo print DCEL
