// header files 
#include<stdio.h>

// function declaration
int Divide(int, int);

// entry point function
int main(void)
{
	//variable declaration
	int iValue1 = 15, iValue2 = 5;
	int iReturn = 0;

	// code

	iReturn = Divide(iValue1, iValue2);

	printf("Division is %d \n", iReturn);

	return(0);
}

int Divide(int iNumber1, int iNumber2)
{
	// variable declaration
	int iAnswer = 0;
	
	// code
	if (iNumber2 == 0)
	{
		return(-1);
	}

	iAnswer = iNumber1 / iNumber2;

	return(iAnswer);
}

