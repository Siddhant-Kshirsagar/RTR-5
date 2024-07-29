// header files
#include<Stdio.h>

// function declaration
void Pattern(int);

// entry point function 
int main(void)
{
	//variable declaration 
	int iValue = 0;

	// code
	printf("Enter number of elements : ");
	scanf("%d", &iValue);

	Pattern(iValue);

	return(0);
}

void Pattern(int iNumber)
{
	// variable declaration
	int iRange = 0;

	//code
	for (iRange = iNumber; iRange > 0; iRange--)
	{
		printf("\t%d\t#",iRange);
	}
}
