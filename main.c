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
void addSegmentationVertex(vertex* a);
void traceSegmentationEdges();
void printSegmentationVertex();
vertex *getVertex(half_edge *edge, int x, int y);
void addInsertVertex(half_edge *edge, vertex *new);
void runInsertVertex(link_list **sweep_line);


vertex** list;
face *in,
	 *out;
int option;
int numberOfVertices;
int numberOfHoles;
link_list ** segmentationEdges;
vertex** segmentationVertex;
int sizeSegmentationVertex = 0;
int size_sweep_line_action = 0;
structInsertVertex **listVertexToInsert;
int sizeListVertexToInsert = 0;

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
	sweep_line_action ** sweep_line_action_list = malloc(sizeof(sweep_line_action*)*numberOfVertices);
    size_sweep_line_action = 0;
	(*sweep_line) = NULL;
    segmentationVertex = malloc(sizeof(vertex*)*numberOfVertices);
    listVertexToInsert = malloc(sizeof(structInsertVertex*)*numberOfVertices);
    segmentationEdges = malloc(sizeof(link_list*));
	while(i < numberOfVertices){
		vertex** listToConsider;
		vertex* temp_vertex = list[i];
		int vertexY = temp_vertex->y;
		int sizeToConsider = 1;
		i++;
		while(i < numberOfVertices){
			if(list[i]->y == vertexY){
				i++;
				sizeToConsider++;
			}
			else
				break;
		}
		listToConsider = initVertexList(sizeToConsider);
		for(int j = 0; j < sizeToConsider; j++){
			listToConsider[j] = list[i-sizeToConsider+j];
		}
		printf("Vertices to consider:\n");
		printVertexList(listToConsider, sizeToConsider);
		for(int j = 0; j < sizeToConsider; j++){
			link_list *tmp = *sweep_line;
			if(j > 0 && vertexConnected(listToConsider[j-1], listToConsider[j])){
                /* If next vertex are colinear, we must treat all as one edge */
                int numberOfColinearVertex = 0;
                while(j < sizeToConsider-1){
                    if(vertexConnected(listToConsider[j], listToConsider[j+1])){
                        j++;
                        numberOfColinearVertex++;
                    }
                    else
                        break;
                }
                vertex *a = listToConsider[j-1-numberOfColinearVertex];
				vertex *b = listToConsider[j];
				/* Case 9:
				 * When we can insert a new segment and there is no segmentation */
				if(tmp == NULL){
                    printf("Case NULL detected\n");
				    size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                    size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                }
				while(tmp != NULL){
					/* Case 5:
					 * When we can insert a new segment and there is no segmentation */
					if(b->x < tmp->item->origin->x){
                        printf("Case 5 detected\n");
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
						break;
					}
					/* Case 1:
					 * When the new segment is in between a segment in the sweep line. */
					else if((a->x > tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        printf("Case 1 detected\n");
						addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                        addSegmentationVertex(a);
                        addSegmentationVertex(b);
                        addSegmentationVertex(getVertex(tmp->next->item, tmp->next->item->origin->x, b->y));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));

						break;
					}
					/* Case 2:
					 * When the new segment is connected with its right side to sweep line. */
					else if((a->x < tmp->item->origin->x) && (b->x == tmp->item->origin->x)){
                        printf("Case 2 detected\n");
                        addSegmentationVertex(b);
                        addSegmentationVertex(getVertex(tmp->next->item, tmp->next->item->origin->x, b->y));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
						break;
					}
					else if(tmp->next->next != NULL){
						/* Case 3:
						 * When the segment is connected with its left side to the sweep line. */
						if((a->x == tmp->next->item->origin->x) && (b->x < tmp->next->next->item->origin->x)){
                            printf("Case 3 detected\n");
							addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                            addSegmentationVertex(a);
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
							break;
						}
						/* Case 4:
						 * When the segment is connected both sides with the sweep line. */
						else if((a->x == tmp->next->item->origin->x) && (b->x == tmp->next->next->item->origin->x)){
                            printf("Case 4 detected\n");
                            addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                            addSegmentationVertex(a);
                            addSegmentationVertex(b);
                            addSegmentationVertex(getVertex(tmp->next->next->next->item, tmp->next->next->next->item->origin->x, b->y));
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 2, tmp->next->next->item);
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 2, tmp->next->item);
							break;
						}
					} 
					/* Case 6:
					 * When inside sweep line, connected at left */
					if((a->x == tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        printf("Case 6 detected\n");
                        addSegmentationVertex(b);
                        addSegmentationVertex(getVertex(tmp->next->item, tmp->next->item->origin->x, b->y));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
						break;
					}
					/* Case 7:
					 * When inside sweep line, connected at right */
					else if((a->x > tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
                        printf("Case 7 detected\n");
						addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                        addSegmentationVertex(a);
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
						break;
					}
					/* Case 8:
					 * when inside sweep line, connected at both sides, no segmentation */
					else if((a->x == tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
                        printf("Case 8 detected\n");
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
						break;
					}
					else{
						tmp = tmp->next->next;
                        /* When the segment is outside of any of the sweep line, no segmentation */
						if(tmp == NULL){
                            printf("Case After is NULL detected\n");
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
							break;
						}
					}
				}	   
			}
			/* When vertical colinear point */
            /* If its the only point on the same line, it's a colinear point */
			else if(sizeToConsider == 1){
                printf("Case Colinear detected\n");
                int i = 1;
                half_edge *dest = tmp->next->item;
				while(tmp->item->origin->x != listToConsider[j]->x){
                    /* If it was left edge and not the solution, switch dest to left */
                    if(i % 2 == 1){
                        dest = tmp->item;
                    }
                    /* If it was right edge and not the solution, switch to next right edge*/
                    else{
                        dest = tmp->next->next->item;
                    }
					tmp = tmp->next;
                    i++;
				}
                if(dest->origin->x < listToConsider[j]->x){
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                    addSegmentationVertex(listToConsider[j]);
                }
                else{
                    addSegmentationVertex(listToConsider[j]);
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                }
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
			}
            /* If it's the first one and not connected to the next */
			else if((j == 0) && !vertexConnected(listToConsider[j], listToConsider[j+1])){
                printf("Case Colinear detected\n");
                int i = 1;
                half_edge *dest = tmp->next->item;
				while(tmp->item->origin->x != listToConsider[j]->x){
                    /* If it was left edge and not the solution, switch dest to left */
                    if(i % 2 == 1){
                        dest = tmp->item;
                    }
                    /* If it was right edge and not the solution, switch to next right edge*/
                    else{
                        dest = tmp->next->next->item;
                    }
					tmp = tmp->next;
                    i++;
				}
                if(dest->origin->x < listToConsider[j]->x){
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                    addSegmentationVertex(listToConsider[j]);
                }
                else{
                    addSegmentationVertex(listToConsider[j]);
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                }
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
			}
            /* if its a middle vertex and not connected to any of its neighbours */
			else if((j > 0) && (j < sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j+1]) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
                printf("Case Colinear detected\n");
                int i = 1;
                half_edge *dest = tmp->next->item;
				while(tmp->item->origin->x != listToConsider[j]->x){
                    /* If it was left edge and not the solution, switch dest to left */
                    if(i % 2 == 1){
                        dest = tmp->item;
                    }
                    /* If it was right edge and not the solution, switch to next right edge*/
                    else{
                        dest = tmp->next->next->item;
                    }
					tmp = tmp->next;
                    i++;
				}
                if(dest->origin->x < listToConsider[j]->x){
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                    addSegmentationVertex(listToConsider[j]);
                }
                else{
                    addSegmentationVertex(listToConsider[j]);
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                }
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
			}
            /* if its the last vertex and not connected to the previous */
			else if((j == sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
                printf("Case Colinear detected\n");
                int i = 1;
                half_edge *dest = tmp->next->item;
				while(tmp->item->origin->x != listToConsider[j]->x){
                    /* If it was left edge and not the solution, switch dest to left */
                    if(i % 2 == 1){
                        dest = tmp->item;
                    }
                    /* If it was right edge and not the solution, switch to next right edge*/
                    else{
                        dest = tmp->next->next->item;
                    }
					tmp = tmp->next;
                    i++;
				}
                if(dest->origin->x < listToConsider[j]->x){
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                    addSegmentationVertex(listToConsider[j]);
                }
                else{
                    addSegmentationVertex(listToConsider[j]);
                    addSegmentationVertex(getVertex(dest, dest->origin->x, listToConsider[j]->y));
                }
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
			}
            size_sweep_line_action = runEventSweepLine(sweep_line_action_list, size_sweep_line_action, sweep_line, 0);
		}

		printf("Vertices to consider:\n");
		printVertexList(listToConsider, sizeToConsider);
		printf("Printing sweep_line list:\n");
        size_sweep_line_action = runEventSweepLine(sweep_line_action_list, size_sweep_line_action, sweep_line, 1);
		link_list *tmp = *sweep_line;
		while(tmp != NULL){
			printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", tmp->item->origin->x, tmp->item->origin->y, tmp->item->next->origin->x, tmp->item->next->origin->y);
			tmp = tmp -> next;
		}
        printf("Run insert vertex\n");
        for(int j = 0; j<sizeListVertexToInsert; j++){
            printf("New Vertex: (%d | %d) on edge (%d, %d) to (%d, %d)\n", listVertexToInsert[j]->new->x, listVertexToInsert[j]->new->y, listVertexToInsert[j]->edge->origin->x, listVertexToInsert[j]->edge->origin->y, listVertexToInsert[j]->edge->next->origin->x, listVertexToInsert[j]->edge->next->origin->y);
        }
        runInsertVertex(sweep_line);
        printf("Run Segmentation\n");
        traceSegmentationEdges();
		printf("Printing sweep_line list:\n");
        size_sweep_line_action = runEventSweepLine(sweep_line_action_list, size_sweep_line_action, sweep_line, 1);
		tmp = *sweep_line;
		while(tmp != NULL){
			printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", tmp->item->origin->x, tmp->item->origin->y, tmp->item->next->origin->x, tmp->item->next->origin->y);
			tmp = tmp -> next;
		}
		printf("Vertices to consider:\n");
		printVertexList(list, numberOfVertices);
        printDCEL(list, numberOfVertices);
        printf("###############################\n");
	}
}

