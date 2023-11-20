// header files
#include<stdio.h>

// function declaration
void Pattern(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code
	printf("Enter number of elements : ");
	scanf("%d", &iValue);

	Pattern(iValue);

	return(0);
}

void Pattern(int iNumber)
{
	// variable declartion
	int iRange = 0;
	int iTemp = 2;
	// code
	for (iRange = 0; iRange < iNumber; iRange++)
	{
		printf("\t%d", iTemp);
		iTemp = iTemp + 2;
	}
}
