// header files
#include<stdio.h>

// function declaration
int CountRange(int);

// entry point function 
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iReturn = 0;

	// code
	printf("Enter the number : \t");
	scanf("%d", &iValue);

	iReturn = CountRange(iValue);

	printf("\t%d\n", iReturn);

	return(0);
}

int CountRange(int iNumber)
{
	// variable declaration
	int iCount = 0;
	int iRemainder = 0;

	//code
	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	while (iNumber > 0)
	{
		iRemainder = iNumber % 10;
		if (iRemainder > 3 && iRemainder < 7)
		{
			iCount++;
		}
		iNumber = iNumber / 10;
	}

	return(iCount);
}

