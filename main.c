#include <stdio.h>
#include <dcel.h>

/*
 * Pseudo code to Horizontal sweep algorithm
 *
 * -Order the outer points by the biggest Y
 * -Iterate for all ordered points with different Y
 *    -Trace horizontal line at that Y
 *    -Determine all line segments that are inside the polygon
 *    by checking intersection between the line and all vertical edges
 *    -Iterate for all line segments inside the polygon
 *        -Determine the intersection points of the line segment
 *        with vertical edges of holes
 *        -Update Partition structure
 */
void input();

int main(int argc, char *argv[]){
    input();
}

void input(){
    int option, numberOfVertices, numberOfHoles;
    int vertexX, vertexY;
    int i;

    printf("Select OPTION\n0 For Horizontal Partition\n1 For Grid Partition\n");
    scanf("%d", &option);

    printf("Number of outer vertices\n");
    scanf("%d", &numberOfVertices);
    vertex **list = initVertexList(numberOfVertices);
    vertex v;
    for(i = 0; i < numberOfVertices; i++){
        scanf("%d %d", &vertexX, &vertexY);
        /* ADD vertex to structure and connect with last vertex*/
	list[i]=createVertex(vertexX, vertexY);
    }
    face *in, *out;
    createPolygon(numberOfVertices, list, in, out);
    printVertexList(list, numberOfVertices);
    sortVertexListX(list, numberOfVertices);
    printVertexList(list, numberOfVertices);
    sortVertexListY(list, numberOfVertices);
    printVertexList(list, numberOfVertices);


    printDCEL(list, numberOfVertices);
    printf("Number of Holes\n");
    scanf("%d", &numberOfHoles);
    for(i = 0; i < numberOfHoles; i++){
        scanf("%d %d", &vertexX, &vertexY);
        /* ADD vertex to structure and connect with last vertex*/
    }
}
