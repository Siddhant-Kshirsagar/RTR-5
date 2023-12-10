#include<stdio.h>
#include<stdlib.h>

#include"Queue.h"

Data_Node *createNode(int iData)
{
	Data_Node *first = NULL;

	first = (Data_Node *)malloc(sizeof(Data_Node));

	first->data = iData;
	first->next = NULL;
	first->prev = NULL;

	return(first);
}

Data_Node *insertData(Data_Node* first,int iData)
{
	Data_Node *temp = first;
	Data_Node *newNode = NULL;

	if (temp != NULL)
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}

		newNode = (Data_Node *)malloc(sizeof(Data_Node));
		if (newNode == NULL)
		{
			printf("\n\tmalloc() Failed at insertData();\n");
			exit(0);
		}
		newNode->data = iData;
		newNode->prev = temp;
		newNode->next = temp->next;

		temp->next = newNode;
	}
	else
	{
		printf("\n\t Please create Node first \n");
	}

	return(first);
}

Data_Node *deleteData(Data_Node *first)
{
	Data_Node *temp = first;
	if (temp != NULL)
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
	else
	{
		return(temp);
	}
}


void displayList(Data_Node *first)
{
	// variable declaration
	Data_Node *temp = first;

	// code
	if (temp == NULL)
	{
		printf("\n\tList is empty\n");
	}
	else
	{
		while (temp != NULL)
		{
			printf("\t%d", temp->data);
			temp = temp->next;
		}
	}
}
