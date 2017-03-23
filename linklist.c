#include <linklist.h>
#include <stdlib.h>

void addToList(link_list **head, half_edge *item){
	link_list *add = malloc(sizeof(link_list));
	add->item = item;
	add->next = *head;
    *head = add;
	return;
}

void rmFromList(link_list **head, half_edge *item){
	link_list **ptr=head,
		  *tmp;
	while((*ptr)->item!=item){
		ptr=&((*ptr)->next);
	}
	tmp=*ptr;
	*ptr=(*ptr)->next;
	free(tmp);
	return;
}
