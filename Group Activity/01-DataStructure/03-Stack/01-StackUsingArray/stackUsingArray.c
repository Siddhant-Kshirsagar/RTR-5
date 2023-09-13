//header file
#include<stdio.h>
#include<stdlib.h>

#define STACK_SIZE 5 // total size of stack

int top = -1; // In Stack data structure operation is done at top of the array so this variable represent top of the array
			 // -1 = represent the stack is empty

int iStack[STACK_SIZE];

//user defined function prototypes
void push(int); // push operation ( data insert in stack) 
void pop(void); // pop operation (data deleted from stack)

void displayStack(void); // display stack

int main(void)
{
	//local variable 
	int choice;
	int iData;
	do
	{
		printf(" Enter The choice\n");
		printf(" 1 = push\n");
		printf(" 2 = pop\n");
		printf(" 3 = display\n");
		printf(" 4 = exit\n");
		scanf("%d", &choice);
		switch (choice)
		{
			case 1:
			{
				printf(" Enter the number \n");
				scanf("%d", &iData);
				push(iData);
			}
			break;
			case 2:
			{
				pop();
				printf(" POP operation done\n");
			}
			break;
			case  3:
			{
				displayStack();
			}
			break;
			case 4:
			{
				printf(" Exitting Now...\n");
				exit(0);
			}
			default:
				printf("Enter Corret Choice\n");
				break;
		}
	} while (choice > 0 && choice < 5 );
	//push(101);
	//printf("\n stack : push \n");
	//displayStack();
	//push(102);
	//printf("\n stack : push \n");
	//displayStack();
	//push(103);
	//printf("\n stack : push \n");
	//displayStack();
	//push(104);
	//printf("\n stack : push \n");
	//displayStack();
	//push(105);
	//printf("\n stack : push \n");
	//displayStack();
	///*push(106);
	//printf("\n stack : push \n");
	//displayStack();*/
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
	//pop();
	//printf("\n stack : pop\n");
	//displayStack();
}

void push(int iData)
{
	if (top == STACK_SIZE - 1) // check if stack is full or not 
	{
		printf("\n\n Stack is full. You can not add new data in full stack.\n");
		exit(0);
	}
	// push data in stack increment current top and insert data in array
	top = top + 1;
	iStack[top] = iData;
}

void pop(void)
{
	if (top == -1) // check if stack is empty or not
	{
		printf("\n\n Stack is empty . you can not delete from empty stack.\n");
		exit(0);
	}
	top = top - 1;
}

void displayStack(void)
{
	if (top == -1)
	{
		printf("\n\n Stack is empty\n");
	}
	else
	{
		printf("\n\n Stack elements :\n\n");
		for (int i = 0; i <=top; i++)
		{
			printf("\t %d \t", iStack[i]);
		}
		printf("\n\n");
	}
}