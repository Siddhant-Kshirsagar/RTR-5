#include<stdio.h>

#define MAX_STRING_LENGTH 512

#define SPACE ' '

#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'

int main(void)
{
	//function prototype
	int MyStrlen(char[]);
	char MyToUpper(char);

	//variable declarations 
	char SGK_chArray[MAX_STRING_LENGTH], SGK_chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH]; // A character Array Is A String 
	int SGK_iStringLength;
	int SGK_i, SGK_j;

	//code 

	// ***** STRING INPUT *****
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_chArray, MAX_STRING_LENGTH);

	SGK_iStringLength = MyStrlen(SGK_chArray);
	SGK_j = 0;
	for (SGK_i = 0; SGK_i < SGK_iStringLength; SGK_i++)
	{
		if (SGK_i == 0) // First Letter Of Any Sentence Must Be A Capital Letter
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = MyToUpper(SGK_chArray[SGK_i]);

		else if (SGK_chArray[SGK_i] == SPACE) //First Letter Of Every Word In The Sentence Must Be A Capital Letter. Words Are Separated By Spaces.
		{
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = SGK_chArray[SGK_i];
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j + 1] = MyToUpper(SGK_chArray[SGK_i + 1]);

			// SINCE, Already Two Characters (At Indices 'i' And i + 1 Have Been Considered In This else-if Block... We Are Extra-Incrementing 'i' And 'j' By 1
			SGK_j++;
			SGK_i++;
		}

		else if ((SGK_chArray[SGK_i] == FULLSTOP || SGK_chArray[SGK_i] == COMMA || SGK_chArray[SGK_i] == EXCLAMATION || SGK_chArray[SGK_i] == QUESTION_MARK) && (SGK_chArray[SGK_i] != SPACE)) // First Letter Of Ever Word After Punctuation Mark, In the Sentence Must Be A Capital Letter. Words Are Seperated By Punctions.
		{
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = SGK_chArray[SGK_i];
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j + 1] = SPACE;
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j + 2] = MyToUpper(SGK_chArray[SGK_i + 1]);

			//Since, Already Two Characters (At Indices 'i' And i + 1 have been considered in this else-if block...We Are Extra-Incrementing 'i' By 1
			//Since, Already Three Characters (At Indices 'j' And (j+1) And (j+2) Have Been Considered In This else-if Block... We Are Extra-Incrementing 'j' By 2
			SGK_j = SGK_j + 2;
			SGK_i++;
		}

		else
			SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = SGK_chArray[SGK_i];

		SGK_j++;
	}

	SGK_chArray_CapitalizedFirstLetterOfEveryWord[SGK_j] = '\0';

	// ***** STRING OUTPUT *****
	printf("\n\n");
	printf("String Entered By You Is : \n\n");
	printf("%s\n", SGK_chArray);

	printf("\n\n");
	printf("String After Capitalizing First Letter Of Every Word : \n\n");
	printf("%s\n", SGK_chArray_CapitalizedFirstLetterOfEveryWord);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// ***** Determining Exact Length Of the String, By Detecting The First Occurence Of NULL - Terminating Character (\0) *****
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}

char MyToUpper(char SGK_ch)
{
	//variable declaration
	int SGK_num;
	int SGK_c;

	//code 

	//ASCII Value Of 'a' (97) - ASCII Value Of 'A' (65) = 32
	//This Subtraction Will Give The Exact Difference Between The Upper And Lower Case Counterparts Of Each Letter Of The Alphabet
	//If This Difference Is Subtracted From The ASCII Value Of A Lower Case Letter, The Resultant ASCII Value Will Be That Of That Of Its Upper Case Counter-Part, Hence, Helping Us to find its upper case letter !!!
	//ASCII Values Of 'a' to 'z' => 97 to 122
	//ASCII Values Of 'A' to 'Z' => 65 to 90
	SGK_num = 'a' - 'A';

	if ((int)SGK_ch >= 97 && (int)SGK_ch <= 122)
	{
		SGK_c = (int)SGK_ch - SGK_num;
		return((char)SGK_c);
	}
	else
		return(SGK_ch);
}
