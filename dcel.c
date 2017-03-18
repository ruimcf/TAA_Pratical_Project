#include <dcel.h>

vertex createVertex(int x, int y){
	vertex v;
	v.x=x;
	v.y=y;
	v.rep=NULL;
	return node;
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
	return listFront;
}
