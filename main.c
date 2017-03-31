#include <stdio.h>
#include <stdlib.h>
#include <linklist.h>
#include <string.h>
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
void horizontalColinearCase(link_list *tmp, vertex *point);
int getVertexToConsider(vertex ***listToConsider, int i, int direction);
void updateFaces();


void gridPartition();

/* Vertex lists */
vertex** list;
vertex** listWithSegmentation;
int numberOfVerticesWithSegmentation;
/* Initial two faces */
face *in,
	 *out;
/* Input arguments */
int option;
int numberOfVertices;
int numberOfHoles;
/* The Sweep Line that contains the current edges to be considered */
link_list **sweep_line;
/* Where we store the vertex that will be be used to trace segmentation edges */
vertex** segmentationVertex;
int sizeSegmentationVertex;
/* Where we store the new vertex that need to be added to DCEL at the end of the iteration */
structInsertVertex **listVertexToInsert;
int sizeListVertexToInsert;
/* Where we need to do changes to sweepLine only at the end of an iteration - CASE 4*/
sweep_line_action ** sweep_line_action_list;
int size_sweep_line_action;
/* The List that contains all segmentation edges traced */
link_list ** segmentationEdges;

int main(int argc, char *argv[]){
	int listSize = input();
	horizontalGrid();
	free(list);
	list=listWithSegmentation;
	numberOfVertices=numberOfVerticesWithSegmentation;
	listWithSegmentation=NULL;
	updateFaces();
	if(option){
		gridPartition();
	}
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
		printVertexList(listHole, numberOfVerticesHole);
		catVertexList(&list, numberOfVertices, &listHole, numberOfVerticesHole);
		numberOfVertices+=numberOfVerticesHole;
	}
	printVertexList(list, numberOfVertices);
	printDCEL(list, numberOfVertices);
	return numberOfVertices;
}

void initStructures(){
	/* Init Sweep Line */
	sweep_line = malloc(sizeof(link_list*));
	(*sweep_line) = NULL;
	/* Init Sweep Line Action List */
	sweep_line_action_list = malloc(sizeof(sweep_line_action*)*numberOfVertices);
	size_sweep_line_action = 0;
	/* Init Segmentation Vertex list */
	segmentationVertex = malloc(sizeof(vertex*)*numberOfVertices);
	sizeSegmentationVertex = 0;
	/* Init Vertex need to be Inserted list */
	listVertexToInsert = malloc(sizeof(structInsertVertex*)*numberOfVertices);
	sizeListVertexToInsert = 0;
	/* Init Segmentation Edges List */
	segmentationEdges = malloc(sizeof(link_list*));
	/* Init List With segmentation */
	listWithSegmentation = initVertexList(numberOfVertices);
	memcpy(listWithSegmentation, list, sizeof(vertex*)*numberOfVertices);
	numberOfVerticesWithSegmentation = numberOfVertices;
	printf("List with segmentaion\n");
	printVertexList(listWithSegmentation, numberOfVerticesWithSegmentation);
}

int getVertexToConsider(vertex ***listToConsider, int i, int direction){
	int vertexPos;
	if(direction == 0){
		vertexPos = list[i]->y;
	}
	else if(direction == 1){
		vertexPos = list[i]->x;
	}
	int sizeToConsider = 1;
	while(i+sizeToConsider < numberOfVertices){
		if(direction == 0 && list[i+sizeToConsider]->y == vertexPos){
			sizeToConsider++;
		}
		else if(direction == 1 && list[i+sizeToConsider]->x == vertexPos){
			sizeToConsider++;
		}
		else
			break;
	}
	*listToConsider = initVertexList(sizeToConsider);
	for(int j = 0; j < sizeToConsider; j++){
		(*listToConsider)[j] = list[i+j];
	}
	return sizeToConsider;
}

