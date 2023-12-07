#include<stdio.h>
#include<stdlib.h>


// struct declaration
struct Node
{
	int data;
	struct Node *prev;
	struct Node *next;
};

typedef struct Node MYNODE;

MYNODE *head = NULL;
MYNODE *head2 = NULL;

MYNODE *createList(int);

MYNODE *insertNode(MYNODE *, int, int);

MYNODE *deleteNode(MYNODE *,int);

void displayList(MYNODE *);

int main(void)
{
	head = createList(5);

	if (head == NULL)
	{
		printf("createList() Failed.\n");
		exit(0);
	}
	else
	{
		printf("createList() Success.\n");
	}

	displayList(head);

	head = insertNode(head, 1, 12);

	displayList(head);

	head = insertNode(head, 3, 14);

	displayList(head);

	head = insertNode(head, 7, 15);

	displayList(head);

	head = insertNode(head, 7, 16);

	displayList(head);

	head = insertNode(head, 4, 17);

	displayList(head);

	head = insertNode(head, 3, 18);

	displayList(head);

	head = insertNode(head, 12, 29);

	displayList(head);

	head = deleteNode(head, 1);
	displayList(head);

	head = deleteNode(head, 2);
	displayList(head);

	head = deleteNode(head, 10);
	displayList(head);

	head = deleteNode(head, 10);
	displayList(head);

	puts("Program ends here\n");

	return(0);
}


//MYNODE *createList(int noOfNodes)
//{
//	MYNODE *first = NULL;
//	MYNODE *temp = NULL;
//	MYNODE *newNode = NULL;
//	
//	first = (MYNODE *)malloc(sizeof(MYNODE));
//
//	first->data = 10;
//	first->next = NULL;
//	first->prev = NULL;
//
//	temp = first;
//
//	for (int i = 1; i < noOfNodes; i++)
//	{
//		temp->next = (MYNODE *)malloc(sizeof(MYNODE));
//
//		newNode = temp->next;
//
//		newNode->data = 10;
//		newNode->next = NULL;
//		newNode->prev = temp;
//
//		temp = temp->next;
//	}
//
//	return(first);
//
//}

MYNODE *createList(int noOfNodes)
{
	if (noOfNodes <= 0)
	{
		return(NULL);
	}

	MYNODE *first = NULL;
	MYNODE *temp = NULL;

	first = (MYNODE *)malloc(sizeof(MYNODE));

	first->data = 10;
	first->next = NULL;
	first->prev = NULL;

	temp = first;

	for (int i = 1; i < noOfNodes; i++)
	{
		temp->next = (MYNODE *)malloc(sizeof(MYNODE));

		
		temp->next->prev = temp;

		temp = temp->next;

		temp->data = i;
		temp->next = NULL;
	}

	return(first);

}

void displayList(MYNODE *first)
{
	MYNODE *temp = first;
	if (temp == NULL)
	{
		printf("\n\tList is empty\n");
		return;
	}
	else
	{
		printf("\n\tDisplay List \n");

		while (temp != NULL)
		{
			printf("\t %d", temp->data);
			temp = temp->next;
		}
		printf("\n\n");
	}


}

MYNODE *insertNode(MYNODE *first, int position, int data)
{
	//variable declaration
	MYNODE *temp = first;
	MYNODE *newNode = NULL;
	
	// add data at first position, if head/first is null thant new node will be created
	if (position == 1)
	{
		newNode = (MYNODE *)malloc(sizeof(MYNODE));

		newNode->data = data;
		newNode->next = temp;
		newNode->prev = NULL;

		if (temp != NULL) // if head is not null
		{
			temp->prev = newNode;
		}	

		return(newNode);
		
	}

	// if head/first not null and position is greater than 1(position is in the middle of list)
	// if head/first is null and position is greater than 1, than we first check at for loop iteration than we exit from here head will be return
		
	for (int i = 1; (i < position - 1) && (temp!=NULL); i++)// traverse to the previous of the entered position
	{
		temp = temp->next;
	}

	// if position is invalid (temp is only null when position is invalid)
	if (temp == NULL)
	{
		printf("\n\tInvalid Position\n");
		return(head);
	}

	// insert new node at given position
	newNode = (MYNODE *)malloc(sizeof(MYNODE));

	newNode->data = data;
	newNode->prev = temp;
	newNode->next = temp->next;

	if (temp->next != NULL)
	{
		temp->next->prev = newNode;
	}

	temp->next = newNode;

	return(first);
}

MYNODE *deleteNode(MYNODE *first,int position)
{
	MYNODE *temp = first;
	MYNODE *delNode = NULL;

	if (position == 1)
	{
		first = temp->next;

		if (temp->next != NULL)
		{
			first->prev = NULL;
		}

		free(temp);
		temp = NULL;

		return(first);
	}

	if (position > 1)
	{
		for (int i = 1; (i < position - 1) && (temp->next != NULL); i++)
		{
			temp = temp->next;
		}

		if (temp->next == NULL)
		{
			printf("\n\t Invalid Position\n");
			return(head);
		}

		delNode = temp->next;	
		
		if (delNode->next != NULL)
		{
			temp->next = delNode->next;
			delNode->next->prev = temp;
		}
		else
		{
			temp->next = NULL;
		}
			
		free(delNode);
		delNode = NULL;
	}

	return(first);
}
