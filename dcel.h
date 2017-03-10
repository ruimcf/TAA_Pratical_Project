
typedef struct s_dcel {
	int x, y;
	struct s_dcel *up, *right, *down, *left;
} Dcel;

Dcel createNode(int x, int y);

void connectNode(Dcel *node1, Dcel *node2);

//inserts "insert" node between node1 and node2
void insertNode(Dcel insert, Dcel *node1, Dcel *node2);

