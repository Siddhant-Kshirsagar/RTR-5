// header files
#include<stdio.h>

// function declaration
void NonFactor(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	printf("Enter the number \n");
	scanf("%d", &iValue);

	NonFactor(iValue);

	return(0);
}

void NonFactor(int iNumber)
{
	// variable declaration
	int iCount = 0;
	
	// code
	for (iCount = 1; iCount < iNumber; iCount++)
	{
		if ((iNumber % iCount) != 0)
		{
			printf("\t %d", iCount);
		}
	}
}
 