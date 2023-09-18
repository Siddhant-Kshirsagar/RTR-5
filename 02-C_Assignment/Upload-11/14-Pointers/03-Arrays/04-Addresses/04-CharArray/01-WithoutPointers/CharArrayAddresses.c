#include<stdio.h>

int main(void)
{
	//variable declarations
	char SGK_cArray[10];
	int SGK_i;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_cArray[SGK_i] = (char)(SGK_i + 65);

	printf("\n\n");
	printf("Elements Of The Character Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("cArray[%d] = %c\n", SGK_i, SGK_cArray[SGK_i]);

	printf("\n\n");
	printf("Elements Of The Character Array : \n\n");
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("cArray[%d] = %c \t \t Address = %p\n", SGK_i, SGK_cArray[SGK_i], &SGK_cArray[SGK_i]);

	printf("\n\n");

	return(0);
}

