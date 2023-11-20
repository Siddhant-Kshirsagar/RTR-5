// header files
#include<stdio.h>

// function declaration
int RangeSumEven(int, int);

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

	if (iValue1 > 0 && iValue2 > 0 && iValue1 <= iValue2)
	{
		iReturn = RangeSumEven(iValue1, iValue2);

		printf("\tAddition is %d \n", iReturn);
	}
	else
	{
		printf("\tInvalid Range\n");
	}
	

	return(0);
}

int RangeSumEven(int iStart, int iEnd)
{
	// variable declaration
	int iSum = 0;
	int iRange = 0;

	// code
	for (iRange = iStart; iRange <= iEnd; iRange++)
	{
		if ((iRange % 2) == 0)
		{
			iSum = iSum + iRange;
		}
	}

	return(iSum);
}
