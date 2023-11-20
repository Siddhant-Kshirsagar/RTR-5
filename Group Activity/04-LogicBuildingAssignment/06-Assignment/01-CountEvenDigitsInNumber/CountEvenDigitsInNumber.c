// header files
#include<stdio.h>

// function declaration
int CountEven(int);

// entry point function
int main(void)
{
	//variable declaration
	int iValue = 0;
	int iReturn = 0;

	printf("Enter the number : \t");
	scanf("%d", &iValue);

	iReturn = CountEven(iValue);

	printf("\t%d\n", iReturn);

	return(0);
}

int CountEven(int iNumber)
{
	//variable declaration
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
		if ((iRemainder % 2) == 0)
		{
			iCount++;
		}
		iNumber = iNumber / 10;
	}

	return(iCount);
}
