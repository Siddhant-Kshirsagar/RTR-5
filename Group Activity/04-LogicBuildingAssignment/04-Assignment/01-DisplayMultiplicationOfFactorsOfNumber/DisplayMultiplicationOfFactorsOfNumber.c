// header files
#include<stdio.h>

// function declaration
int MultiplicationOfFactors(int);

// entry point functino
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	// code
	printf("Enter The Number : \t");
	scanf("%d", &iValue);

	iReturn = MultiplicationOfFactors(iValue);

	printf("\tResult = %d\n", iReturn);

	return(0);
}

int MultiplicationOfFactors(int iNumber)
{
	// variable declaration
	int iTotal = 0;
	int iCount = 0;

	// code
	for (iCount = 1; iCount < iNumber; iCount++)
	{
		if ((iNumber % iCount) == 0)
		{
			if (iTotal > 0)
			{
				iTotal = iTotal * iCount;
			}
			else
			{
				iTotal = iCount;
			}
			
		}
	}

	return(iTotal);
}

