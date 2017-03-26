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
<<<<<<< HEAD
	link_list **sweep_line = malloc(sizeof(link_list*));
	(*sweep_line) = NULL;
=======
    link_list **sweep_line = malloc(sizeof(link_list*));
    sweep_line_action ** sweep_line_action_list = malloc(sizeof(sweep_line_action*)*numberOfVertices);
    int size_sweep_line_action = 0;
    (*sweep_line) = NULL;
>>>>>>> 1891a59636240d93f8c479e2f43deea21b403187
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
		for(int j = 0; j < sizeToConsider; j++){
<<<<<<< HEAD
			link_list *tmp = *sweep_line;
			if(j > 0 && vertexConnected(listToConsider[j-1], listToConsider[j])){
				vertex *a = listToConsider[j-1];
				vertex *b = listToConsider[j];
				/* Case 9:
				 * When we can insert a new segment and there is no segmentation */
				if(tmp == NULL){
					addToListByX(sweep_line, getUpEdge(a));
					addToListByX(sweep_line, getUpEdge(b));
				}
				while(tmp != NULL){
					/* Case 5:
					 * When we can insert a new segment and there is no segmentation */
					if(b->x < tmp->item->origin->x){
						addToListByX(sweep_line, getUpEdge(a));
						addToListByX(sweep_line, getUpEdge(b));
						break;
					}
					/* Case 1:
					 * When the new segment is in between a segment in the sweep line. */
					else if((a->x > tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
						/* NEED TO TRACE SEGMENTATION */
						addToListByX(sweep_line, getUpEdge(a));
						addToListByX(sweep_line, getUpEdge(b));
						break;
					}
					/* Case 2:
					 * When the new segment is connected with its right side to sweep line. */
					else if((a->x < tmp->item->origin->x) && (b->x == tmp->item->origin->x)){
						addToListByX(sweep_line, getUpEdge(a));
						rmFromList(sweep_line, tmp->item);
						break;
					}
					else if(tmp->next->next != NULL){
						/* Case 3:
						 * When the segment is connected with its left side to the sweep line. */
						if((a->x == tmp->next->item->origin->x) && (b->x < tmp->next->next->item->origin->x)){
							/* NEEDS SEGMENTATION */
							addToListByX(sweep_line, getUpEdge(b));
							rmFromList(sweep_line, tmp->next->item);
							break;
						}
						/* Case 4:
						 * When the segment is connected both sides with the sweep line. */
						else if((a->x == tmp->next->item->origin->x) && (b->x == tmp->next->next->item->origin->x)){
							/* NEEDS SEGMENTATION */
							rmFromList(sweep_line, tmp->next->next->item);
							rmFromList(sweep_line, tmp->next->item);
							break;
						}
					} 
					/* Case 6:
					 * When inside sweep line, connected at left */
					if((a->x == tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
						/* NEEDS SEGMENTATION */
						rmFromList(sweep_line, tmp->item);
						addToListByX(sweep_line, getUpEdge(b));
						break;
					}
					/* Case 7:
					 * When inside sweep line, connected at right */
					else if((a->x > tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
						/* NEEDS SEGMENTATION */
						addToListByX(sweep_line, getUpEdge(a));
						rmFromList(sweep_line, tmp->next->item);
						break;
					}
					/* Case 8:
					 * when inside sweep line, connected at both sides, no segmentation */
					else if((a->x == tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
						rmFromList(sweep_line, tmp->next->item);
						rmFromList(sweep_line, tmp->item);
						break;
					}
					else{
						tmp = tmp->next->next;
						if(tmp == NULL){
							addToListByX(sweep_line, getUpEdge(a));
							addToListByX(sweep_line, getUpEdge(b));
							break;
						}
					}
				}	   
			}
			/* When vertical colinear point */
			else if(sizeToConsider == 1){
				while(tmp->item->origin->x != listToConsider[j]->x){
					tmp = tmp->next;
				}
				rmFromList(sweep_line, tmp->item);
				addToListByX(sweep_line, getUpEdge(listToConsider[j]));
				/* NEEDS SEGMENTATION */
			}
			else if((j == 0) && !vertexConnected(listToConsider[j], listToConsider[j+1])){
				while(tmp->item->origin->x != listToConsider[j]->x){
					tmp = tmp->next;
				}
				rmFromList(sweep_line, tmp->item);
				addToListByX(sweep_line, getUpEdge(listToConsider[j]));
				/* NEEDS SEGMENTATION */
			}
			else if((j > 0) && (j < sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j+1]) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
				while(tmp->item->origin->x != listToConsider[j]->x){
					tmp = tmp->next;
				}
				rmFromList(sweep_line, tmp->item);
				addToListByX(sweep_line, getUpEdge(listToConsider[j]));
				/* NEEDS SEGMENTATION */
			}
			else if((j == sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
				while(tmp->item->origin->x != listToConsider[j]->x){
					tmp = tmp->next;
				}
				rmFromList(sweep_line, tmp->item);
				addToListByX(sweep_line, getUpEdge(listToConsider[j]));
				/* NEEDS SEGMENTATION */
			}
		}

		printf("Printing sweep_line list:\n");
		link_list *tmp = *sweep_line;
		while(tmp != NULL){
			printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", tmp->item->origin->x, tmp->item->origin->y, tmp->item->next->origin->x, tmp->item->next->origin->y);
			tmp = tmp -> next;
		}
=======
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
                    /* addToListByX(sweep_line, getUpEdge(a)); */
                    /* addToListByX(sweep_line, getUpEdge(b)); */
                    size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                    size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                }
                while(tmp != NULL){
                    /* Case 5:
                     * When we can insert a new segment and there is no segmentation */
                    if(b->x < tmp->item->origin->x){
                        /* addToListByX(sweep_line, getUpEdge(a)); */
                        /* addToListByX(sweep_line, getUpEdge(b)); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                        break;
                    }
                    /* Case 1:
                     * When the new segment is in between a segment in the sweep line. */
                    else if((a->x > tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        /* NEED TO TRACE SEGMENTATION */
                        /* addToListByX(sweep_line, getUpEdge(a)); */
                        /* addToListByX(sweep_line, getUpEdge(b)); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                        break;
                    }
                    /* Case 2:
                     * When the new segment is connected with its right side to sweep line. */
                    else if((a->x < tmp->item->origin->x) && (b->x == tmp->item->origin->x)){
                        /* addToListByX(sweep_line, getUpEdge(a)); */
                        /* rmFromList(sweep_line, tmp->item); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                        break;
                    }
                    else if(tmp->next->next != NULL){
                        /* Case 3:
                         * When the segment is connected with its left side to the sweep line. */
                        if((a->x == tmp->next->item->origin->x) && (b->x < tmp->next->next->item->origin->x)){
                            /* NEEDS SEGMENTATION */
                            /* addToListByX(sweep_line, getUpEdge(b)); */
                            /* rmFromList(sweep_line, tmp->next->item); */
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
                            break;
                        }
                        /* Case 4:
                         * When the segment is connected both sides with the sweep line. */
                        else if((a->x == tmp->next->item->origin->x) && (b->x == tmp->next->next->item->origin->x)){
                            /* NEEDS SEGMENTATION */
                            /* rmFromList(sweep_line, tmp->next->next->item); */
                            /* rmFromList(sweep_line, tmp->next->item); */
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->next->item);
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
                            break;
                        }
                    } 
                    /* Case 6:
                     * When inside sweep line, connected at left */
                    if((a->x == tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        /* NEEDS SEGMENTATION */
                        /* rmFromList(sweep_line, tmp->item); */
                        /* addToListByX(sweep_line, getUpEdge(b)); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                        break;
                    }
                    /* Case 7:
                     * When inside sweep line, connected at right */
                    else if((a->x > tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
                        /* NEEDS SEGMENTATION */
                        /* addToListByX(sweep_line, getUpEdge(a)); */
                        /* rmFromList(sweep_line, tmp->next->item); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
                        break;
                    }
                    /* Case 8:
                     * when inside sweep line, connected at both sides, no segmentation */
                    else if((a->x == tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
                        /* rmFromList(sweep_line, tmp->next->item); */
                        /* rmFromList(sweep_line, tmp->item); */
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
                        size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                        break;
                    }
                    else{
                        tmp = tmp->next->next;
                        if(tmp == NULL){
                            /* addToListByX(sweep_line, getUpEdge(a)); */
                            /* addToListByX(sweep_line, getUpEdge(b)); */
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(a));
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(b));
                            break;
                        }
                    }
                }       
            }
            /* When vertical colinear point */
            else if(sizeToConsider == 1){
                while(tmp->item->origin->x != listToConsider[j]->x){
                    tmp = tmp->next;
                }
                /* rmFromList(sweep_line, tmp->item); */
                /* addToListByX(sweep_line, getUpEdge(listToConsider[j])); */
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
                /* NEEDS SEGMENTATION */
            }
            else if((j == 0) && !vertexConnected(listToConsider[j], listToConsider[j+1])){
                while(tmp->item->origin->x != listToConsider[j]->x){
                    tmp = tmp->next;
                }
                /* rmFromList(sweep_line, tmp->item); */
                /* addToListByX(sweep_line, getUpEdge(listToConsider[j])); */
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
                /* NEEDS SEGMENTATION */
            }
            else if((j > 0) && (j < sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j+1]) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
                while(tmp->item->origin->x != listToConsider[j]->x){
                    tmp = tmp->next;
                }
                /* rmFromList(sweep_line, tmp->item); */
                /* addToListByX(sweep_line, getUpEdge(listToConsider[j])); */
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
                /* NEEDS SEGMENTATION */
            }
            else if((j == sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
                while(tmp->item->origin->x != listToConsider[j]->x){
                    tmp = tmp->next;
                }
                /* rmFromList(sweep_line, tmp->item); */
                /* addToListByX(sweep_line, getUpEdge(listToConsider[j])); */
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->item);
                size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 0, getUpEdge(listToConsider[j]));
                /* NEEDS SEGMENTATION */
            }
		}

        printf("Printing sweep_line list:\n");
        size_sweep_line_action = runEventSweepLine(sweep_line_action_list, size_sweep_line_action, sweep_line);
        link_list *tmp = *sweep_line;
        while(tmp != NULL){
            printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", tmp->item->origin->x, tmp->item->origin->y, tmp->item->next->origin->x, tmp->item->next->origin->y);
            tmp = tmp -> next;
        }
>>>>>>> 1891a59636240d93f8c479e2f43deea21b403187
		printf("Vertices to consider:\n");
		printVertexList(listToConsider, sizeToConsider);
	}
}