void horizontalGrid(){
	int i = 0;
	initStructures();
	sortVertexListY(list, numberOfVertices);
	while(i < numberOfVertices){
		vertex** listToConsider;
		int sizeToConsider = getVertexToConsider(&listToConsider, i, 0);
		i += sizeToConsider;
		/* For each vertex that we have to consider, we have to find the case that it satisfies */
		for(int j = 0; j < sizeToConsider; j++){
			link_list *tmp = *sweep_line;
			/* If the vertex forms an horizontal edge with another */
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
					addToListByX(sweep_line, getUpEdge(a));
					addToListByX(sweep_line, getUpEdge(b));
                }
				while(tmp != NULL){
					/* Case 5:
					 * When we can insert a new segment and there is no segmentation */
					if(b->x < tmp->item->origin->x){
                        printf("Case 5 detected\n");
						addToListByX(sweep_line, getUpEdge(a));
						addToListByX(sweep_line, getUpEdge(b));
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
						addToListByX(sweep_line, getUpEdge(a));
						addToListByX(sweep_line, getUpEdge(b));
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
						printf("Sweepline\n");
						printLinkList(*sweep_line);
						break;
					}
                    /* Case 3:
                     * When the segment is connected with its left side to the sweep line. */
                    if((tmp->next->next == NULL) && (a->x == tmp->next->item->origin->x)){
                        printf("Case 3 detected\n");
                        addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                        addSegmentationVertex(a);
						addToListByX(sweep_line, getUpEdge(b));
						rmFromList(sweep_line, tmp->next->item);
                        break;
                    }
					else if(tmp->next->next != NULL){
						/* Case 3:
						 * When the segment is connected with its left side to the sweep line. */
						if((a->x == tmp->next->item->origin->x) && (b->x < tmp->next->next->item->origin->x)){
							printf("Case 3 detected\n");
							addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                            addSegmentationVertex(a);
							addToListByX(sweep_line, getUpEdge(b));
							rmFromList(sweep_line, tmp->next->item);
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
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->next->item);
                            size_sweep_line_action = addEventSweepLine(sweep_line_action_list, size_sweep_line_action, 1, tmp->next->item);
							break;
						}
					} 
					/* Case 6:
					 * When inside sweep line, connected at left */
					if((a->x == tmp->item->origin->x) && (b->x < tmp->next->item->origin->x)){
                        printf("Case 6 detected\n");
                        addSegmentationVertex(b);
                        addSegmentationVertex(getVertex(tmp->next->item, tmp->next->item->origin->x, b->y));
						rmFromList(sweep_line, tmp->item);
						addToListByX(sweep_line, getUpEdge(b));
						break;
					}
					/* Case 7:
					 * When inside sweep line, connected at right */
					else if((a->x > tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
						printf("Case 7 detected\n");
						addSegmentationVertex(getVertex(tmp->item, tmp->item->origin->x, a->y));
                        addSegmentationVertex(a);
						rmFromList(sweep_line, tmp->next->item);
						addToListByX(sweep_line, getUpEdge(a));
						break;
					}
					/* Case 8:
					 * when inside sweep line, connected at both sides, no segmentation */
					else if((a->x == tmp->item->origin->x) && (b->x == tmp->next->item->origin->x)){
                        printf("Case 8 detected\n");
						rmFromList(sweep_line, tmp->next->item);
						rmFromList(sweep_line, tmp->item);
						break;
					}
					else{
						tmp = tmp->next->next;
						/* When the segment is outside of any of the sweep line, no segmentation */
						if(tmp == NULL){
                            printf("Case After is NULL detected\n");
							addToListByX(sweep_line, getUpEdge(a));
							addToListByX(sweep_line, getUpEdge(b));
							break;
						}
					}
				}	   
			}
			/* When vertical colinear point */
			/* If its the only point on the same line, it's a colinear point */
			else if(sizeToConsider == 1){
				horizontalColinearCase(tmp, listToConsider[j]);
			}
			/* If it's the first one and not connected to the next, it's a colinear point */
			else if((j == 0) && !vertexConnected(listToConsider[j], listToConsider[j+1])){
				horizontalColinearCase(tmp, listToConsider[j]);
			}
			/* if its a middle vertex and not connected to any of its neighbours */
			else if((j > 0) && (j < sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j+1]) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
				horizontalColinearCase(tmp, listToConsider[j]);
			}
			/* if its the last vertex and not connected to the previous */
			else if((j == sizeToConsider-1) && !vertexConnected(listToConsider[j], listToConsider[j-1])){
				horizontalColinearCase(tmp, listToConsider[j]);
			}
	    }

		printf("Vertices to consider:\n");
		printVertexList(listToConsider, sizeToConsider);
		printf("Printing sweep_line list:\n");
        size_sweep_line_action = runEventSweepLine(sweep_line_action_list, size_sweep_line_action, sweep_line);
        printLinkList(*sweep_line);
        /* Insert the new vertex on the dcel */
        vertex** tmp_list = initVertexList(sizeListVertexToInsert);
        for(int k = 0; k<sizeListVertexToInsert; k++){
            tmp_list[k] = listVertexToInsert[k]->new;
        }
        catVertexList(&listWithSegmentation, numberOfVerticesWithSegmentation, &tmp_list, sizeListVertexToInsert);
        numberOfVerticesWithSegmentation += sizeListVertexToInsert;
        printf("Run insert vertex\n");
        runInsertVertex(sweep_line);
        printf("Run Segmentation\n");
        traceSegmentationEdges();
		printf("All vertices:\n");
		printVertexList(listWithSegmentation, numberOfVerticesWithSegmentation);
		printDCEL(listWithSegmentation, numberOfVerticesWithSegmentation);
		printf("###############################\n");
	}
}


