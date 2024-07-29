// header files
#include<stdio.h>

// function declaration
double FahrenheitToCelsius(float);

// entry point function
int main(void)
{
	// variable declaration
	float fValue = 0.0f;
	double dReturn = 0.0f;

	// code
	printf("Enter the temperature in Fahrenheit : \t");
	scanf("%f", &fValue);

	dReturn = FahrenheitToCelsius(fValue);

	printf("\t Temperature in Fahrenheit = %f \t In Celsius = %0.4lf\n", fValue, dReturn);

	return(0);
}

double FahrenheitToCelsius(float fTemperature)
{
	// variable declaration
	double dCelsius = 0.0;

	dCelsius = ((fTemperature - 32.0f) * (5.0f / 9.0f));
	return(dCelsius);
}
