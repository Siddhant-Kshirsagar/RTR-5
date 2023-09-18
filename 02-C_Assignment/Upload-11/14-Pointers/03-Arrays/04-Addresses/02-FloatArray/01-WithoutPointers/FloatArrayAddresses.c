#include<stdio.h>

int main(void)
{
	//variable declarations
	float SGK_fArray[10];
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_fArray[SGK_i] = (float)(SGK_i + 1) * 1.5f;

	printf("\n\n");
	printf("Elements Of The 'float' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("fArray[%d] = %f\n", SGK_i, SGK_fArray[SGK_i]);

	printf("\n\n");
	printf("Elements of the 'float' Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("fArray[%d] = %f \t\t Address = %p\n", SGK_i, SGK_fArray[SGK_i],&SGK_fArray[SGK_i]);

	printf("\n\n");

	return(0);
}
