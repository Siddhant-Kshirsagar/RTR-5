#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	printf("Outer Loop Prints Odd Numbers Between 1 and 10. \n\n");
	printf("Inner Loop Prints Even Numbers Between 1 and 10. For Every Odd Number Printed By Outer Loop. \n\n");

	//condition for a number to be a even number => division of number by 2 leaves no remainder (remainder =0)
	//condition for a number to be odd number => division of number by 2 leaves remainder (remainder = 1(usually))

	for (SGK_i = 1; SGK_i <= 10; SGK_i++)
	{
		if (SGK_i % 2 != 0) //If Number (i) Is Odd...
		{
			printf("i = %d \n", SGK_i);
			printf("----------\n\n");
			for (SGK_j = 1; SGK_j <= 10; SGK_j++)
			{
				if (SGK_j % 2 == 0)//If Number (j) Is Even...
				{
					printf("\tj = %d\n", SGK_j);
				}
				else//If Number (j) Is Odd...
				{
					continue;
				}
			}
			printf("\n\n");
		}
		else//If Number (i) Is Even...
		{
			continue;
		}
	}
	printf("\n\n");

	return(0);
}