void addSegmentationVertex(vertex* a){
    printf("Novo vertice de segmentaçao (%d, %d)\n", a->x, a->y);
    segmentationVertex[sizeSegmentationVertex] = a;
    sizeSegmentationVertex++;
}

void traceSegmentationEdges(){
    int i = 0;
    while(i < sizeSegmentationVertex){
        /* When Grid Mode, we have to consider the other segmentation edges*/
        if(option == 2){

        }
        else{
            vertex *a = segmentationVertex[i];
            vertex *b = segmentationVertex[i+1];
            if(!vertexConnected(a, b)){
                printf("New edge on (%d, %d) to (%d, %d)\n", a->x, a->y, b->x, b->y);
                face *new_face = createFace();
                // if(a->rep->face != out){
                    insertEdge(a, b, a->rep->face, new_face);
                // }
                // else{
                    // insertEdge(a, b, a->rep->twin->next->face, new_face);
                // }
                addToList(segmentationEdges, getConnectedEdge(a, b));
            }
            i+=2;
        }
    }
    sizeSegmentationVertex = 0;
}

void printSegmentationVertex(){
    printf("Segmentation vertex list:\n");
    for(int i = 0; i<sizeSegmentationVertex; i++){
        printf("Vertex (%d, %d)\n", segmentationVertex[i]->x, segmentationVertex[i]->y);
    }
    printf("------------------\n");
}

