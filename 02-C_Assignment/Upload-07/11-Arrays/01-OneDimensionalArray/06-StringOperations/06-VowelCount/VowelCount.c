#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations
	char SGK_chArray[MAX_STRING_LENGTH]; //A Character Array Is A String 
	int SGK_iStringLength;
	int SGK_Count_A = 0, SGK_Count_E = 0, SGK_Count_I = 0, SGK_Count_O = 0, SGK_Count_U = 0;// Initial Counts = 0
	int SGK_i;

	//code 
	
	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n",SGK_chArray);

	SGK_iStringLength = MyStrlen(SGK_chArray);

	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		switch (SGK_chArray[SGK_i])
		{
		case 'A':
		case 'a':
			SGK_Count_A++;
			break;
		case 'E':
		case 'e':
			SGK_Count_E++;
			break;
		case 'I':
		case 'i':
			SGK_Count_I++;
			break;
		case 'O':
		case 'o':
			SGK_Count_O++;
			break;
		case 'U':
		case 'u':
			SGK_Count_U++;
			break;
		default:
			break;
		}
	}

	printf("\n\n");
	printf("In The String Entered By You, The Vowels And The Number Of Their Occurences Are As Follows : \n\n");
	printf("'A' Has Occured = %d Times !!!\n\n", SGK_Count_A);
	printf("'E' Has Occured = %d Times !!!\n\n", SGK_Count_E);
	printf("'I' Has Occured = %d Times !!!\n\n", SGK_Count_I);
	printf("'O' Has Occured = %d Times !!!\n\n", SGK_Count_O);
	printf("'U' Has Occured = %d Times !!!\n\n", SGK_Count_U);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//Variable Declarations 
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// ***** Determining Exact Length Of The String, By Detecting The First Occurence Of Null-Terminating Character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
