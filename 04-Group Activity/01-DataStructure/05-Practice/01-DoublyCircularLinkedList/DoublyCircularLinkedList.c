// header files
#include<stdio.h>
#include<stdlib.h>

// struct declaration
struct Node
{
	int data;
	struct Node *next;
	struct Node *prev;
};

typedef struct Node MyNode;

// variable declaration
MyNode *head = NULL;

// entry point function
int main(void)
{
	// function declaration
	MyNode* createDoublyLinkedList(int);
	MyNode* insertAtBeggining(MyNode*, int);
	void DeleteLastNode(MyNode*);
	void DisplayList(MyNode*);
	void DestroyList(MyNode*);



	// code
	head = createDoublyLinkedList(10);

	DisplayList(head);

	head =  insertAtBeggining(head, 14);

	DisplayList(head);

	head = insertAtBeggining(head, 16);

	DisplayList(head);

	head = insertAtBeggining(head, 19);
	
	DisplayList(head);

	DeleteLastNode(head);

	DisplayList(head);

	DestroyList(head);

	DisplayList(head);
}

MyNode* createDoublyLinkedList(int iData)
{
	// variable declaration
	MyNode *head = NULL;

	// code
	head = (MyNode*) malloc(sizeof(MyNode));

	head->data = iData;
	head->next = head;
	head->prev = head;

	return(head);
}

MyNode* insertAtBeggining(MyNode* head, int iData)
{
	// variable declaration
	MyNode *tempHead = NULL;

	//code
	tempHead = (MyNode*)malloc(sizeof(MyNode));
	
	if (head->next == head)
	{
		tempHead->data = iData;
		tempHead->next = head;
		tempHead->prev = head;

		head->prev = tempHead;
		head->next = tempHead;
	}
	else
	{
		tempHead->data = iData;
		tempHead->next = head;
		tempHead->prev = head->prev;

		head->prev = tempHead;
		tempHead->prev->next = tempHead;
	}
	return(tempHead);
}

void DeleteLastNode(MyNode* head)
{
	// variable declaration
	MyNode *temp = head;
	MyNode *tempHead = head;
	
	//code
	if (tempHead != NULL)
	{

		tempHead = head->prev->prev;

		temp = tempHead->next;

		// second last node pointing to the head because we are going to delete last node
		tempHead->next = head;
		// previous of head poiting to the second last node
		head->prev = tempHead;

		// for delete last node

		free(temp);
		temp = NULL;
	}
}

void DisplayList(MyNode* head)
{
	// variable declaration
	MyNode *temp = head;
	// code
	if (temp)
	{
		printf("\nDoubly Circular Linked List Data : \n");
		do
		{
			printf("\t %d ", temp->data);

			temp = temp->next;
		}while (temp != head);

		printf("\n-----------------------------------\n");
	}
	else
	{
		printf("List is empty\n");
	}
	
}

void DestroyList(MyNode* first)
{
	// variable declaration
	MyNode *temp = first->next;
	MyNode *tempNext = NULL;

	//code
	do
	{
		tempNext = temp->next;

		free(temp);
		temp = NULL;
		temp = tempNext;

	} while (temp != first);

	free(head);
	head = NULL;

}