#include <stdio.h>
#include <stdlib.h>
#include <linklist.h>

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
void horizontalGrid();

vertex** list;
face *in,
     *out;
int option;
int numberOfVertices;
int numberOfHoles;

int main(int argc, char *argv[]){
	int listSize = input();
	if(option == 0)
		horizontalGrid();
	free(list);
}

int input(){
	int numberOfVerticesHole;
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
	in=createFace();
	out=createFace();
	createPolygon(numberOfVertices, list, in, out);
	printf("Number of Holes\n");
	scanf("%d", &numberOfHoles);
	for(int i = 0; i < numberOfHoles; i++){
		printf("Number of vertices in Hole[%d]\n", i);
		scanf("%d", &numberOfVerticesHole);
		vertex **listHole = initVertexList(numberOfVerticesHole);
		for(int j=0; j<numberOfVerticesHole; j++){
			scanf("%d %d", &vertexX, &vertexY);
			listHole[j]=createVertex(vertexX, vertexY);
		}
		createPolygon(numberOfVerticesHole, listHole, in, out);
		catVertexList(list, numberOfVertices, listHole, numberOfVerticesHole);
		numberOfVertices+=numberOfVerticesHole;
	}
	printDCEL(list, numberOfVertices);
	return numberOfVertices;
}


void horizontalGrid(){
	int i = 0;

	printVertexList(list, numberOfVertices);
	sortVertexListY(list, numberOfVertices);
	printVertexList(list, numberOfVertices);
    link_list **sweep_line = malloc(sizeof(link_list*));
    (*sweep_line) = NULL;
	while(i < numberOfVertices){
		vertex** listToConsider;
		vertex* temp_vertex = list[i];
		int vertexY = temp_vertex->y;
		int sizeToConsider = 1;
		i++;
		while(i < numberOfVertices && list[i]->y == vertexY){
			i++;
			sizeToConsider++;
		}
		listToConsider = initVertexList(sizeToConsider);
		for(int j = 0; j < sizeToConsider; j++){
			listToConsider[j] = list[i-sizeToConsider+j];
            if(j > 0 && vertexConnected(listToConsider[j-1], listToConsider[j])){
                link_list *tmp = *sweep_line;
                vertex *a = listToConsider[j-1];
                vertex *b = listToConsider[j];
                int not_done = 1;
                while(not_done){
                    /* Case 5 and 9:
                     * When we can insert a new segment and there is no segmentation */
                    if(tmp == NULL || (b->x < tmp->item->origin->x)){
                        addToListByX(sweep_line, getUpEdge(a));
                        addToListByX(sweep_line, getUpEdge(b));
                    }

                    /* Case 1:
                     * When the new segment is in between a segment in the sweep line. */
                    else if((a->x > tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        /* NEED TO TRACE SEGMENTATION */
                        addToListByX(sweep_line, getUpEdge(a));
                        addToListByX(sweep_line, getUpEdge(b));
                    }

                    /* Case 2:
                     * When the new segment is connected with its right side to sweep line. */
                    else if((a->x < tmp->item->origin->x) && (b->x == tmp->item->origin->x)){
                        addToListByX(sweep_line, getUpEdge(a));
                        rmFromList(sweep_line, tmp->item);
                    }
                    not_done = 0;
                }       
            }
		}
        printf("Printing sweep_line list:\n");
        link_list *tmp = *sweep_line;
        while(tmp != NULL){
            printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", tmp->item->origin->x, tmp->item->origin->y, tmp->item->next->origin->x, tmp->item->next->origin->y);
            tmp = tmp -> next;
        }
		printf("Vertices to consider:\n");
		printVertexList(listToConsider, sizeToConsider);
	}
}
