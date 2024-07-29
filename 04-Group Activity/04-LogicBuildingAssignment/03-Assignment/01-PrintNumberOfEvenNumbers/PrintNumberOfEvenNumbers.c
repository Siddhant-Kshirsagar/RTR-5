// header files
#include<stdio.h>

// function declaration
void PrintEven(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code
	printf("Enter the number \n");
	scanf("%d", &iValue);

	PrintEven(iValue);

	return(0);
}

void PrintEven(int iNumber)
{
	// variable declaration
	int i = 2;
	int iCount = 0;
	// code
	if (iNumber <= 0)
	{
		return;
	}

	for (iCount = 0; iCount < iNumber; iCount++, i = i + 2)
	{
		printf("\t%d", i);
	}
}
