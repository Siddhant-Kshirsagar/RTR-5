#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrcat(char *, char *);
	int MyStrlen(char *);

	//variable declarations
	char *SGK_chArray_One = NULL, *SGK_chArray_Two = NULL; // A Character Array Is A String 

	//code

	// *** String Input ***
	printf("\n\n");
	SGK_chArray_One = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
	if (SGK_chArray_One == NULL)
	{
		printf("Memory Allocation To First String Failed !!! Exitting Now...\n\n");
		exit(0);
	}

	printf("Enter First String : \n\n");
	gets_s(SGK_chArray_One, MAX_STRING_LENGTH);

	printf("\n\n");
	SGK_chArray_Two = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
	if (SGK_chArray_Two == NULL)
	{
		printf("Memory Allocation To Second String Failed !!! Exitting Now...\n\n");
		exit(0);
	}

	printf("Enter Second String : \n\n");
	gets_s(SGK_chArray_Two, MAX_STRING_LENGTH);

	// *** String Concat ***
	printf("\n\n");
	printf(" ***** Before Concatenation *****");
	printf("\n\n");
	printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
	printf("%s\n", SGK_chArray_One);

	printf("\n\n");
	printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Two);

	MyStrcat(SGK_chArray_One, SGK_chArray_Two);

	printf("\n\n");
	printf(" ***** After Concatenation *****");
	printf("\n\n");
	printf(" 'chArray_One[]' Is : \n\n");
	printf(" %s\n", SGK_chArray_One);

	printf("\n\n");
	printf(" 'chArray_Two[]' Is : \n\n");
	printf(" %s\n", SGK_chArray_Two);

	if (SGK_chArray_Two)
	{
		free(SGK_chArray_Two);
		SGK_chArray_Two = NULL;
		printf("\n\n");
		printf("Memory Allocated To Second String Has Been Successfully Freed !!!\n\n");
	}

	if (SGK_chArray_One)
	{
		free(SGK_chArray_One);
		SGK_chArray_One = NULL;
		printf("\n\n");
		printf("Memory Allocated To First String Has Been SuccessFully Freed !!!\n\n");
	}

	return(0);
}

void MyStrcat(char *SGK_str_destination, char *SGK_str_source)
{
	//variable declarations
	int SGK_iStringLength_Source = 0, SGK_iStringLength_Destination = 0;
	int SGK_i, SGK_j;

	//code
	SGK_iStringLength_Source = MyStrlen(SGK_str_source);
	SGK_iStringLength_Destination = MyStrlen(SGK_str_destination);

	// Array Indices Begin From 0, Hence Last Valid Index Of Array Will Always Be (Length -1)
	// So, Concatenation Must Begin From Index Number Equal To Length Of The Array 'str_destination'
	// We Need To Put The Character Which Is At First Index of 'str_source' To The (Last Index + 1) Of 'str_destination'
	for (SGK_i = SGK_iStringLength_Destination, SGK_j = 0; SGK_j < SGK_iStringLength_Source; SGK_i++, SGK_j++)
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
	// *** Determining Exact Length Of The String, By Detecting The First Occurence Of NULL-Terminating Character ('\0') ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
