#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a;
	int SGK_b;
	int SGK_result;

	//code 
	printf("\n\n");
	printf("Enter Another Number : \n");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Another Number : \n");
	scanf("%d", &SGK_b);

	printf("\n");

	// *** The Following Are The 5 Arithmetic Operators +,-,*,/ and % ***
	//*** Also, The Resultants Of The Arithmetic Operators In All The Below Five Cases Have Been Assigned To The Variable 'result' Using the Assignment Operator (=) ***

	SGK_result = SGK_a + SGK_b;
	printf("Addition Of A = %d And B = %d Gives %d . \n", SGK_a, SGK_b, SGK_result);

	SGK_result = SGK_a - SGK_b;
	printf("Subtraction Of A = %d And B = %d Gives %d . \n", SGK_a, SGK_b, SGK_result);

	SGK_result = SGK_a * SGK_b;
	printf("Multiplication A = %d And B = %d Gives %d . \n", SGK_a, SGK_b, SGK_result);

	SGK_result = SGK_a / SGK_b;
	printf("Division Of A = %d And B = %d Gives Quotient %d .\n", SGK_a, SGK_b, SGK_result);

	SGK_result = SGK_a % SGK_b;
	printf("Division Of A = %d And B = %d Gives Remainder %d .\n", SGK_a, SGK_b, SGK_result);

	printf("\n\n");

	return(0);
}
