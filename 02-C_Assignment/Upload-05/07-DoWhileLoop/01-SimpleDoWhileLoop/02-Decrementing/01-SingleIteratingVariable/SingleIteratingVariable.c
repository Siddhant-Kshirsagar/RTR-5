#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i;

	//code 
	printf("\n\n");

	printf("Printing Digits 10 to 1 : \n\n");

	SGK_i = 10;
	do
	{
		printf("\t%d\n", SGK_i);
		SGK_i--;
	} while (SGK_i >= 1);

	printf("\n\n");

	return(0);
}
