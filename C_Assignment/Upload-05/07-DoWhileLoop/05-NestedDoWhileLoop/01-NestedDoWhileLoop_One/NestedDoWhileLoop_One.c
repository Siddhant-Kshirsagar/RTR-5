#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	SGK_i = 1;
	do
	{
		printf("i = %d\n", SGK_i);
		printf("----------\n\n");
		SGK_j = 1;
		do
		{
			printf("\tj = %d\n", SGK_j);
			SGK_j++;
		} while (SGK_j <= 5);
		SGK_i++;
		printf("\n\n");
	} while (SGK_i <= 10);
	return(0);
}
