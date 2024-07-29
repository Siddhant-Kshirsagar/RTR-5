//header files
#include<stdio.h>

// variable declaration

typedef int BOOL;

#define TRUE 1
#define FALSE 0

// function declaration
BOOL checkVowel(char);

// entry point function
int main(void)
{
	// variable declaration
	char cValue = '\0';
	BOOL bReturn = FALSE;

	printf("Enter the character \n");
	scanf("%c", &cValue);

	bReturn = checkVowel(cValue);

	if (bReturn == TRUE)
	{
		printf("It is Vowel\n");
	}
	else
	{
		printf("It is not vowel\n");
	}

	return(0);
}

BOOL checkVowel(char cValue)
{
	if (((cValue == 'a') || ( cValue == 'A')) || ((cValue == 'e') || (cValue == 'E')) || ((cValue == 'i') || (cValue == 'I')) || ((cValue == 'O') || (cValue == 'o')) || ((cValue == 'u') || (cValue == 'U')))

	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}
