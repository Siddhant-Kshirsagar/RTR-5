#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j, SGK_k;

	//code 
	printf("\n\n");
	for (SGK_i = 1; SGK_i <= 10; SGK_i++)
	{
		printf("i = %d\n", SGK_i);
		printf("---------\n\n");
		for (SGK_j = 1; SGK_j <= 5; SGK_j++)
		{
			printf("\tj = %d \n", SGK_j);
			printf("\t---------\n\n");
			for (SGK_k = 1; SGK_k <= 3; SGK_k++)
			{
				printf("\t\tk = %d \n", SGK_k);
			}
			printf("\n\n");
		}
		printf("\n\n");
	}
	return(0);
}
