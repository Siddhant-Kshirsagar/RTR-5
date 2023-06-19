#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrcat(char[], char[]);

	//variable declarations
	char SGK_chArray_One[MAX_STRING_LENGTH], SGK_chArray_Two[MAX_STRING_LENGTH]; // A Character Array Is A String

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter First String : \n\n");
	gets_s(SGK_chArray_One, MAX_STRING_LENGTH);

	printf("\n\n");
	printf("Enter Second String : \n\n");
	gets_s(SGK_chArray_Two, MAX_STRING_LENGTH);

	// ***** STRING CONCAT *****
	printf("\n\n");
	printf("***** Before Concatenation *****");
	printf("\n\n");
	printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is \n\n");
	printf("%s\n",SGK_chArray_One);

	printf("\n\n");
	printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Two);

	MyStrcat(SGK_chArray_One, SGK_chArray_Two);

	printf("\n\n");
	printf("***** After Concatenation *****");
	printf("\n\n");
	printf("'chArray_One[]' Is : \n\n");
	printf("%s\n", SGK_chArray_One);

	printf("\n\n");
	printf("'chArrayTwo[]' Is : \n\n");
	printf("%s\n", SGK_chArray_Two);

	return(0);
}

void MyStrcat(char SGK_str_destination[], char SGK_str_source[])
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations
	int SGK_iStringLength_Source = 0, SGK_iStringLength_Destination = 0;
	int SGK_i, SGK_j;

	//code 
	SGK_iStringLength_Source = MyStrlen(SGK_str_source);
	SGK_iStringLength_Destination = MyStrlen(SGK_str_destination);

	//Array Indices Begin From 0, Hence, Last Valid Index Of Array Will Always Be (Length - 1)
	//SO, Concatenation Must Begin From Index Number Equal To Length Of The Array 'str_destination'
	//We need to put the character which is at first index of 'str_source' to the (last Index + 1) of 'str_destination'

	for (SGK_i = SGK_iStringLength_Destination, SGK_j = 0; SGK_j < SGK_iStringLength_Source; SGK_i++, SGK_j++)
	{
		SGK_str_destination[SGK_i] = SGK_str_source[SGK_j];
	}

	SGK_str_destination[SGK_i] = '\0';
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_iStringLength = 0;
	
	//code 
	// ***** Determining Exact Length Of the String, By Detecting The First Occurence Of Null-Terminating Character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_iStringLength++;
	}
	return(SGK_iStringLength);
}
