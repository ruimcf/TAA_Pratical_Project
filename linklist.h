#ifndef LINKLIST_H
#define LINKLIST_H
#include <dcel.h>
typedef struct s_edge_list{
	struct s_edge_list *next;
	half_edge *item;
} edge_list;

typedef struct s_face_list{
	struct s_face_list *next;
	face *item;
} face_list;

typedef struct s_sweep_line_action{
	int action;
	half_edge *edge;
} sweep_line_action;
/*
typedef struct s_visibility_cone{
	vertex *origin;
	vertex *a;
	vertex *b;
} visibility_cone;

typedef struct s_queue{
	face *face;
	visibility_cone *visibility_cone;
	struct s_queue *next;
} queue;

void addToQueue(queue *head, face *face);
queue *popQueue(queue *head);
void printQueue(queue *head);
*/

//takes the adress of head ptr and ptr to item and adds it to the begining of the list
void addToList(edge_list **head, half_edge *item);

void addToListByX(edge_list **head, half_edge *item);
//adds item before the 1st item with bigger Y than its own
void addToListByY(edge_list **head, half_edge *item);

half_edge *getBeforeY(edge_list *head, int y);

half_edge *getAfterY(edge_list *head, int y);

half_edge *getBeforeX(edge_list *head, int x);

half_edge *getAfterX(edge_list *head, int x);
//takes adress of head ptr and item, and removes it from list (item needs to exist in the list)
void rmFromList(edge_list **head, half_edge *item);
//takes head ptr of the list and prints the origin and dest of every edge on list
void printLinkList(edge_list *head);

int addEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, int action, half_edge *edge);

int runEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, edge_list ** sweep_line);

void addToFaceList(face_list **head, face *item);

void rmFromFaceList(face_list **head, face *item);

int faceListSize(face_list *head);

void printFaceList(face_list *head);

#endif /* LINKLIST_H */
