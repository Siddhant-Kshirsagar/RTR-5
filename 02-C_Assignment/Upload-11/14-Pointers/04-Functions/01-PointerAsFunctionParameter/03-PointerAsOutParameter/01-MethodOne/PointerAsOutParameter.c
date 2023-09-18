#include<stdio.h>

int main(void)
{
	//function declarations
	void MathematicalOperations(int, int, int *, int *, int *, int *, int *);

	//variable declarations
	int SGK_a;
	int SGK_b;
	int SGK_answer_sum;
	int SGK_answer_difference;
	int SGK_answer_product;
	int SGK_answer_quotient;
	int SGK_answer_remainder;

	//code
	printf("\n\n");
	printf("Enter Value Of 'A' : ");
	scanf("%d", &SGK_a);
	
	printf("\n\n");
	printf("Enter Value Of 'B' : ");
	scanf("%d", &SGK_b);

	// Passing Addresses To Function ... Function will till them up with values ... Hence, they go into the function as address parameter and come out of the function filled with valid values
	// Thus , (&answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder) Are called "Out Parameters" or "Parameterized return values" ... return values of function coming via parameters
	// Hence, Although each function has only one return value, using the concept of "Parameterized return values" our function "MathematicalOperations()" Has Given Us 5 Return values !!!

	MathematicalOperations(SGK_a, SGK_b, &SGK_answer_sum, &SGK_answer_difference, &SGK_answer_product, &SGK_answer_quotient, &SGK_answer_remainder);

	printf("\n\n");
	printf(" ***** RESULT ***** : \n\n");
	printf("Sum = %d\n\n",SGK_answer_sum);
	printf("Difference = %d\n\n", SGK_answer_difference);
	printf("Product = %d\n\n", SGK_answer_product);
	printf("Quotient = %d\n\n", SGK_answer_quotient);
	printf("Remainder = %d\n\n", SGK_answer_remainder);
	return(0);
}

void MathematicalOperations(int SGK_x, int SGK_y, int *SGK_sum, int *SGK_difference, int *SGK_product, int *SGK_quotient, int *SGK_remainder)
{
	//code
	*SGK_sum = SGK_x + SGK_y; // Value at address 'sum' = (x+y)
	*SGK_difference = SGK_x - SGK_y; // Value at address 'difference' = (x-y)
	*SGK_product = SGK_x * SGK_y; // Value at address 'product' = (x*y)
	*SGK_quotient = SGK_x / SGK_y; // Value at address 'quotient' = (x/y)
	*SGK_remainder = SGK_x % SGK_y; // Value at address 'remainder' = (X%y)
}
