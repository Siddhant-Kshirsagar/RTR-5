// header  files
#include<stdio.h>

// function declaration
void RangeDisplay(int, int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue1 = 0, iValue2 = 0;

	// code
	printf("\tEnter starting point : ");
	scanf("%d", &iValue1);

	printf("\tEnter the ending point : ");
	scanf("%d", &iValue2);

	RangeDisplay(iValue1, iValue2);

	return(0);
}

void RangeDisplay(int iStart, int iEnd)
{
	// variable declaration
	int iRange = 0;
	// code
	if (iStart <= iEnd)
	{
		for (iRange = iStart; iRange <= iEnd; iRange++)
		{
			printf("\t%d", iRange);
		}
	}
	else
	{
		printf("\tInvalid Range\n");
	}

}
