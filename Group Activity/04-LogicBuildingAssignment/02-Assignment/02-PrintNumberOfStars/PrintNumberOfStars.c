// Header Files
#include<stdio.h>

//function declaration
void Display(int);

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
	while (iNumber > 0)
	{
		printf("\t*");
		iNumber--;
	}
}
