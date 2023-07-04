#include<stdio.h>

int main(void)
{
	//variable declarations
	double SGK_dArray[10];
	int SGK_i;

	//code 
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_dArray[SGK_i] = (float)(SGK_i + 1) * 1.333333f;

	printf("\n\n");
	printf("Elements Of The 'double' Array :\n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("dArray[%d] = %lf\n", SGK_i, SGK_dArray[SGK_i]);

	printf("\n\n");
	printf("Elements Of The 'double' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("dArray[%d] = %lf \t \t Address = %p\n", SGK_i, SGK_dArray[SGK_i], &SGK_dArray[SGK_i]);

	printf("\n\n");

	return(0);
}
