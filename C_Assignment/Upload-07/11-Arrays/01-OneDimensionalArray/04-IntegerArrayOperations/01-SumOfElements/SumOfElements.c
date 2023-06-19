#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	//variable declarations
	int SGK_iArray[NUM_ELEMENTS];
	int SGK_i, SGK_num,SGK_sum = 0;

	//code 
	printf("\n\n");
	printf("Enter Integer Elemenst For Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		scanf("%d", &SGK_num);
		SGK_iArray[SGK_i] = SGK_num;
	}

	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		SGK_sum = SGK_sum + SGK_iArray[SGK_i];
	}

	printf("\n\n");
	printf("Sum Of All Elements Of Array = %d \n\n",SGK_sum);

	return(0);
}
