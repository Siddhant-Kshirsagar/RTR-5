#include<stdio.h>
#include<stdlib.h>

#include "Queue.h"

// global MACRO declaration 

#define MAX_SIZE 5
#define CREATE_LIST 1
#define INSERT_LIST 2
#define DELETE_LIST 3
#define DISPLAY_LIST 4
#define EXIT 0

// global variable declaration
Data_Node *first = NULL;
int iCurrentCount = 0;

void create(void);
void enqueue(void);
void dequeue(void);
void display(void);

int main(void)
{
	// variable declaration
	int iChoice = 0;
	Data_Node *temp = NULL;

	// code
	do
	{
		printf("\n\tEnter the choice\n");
		printf("\t 0: exit\n");
		printf("\t 1 : Create queue\n");
		printf("\t 2 : Insert data\n");
		printf("\t 3 : Delete node\n");
		printf("\t 4 : Display List\n");
		
		scanf("%d", &iChoice);
		
		switch (iChoice)
		{
		case EXIT:
			if (first != NULL)
			{
				printf("\n\tFree List before exit\n");
				temp = first;
				while (temp != NULL)
				{

					first = temp->next;

					printf("\t Data :- %d Deleted\n", temp->data);

					free(temp);
					temp = NULL;

					temp = first;
				}

			}
			exit(0);
			break;
		case CREATE_LIST:
			create();
			break;
		case INSERT_LIST:
			enqueue();
			break;
		case 3:
			dequeue();
			break;
		case 4:
			display();
			break;
		default:
			printf("\n\tPlease enter correct choice\n");
			break;
		}
	} while (iChoice != 5);

	return(0);
}

void create(void)
{
	//variable declaration
	int iData;

	// code
	if (first == NULL)
	{
		printf("\n\tEnter data to save in queue\n");
		scanf("%d", &iData);

		// global counter to keep track of size of queue;
		iCurrentCount = iCurrentCount + 1;
		first = createNode(iData);

		if (first == NULL)
		{
			printf("\n\tcreateNode() failed, exiting now...\n");
			exit(0);
		}
	}
	else
	{
		printf("\n\tList is already created, Please insert the data\n");
	}
	
}

void enqueue(void)
{
	//variable declaration
	int iData;

	//code
	if (iCurrentCount < MAX_SIZE)
	{
		iCurrentCount = iCurrentCount + 1;
		printf("\n\tEnter data to save in queue : ");
		scanf("%d",&iData);

		first = insertData(first, iData);
		displayList(first);
	}
	else
	{
		printf("\n\tList is full, STATUS :- OVERFLOW\n");
		return;
	}
}

void dequeue(void)
{
	// code
	if (iCurrentCount < 1)
	{
		printf("\n\tList is empty, STATUS :- UNDERFLOW\n");
	}
	else
	{
		
		first = deleteData(first);
		iCurrentCount = iCurrentCount - 1;
		displayList(first);
	}

}

void display(void)
{
	// code
	if (iCurrentCount > 0)
	{
		printf("\n\tCurrent Count of list : %d \n", iCurrentCount);
		displayList(first);
	
	}
	else
	{
		printf("\n\tList is empty\n");
	}
}
