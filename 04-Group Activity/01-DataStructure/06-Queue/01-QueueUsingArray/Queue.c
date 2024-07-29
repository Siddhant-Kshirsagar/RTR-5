#include<stdio.h>
#include<stdlib.h>



//function declaration
void enqueue(void);
void dequeue(void);
void display(void);

// global variable 
#define MAX_SIZE 5
int queueArray[MAX_SIZE] = { 0 };

int front = -1;
int rear = -1;

int main(void)
{
	// variable declaration
	int iChoice = 0;

	// code

	do
	{
		printf(" Max size = %d \n", MAX_SIZE);
		printf("\n\t Enter your choice\n");
		printf("\n\t 1 :- Enqueue data \n");
		printf("\n\t 2 :- Dequeue data \n");
		printf("\n\t 3 :- Display quque\n");
		printf("\n\t 4 :- Exit\n");
		printf("\n\t ");
		scanf("%d", &iChoice);

		switch (iChoice)
		{
		case 1:
			enqueue();
			break;
		case 2:
			dequeue();
			break;
		case 3:
			display();
			break;
		case 4:
			exit(0);
			break;
		default:
			printf("\n\t Please enter corret choice\n");
			break;
		}

	} while (iChoice != 4);

	return(0);
}

void enqueue(void)
{
	// code
	
	if (rear == (MAX_SIZE - 1) )
	{
		printf("\n\t****** Queue is full ******\n");

	}
	else
	{
		
		rear = rear + 1;
		printf("\n\tEnter data to save in queue : ");
		scanf("%d", &queueArray[rear]);
		
		printf("\n");
	}
	
}

void dequeue(void)
{
	if (front == rear)
	{
		printf("\n\t****** Queue is empty ******\n");
	}
	else
	{
		front = front + 1;
		printf("\n\tData has been deleted from queue\n");
	}
	

}

void display(void)
{
	// code
	if (front == rear)
	{
		printf("\n\t ****** Queue is empty ******\n");
	}
	else
	{
		printf("\n\t Queue Data \n");
		for (int i = front + 1; i <= rear; i++)
		{
			printf("\t%d", queueArray[i]);
		}
		printf("\n");
	}
	
}
