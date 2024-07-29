// header files
#include<stdio.h>

// function declaration

int FactorsDifference(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	// code
	printf("Enter the number : \t ");
	scanf("%d", &iValue);

	iReturn = FactorsDifference(iValue);

	printf("\t %d ", iReturn);

	return(0);
}

int FactorsDifference(int iNumber)
{
	// variable declaration
	int iDifference = 0;
	int iCount = 0;
	int iFactorSum = 0;
	int iNonFactorSum = 0;

	// code
	for (iCount = 1; iCount < iNumber; iCount++)
	{
		if ((iNumber % iCount) == 0)
		{
			iFactorSum = iFactorSum + iCount;
		}
		else
		{
			iNonFactorSum = iNonFactorSum + iCount;
		}
	}

	iDifference = iFactorSum - iNonFactorSum;

	return(iDifference);
}
