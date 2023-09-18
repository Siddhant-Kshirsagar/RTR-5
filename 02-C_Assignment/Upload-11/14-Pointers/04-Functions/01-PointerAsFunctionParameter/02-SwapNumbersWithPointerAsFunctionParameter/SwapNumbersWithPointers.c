#include<stdio.h>

int main(void)
{
	//function declarations
	void SwapNumbers(int *, int *);

	//variable declaration
	int SGK_a;
	int SGK_b;

	//code
	printf("\n\n");
	printf("Enter Value For 'A' : ");
	scanf("%d", &SGK_a);
	
	printf("\n\n");
	printf("Enter Value For 'B' : ");
	scanf("%d", &SGK_b);

	printf("\n\n");
	printf(" ***** BEFORE SWAPPING *****\n\n");
	printf("Value Of 'A' = %d\n\n", SGK_a);
	printf("Value Of 'B' = %d\n\n", SGK_b);

	SwapNumbers(&SGK_a, &SGK_b); // ***** Arguments Passed 'By Reference / Address' ... *****

	printf("\n\n");
	printf(" ***** AFTER SWAPPING *****\n\n");
	printf("Value Of 'A' = %d\n\n", SGK_a);
	printf("Value Of 'B' = %d\n\n", SGK_b);

	return(0);
}

// Address of 'a' is copied into 'x' and address of 'b' is copied into 'y' ... so, '&a' and 'x' are pointing to One And The Same Address and '&b' and 'y' are pointing to One And the Same Address...
// Swapping Takes Place between 'value at address of x' (value at &a i.e : 'a') and 'value at address of  y' (value at &b i.e : 'b') ...
// Hence, Swapping int this case takes place between '*x' and '*y' As Well As Between 'a' and 'b' ...

void SwapNumbers(int *SGK_x, int *SGK_y)
{
	//variable declarations
	int SGK_temp;

	//code
	printf("\n\n");
	printf(" ***** BEFORE SWAPPING *****\n\n");
	printf("Value Of 'x' = %d\n\n", *SGK_x);
	printf("Value Of 'y' = %d\n\n", *SGK_y);

	SGK_temp = *SGK_x;
	*SGK_x = *SGK_y;
	*SGK_y = SGK_temp;

	printf("\n\n");
	printf(" ***** AFTER SWAPPING *****\n\n");
	printf("Value Of 'X' = %d\n\n", *SGK_x);
	printf("Value Of 'Y' = %d\n\n", *SGK_y);
}
