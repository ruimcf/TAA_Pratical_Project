#include <dcel.h>

typedef struct s_link_list{
	struct s_link_list *next;
	half_edge *item;
} link_list;

void addToList(link_list **head, half_edge *item);

void rmFromList(link_list **head, half_edge *item);
