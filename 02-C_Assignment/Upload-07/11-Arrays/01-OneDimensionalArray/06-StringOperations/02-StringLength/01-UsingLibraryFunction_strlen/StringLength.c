#include<stdio.h>

#define MAX_STRING_LENGTH 512 

int main(void)
{
	//variable declarations
	char SGK_chArray[MAX_STRING_LENGTH]; // A Character Array Is A String 
	int SGK_iStringLength = 0;

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	// ***** STRING OUTPUT ***** 
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", SGK_chArray);

	// ***** STRING LENGTH *****
	printf("\n\n");
	SGK_iStringLength = strlen(SGK_chArray);
	printf("Length Of String Is = %d Characters !!! \n\n", SGK_iStringLength);

	return(0);
}
