#include<stdio.h>

enum
{
	NEGATIVE = -1,
	ZERO,
	POSITIVE
};

int main(void)
{
	//function declarations
	int Difference(int, int, int *);

	//variable declaration
	int SGK_a;
	int SGK_b;
	int SGK_answer, SGK_ret;

	//code
	printf("\n\n");
	printf("Enter Value of 'A' : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Value of 'B' : ");
	scanf("%d", &SGK_b);

	SGK_ret = Difference(SGK_a, SGK_b, &SGK_answer);

	printf("\n\n");
	printf("Difference of %d And %d = %d\n\n", SGK_a, SGK_b, SGK_answer);

	if (SGK_ret == POSITIVE)
		printf("The Difference of %d and %d is Positive !!!\n\n", SGK_a, SGK_b);

	else if (SGK_ret == NEGATIVE)
		printf("The Difference of %d and %d is Negative !!!\n\n", SGK_a, SGK_b);

	else
		printf("The Difference of %d and %d is Zero !!!\n\n", SGK_a, SGK_b);

	return(0);
}

// We want our function difference() to perform 2 jobs..
// One, is to subract the input number ('y' from 'x') and the second, is to tell whether the difference of 'x' and 'y' is positive or negative or zero ...
// But any function has only one valid return value, then how can we manage to return two values to the calling function?
// This is where parameterized return value comes into the picture...
// We can return the actual difference of 'x' and 'y' , that is, the actual answer value , via out-parameter / parameterized return value
// And We can return the status of the answer(positive / negative / zero) via the actual return value of the function ...

int Difference(int SGK_x, int SGK_y, int *SGK_diff)
{
	//code
	*SGK_diff = SGK_x - SGK_y;

	if (*SGK_diff > 0)
		return(POSITIVE);

	else if (*SGK_diff < 0)
		return(NEGATIVE);

	else
		return(ZERO);
}
