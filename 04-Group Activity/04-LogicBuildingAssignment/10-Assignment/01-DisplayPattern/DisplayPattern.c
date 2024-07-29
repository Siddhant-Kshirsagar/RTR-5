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
	for (i = 0; i < iRow; i++) // for rows
	{
		for (j = 0; j < iColumn; j++) // for columns
		{
			printf("\t*");
		}
		printf("\n");
	}
}

