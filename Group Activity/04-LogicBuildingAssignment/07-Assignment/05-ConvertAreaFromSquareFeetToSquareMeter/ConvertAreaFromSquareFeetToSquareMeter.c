// header files
#include<stdio.h>

// function declaration
double SquareMeter(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	double dReturn = 0.0;

	// code
	printf("Enter area in square feet\n");
	scanf("%d", &iValue);

	dReturn = SquareMeter(iValue);

	printf("\t Area in square feet = %d \t Area in square meter = %0.6lf\n", iValue, dReturn);

	return(0);
}

double SquareMeter(int iValue)
{
	// variable declaration
	double dSquareMeter = 0.0;

	// code
	dSquareMeter = (((double)iValue) * 0.092903);

	return(dSquareMeter);
}
