// header files
#include<stdio.h>

// function declaration
void Pattern(int, int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue1 = 0, iValue2 = 0;

	// code
	printf("Enter number of rows and columns : ");
	scanf("%d %d", &iValue1, &iValue2);

	Pattern(iValue1, iValue2);

	return(0);
}

void Pattern(int iRow, int iColumn)
{
	// variable declaration
	int i, j;

	// code
	for (i = 1; i <= iRow; i++)
	{
		for (j = iColumn; j > 0; j--)
		{
			printf("\t%d", j);
		}
		printf("\n");
	}
}
