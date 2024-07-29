// header files
#include<stdio.h>

// function declaration
void RangeDisplayEven(int, int);

// entry point function 
int main(void)
{
	// variable declaration
	int iValue1 = 0, iValue2 = 0;

	printf("\tEnter starting point : ");
	scanf("%d", &iValue1);

	printf("\tEnter ending point : ");
	scanf("%d", &iValue2);

	RangeDisplayEven(iValue1, iValue2);

	return(0);
}

void RangeDisplayEven(int iStart, int iEnd)
{
	// variable declaration
	int iRange = 0;

	//code 
	if (iStart <= iEnd)
	{
		for (iRange = iStart; iRange <= iEnd; iRange++)
		{
			if ((iRange % 2) == 0)
			{
				printf("\t%d", iRange);
			}
			
		}
	}
	else
	{
		printf("\tInvalid range\n");
	}
}