// header files
#include<stdio.h>

// function declaration
void DisplayConvert(char);

// entry point function
int main(void)
{
	// variable declaration
	char cValue = '\0';

	// code
	printf("Enter the character \n");
	scanf("%c", &cValue);

	DisplayConvert(cValue);

	return(0);
}

void DisplayConvert(char cValue)
{
	// ASCII value of capital letter A-Z is from 65 to 90
	if (cValue >= 65 && cValue <= 90)
	{
		printf("\t%c", cValue += 32);
	}
	else if (cValue >= 97 && cValue <= 122)// ASCII value of small letter a-z is from 97 to 120
	{
		printf("\t%c", cValue -= 32);
	}
}
