// header files
#include<stdio.h>

// function declaration
void Display(int, int);

// entry point function
int main(void)
{
	// variable declaration
	int iValue = 0;
	int iCount = 0;

	printf("Enter the number \n");
	scanf("%d", &iValue);

	printf("Enter the frequency\n");
	scanf("%d", &iCount);

	Display(iValue, iCount);

	return(0);
}

void Display(int iNumber, int iFrequency)
{
	// variable declaration
	int i = 0;
	if (iFrequency < 0)
	{
		for (i = 0; i > iFrequency; i--)
		{
			printf("\t%d", iNumber);
		}
	}
	else
	{
		for (i = 0; i < iFrequency; i++)
		{
			printf("\t%d", iNumber);
		}
	}
	
}
