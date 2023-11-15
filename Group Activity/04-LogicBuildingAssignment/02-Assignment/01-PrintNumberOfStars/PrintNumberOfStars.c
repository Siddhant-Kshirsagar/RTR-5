//header files
#include<stdio.h>

// function declaration
void Display(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	printf("Enter The Number \n");
	scanf("%d", &iValue);

	Display(iValue);

	return(0);
}

void Display(int iNumber)
{
	int iCount = 0;
	for (iCount = 0; iCount < iNumber; iCount++)
	{
		printf("\t*");
	}
}
