typedef struct s_link_list{
	struct s_link_list *next;
	void *item;
} link_list;

void addToList(link_list **head, void *item);

void rmFromList(link_list **head, void *item);
