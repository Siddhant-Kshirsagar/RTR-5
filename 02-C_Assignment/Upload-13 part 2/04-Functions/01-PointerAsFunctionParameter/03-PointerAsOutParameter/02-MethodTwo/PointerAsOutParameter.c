#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//function declarations
	void MathematicalOperations(int, int, int *, int *, int *, int *, int *);

	//variable declaration
	int SGK_a;
	int SGK_b;
	int *SGK_answer_sum = NULL;
	int *SGK_answer_difference = NULL;
	int *SGK_answer_product = NULL;
	int *SGK_answer_quotient = NULL;
	int *SGK_answer_remainder = NULL;

	//code
	printf("\n\n");
	printf("Enter value of 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter value of 'B' : ");
	scanf("%d", &SGK_b);

	// Passing Address To Function ... Function Will Fill Them Up With Values ... Hence , They Go Into The Function As Address Parameters And Come out of the function filled with valid values
	// Thus, (&answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder) Are Called 'Out parameters' or 'Parameterized return values' ... Return Values of functions comming via parameters
	// Hence, Although each function has only one return value, Using The Concept of 'Parameterized return values', Our function 'MathematicalOperation()' has given us 5 return values !!!

	SGK_answer_sum = (int *)malloc(1 * sizeof(int));
	if (SGK_answer_sum == NULL)
	{
		printf("Could Not Allocate memory for 'answer_sum'. Exitting Now ...\n\n");
		exit(0);
	}

	SGK_answer_difference = (int *)malloc(1 * sizeof(int));
	if (SGK_answer_difference == NULL)
	{
		printf("Could Not Allocate memory for 'answer_difference'. Exitting Now...\n\n");
		exit(0);
	}

	SGK_answer_product = (int *)malloc(1 * sizeof(int));
	if (SGK_answer_product == NULL)
	{
		printf("Could Not Allocate memory for 'answer_product'. Exitting Now...\n\n");
		exit(0);
	}

	SGK_answer_quotient = (int *)malloc(1 * sizeof(int));
	if (SGK_answer_quotient == NULL)
	{
		printf("Could Not Allocate memory for 'answer_quotient'. Exitting Now...\n\n");
		exit(0);
	}

	SGK_answer_remainder = (int *)malloc(1 * sizeof(int));
	if (SGK_answer_remainder == NULL)
	{
		printf("Could Not Allocate memory for 'answer_remainder'. Exitting Now...\n\n");
		exit(0);
	}

	MathematicalOperations(SGK_a, SGK_b, SGK_answer_sum, SGK_answer_difference, SGK_answer_product, SGK_answer_quotient, SGK_answer_remainder);

	printf("\n\n");
	printf(" ***** RESULT *****\n\n");
	printf("Sum = %d\n\n", *SGK_answer_sum);
	printf("Difference = %d\n\n", *SGK_answer_difference);
	printf("Product = %d\n\n", *SGK_answer_product);
	printf("Quotient = %d\n\n", *SGK_answer_quotient);
	printf("Remainder = %d\n\n", *SGK_answer_remainder);

	if (SGK_answer_remainder)
	{
		free(SGK_answer_remainder);
		SGK_answer_remainder = NULL;
		printf("Memory Allocated For 'answer_remainder' Successfully freed !!!\n\n");
	}

	if (SGK_answer_quotient)
	{
		free(SGK_answer_quotient);
		SGK_answer_quotient = NULL;
		printf("Memory Allocated  For 'answer_quotient' Successfully freed !!!\n\n");
	}

	if (SGK_answer_product)
	{
		free(SGK_answer_product);
		SGK_answer_product = NULL;
		printf("Memory Allocated For 'answer_product' successfully freed !!!\n\n");
	}

	if (SGK_answer_difference)
	{
		free(SGK_answer_difference);
		SGK_answer_difference = NULL;
		printf("Memory Allocated For 'answer_difference' successfully freed !!!\n\n");
	}

	if (SGK_answer_sum)
	{
		free(SGK_answer_sum);
		SGK_answer_sum = NULL;
		printf("Memory Allocated For 'answer_sum' freed !!!\n\n");
	}

	return(0);
}

void MathematicalOperations(int SGK_x, int SGK_y, int *SGK_sum, int *SGK_difference, int *SGK_product, int *SGK_quotient, int *SGK_remainder)
{
	//code
	*SGK_sum = SGK_x + SGK_y; // Value At Address 'sum' = (x+y)
	*SGK_difference = SGK_x - SGK_y; // Value At Address 'difference' = (x-y)
	*SGK_product = SGK_x * SGK_y; // Value At Address 'product' = (x*y)
	*SGK_quotient = SGK_x / SGK_y; // Value At Address 'quotient' = (x/y)
	*SGK_remainder = SGK_x % SGK_y; // Value At Address 'remainder' = (x%y)
}
