#ifndef LINKLIST_H
#define LINKLIST_H
#include <dcel.h>
typedef struct s_link_list{
	struct s_link_list *next;
	half_edge *item;
} link_list;

typedef struct s_sweep_line_action{
	int action;
	half_edge *edge;
} sweep_line_action;


//takes the adress of head ptr and ptr to item and adds it to the begining of the list
void addToList(link_list **head, half_edge *item);

void addToListByX(link_list **head, half_edge *item);
//adds item before the 1st item with bigger Y than its own
void addToListByY(link_list **head, half_edge *item);

half_edge *getBeforeY(link_list *head, int y);

half_edge *getAfterY(link_list *head, int y);
//takes adress of head ptr and item, and removes it from list (item needs to exist in the list)
void rmFromList(link_list **head, half_edge *item);
//takes head ptr of the list and prints the origin and dest of every edge on list
void printLinkList(link_list *head);

int addEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, int action, half_edge *edge);

int runEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, link_list ** sweep_line);

#endif /* LINKLIST_H */