void horizontalColinearCase(link_list *tmp, vertex *point){
    printf("Case Colinear detected\n");
    int i = 1;
    /* Dest is the destination of the colinear point segmentation trace */
    half_edge *dest = tmp->next->item;
    while(tmp->item->origin->x != point->x){
        if(i % 2 == 1)
            dest = tmp->item;
        else
            dest = tmp->next->next->item;
        tmp = tmp->next;
        i++;
    }
    if(dest->origin->x < point->x){
        addSegmentationVertex(getVertex(dest, dest->origin->x, point->y));
        addSegmentationVertex(point);
    }
    else{
        addSegmentationVertex(point);
        addSegmentationVertex(getVertex(dest, dest->origin->x, point->y));
    }
	rmFromList(sweep_line, tmp->item);
	addToListByX(sweep_line, getUpEdge(point));
}

void addSegmentationVertex(vertex* a){
	segmentationVertex[sizeSegmentationVertex] = a;
	sizeSegmentationVertex++;
}

void traceSegmentationEdges(){
	int i = 0;
	while(i < sizeSegmentationVertex){
		/* When Grid Mode, we have to consider the other segmentation edges*/
		vertex *a = segmentationVertex[i];
		vertex *b = segmentationVertex[i+1];
		if(!vertexConnected(a, b)){
			insertEdge(a, b, in);
			addToList(segmentationEdges, getConnectedEdge(a, b));
		}
		i+=2;
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
		rmFromList(sweep_line, listVertexToInsert[i]->edge);
		if(listVertexToInsert[i]->edge->face == out){
			insertVertex(listVertexToInsert[i]->new, listVertexToInsert[i]->edge->twin);
		}
		else{
			insertVertex(listVertexToInsert[i]->new, listVertexToInsert[i]->edge);
		}
		addToListByX(sweep_line, getUpEdge(listVertexToInsert[i]->new));
		free(listVertexToInsert[i]);
	}
	sizeListVertexToInsert = 0;
}

