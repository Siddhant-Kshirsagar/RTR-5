#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i, SGK_j, SGK_k;
	
	//code 
	printf("\n\n");

	SGK_i = 1;
	do
	{
		printf("i = %d\n", SGK_i);
		printf("---------\n\n");
		SGK_j = 1;
		do
		{
			printf("\tj = %d \n", SGK_j);
			printf("\t----------\n\n");

			SGK_k = 1;
			do
			{
				printf("\t\tk = %d\n", SGK_k);
				SGK_k++;
			} while (SGK_k <= 3);
			printf("\n\n");
			SGK_j++;
		} while (SGK_j <= 5);
		printf("\n\n");
		SGK_i++;
	} while (SGK_i <= 10);
	return(0);
}
