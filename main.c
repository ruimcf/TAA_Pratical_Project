#include <stdio.h>
#include <dcel.h>

/*
 * Pseudo code to Horizontal sweep algorithm
 *
 * -Order the outer points by the biggest Y
 * -Iterate for all ordered points with different Y
 *	-Trace horizontal line at that Y
 *	-Determine all line segments that are inside the polygon
 *	by checking intersection between the line and all vertical edges
 *	-Iterate for all line segments inside the polygon
 *		-Determine the intersection points of the line segment
 *		with vertical edges of holes
 *		-Update Partition structure
 */
int input();

int main(int argc, char *argv[]){
	vertex **vertices;
	int listSize = input(vertices);
}

int input(vertex **list){
	int option, numberOfVertices, numberOfHoles, numberOfVerticesHole;
	int vertexX, vertexY;

	printf("Select OPTION\n0 For Horizontal Partition\n1 For Grid Partition\n");
	scanf("%d", &option);

	printf("Number of outer vertices\n");
	scanf("%d", &numberOfVertices);
	list = initVertexList(numberOfVertices);
	vertex v;
	for(int i = 0; i < numberOfVertices; i++){
		scanf("%d %d", &vertexX, &vertexY);
		list[i]=createVertex(vertexX, vertexY);
	}
	face *in=createFace(),
	     *out=createFace();
	createPolygon(numberOfVertices, list, in, out);
	printf("Number of Holes\n");
	scanf("%d", &numberOfHoles);
	face **listFaceHole=malloc(sizeof(face*));
	for(int i = 0; i < numberOfHoles; i++){
		listFaceHole[i]=createFace();
		printf("Number of vertices in %d Hole\n", i+1);
		scanf("%d", &numberOfVerticesHole);
		vertex **listHole = initVertexList(numberOfVerticesHole);
		for(int e=0; e<numberOfVerticesHole; e++){
			scanf("%d %d", &vertexX, &vertexY);
			listHole[e]=createVertex(vertexX, vertexY);
		}
		createPolygon(numberOfVerticesHole, listHole, in, listFaceHole[i]);
		catVertexList(list, numberOfVertices, listHole, numberOfVerticesHole);
		numberOfVertices+=numberOfVerticesHole;
	}
	free(listFaceHole);
	return numberOfVertices;
}
