#include<stdio.h>
#include<stdlib.h>
#define MAXSIZE 5

// user defined function
void push(int); // push operation
int pop(void); // pop operation

void displayStack(void); // display stack 

struct Node
{
	int data;
	struct Node *next;
	struct Node *prev;
}*top,*top1,*temp,*head;

int count = 0;

//entry point function 
int main(void)
{
	int choice, value;
	printf("\n \033[1;36m ------------ Stack Using Linked List --------------- \033[1;0m\n");

	do
	{
		printf("\n \t 1. PUSH Data \n\t 2. POP Data \n\t 3. Display Stack \n\t 4. Exit \n\t Enter your choice : ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
		{
			printf("\n Enter The Number : ");
			scanf("%d", &value);
			push(value);
			break;
		}
		case 2:
		{
			int deletedNode = pop();
			printf("\n Deleted Node is : %d \n", deletedNode);
			break;
		}
		case 3:
		{
			printf("\nDisplay Stack\n");
			displayStack();
			break;
		}
		case 4:
		{
			exit(0);
			break;
		}
		default:
			printf("\n\t \033[1;31m Wrong Choice ... Enter 1-4 Numbers Only \033[1;0m \n");
			break;
		}
	} while (choice != 4);

	return(0);
}

void push(int num)
{
	//code
	if (count == MAXSIZE)
	{
		printf("\n\t \033[1;31m Stack is Full. You Can Not Push the Data. \033[1;0m \n");
	}
	else
	{
		if (top == NULL)
		{
			top = (struct Node *)malloc(sizeof(struct Node));
			top->data = num;
			top->next = NULL;
			top->prev = NULL;
			head = top;
		}
		else
		{
			temp = (struct Node *)malloc(sizeof(struct Node));
			temp->prev = top;
			temp->next = NULL;
			temp->data = num;
			
			top->next = temp;
			top = temp;
		}
		count++;
		printf("\n\t \033[1;32m %d Push Successfully at top position \033[1;0m \n", num);
	}
}

int pop(void)
{
	struct Node *deleteNode;
	deleteNode = top;

	//code
	if (top == NULL)
	{
		printf("\n\t \033[1;31m Stack is empty \033[1;0m \n");
		return(-1);
	}
	int poppedData = top->data;

	if (count == 1)
	{

		free(head);
		head = top = NULL;
		count--;
		return(poppedData);

	}

	top = top->prev;
	top->next = NULL;

	free(deleteNode);
	deleteNode = NULL;

	count--;
	return(poppedData);
}

void displayStack(void)
{
	if (head == NULL)
	{
		printf("\n\t \033[1;31m Stack is Empty \033[1;0m \n");
		return;
	}
	
	printf("\n\t \033 Stack elements are : \033\n");
	for (temp = head; temp != NULL; temp = temp->next)
	{
		printf("\n\t %d ", temp->data);
	}
}

