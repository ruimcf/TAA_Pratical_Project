#include <linklist.h>
#include <stdlib.h>
#include <stdio.h>

void addToList(link_list **head, half_edge *item){
	link_list *add = malloc(sizeof(link_list));
	add->item = item;
	add->next = *head;
	*head = add;
	return;
}

void addToListByX(link_list **head, half_edge *item){
	link_list *add = malloc(sizeof(link_list));
	link_list *tmp = *head;
	add->item = item;
	/* When list is empty */
	if(tmp == NULL){
		add->next = tmp;
		*head = add;
	}else{
		int not_added = 1;
		while(not_added){
			/* When the first item has bigger X */
			if(tmp->item->origin->x > item->origin->x){
				add->next = tmp;
				*head = add;
				not_added = 0;
			}
			else{
				if(tmp->next != NULL){
					/* When at the right place */
					if(tmp->next->item->origin->x > item->origin->x){
						add->next = tmp->next;
						tmp->next = add;
						not_added = 0;
					}
				}
				/* When at the end of the list */
				else{
					add->next = NULL;
					tmp->next = add;
					not_added = 0;
				}
				tmp = tmp->next;
			}
		}
	}
}

void addToListByY(link_list **head, half_edge *item){
	if(item==NULL){
		return;
	}
	link_list **ptr=head,
		  *add=malloc(sizeof(link_list));
	add->item=item;
	//advances list untill end or find an item with y > than add item
	while((*ptr)!=NULL && (*ptr)->item->origin->y < item->origin->y){
		ptr=&((*ptr)->next);
	}
	//insert the item before the 1st bigger item found or at the end if not found
	add->next=*ptr;
	*ptr=add;
	return;
}

half_edge *getBeforeY(link_list *head, int y){
	link_list *ptr=head;
	while(ptr->next->item->origin->y < y){
		ptr=ptr->next;
	}
	return ptr->item;
}

half_edge *getAfterY(link_list *head, int y){
	link_list *ptr=head;
	while(ptr->item->origin->y <= y){
		ptr=ptr->next;
	}
	return ptr->item;
}


void rmFromList(link_list **head, half_edge *item){
	link_list **ptr=head,
		  *tmp;
	while((*ptr)->item!=item){
		ptr=&((*ptr)->next);
	}
	//store item to be freed
	tmp=*ptr;
	//make the item that points to the item being deleted point to the next
	*ptr=(*ptr)->next;
	free(tmp);
	return;
}

int addEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, int action, half_edge *edge){
	sweep_line_action * new = malloc(sizeof(sweep_line_action));
	new->action = action;
	new->edge = edge;
	sweep_line_action_list[size] = new;
	return size+1;
}

int runEventSweepLine(sweep_line_action ** sweep_line_action_list, int size, link_list ** sweep_line){
    for(int i = 0; i < size; i++){
        if(sweep_line_action_list[i]->action == 0){
            addToListByX(sweep_line, sweep_line_action_list[i]->edge);
        }
        else if(sweep_line_action_list[i]->action == 1){
            rmFromList(sweep_line, sweep_line_action_list[i]->edge);
        }
        free(sweep_line_action_list[i]);
    }
    return 0;
}

void printLinkList(link_list *head){
	link_list *ptr=head;
	while(ptr!=NULL){
		printf("Edge origin: (%d,%d) | destination: (%d, %d)\n", 
				ptr->item->origin->x,
				ptr->item->origin->y,
				ptr->item->twin->origin->x,
				ptr->item->twin->origin->y);
		ptr=ptr->next;
	}
	return;
}
