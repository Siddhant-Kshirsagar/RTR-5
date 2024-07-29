#include<stdio.h>
#include<stdlib.h>

struct Node
{
	int data;
	struct Node *next;
	struct Node *prev;
};

struct Node *create_List(int numNodes);
void display_list(struct Node *first);
struct Node *insert_node(struct Node *first, int input, int position);
struct Node *delete_node(struct Node *first, int position);
struct Node *destroy_list(struct Node *first);

int main(void)
{
	int numNodes = 10;
	struct Node *head = create_List(numNodes);
	printf("Create List\n");
	display_list(head);

	return(0);
}

struct Node *create_List(int numNodes)
{
	struct Node *head = NULL;

	head = (struct Node *)malloc(sizeof(struct Node));
	head->data = 10;
	head->next = NULL;
	head->prev = NULL;

	struct Node *nTemp = head;
	struct Node *pTemp = head;

	for (int i = 1; i < numNodes; i++)
	{
		nTemp->next = (struct Node*)malloc(sizeof(struct Node));
		nTemp = nTemp->next;

		nTemp->data = 10;
		nTemp->next = NULL; 
		nTemp->prev = pTemp; 

		pTemp = pTemp->next; 
	}

	return head;
}

void display_list(struct Node *first)
{
	struct Node *temp = first;
	printf("Doubly Linked List Data \n\n");
	while (temp != NULL)
	{
		printf("%d \t", temp->data);
		temp = temp->next;
	}

	printf("\n\n");
}

struct Node *insert_node(struct Node *first, int input, int position)
{
	if (first == NULL && position != 1)
	{
		return(first);
	}

	if (position == 1)
	{
		struct Node *head = (struct Node *)malloc(sizeof(struct Node));
		head->data = input;
		head->next = first;
		head->prev = NULL;

		first->prev = head;

		return(head);
	}

	struct Node *temp = first;
	for (int i = 1; i < (position - 1) && temp != NULL; i++)
	{
		temp = temp->next;
	}

	//linked is empty, means position is invalid 
	if (temp == NULL)
	{
		return(first);
	}

	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

	newNode->data = input;
	newNode->next = temp->next;
	newNode->prev = temp;

	temp->next = newNode;

	//newNode->next->prev = newNode; // (this works same as below code work same)
	temp = newNode->next; 
	temp->prev = newNode;

	return(first);
}

struct Node *delete_node(struct Node *first, int position)
{
	if (first == NULL)
	{
		return(first);
	}
	if (position == 1)
	{
		struct Node *temp = first;
		first = first->next;
		free(temp);

		first->prev = NULL;

		return(first);
	}

	struct Node *temp = first;
	for (int i = 1; i < (position - 1) && temp != NULL; i++)
	{
		temp = temp->next;
	}

	if (temp == NULL)
	{
		return(first);
	}

	struct Node *delNode = temp->next;
	struct Node *nTemp = delNode->next;

	temp->next = nTemp;

	if (nTemp != NULL)
	{
		nTemp->prev = temp;
	}

	free(delNode);

	return(first);
}

struct Node *destroy_list(struct Node *first)
{
	struct Node *temp = first;
	while (temp != NULL)
	{
		temp = first->next;
		free(first);
		first = temp;
	}
	return(temp);
}
