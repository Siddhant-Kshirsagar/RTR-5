#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrcpy(char[], char[]);

	//variable declaraions
	char SGK_chArray_Original[MAX_STRING_LENGTH], SGK_chArray_Copy[MAX_STRING_LENGTH];// A Character Array Is A String

	//code 

	//***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	// ***** STRING COPY *****
	MyStrcpy(SGK_chArray_Copy, SGK_chArray_Original);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("The Original String Entered By You (i.e : 'chArray_Original[]') IS : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Copy);

	return(0);
}

void MyStrcpy(char SGK_str_destination[], char SGK_str_source[])
{
	//function prototype 
	int MyStrlen(char[]);

	//variable declarations
	int SGK_iStringLength = 0;
	int SGK_j;

	//code 
	SGK_iStringLength = MyStrlen(SGK_str_source);

	for (SGK_j = 0; SGK_j < SGK_iStringLength; SGK_j++)
		SGK_str_destination[SGK_j] = SGK_str_source[SGK_j];

	SGK_str_destination[SGK_j] = '\0';
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// *****DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}

	return(SGK_string_length);
}
