// header files
#include<stdio.h>

// variable declaration
#define TRUE 1
#define FALSE 0

typedef int BOOL;

// function declaration
BOOL checkEven(int);

// entry point function
int main(void)
{
	//variable declaration
	int iValue = 0;
	BOOL bReturn = FALSE;

	printf("Enter the number \n");
	scanf("%d", &iValue);

	bReturn = checkEven(iValue);

	if (bReturn == TRUE)
	{
		printf("\t \"%d\"  is an even number \n", iValue);
	}
	else
	{
		printf("\t \"%d\" is an odd number \n", iValue);
	}

	return(0);
}

BOOL checkEven(int iNumber)
{
	if (iNumber % 2 == 0)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}


