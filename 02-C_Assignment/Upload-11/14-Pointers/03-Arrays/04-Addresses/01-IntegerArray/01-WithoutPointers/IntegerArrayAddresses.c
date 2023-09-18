#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_iArray[10];
	int SGK_i;

	//code 
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_iArray[SGK_i] = (SGK_i + 1) * 3;

	printf("\n\n");
	printf("Elements Of The Integer Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("iArray[%d] = %d\n", SGK_i, SGK_iArray[SGK_i]);

	printf("\n\n");
	printf("Elements Of The Integer Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("iArray[%d] = %d \t\t Address = %p\n", SGK_i, SGK_iArray[SGK_i], &SGK_iArray[SGK_i]);

	printf("\n\n");

	return(0);
}
