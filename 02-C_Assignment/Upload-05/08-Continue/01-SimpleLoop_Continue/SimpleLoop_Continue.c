#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i;

	//code 
	printf("\n\n");

	printf("Printing Even Numbers From 0 to 100 : \n\n");

	for (SGK_i = 0; SGK_i <= 100; SGK_i++)
	{
		//condition for a number to be even number => division of number 2 leaves no remainder (remainder = 0) 
		//if remainder is not 0, the number is odd number...
		if (SGK_i % 2 != 0)
		{
			continue;
		}
		else
		{
			printf("\t%d\n", SGK_i);
		}
	}

	printf("\n\n");
	
	return(0);
}
