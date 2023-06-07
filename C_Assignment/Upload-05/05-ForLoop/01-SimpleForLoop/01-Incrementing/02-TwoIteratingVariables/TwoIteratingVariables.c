#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	printf("Printing Digits 1 to 10 and 10 to 100 : \n\n");

	for (SGK_i = 1, SGK_j = 10; SGK_i <= 10, SGK_j <= 100; SGK_i++, SGK_j = SGK_j + 10)
	{
		printf("\t %d \t %d \n",SGK_i,SGK_j);
	}

	printf("\n\n");

	return(0);
}
