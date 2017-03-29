#include <dcel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

half_edge * getUpEdge(vertex * vertex1){
	half_edge *tmp = vertex1->rep;
    do{
        if(tmp->twin->origin->x == vertex1->x && tmp->twin->origin->y > vertex1->y){
            return tmp;
        }
        tmp=tmp->twin->next;
    }while(tmp!=vertex1->rep);
    return NULL;
}

face *createFace(){
	face *f = malloc(sizeof(face));
	return f;
}

vertex *createVertex(int x, int y){
	vertex *v = malloc(sizeof(vertex));
	v->x=x;
	v->y=y;
	v->rep=NULL;
	return v;
}

vertex **initVertexList(int size){
	vertex **list;
	list=malloc(sizeof(vertex *) * size);
	return list;
}

void changeVertexListSize(vertex ***list, int newSize){
	*list = realloc(*list, sizeof(vertex *) * newSize);
	return;
}

void catVertexList(vertex ***dest, int destSize, vertex ***src, int srcSize){
	changeVertexListSize(dest, destSize+srcSize);
	memcpy(*dest+destSize, *src, srcSize*sizeof(vertex*));
	free(*src);
	return;
}
//bubble sort to be replaced with a more eficient one
void sortVertexListX(vertex **list, int size){ 
	vertex *tmp;
	int newn;
	do{
		newn=0;
		for(int i=1; i<size; i++){
			if((list[i-1]->x > list[i]->x) || 
			  ((list[i-1]->x == list[i]->x) && 
			   (list[i-1]->y > list[i]->y))){
				tmp=list[i-1];
				list[i-1]=list[i];
				list[i]=tmp;
				newn=i;
			}
		}
		size=newn;
	} while(size!=0);
}
		
void sortVertexListY(vertex **list, int size){
	vertex *tmp;
	int newn;
	do{
		newn=0;
		for(int i=1; i<size; i++){
			if((list[i-1]->y > list[i]->y) || 
			  ((list[i-1]->y == list[i]->y) && 
			   (list[i-1]->x > list[i]->x))){
				tmp=list[i-1];
				list[i-1]=list[i];
				list[i]=tmp;
				newn=i;
			}
		}
		size=newn;
	} while(size!=0);
}

void createPolygon(int num, vertex **listCCW, face *in, face *out){
	half_edge **listFront, **listBack;
	listFront=malloc(sizeof(half_edge *) * num);
	listBack=malloc(sizeof(half_edge *) * num);
	for(int i=0; i<num; i++){
		listFront[i]=malloc(sizeof(half_edge));
		listFront[i]->face=in;
		listFront[i]->origin=listCCW[i];
		listBack[i]=malloc(sizeof(half_edge));
		listBack[i]->face=out;
		listBack[i]->origin=listCCW[(i+1)%num];
		listFront[i]->twin=listBack[i];
		listBack[i]->twin=listFront[i];
		listCCW[i]->rep=listFront[i];
	}
	if(in->rep==NULL){
		in->rep=listFront[0];
	}
	if(out->rep==NULL){
		out->rep=listBack[num-1];
	}
	for(int i=0; i<num; i++){
		listFront[i]->next=listFront[(i+1)%num];
		listFront[i]->prev=listFront[(i+num-1)%num];
		listBack[i]->next=listBack[(i+num-1)%num];
		listBack[i]->prev=listBack[(i+1)%num];
	}
	free(listFront);
	free(listBack);
	return;
}

void insertVertex(vertex *v, half_edge *he){
    printf("inserting vertex\n");
	half_edge *out1 = malloc(sizeof(half_edge)),
		  *out2 = malloc(sizeof(half_edge)),
		  *in1 = malloc(sizeof(half_edge)),
		  *in2 = malloc(sizeof(half_edge));
	out1->origin=v;
	out1->face=he->face;
	out1->next=he->next;
	out1->prev=in2;
	out1->twin=in1;
	in1->origin=he->twin->origin;
	in1->face=he->twin->face;
	in1->next=out2;
	in1->prev=he->twin->prev;
	in1->twin=out1;
	out2->origin=v;
	out2->face=he->twin->face;
	out2->next=he->twin->next;
	out2->prev=in1;
	out2->twin=in2;
	in2->origin=he->origin;
	in2->face=he->face;
	in2->next=out1;
	in2->prev=he->prev;
	in2->twin=out2;
	he->prev->next=in2;
	he->next->prev=out1;
	he->twin->next->prev=out2;
	he->twin->prev->next=in1;
	v->rep=out1;
	if(he->face->rep==he){
		he->face->rep=in2;
	}
	if(he->twin->face->rep==he->twin){
		he->twin->face->rep=in1;
	}
	he->origin->rep=in2;
	free(he->twin);
	free(he);
	return;
}

