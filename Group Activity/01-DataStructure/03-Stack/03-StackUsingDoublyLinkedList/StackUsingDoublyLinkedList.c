#include<stdio.h>
#include<stdlib.h>

#include "Stack.h"

#define MAX_SIZE 5

enum SWITCH_CHOICE
{
	EXIT,
	CREATE_LIST,
	PUSH,
	POP,
	DISPLAY,
};

void create(void);
void push(void);
void pop(void);
void display(void);

Data_Node *first = NULL;

int Top = 0;

int main(void)
{
	// variable declaration
	int iChoice;
	Data_Node *temp = NULL;
	// code
	do
	{
		printf("\n\tEnter the choice :- \n");
		printf("\t 0 :- Exit\n");
		printf("\t 1 :- Create \n");
		printf("\t 2 :- Push \n");
		printf("\t 3 :- Pop \n");
		printf("\t 4 :- Display \n");

		scanf("%d", &iChoice);

		switch (iChoice)
		{
		case EXIT:
			if (first != NULL)
			{
				temp = first;
				while (temp != NULL)
				{
					first = temp->next;
					printf("\n\tDelete data '%d' before exiting", temp->data);
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
		case PUSH:
			push();
			break;
		case POP:
			pop();
			break;
		case DISPLAY:
			display();
			break;
		default:
			printf("\n\tPlease enter correct choice\n");
			break;
		}
	} while (iChoice != 0);
	return(0);
}

void create(void)
{
	//variable declaration
	int iData;

	//code
	Top = 0; // reseting Top of stack before creating new stack
	if (first == NULL)
	{
		printf("\n\tEnter data to insert in stack while creating :- \t");
		scanf("%d", &iData);

		first = createNode(iData);
		Top = Top + 1; // incrementing global variable to keep track of top and total data present in stack

		if (first == NULL)
		{
			printf("\n\tcreateNode() failed, exiting now...\n");
			exit(0);
		}
	}
	else
	{
		printf("\n\tStack is already created, Please push data\n");
	}
	
}

void push(void)
{
	// variable declaration
	int iData; 

	if (Top < MAX_SIZE)
	{
		printf("\n\tEnter data to push in stack :- ");
		scanf("%d", &iData);

		first = insertData(first, iData);
		Top = Top + 1;
		display();
	}
	else
	{
		printf("\n\tStack is full\n");
	}
}

void pop(void)
{
	if (Top < 1)
	{
		printf("\n\tStack is empty\n");
	}
	else
	{
		first = deleteData(first);
		Top = Top - 1;
		display();
	}
}

void display(void)
{
	if (Top < 1)
	{
		printf("\n\tStack is empty\n");
	}
	else
	{
		printf("\n\tCurrent count of Stack :- %d\n", Top);
		displayList(first);
	}
}
