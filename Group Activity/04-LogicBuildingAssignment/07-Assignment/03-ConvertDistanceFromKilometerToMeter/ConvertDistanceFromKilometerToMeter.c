// header files
#include<stdio.h>

// function declaration
int KilometerToMeter(int);

// entry point function
int main(void)
{
	//variable declaration
	int iValue = 0;
	int iReturn = 0;

	printf("Enter The Distance : \t");
	scanf("%d", &iValue);

	iReturn = KilometerToMeter(iValue);

	printf("\t Kilometer = %d In Meter = %d \n", iValue, iReturn);

	return(0);
}


int KilometerToMeter(int iKilometer)
{
	// variable declaration
	int iMeter = 0;

	// code
	if (iKilometer > 0)
	{
		iMeter = iKilometer * 1000;
	}

	return(iMeter);
}