void updateFaces(){
	link_list *heList = NULL;
	half_edge *he;
	face *tmp;
	for(int i=0; i<numberOfVertices; i++){
		he=list[i]->rep;
		do{
			addToList(&heList, he);
			printf("(%d,%d)->(%d,%d)\n",
					he->origin->x,
					he->origin->y,
					he->twin->origin->x,
					he->twin->origin->y);
			he=he->twin->next;
		} while(he!=list[i]->rep);
	}
	while(heList!=NULL){
		he=heList->item;
		if(he->face==out){
			rmFromList(&heList, he);
		}
		else{
			tmp=malloc(sizeof(face));
			tmp->rep=he;
			printf("face:\n");
			while(he->face!=tmp){
				printf("(%d,%d)->(%d,%d)\n",
						he->origin->x,
						he->origin->y,
						he->twin->origin->x,
						he->twin->origin->y);
				he->face=tmp;
				rmFromList(&heList, he);
				he=he->next;
			}
		}
	}
}
int getVerticalLine(int start){
	int x = list[start]->x;
	while(start < numberOfVertices && list[start]->x==x){
		start++;
	}
	return start;
}

face *faceUp(vertex *v){
	half_edge *e = getRightEdge(v);
	if(e!=NULL){
		return e->face;
	}
	else{
		e=getLeftEdge(v);
		return e->twin->face;
	}
}

face *faceDown(vertex *v){
	half_edge *e = getLeftEdge(v);
	if(e!=NULL){
		return e->face;
	}
	else{
		e=getRightEdge(v);
		return e->twin->face;
	}
}

int traceUp(vertex *v, link_list **edgeList){
	int counter=0;
	if(getUpEdge(v)!=NULL){
		return counter;
	}
	face *f = faceUp(v);
	if(f==out){
		return counter;
	}
	half_edge *destHedge=getAfterY(*edgeList, v->y);
	if(destHedge->twin->origin->x == v->x){
		insertEdgeUpdateFace(v, destHedge->twin->origin, f);
		counter+= traceUp(destHedge->twin->origin, edgeList);
		counter++;
	}
	else{
		vertex *newVertex=createVertex(v->x, destHedge->origin->y);
		insertVertexKeep(newVertex, destHedge);
		insertEdgeUpdateFace(v, newVertex, f);
		changeVertexListSize(&list, numberOfVertices+1);
		list[numberOfVertices]=newVertex;
		numberOfVertices++;
		counter+= traceUp(newVertex, edgeList);
	}
	return counter;
}

void traceDown(vertex *v, link_list **edgeList){
	if(getDownEdge(v)!=NULL){
		return;
	}
	face *f = faceDown(v);
	if(f==out){
		return;
	}
	half_edge *destHedge=getBeforeY(*edgeList, v->y);
	if(destHedge->twin->origin->x == v->x){
		insertEdgeUpdateFace(v, destHedge->twin->origin, f);
		traceDown(destHedge->twin->origin, edgeList);
	}
	else{
		vertex *newVertex=createVertex(v->x, destHedge->origin->y);
		insertVertexKeep(newVertex, destHedge);
		insertEdgeUpdateFace(v, newVertex, f);
		changeVertexListSize(&list, numberOfVertices+1);
		list[numberOfVertices]=newVertex;
		numberOfVertices++;
		traceDown(newVertex, edgeList);
	}
	return;
}

void gridPartition(){
	sortVertexListX(list, numberOfVertices);
	link_list *edgeList=NULL;
	int linePos=0;
	int endOfLine;
	int endOfPoly=numberOfVertices;
	while(linePos<endOfPoly){
		endOfLine=getVerticalLine(linePos);
		for(int i=linePos; i<endOfLine; i++){
			traceDown(list[i], &edgeList);
			i+= traceUp(list[i], &edgeList);
		}
		for(int i=linePos; i<endOfLine; i++){
			half_edge *tmp = getLeftEdge(list[i]);
			if(tmp!=NULL){
				rmFromList(&edgeList, tmp->twin);
			}
			tmp = getRightEdge(list[i]);
			if(tmp!=NULL){
				addToListByY(&edgeList, tmp);
			}
		}
		linePos=endOfLine;
		printDCEL(list, numberOfVertices);
		printVertexList(list,numberOfVertices);
	}
}
		


			





