// header files
#include<stdio.h>

// function declaration
int CountTwo(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	printf("Enter the number : \t");
	scanf("%d", &iValue);

	iReturn = CountTwo(iValue);

	printf("\tFrequency Of 2's = %d\n", iReturn);

	return(0);
}

int CountTwo(int iNumber)
{
	// variable declaration
	int iCount = 0;
	int iRemainder = 0;

	// code
	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	while (iNumber > 0)
	{
		iRemainder = iNumber % 10;
		if (iRemainder == 2)
		{
			iCount++;
		}
		iNumber = iNumber / 10;
	}

	return(iCount);
}
