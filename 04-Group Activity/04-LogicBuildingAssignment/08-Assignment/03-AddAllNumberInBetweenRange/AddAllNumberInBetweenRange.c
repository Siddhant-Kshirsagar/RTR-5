// header files
#include<stdio.h>

// function declaration
int RangeSum(int, int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue1 = 0, iValue2 = 0, iReturn = 0;

	// code
	printf("\tEnter starting point : ");
	scanf("%d", &iValue1);

	printf("\tEnter ending point : ");
	scanf("%d", &iValue2);

	if (iValue1 > 0 && iValue2 > 0 && iValue1<= iValue2)
	{
	
		iReturn = RangeSum(iValue1, iValue2);

		printf("\tAddition is %d\n", iReturn);
	}
	else
	{
		printf("\t Invalid Range\n");
	}

	return(0);
}

int RangeSum(int iStart, int iEnd)
{
	// variable declaration
	int iSum = 0;
	int iRange = 0;

	// code
	if (iStart > 0 && iEnd > 0)
	{
		if (iStart <= iEnd)
		{
			for (iRange = iStart; iRange <= iEnd; iRange++)
			{
				iSum = iSum + iRange;
			}
		}
		else
		{
			printf("\tInvalid Range \n");
		}
	}
	else
	{
		printf("\tInvalid Range \n");
	}

	return(iSum);
}

