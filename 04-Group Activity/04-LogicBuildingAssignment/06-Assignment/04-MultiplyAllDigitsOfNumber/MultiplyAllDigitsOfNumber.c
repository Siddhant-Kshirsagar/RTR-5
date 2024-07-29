// header files
#include<stdio.h>

// function declaration
int MultiplyDigits(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	// code
	printf("Enter the number : \t ");
	scanf("%d", &iValue);

	iReturn = MultiplyDigits(iValue);

	printf("\t%d\n", iReturn);

	return(0);
}

int MultiplyDigits(int iNumber)
{
	//variable declaration
	int iResult = 1;
	int iRemainder = 0;

	// code
	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	while (iNumber > 0)
	{
		iRemainder = iNumber % 10;
		if (iRemainder > 0)
		{
			iResult = iResult * iRemainder;
		}

		iNumber = iNumber / 10;
	}

	return(iResult);
}

