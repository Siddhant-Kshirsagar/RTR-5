#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a;
	int SGK_b;
	int SGK_result;

	//code
	printf("\n\n");
	printf("Enter One Integer : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Another Integer : ");
	scanf("%d", &SGK_b);

	printf("\n\n");
	printf("If Answer = 0, It Is 'FALSE' .\n");
	printf("If Answer = 1, It Is 'TRUE' .\n");

	SGK_result = (SGK_a < SGK_b);
	printf("(SGK_a < SGK_b) SGK_A = %d Is Less Than SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	SGK_result = (SGK_a > SGK_b);
	printf("(SGK_a > SGK_b) SGK_A = %d Is Greater Than SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	SGK_result = (SGK_a <= SGK_b);
	printf("(SGK_a <= SGK_b) SGK_A = %d Is Less Than Or Equal To SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	SGK_result = (SGK_a >= SGK_b);
	printf("(SGK_a >= SGK_b) SGK_A = %d Is Greater Than Or Equal To SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	SGK_result = (SGK_a == SGK_b);
	printf("(SGK_a == SGK_b) SGK_A = %d Is Equal To SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	SGK_result = (SGK_a != SGK_b);
	printf("(SGK_a != SGK_b) SGK_A = %d Is Not Equal SGK_B = %d		\t Answer = %d \n", SGK_a, SGK_b, SGK_result);

	return(0);
}
