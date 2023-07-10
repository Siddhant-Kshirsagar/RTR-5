#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char *);

	//variable declarations
	char *SGK_chArray = NULL; // character Array can be represented by  pointer to mark the base address (char *)
	int SGK_iStringLength = 0;

	//code
	printf("\n\n");
	SGK_chArray = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
	if (SGK_chArray == NULL)
	{
		printf("Memory Alocation to character  array failed !!! Exitting Now...\n\n");
		exit(0);
	}

	// *** String Input ***
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	// *** String Output ***
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", SGK_chArray);

	// *** String Length ***
	printf("\n\n");
	SGK_iStringLength = MyStrlen(SGK_chArray);
	printf("Length Of String Is = %d Characters !!! \n\n", SGK_iStringLength);

	if (SGK_chArray)
	{
		free(SGK_chArray);
		SGK_chArray = NULL;
	}

	return(0);
}

int MyStrlen(char *SGK_str)
{
	//variable declarations
	int SGK_j; 
	int SGK_string_length = 0;

	//code
	// *** Determining exact length of the string, by detecting the first occurence of NULL-Terminating Character (\0) *** 
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (*(SGK_str + SGK_j) == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
