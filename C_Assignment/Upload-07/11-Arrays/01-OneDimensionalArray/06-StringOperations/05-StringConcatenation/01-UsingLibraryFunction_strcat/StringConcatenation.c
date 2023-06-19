#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declarations 
	char SGK_chArray_One[MAX_STRING_LENGTH], SGK_chArray_Two[MAX_STRING_LENGTH];// A Character Array Is A String 

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
	printf("******** BEFORE CONCATENATION ********");
	printf("\n\n");
	printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
	printf("%s\n",SGK_chArray_One);

	printf("\n\n");
	printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Two);

	strcat(SGK_chArray_One, SGK_chArray_Two);

	printf("\n\n");
	printf("***** After Concatenation *****");
	printf("\n\n");
	printf("'chArray_One[]' Is : \n\n");
	printf("%s\n", SGK_chArray_One);

	printf("\n\n");
	printf("'chArray_Two[]' Is : \n\n");
	printf("%s\n", SGK_chArray_Two);

	return(0);	
}
