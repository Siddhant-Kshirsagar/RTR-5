#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations

	// *** A 'STRING' IS AN ARRAY OF CHARCTERS ... so char[] is a char Array and Hence, char[] is a 'STRING' ***
	// *** AN ARRAY OF char ARRAYS IS AN ARRAY OF STRINGS !!! ***
	// *** HENCE, char[] IS ONE char ARRAY AND HENCE, IS ONE STRING ***
	// *** HENCE, CHAR[][] IS AN ARRAY OF char ARRAYS AND HENCE, IS AN ARRAY OF STRINGS ***

	//Here, the string array can allow a maximum number of 10 strings (10 rows) and each of these 10 strings can have only upto 15 characters maximum (15 columns) 
	char SGK_strArray[10][15] = { "Hello!","Welcome","To", "Real", "Time", "Rendering", "Batch","(2023-24)","Of","ASTROMEDICOMP." }; //IN-LINE INITIALIZATION
	int SGK_char_size;
	int SGK_strArray_size;
	int SGK_strArray_num_elements, SGK_strArray_num_rows, SGK_strArray_num_columns;
	int SGK_strActual_num_chars = 0;
	int SGK_i;

	//code
	printf("\n\n");

	SGK_char_size = sizeof(char);

	SGK_strArray_size = sizeof(SGK_strArray);
	printf("Size Of Two Dimensional (2D) Character Array (String Array) Is = %d \n\n",SGK_strArray_size);

	SGK_strArray_num_rows = SGK_strArray_size / sizeof(SGK_strArray[0]);
	printf("Number of Rows (Strings) In Two Dimensional (2D) Character Array (String Array) Is = %d\n\n",SGK_strArray_num_rows);

	SGK_strArray_num_columns = sizeof(SGK_strArray[0]) / SGK_char_size;
	printf("Number of Columns In Two Dimensional (2D) Characters Array (String Array) Is = %d\n\n",SGK_strArray_num_columns);

	SGK_strArray_num_elements = SGK_strArray_num_rows * SGK_strArray_num_columns;
	printf("Maximum Number of Elemens = (characters) In Two Dimensional (2D) Character Array Is = %d\n\n", SGK_strArray_num_elements);

	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
	{
		SGK_strActual_num_chars = SGK_strActual_num_chars + MyStrlen(SGK_strArray[SGK_i]);
	}
	printf("Actual Number Of Elements (characters) In Two Dimensional (2D) Character Array (String Array) Is = %d\n\n", SGK_strActual_num_chars);

	printf("\n\n");
	printf("Strings In The 2d Array : \n\n");

	//Since, char[][] is an array of strings, referencing only by the row number (first[]) will give the row or the string 
	//The Column Number (second []) is the particular character in that string /row

	printf("%s ", SGK_strArray[0]);
	printf("%s ", SGK_strArray[1]);
	printf("%s ", SGK_strArray[2]);
	printf("%s ", SGK_strArray[3]);
	printf("%s ", SGK_strArray[4]);
	printf("%s ", SGK_strArray[5]);
	printf("%s ", SGK_strArray[6]);
	printf("%s ", SGK_strArray[7]);
	printf("%s ", SGK_strArray[8]);
	printf("%s \n\n", SGK_strArray[9]);

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
	int SGK_j;
	int SGK_string_length = 0;

	//code 
	//*** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL - TERMINATING CHARACTER (\0) ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}

