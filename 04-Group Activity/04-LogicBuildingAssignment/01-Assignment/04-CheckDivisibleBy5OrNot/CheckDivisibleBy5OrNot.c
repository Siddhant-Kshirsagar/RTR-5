// header file
#include<stdio.h>

// variable declaration
typedef int BOOL;

#define TRUE 1
#define FALSE 0

// function declaration
BOOL Check(int);

// entry point function
int main(void)
{
	int iValue = 0;
	BOOL bReturn = FALSE;

	printf("Enter The Number \n");
	scanf("%d", &iValue);

	bReturn = Check(iValue);

	if (bReturn == TRUE)
	{
		printf("Divisible by 5\n");
	}
	else
	{
		printf("Not Divisible by 5\n");
	}

	return(0);
}

BOOL Check(int iNumber)
{
	if ((iNumber % 5) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
