#include <dcel.h>

Dcel createCode(int x, int y){
	Dcel node;
	node.x=x;
	node.y=y;
	node.up=NULL;
	node.right=NULL;
	node.down=NULL;
	node.left=NULL;
	return node;
}

void connectNode(Dcel *node1, Dcel *node2){
	if(*node1.x == *node2.x){
		if(*node1.y > *node2.y){
			*node2.up=node1;
			*node1.down=node2;
			return;
		}
		else if(*node1.y < *node2.y){
			*node2.down=node1;
			*node1.up=node2;
			return;
		}
	}
	else if(*node1.y == *node2.y){
		if(*node1.x > *node2.x){
			*node2.right=node1;
			*node1.left=node2;
			return;
		}
		else if(*node1.x < *node2.x){
			*node2.left=node1;
			*node1.right=node2;
			return;
		}
	}
	printf("Error connecting (%d,%d) and (%d,%d)\n", 
			*node1.x, *node1.y,
		       	*node2.x, *node2.y);
	return;
}

void insertNode(Dcel *insert, Dcel *node1, Dcel *node2){
	connectNode(*insert, node1);
	connectNode(*insert, node2);
}
	

