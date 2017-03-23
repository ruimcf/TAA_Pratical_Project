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
    printf("Added an edge\n");
}

void rmFromList(link_list **head, half_edge *item){
	link_list **ptr=head;
	link_list *tmp;
    link_list *prev = *head;
    int i = 0;
	while((*ptr)->item!=item){
        if(i != 0){
            prev = prev->next;
        }
		ptr=&((*ptr)->next);
        i++;
	}
    /* When we remove the first one */
    if(i == 0){
    printf("The edge was found! origin (%d,%d)\n",(*ptr)->item->origin->x, (*ptr)->item->origin->x);
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    /* When we remove some other, need to connect the previous */
    else{
        tmp=*ptr;
        prev->next=(*ptr)->next;
        free(tmp);
    }
	return;
}