void insertEdge(vertex *src, vertex *dest,face *keepFace){
	half_edge *front = malloc(sizeof(half_edge));
    half_edge *back = malloc(sizeof(half_edge));
    half_edge *srcIn;
    half_edge *srcOut;
    half_edge *destIn;
    half_edge *destOut;
	destOut=dest->rep;
	while(destOut->face!=keepFace){
		destOut=destOut->twin->next;
	}
	srcIn=src->rep->twin;
	while(srcIn->face!=keepFace){
		srcIn=srcIn->next->twin;
	}
	destIn=dest->rep->twin;
	while(destIn->face!=keepFace){
		destIn=destIn->next->twin;
	}
	srcOut=src->rep;
	while(srcOut->face!=keepFace){
		srcOut=srcOut->twin->next;
	}
	front->origin=src;
	front->twin=back;
	front->face=keepFace;
	back->origin=dest;
	back->twin=front;
	back->face=keepFace;
	front->next=destOut;
	destOut->prev=front;
	front->prev=srcIn;
	srcIn->next=front;
	back->prev=destIn;
	destIn->next=back;
	back->next=srcOut;
    srcOut->prev=back;
	keepFace->rep=front;
	return;
}

void printVertexList(vertex **list, int size){
	half_edge *tmp;
	for(int i=0; i<size; i++){
		printf("(%d,%d) ligado a: ", list[i]->x, list[i]->y);
		tmp=list[i]->rep;
		do{
			printf("(%d,%d) ", tmp->twin->origin->x, tmp->twin->origin->y);
			tmp=tmp->twin->next;
		}while(tmp!=list[i]->rep);
		printf("\n");
	}
}

int vertexConnected(vertex* vertex1, vertex* vertex2){
	half_edge *tmp = vertex1->rep;
    do{
        if(tmp->twin->origin->x == vertex2->x && tmp->twin->origin->y == vertex2->y){
            return 1;
        }
        tmp=tmp->twin->next;
    }while(tmp!=vertex1->rep);
    return 0;
}

void printDCEL(vertex **list, int size){
	int xMax,
	    yMax,
	    pos;
	xMax = list[0]->x;
	yMax = list[0]->y;
	for(int i=1; i<size; i++){
		if(list[i]->x > xMax){
			xMax=list[i]->x;
		}
		if(list[i]->y > yMax){
			yMax=list[i]->y;
		}
	}
	yMax++;
	xMax++;
	char *screen=malloc(xMax*yMax);
	memset(screen, ' ', xMax*yMax);
	for(int i=0; i<size; i++){
		pos=(list[i]->x) + (list[i]->y * xMax);
		screen[pos]='0';
		half_edge *edge=list[i]->rep;
		do{
			vertex *connect = edge->twin->origin;
			if(connect->x == list[i]->x){
				if(connect->y > list[i]->y){
					int up=connect->y,
					    down=list[i]->y;
					for(int e=down+1; e<down+(up-down-1)/2; e++){
						screen[connect->x + e*xMax]='|';
					}
					screen[connect->x + (down+(up-down-1)/2)*xMax]='^';
				}
				else{
					int up=list[i]->y,
					    down=connect->y;
					for(int e=up-1; e>up-(up-down-1)/2; e--){
						screen[connect->x + e*xMax]='|';
					}
					screen[connect->x + (up-(up-down-1)/2)*xMax]='v';
				}
			}
			else if(connect->y == list[i]->y){
				if(connect->x > list[i]->x){
					int right=connect->x,
					    left=list[i]->x;
					for(int e=left+1; e<left+(right-left-1)/2; e++){
						screen[e + connect->y*xMax]='-';
					}
					screen[(left+(right-left-1)/2) + connect->y*xMax]='>';
				}
				else{
					int right=list[i]->x,
					    left=connect->x;
					for(int e=right-1; e>left-(left-right-1)/2; e--){
						screen[e + connect->y*xMax]='-';
					}
					screen[(left-(left-right-1)/2) + connect->y*xMax]='<';
				}
			}
			edge=edge->twin->next;
		}while(edge!=list[i]->rep);
	}
	for(int i=yMax-1; i>=0; i--){
		for(int e=0; e<xMax; e++){
			printf("%c", screen[i*xMax+e]);
		}
		printf("\n");
	}
	free(screen);
	return;
}

half_edge *getConnectedEdge(vertex* vertex1, vertex* vertex2){
	half_edge *tmp = vertex1->rep;
    do{
        if(tmp->twin->origin->x == vertex2->x && tmp->twin->origin->y == vertex2->y){
            return tmp;
        }
        tmp=tmp->twin->next;
    }while(tmp!=vertex1->rep);
    return NULL;
}

