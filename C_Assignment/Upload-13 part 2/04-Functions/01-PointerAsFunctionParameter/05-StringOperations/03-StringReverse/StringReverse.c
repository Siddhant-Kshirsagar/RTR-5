#include<stdio.h>
#include<stdlib.h>
#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrrev(char *, char *);
	int MyStrlen(char *);

	//variable declarations
	char *SGK_chArray_Original = NULL, *SGK_chArray_Reversed = NULL; // A Character Array Is A String
	int SGK_original_string_length;

	//code

	// *** String Input ***
	printf("\n\n");
	SGK_chArray_Original = (char*)malloc(MAX_STRING_LENGTH * sizeof(char));
	if (SGK_chArray_Original == NULL)
	{
		printf("Memory Allocation For Original String Failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	// *** String Reverse ***
	SGK_original_string_length = MyStrlen(SGK_chArray_Original);
	SGK_chArray_Reversed = (char*)malloc(SGK_original_string_length * sizeof(char));
	if (SGK_chArray_Reversed == NULL)
	{
		printf("Memory Allocation For Reversed String Failed !!! Exitting Now ...\n\n");
		exit(0);
	}

	MyStrrev(SGK_chArray_Reversed, SGK_chArray_Original);

	// *** String Output ***
	printf("\n\n");
	printf("The Original String Enter By You (i.e : 'chArray_Original[]' Is : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Reversed String (i.e : 'chArray_Reversed[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Reversed);

	if (SGK_chArray_Reversed)
	{
		free(SGK_chArray_Reversed);
		SGK_chArray_Reversed = NULL;
		printf("\n\n");
		printf("Memory Allocated To Reversed String Has Been Successfully Freed !!!\n\n");
	}
	
	if (SGK_chArray_Original)
	{
		free(SGK_chArray_Original);
		SGK_chArray_Original = NULL;
		printf("\n\n");
		printf("Memory Allocated To Original String Has Been Successfully Freed !!!\n\n");
	}

	return(0);
}

void MyStrrev(char *SGK_str_destination, char *SGK_str_source)
{
	//variable declarations
	int SGK_iStringLength = 0;
	int SGK_i, SGK_j, SGK_len;

	//code
	SGK_iStringLength = MyStrlen(SGK_str_source);

	//Array Indices Begin From 0, Hence Last Index Will Always Be (Length - 1)
	SGK_len = SGK_iStringLength - 1;

	// We Need To Put The Character Which Is At Last Index Of 'str_source' To The First Index of 'str_destination'
	// And Second-Last Character of 'str_source' To The Second Character Of 'str_destination' and so on...
	for (SGK_i = 0, SGK_j = SGK_len; SGK_i < SGK_iStringLength, SGK_j >= 0; SGK_i++, SGK_j--)
	{
		*(SGK_str_destination + SGK_i) = *(SGK_str_source + SGK_j);
	}

	*(SGK_str_destination + SGK_i) = '\0';
}

int MyStrlen(char *SGK_str)
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code
	// *** Determining Exact Length Of The String, By Detecting The First Occurence Of Null-Terminating Character ('\0') ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}

