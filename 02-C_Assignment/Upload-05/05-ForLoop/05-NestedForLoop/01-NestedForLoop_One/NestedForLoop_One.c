#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j;

	//code 
	printf("\n\n");
	for (SGK_i = 1; SGK_i <= 10; SGK_i++)
	{
		printf("i = %d \n", SGK_i);
		printf("---------\n\n");
		for (SGK_j = 1; SGK_j <= 5; SGK_j++)
		{
			printf("\t j = %d \n", SGK_j);
		}
		printf("\n\n");
	}
	return(0);
}
