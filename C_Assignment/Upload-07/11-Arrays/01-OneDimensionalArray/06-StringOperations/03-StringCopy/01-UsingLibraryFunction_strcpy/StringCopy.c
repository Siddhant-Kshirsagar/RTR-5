#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declarations
	char SGK_chArray_Original[MAX_STRING_LENGTH], SGK_chArray_Copy[MAX_STRING_LENGTH];// A Character Array Is A String 

	//code 

	// ***** String Input *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	// ***** STRING COPY *****
	strcpy(SGK_chArray_Copy, SGK_chArray_Original);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Copy);

	return(0);
}
