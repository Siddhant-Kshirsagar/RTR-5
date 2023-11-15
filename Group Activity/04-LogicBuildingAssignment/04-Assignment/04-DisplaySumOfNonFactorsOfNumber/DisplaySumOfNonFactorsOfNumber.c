// header files
#include<stdio.h>

// function declaration
int SumNonFactors(int);

// entry point function 
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	printf("Enter the number : \t");
	scanf("%d", &iValue);

	iReturn = SumNonFactors(iValue);

	printf("\t %d \n", iReturn);

	return(0);
}

int SumNonFactors(int iNumber)
{
	// variable declaration
	int iCount = 0;
	int iTotal = 0;

	// code
	for (iCount = 1; iCount < iNumber; iCount++)
	{
		if ((iNumber % iCount) != 0)
		{
			iTotal = iTotal + iCount;
		}
	}

	return(iTotal);
}