vertex *getVertex(half_edge* edge, int x, int y){
    if(edge->origin->x == x && edge->origin->y == y){
        return edge->origin;
    }
    else if(edge->twin->origin->x == x && edge->twin->origin->y == y){
        return edge->twin->origin;
    }
    else{
        vertex *new = createVertex(x, y);
        addInsertVertex(edge, new);
        return new;
    }
}

void addInsertVertex(half_edge *edge, vertex *new){
    structInsertVertex * add = malloc(sizeof(structInsertVertex));
    add->edge = edge;
    add->new = new;
    listVertexToInsert[sizeListVertexToInsert] = add;
    sizeListVertexToInsert++;
}

void runInsertVertex(link_list **sweep_line){
    for(int i = 0; i < sizeListVertexToInsert; i++){
        printf("A remover a aresta (%d, %d) to (%d, %d)\n", listVertexToInsert[i]->edge->origin->x, listVertexToInsert[i]->edge->origin->y, listVertexToInsert[i]->edge->next->origin->x, listVertexToInsert[i]->edge->next->origin->y);
        rmFromList(sweep_line, listVertexToInsert[i]->edge);
        if(listVertexToInsert[i]->edge->face == out){
            insertVertex(listVertexToInsert[i]->new, listVertexToInsert[i]->edge->twin);
        }
        else{
            insertVertex(listVertexToInsert[i]->new, listVertexToInsert[i]->edge);
        }
        printf("A inserir a aresta (%d, %d) to (%d, %d)\n", getUpEdge(listVertexToInsert[i]->new)->origin->x, getUpEdge(listVertexToInsert[i]->new)->origin->y, getUpEdge(listVertexToInsert[i]->new)->next->origin->x, getUpEdge(listVertexToInsert[i]->new)->next->origin->y);
        addToListByX(sweep_line, getUpEdge(listVertexToInsert[i]->new));
        free(listVertexToInsert[i]);
    }
    sizeListVertexToInsert = 0;
}
