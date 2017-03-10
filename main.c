#include <stdio.h>

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

int main(int argc, char *argv[]){
    input();
}

void input(){
    int option, numberOfVertices, numberOfHoles;
    int vertexX, vertexY;
    int i;

    printf("Select OPTION\n0 For Horizontal Partition\n1 For Grid Partition\n");
    scanf("%d\n", &option);

    printf("Number of outer vertices\n");
    scanf("%d\n", &numberOfVertices);
    for(i = 0; i < numberOfVertices; i++){
        scanf("%d %d\n", &vertexX, &vertexY);
        /* ADD vertex to structure and connect with last vertex*/
    }

    printf("Number of Holes\n");
    scanf("%d\n", &numberOfHoles);
    for(i = 0; i < numberOfHoles; i++){
        scanf("%d %d\n", &vertexX, &vertexY);
        /* ADD vertex to structure and connect with last vertex*/
    }

}
