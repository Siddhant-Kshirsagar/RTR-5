// header files
#include<stdio.h>

// function declaration
int CountDifference(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	// code
	printf("Enter the number : \t ");
	scanf("%d", &iValue);

	iReturn = CountDifference(iValue);

	printf("\t%d\n", iReturn);

	return(0);
}

int CountDifference(int iNumber)
{
	// variable declaration
	int iRemainder = 0;
	int iEvenSum = 0;
	int iOddSum = 0;
	int iDifference = 0;

	// code
	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	while (iNumber > 0)
	{
		iRemainder = iNumber % 10;
		if ((iRemainder % 2) == 0)
		{
			iEvenSum = iEvenSum + iRemainder;
		}
		else
		{
			iOddSum = iOddSum + iRemainder;
		}
		iNumber = iNumber / 10;
	}

	iDifference = iEvenSum - iOddSum;

	return(iDifference);
}
