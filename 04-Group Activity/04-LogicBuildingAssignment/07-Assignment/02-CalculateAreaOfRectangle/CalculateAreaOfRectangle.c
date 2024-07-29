// header files
#include<stdio.h>

// function declaration
double RectangleArea(float, float);

// entry point function
int main(void)
{
	// variable declaration
	float fWidth = 0.0f, fHeight = 0.0f;
	double dReturn = 0.0f;

	// code
	printf("Enter the width : \t");
	scanf("%f", &fWidth);

	printf("Enter the height : \t");
	scanf("%f", &fHeight);

	dReturn = RectangleArea(fWidth, fHeight);

	printf("\t Area Of Rectangle = %lf\n", dReturn);

	return(0);
}

double RectangleArea(float fWidth, float fHeight)
{
	double dAreaOfRectangle = 0.0;

	if (fWidth > 0.0 && fHeight > 0.0)
	{
		dAreaOfRectangle = fWidth * fHeight;
	}
	
	return(dAreaOfRectangle);
}
