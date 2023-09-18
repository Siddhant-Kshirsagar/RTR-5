#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	//variable declarations
	int SGK_iArray[NUM_ELEMENTS];
	int SGK_i, SGK_num, SGK_sum = 0;

	//code 
	printf("\n\n");

	// ***** ARRAY ELEMENTS INPUT *****
	printf("Enter Integer Elements For Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		scanf("%d",&SGK_num);
		SGK_iArray[SGK_i] = SGK_num;
	}

	// ***** SEPERATING OUT EVEN NUMBERS FROM ARRAY ELEMENTS *****
	printf("\n\n");
	printf("Even Numbers Amongst The Array Elements Are : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		if ((SGK_iArray[SGK_i] % 2) == 0)
			printf("%d\n", SGK_iArray[SGK_i]);
	}

	//***** SEPERATING OUT ODD NUMBERS FROM ARRAY ELEMENTS *****
	printf("\n\n");
	printf("Odd Numbers Amongst The Array Elements Are : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		if ((SGK_iArray[SGK_i] % 2) != 0)
			printf("%d\n", SGK_iArray[SGK_i]);
	}

	return(0);
}
