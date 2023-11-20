// header files
#include<stdio.h>

//constant declaration
#define TRUE 1
#define FALSE 0

typedef int BOOL;

// function declaration
BOOL CheckZero(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	BOOL bReturn = FALSE;

	// code
	printf("Enter the number : \t");
	scanf("%d", &iValue);

	bReturn = CheckZero(iValue);

	if (bReturn == TRUE)
	{
		printf("\t It Contains Zero\n");
	}
	else
	{
		printf("\t There is no Zero\n");
	}

	return(0);
}

BOOL CheckZero(int iNumber)
{
	// variable declaration
	int iRemainder = 0;

	// code
	while (iNumber > 0)
	{
		iRemainder = iNumber % 10;
		if (iRemainder == 0)
		{
			return(TRUE);
		}
		iNumber = iNumber / 10;
	}

	return(FALSE);
}

