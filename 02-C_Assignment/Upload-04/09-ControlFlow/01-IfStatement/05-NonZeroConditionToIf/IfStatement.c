#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_a;

	//code 
	printf("\n\n");

	SGK_a = 5;
	if (SGK_a)//Non-Zero Positive Value
	{
		printf("if-block 1 : 'A' Exists And Has Value = %d !!!\n\n", SGK_a);
	}

	SGK_a = -5;
	if (SGK_a)//Non-Zero Negative Value 
	{
		printf("if-block 3:'A' Exists And Has Value = %d !!!\n\n", SGK_a);
	}

	printf("All Three if-statements Are Done !!! \n\n");

	return(0);
}
