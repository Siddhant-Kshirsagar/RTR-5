// ***** This Program Replace All Vowels In The Input String With The * (asterisk) Symbol *****
// ***** For Example, Original String 'Hello World !!! I am Learning C' Will Become 'H*ll* W*rld !!! * *m L**rn*ng C'

#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	void MyStrcpy(char[], char[]);

	//variable declarations
	char SGK_chArray_Original[MAX_STRING_LENGTH], SGK_chArray_VowelsReplaced[MAX_STRING_LENGTH];// A Character Array Is A String 
	int SGK_iStringLength;
	int SGK_i;

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	// ***** STRING OUTPUT *****
	MyStrcpy(SGK_chArray_VowelsReplaced, SGK_chArray_Original);

	SGK_iStringLength = MyStrlen(SGK_chArray_VowelsReplaced);
	
	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		switch (SGK_chArray_VowelsReplaced[SGK_i])
		{
		case 'A':
		case 'a':
		case 'E':
		case 'e':
		case 'I':
		case 'i':
		case 'O':
		case 'o':
		case 'U':
		case 'u':
			SGK_chArray_VowelsReplaced[SGK_i] = '*';
			break;
		default:
			break;
		}
	}
	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By Is : \n\n");
	printf("%s\n",SGK_chArray_Original);

	printf("\n\n");
	printf("String After Replacement Of Vowels By * Is : \n\n");
	printf("%s\n",SGK_chArray_VowelsReplaced);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_String_length = 0;

	//code 
	// ***** Determining Exact Length Of the String, By Detecting The First Occurence Of Null-Terminating Character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_String_length++;
	}
	return(SGK_String_length);
}

void MyStrcpy(char SGK_str_destination[], char SGK_str_source[])
{
	//Function Prototype
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
