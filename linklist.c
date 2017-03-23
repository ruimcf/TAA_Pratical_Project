#include <linklist.h>
#include <stdlib.h>

void addToList(link_list **head, void *item){
	link_list *add=malloc(sizeof(link_list)),
		  *tmp=*head;
	add->item=item;
	*head=add;
	add->next=tmp;
	return;
}

void rmFromList(link_list **head, void *item){
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
