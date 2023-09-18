#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function Prototype
	void MyStrcpy(char *, char *);
	int MyStrlen(char *);

	//variable declarations
	char *SGK_chArray_Original = NULL, *SGK_chArray_Copy = NULL; // A character array is a string 
	int SGK_original_string_length;

	//code

	// *** String Input ****
	printf("\n\n");
	SGK_chArray_Original = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
	if (SGK_chArray_Original == NULL)
	{
		printf("Memory Allocation For Original String Failed !!! Exitting Now...\n\n");
		exit(0);
	}

	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	SGK_original_string_length = MyStrlen(SGK_chArray_Original);
	SGK_chArray_Copy = (char *)malloc(SGK_original_string_length * sizeof(char));
	if (SGK_chArray_Copy == NULL)
	{
		printf("Memory Allocation For Copied String Failed !!! Exitting Now...\n\n");
		exit(0);
	}

	// *** String Copy ***
	MyStrcpy(SGK_chArray_Copy, SGK_chArray_Original);

	// *** String Output ***
	printf("\n\n");
	printf("The Original String Entered By You (i.e : 'chArray_Original') Is : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Copied String (i.e : 'chArray_Copy') Is : \n\n");
	printf("%s\n", SGK_chArray_Copy);

	if (SGK_chArray_Copy)
	{
		free(SGK_chArray_Copy);
		SGK_chArray_Copy = NULL;
		printf("\n\n");
		printf("Memory Allocated For Copied Array Has Been  Successfully Freed !!!\n\n");
	}

	if (SGK_chArray_Original)
	{
		free(SGK_chArray_Original);
		SGK_chArray_Original = NULL;
		printf("\n\n");
		printf("Memory Allocated For Original String Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}

void MyStrcpy(char *SGK_str_destination, char *SGK_str_source)
{
	//function prototype
	int MyStrlen(char *);

	//variable declartaions 
	int SGK_iStringLength = 0;
	int SGK_j;

	//code
	SGK_iStringLength = MyStrlen(SGK_str_source);
	for (SGK_j = 0; SGK_j < SGK_iStringLength; SGK_j++)
	{
		*(SGK_str_destination + SGK_j) = *(SGK_str_source + SGK_j);
	}

	*(SGK_str_destination + SGK_j) = '\0';
}

int MyStrlen(char *SGK_str)
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code
	// *** Determining Exact Length Of The String, By Detecting The First Occurence of NULL-Terminating Character ('\0') ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}

