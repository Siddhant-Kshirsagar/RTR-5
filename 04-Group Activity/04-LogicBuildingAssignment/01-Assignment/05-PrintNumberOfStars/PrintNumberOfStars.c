// header files
#include<stdio.h>

// function declaration
void Accept(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code

	iValue = 5;
	Accept(iValue);

	return(0);
}

void Accept(int iNumber)
{
	// variable declaration
	int iCount = 0;

	// code
	for (iCount = 0; iCount < iNumber; iCount++)
	{
		printf("\t*");
	}
}
