// header files
#include<stdio.h>

// function declaration
void DisplayFactor(int);

// entry point function
int main(void)
{
	// variabler declaration
	int iValue = 0;

	// code
	printf("Enter the number \n");
	scanf("%d", &iValue);

	DisplayFactor(iValue);

	return(0);
}

void DisplayFactor(int iNumber)
{
	// variable declaration
	int i = 0;

	// code
	if (iNumber <= 0)
	{
		return;
	}

	for (i = 1; i <= iNumber; i++)
	{
		if (((iNumber % i)  == 0) && ((i%2) == 0))
		{
			printf("\t%d", i);
		}
	}
}
