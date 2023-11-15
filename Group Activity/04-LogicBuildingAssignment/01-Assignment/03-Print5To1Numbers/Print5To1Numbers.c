// header files
#include<stdio.h>

// function declartation
void Display(void);

// entry point function
int main(void)
{
	// code
	Display();

	return(0);
}

void Display(void)
{
	// variable declaration
	int i = 0;

	int j = i = 5;

	for (j = 5; j >= 1; j--, i =((j * i) / i))
	{
		printf("\t%d", i);
		i++;
	}
}
