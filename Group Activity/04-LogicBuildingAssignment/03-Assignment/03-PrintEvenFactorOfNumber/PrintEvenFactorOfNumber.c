// header files
#include<stdio.h>

// function declaration
void DisplayEvenFactor(int);

int main(void)
{
	int iValue = 0;
	
	printf("Enter the number \n");
	scanf("%d", &iValue);

	DisplayEvenFactor(iValue);

	return(0);
}

void DisplayEvenFactor(int iNumber)
{
	// variable declaration
	int i = 0;

	if (iNumber <= 0)
	{
		iNumber = -iNumber;
	}

	for (i = 1; i <= iNumber; i++)
	{
		if (((iNumber % i) == 0) && ((i % 2) == 0))
		{
			printf("\t%d", i);
		}
	}
}

