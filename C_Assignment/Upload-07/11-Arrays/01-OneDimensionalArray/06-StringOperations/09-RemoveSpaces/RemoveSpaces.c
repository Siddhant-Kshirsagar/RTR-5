#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	void MyStrcpy(char[], char[]);

	//variable declarations
	char SGK_chArray[MAX_STRING_LENGTH], SGK_chArray_SpacesRemoved[MAX_STRING_LENGTH];// A Character Array Is A String
	int SGK_iStringLength;
	int SGK_i, SGK_j;

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	SGK_iStringLength = MyStrlen(SGK_chArray);
	SGK_j = 0;
	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		if (SGK_chArray[SGK_i] == ' ')
			continue;
		else
		{
			SGK_chArray_SpacesRemoved[SGK_j] = SGK_chArray[SGK_i];
			SGK_j++;
		}
	}

	SGK_chArray_SpacesRemoved[SGK_j] = '\0';

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n",SGK_chArray);

	printf("\n\n");
	printf("String After Removal Of Spaces Is : \n\n");
	printf("%s\n",SGK_chArray_SpacesRemoved);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// ***** Determining exact length of the string, by detecting the first Occurence of null-terminating character (\0) *****

	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
