#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	printf("Printing Digits 10 to 1 100 to 10 : \n\n");

	SGK_i = 10;
	SGK_j = 100;
	while (SGK_i >= 1, SGK_j >= 10)
	{
		printf("\t %d \t %d \n", SGK_i, SGK_j);
		SGK_i--;
		SGK_j = SGK_j - 10;
	}

	printf("\n\n");

	return(0);
}
