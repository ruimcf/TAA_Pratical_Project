#include <dcel.h>

typedef struct s_link_list{
	struct s_link_list *next;
	half_edge *item;
} link_list;

typedef struct s_sweep_line_action{
    int action;
    half_edge *edge;
} sweep_line_action;

void addToList(link_list **head, half_edge *item);

void addToListByX(link_list **head, half_edge *item);

void addToListByY(link_list **head, half_edge *item);

void rmFromList(link_list **head, half_edge *item);

int addEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, int action, half_edge *edge);

int runEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, link_list ** sweep_line);
