#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	void MyStrcpy(char[], char[]);

	//variable declarations
	char SGK_chArray[MAX_STRING_LENGTH]; // A Character Array Is A String
	int SGK_iStringLength;
	int SGK_i;
	int SGK_word_count = 0, SGK_space_count = 0;

	//code 
	
	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	SGK_iStringLength = MyStrlen(SGK_chArray);

	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		switch (SGK_chArray[SGK_i])
		{
		case 32: //32 IS THE ASCII VALUE FOR SPACE (' ') CHARACTER
			SGK_space_count++;
			break;
		default:
			break;
		}
	}

	SGK_word_count = SGK_space_count + 1;

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", SGK_chArray);

	printf("\n\n");
	printf("Number Of Spaces In The Input String = %d \n\n",SGK_space_count);
	printf("Number Of Words In The Input String = %d \n\n", SGK_word_count);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	//***** Determining Exact Length Of The String, By Detecting The First Occurence Of Null - Terminating Character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
