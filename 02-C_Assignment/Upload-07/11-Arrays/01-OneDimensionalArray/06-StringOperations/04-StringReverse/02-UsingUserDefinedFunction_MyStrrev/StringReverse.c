#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrrev(char[], char[]);

	//variable declarations
	char SGK_chArray_Original[MAX_STRING_LENGTH], SGK_chArray_Reversed[MAX_STRING_LENGTH]; // A Character Array Is A String 

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	//***** STRING REVERSE
	MyStrrev(SGK_chArray_Reversed, SGK_chArray_Original);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("The Original String Entered By You (i.e : 'chArray_Original[]' ) Is : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Reversed String (i.e : 'chArray_Reversed[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Reversed);

	return(0);
}

void MyStrrev(char SGK_str_destination[], char SGK_str_source[])
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations
	int SGK_iStringLength = 0;
	int SGK_i, SGK_j, SGK_len;

	//cod 
	SGK_iStringLength = MyStrlen(SGK_str_source);

	//ARRAY INDICES BEGIN FROM 0, HENCE, LAST INDEX WILL ALWAYS BE (LENGTH - 1)
	SGK_len = SGK_iStringLength-1;

	//We Need to put the character which is at last index of 'str_source' to the first Index of 'str_destination'
	//And second-last character of 'str_source' to the second character of 'str_destination' and so on...
	for (SGK_i = 0, SGK_j = SGK_len; SGK_i < SGK_iStringLength, SGK_j >= 0; SGK_i++, SGK_j--)
	{
		SGK_str_destination[SGK_i] = SGK_str_source[SGK_j];
	}
	
	SGK_str_destination[SGK_i] = '\0';
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// ***** Determining Exact Length of the string, by detecting the first occurence of  Null-Terminating character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
