// header files
#include<stdio.h>

// function declaration
void Display(int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;

	// code
	printf("Enter The Number\n");
	scanf("%d", &iValue);

	Display(iValue);

	return(0);
}

void Display(int iNumber)
{
	if (iNumber < 10)
	{
		printf("\tHello\n");
	}
	else
	{
		printf("\tDemo\n");
	}
}
