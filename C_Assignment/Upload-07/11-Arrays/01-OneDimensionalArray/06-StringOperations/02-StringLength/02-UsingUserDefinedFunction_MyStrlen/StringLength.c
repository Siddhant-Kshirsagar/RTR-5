#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations 
	char SGK_chArray[MAX_STRING_LENGTH]; // A character Array Is A String
	int SGK_iStringLength = 0;

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s \n", SGK_chArray);

	//*****String Length *****
	printf("\n\n");
	SGK_iStringLength = MyStrlen(SGK_chArray);
	printf("Length Of String Is = %d Characters !!!\n\n", SGK_iStringLength);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	//***** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
