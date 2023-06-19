#include<stdio.h>
#include<string.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declarations
	char SGK_chArray_Original[MAX_STRING_LENGTH]; // A Character array Is A String 

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray_Original, MAX_STRING_LENGTH);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
	printf("%s\n", SGK_chArray_Original);

	printf("\n\n");
	printf("The Reversed String (i.e : 'SGK_chArray_Reverse[]) Is : \n\n");
	printf("%s\n", strrev(SGK_chArray_Original));

	return(0);
}
