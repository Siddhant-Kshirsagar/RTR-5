#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declarations 
	char SGK_chArray[MAX_STRING_LENGTH]; // A Character Array Is A String 

	//code 
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", SGK_chArray);
	return(0);
}
