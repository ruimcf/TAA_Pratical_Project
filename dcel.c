#include <dcel.h>

vertex createVertex(int x, int y){
	vertex v;
	v.x=x;
	v.y=y;
	v.rep=NULL;
	return node;
}

vertex ** initVertexList(int size){
	vertex **list;
	list=malloc(sizeof(vertex *) * size);
	return list;
}

void changeVertexListSize(vertex **list, int newSize){
	realloc(list, sizeof(vertex *) * newSize);
	return;
}

half_edge **createPolygon(int num, vertex **listCCW, face *in, face *out){
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
	free(listBack);
	return listFront;
}

//void insertVertex() todo
//void insertEdge() todo

void printVertex(vertex **list, int size){//todo print DCEL
	int xMax=list[0]->x,
	    xMin=list[0]->x,
	    yMax=list[0]->y,
	    yMin=list[0]->y,
	    yy,
	    xx,
	    pos;
	for(int i=1; i<size; i++){
		if(list[i]->x > xMax){
			xMax=list[i]->x;
		}
		else if(list[i]->x < xMin){
			xMin=list[i]->x;
		}
		if(list[i]->y > yMax){
			yMax=list[i]->y;
		}
		else if(list[i]->y < yMin){
			yMin=list[i]->y;
		}
	}
	xx=xMax-xMin;
	yy=yMax-yMin;
	char *screen=malloc(xx*yy*4);
	for(int i=0; i<size; i++){
		pos=(list[i]->x * 2) + (list[i]->y * xx * 2);
		screen[pos]='X';
		screen[pos+1]='X';
		screen[pos+xx]='X';
		screen[pos+xx+1]='X';
	}
	for(int i=2*yy-1; i>=0; i--){
		for(int e=0; e<xx*2; e++){
			printf("%c", screen[i*xx+e]);
		}
		printf("\n");
	}
	free(screen);
	return;
}


