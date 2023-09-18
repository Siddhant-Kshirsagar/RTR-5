#include<stdio.h>
#include<ctype.h> 

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	
	//variable declarations
	char SGK_chArray[MAX_STRING_LENGTH], SGK_chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH];// A Character Array Is A String
	int SGK_iStringLength;
	int SGK_i, SGK_j;

	//code 

	//***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	SGK_iStringLength = MyStrlen(SGK_chArray);
	SGK_j = 0;
	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		if (SGK_i == 0)
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = toupper(SGK_chArray[SGK_i]);

		else if (SGK_chArray[SGK_i] == ' ')
		{
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = SGK_chArray[SGK_i];
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j + 1] = toupper(SGK_chArray[SGK_i + 1]);

			//SINCE, Already Two Characters (At Indices 'i' And i+1 Have Been Considered In This else-if Block... We Are Extra-Incrementing 'i' And 'j' By 1
			SGK_j++;
			SGK_i++;
		}

		else
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = SGK_chArray[SGK_i];

		SGK_j++;
	}

	SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = '\0';

	// ***** String output *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n",SGK_chArray);

	printf("\n\n");
	printf("String After Capitalizing First Letter Of Every Word : \n\n");
	printf("%s\n",SGK_chArray_CapitalizedFirstLetterOfEveryWord);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations 
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
