// header files
#include<stdio.h>

// function declaration
void DisplayDigit(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code
	printf("Enter the number : \t");
	scanf("%d",&iValue);

	DisplayDigit(iValue);

	return(0);
}

void DisplayDigit(int iNumber)
{
	// variable declaration
	int iDigit = 0;

	// code
	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	while (iNumber > 0)
	{
		iDigit = iNumber % 10;
		printf("\t%d\n", iDigit);

		iNumber = iNumber / 10;
	}
}