#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a = 5;
	int SGK_b = 10;

	//code 
	printf("\n\n");
	printf("SGK_A = %d \n", SGK_a);
	printf("SGK_A = %d \n", SGK_a++);
	printf("SGK_A = %d \n", SGK_a);
	printf("SGK_A = %d \n\n", ++SGK_a);

	printf("SGK_B = %d \n", SGK_b);
	printf("SGK_B = %d \n", SGK_b++);
	printf("SGK_B = %d \n", SGK_b);
	printf("SGK_B = %d \n", --SGK_b);

	return(0);
}

