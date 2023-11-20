// header files
#include<stdio.h>

// function declaration
void RangeDisplayReverse(int, int);

// entry point function 
int main(void)
{
	//variable declaration
	int iValue1 = 0, iValue2 = 0;

	printf("Enter starting point : ");
	scanf("%d", &iValue1);

	printf("Enter ending point : ");
	scanf("%d", &iValue2);

	RangeDisplayReverse(iValue1, iValue2);

	return(0);
}


void RangeDisplayReverse(int iStart, int iEnd)
{
	// variable declaration
	int iRange = 0;

	//code
	if (iStart <= iEnd)
	{
		for (iRange = iEnd; iRange >= iStart; iRange--)
		{
			printf("\t%d", iRange);
		}
	}
	else
	{
		printf("\tInvalid Range \n");
	}
	
}
