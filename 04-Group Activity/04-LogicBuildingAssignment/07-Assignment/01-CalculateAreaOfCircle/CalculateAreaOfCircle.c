// header files
#include<stdio.h>

// function declaration
double CircleArea(float);

// entry point function
int main(void)
{
	// variable declaration
	float fValue = 0.0f;
	double dReturn = 0.0;

	// code
	printf("Enter the radius : \t");
	scanf("%f", &fValue);

	dReturn = CircleArea(fValue);

	printf("\t Area Of Circle = %0.4lf \n", dReturn);

	return(0);
}

double CircleArea(float fRadius)
{
	// variable declaration
	float PI = 3.14f;
	double dAreaOfCircle = 0.0;

	//code
	dAreaOfCircle = PI * fRadius * fRadius;

	return(dAreaOfCircle);
}
