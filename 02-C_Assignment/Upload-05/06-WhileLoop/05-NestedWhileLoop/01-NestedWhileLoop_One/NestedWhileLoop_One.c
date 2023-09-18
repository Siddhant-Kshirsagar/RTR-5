#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");

	SGK_i = 1;
	while (SGK_i <= 10)
	{
		printf("i = %d\n", SGK_i);
		printf("---------\n\n");

		SGK_j = 1;
		while (SGK_j <= 5)
		{
			printf("\t j = %d\n", SGK_j);
			SGK_j++;
		}
		SGK_i++;
		printf("\n\n");
	}
	return(0);
}
