// header files
#include<stdio.h>

// function declaration
void FactorialReverse(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code
	printf("Enter the number : \t");
	scanf("%d", &iValue);

	FactorialReverse(iValue);

	return(0);
}

void FactorialReverse(int iNumber)
{
	// variable declaration
	int iCount = 0;

	// code
	for (iCount = iNumber - 1; iCount > 0; iCount--)
	{
		if ((iNumber % iCount) == 0)
		{
			printf("\t %d", iCount);
		}
	}
}