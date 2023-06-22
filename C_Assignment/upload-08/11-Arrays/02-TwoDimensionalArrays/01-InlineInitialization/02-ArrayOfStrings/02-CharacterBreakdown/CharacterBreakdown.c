#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations
	//*** A 'STRING' IS AN ARRAY OF CHARACTERS ... so char[] is a char array and hence, char[] is a 'string' ***
	//*** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
	//*** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
	// *** HENCE, char[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***

	// Here, the string array can allow a maximum number of 10 strings (10 rows) and each of these 10 strings can have only upto 15 characters maximum ( 15 columns )
	char SGK_strArray[10][15] = { "Hello!","Welcome","To","Real","Time","Rendering","Batch","(2023-24)","Of","ASTROMEDICOMP." }; //IN-LINE INITIALIZATION

	int SGK_iStrLengths[10]; // 1D Integer Array -Stores length of those strings at corresponding indices in 'strArray[]' e.g: iStrlength[0] ill be the length of string at strArray[0] , iStrlengths[1] will be the lenght of strings at strArray[1]... 10 strings, 10 length...
	int SGK_strArray_size;
	int SGK_strArray_num_rows;
	int SGK_i, SGK_j;

	//code 
	SGK_strArray_size = sizeof(SGK_strArray);
	SGK_strArray_num_rows = SGK_strArray_size / sizeof(SGK_strArray[0]);

	//Storing in length of all the strings...
	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
		SGK_iStrLengths[SGK_i] = MyStrlen(SGK_strArray[SGK_i]);

	printf("\n\n");
	printf("The Entire String Array : \n\n");
	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
		printf("%s ", SGK_strArray[SGK_i]);

	printf("\n\n");
	printf("Strings In The 2D Array : \n\n");

	//Since, char[][] is an array of strings, referencing only be the row number (first []) will give the row or the string
	//The column number (second []) is the particular character in that string row
	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
	{
		printf("String Number %d => %s\n\n", (SGK_i + 1), SGK_strArray[SGK_i]);
		for (SGK_j = 0; SGK_j < SGK_iStrLengths[SGK_i]; SGK_j++)
		{
			printf("Character %d = %c\n", (SGK_j + 1), SGK_strArray[SGK_i][SGK_j]);
		}
		printf("\n\n");
	}
	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations 
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	// *** DETERMINING EXACT LENGTH OF THE STRING, BY DECTECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
