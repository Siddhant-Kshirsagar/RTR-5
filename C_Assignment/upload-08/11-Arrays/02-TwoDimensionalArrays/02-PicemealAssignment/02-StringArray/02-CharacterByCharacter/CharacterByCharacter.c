#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//variable declarations

	// *** A 'STRING' IS AN ARRAY OF CHARACTERS ... SO char[] is A char Array and hence, char[] is a 'STRING' ***
	// *** An Array Of Char Arrays Is An Array Of Strings !!! ***
	// *** Hence, char[] IS ONE char Array AND Hence, Is One String ***
	// *** Hence, char[][] Is An Array Of char Arrays And Hence, Is An Array of Strings ***

	// Here, the string array can allow a maximum number of 5 strings (5 rows) and each of these 5 strings can have only upto 10 characters maximum ( 10 columns)
	char SGK_strArray[5][11]; // 5 ROWS (0,1,2,3,4) -> 5 Strings  (EACH STRING CAN HAVE A MAXIMUM OF 11 CHARACTERS)

	int SGK_char_size;
	int SGK_strArray_size;
	int SGK_strArray_num_elements, SGK_strArray_num_rows, SGK_strArray_num_columns;
	int SGK_i;

	//code
	printf("\n\n");

	SGK_char_size = sizeof(char);

	SGK_strArray_size = sizeof(SGK_strArray);
	printf("Size of Two Dimensional ( 2D ) Character Array ( String Array ) Is = %d \n\n", SGK_strArray_size);

	SGK_strArray_num_rows = SGK_strArray_size / sizeof(SGK_strArray[0]);
	printf("Number of Rows (Strings) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n",SGK_strArray_num_rows);

	SGK_strArray_num_columns = sizeof(SGK_strArray[0]) / SGK_char_size;
	printf("Number of Columns In Two Dimensional ( 2D ) Character Array (String Array) Is = %d \n\n",SGK_strArray_num_columns);

	SGK_strArray_num_elements = SGK_strArray_num_rows * SGK_strArray_num_columns;
	printf("Maximum Number of Elements (characters) In Two Dimensional ( 2D) Character Array (String Array) Is = %d\n\n", SGK_strArray_num_elements);

	// *** PIECE-Meal Assignement ***
	// *** ROW 1 / STRING 1 ***
	SGK_strArray[0][0] = 'M';
	SGK_strArray[0][1] = 'y';
	SGK_strArray[0][2] = '\0'; // NULL - TERMINATING Character

	// *** ROW 2 / STRING 2 ***

	SGK_strArray[1][0] = 'N';
	SGK_strArray[1][1] = 'a';
	SGK_strArray[1][2] = 'm';
	SGK_strArray[1][3] = 'e';
	SGK_strArray[1][4] = '\0'; // NULL - TERMINATING CHARACTER

	// *** ROW 3 / STRING 3 *** 
	SGK_strArray[2][0] = 'I';
	SGK_strArray[2][1] = 's';
	SGK_strArray[2][2] = '\0'; // NULL - TERMINATING CHARACTER 

	// *** ROW 4 / STRING 4 *** 
	SGK_strArray[3][0] = 'S';
	SGK_strArray[3][1] = 'i';
	SGK_strArray[3][2] = 'd';
	SGK_strArray[3][3] = 'd';
	SGK_strArray[3][4] = 'h';
	SGK_strArray[3][5] = 'a';
	SGK_strArray[3][6] = 'n';
	SGK_strArray[3][7] = 't';
	SGK_strArray[3][8] = '\0'; // NULL - TERMINATING CHARACTER 

	//*** ROW 5 / STRING 5 ***
	SGK_strArray[4][0] = 'K';
	SGK_strArray[4][1] = 's';
	SGK_strArray[4][2] = 'h';
	SGK_strArray[4][3] = 'i';
	SGK_strArray[4][4] = 'r';
	SGK_strArray[4][5] = 's';
	SGK_strArray[4][6] = 'a';
	SGK_strArray[4][7] = 'g';
	SGK_strArray[4][8] = 'a';
	SGK_strArray[4][9] = 'r';
	SGK_strArray[4][10] = '\0'; // NULL -TERMINATING CHARACTER

	printf("\n\n");
	printf("The Strings In the 2D Character Array Are : \n\n");

	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
		printf("%s ", SGK_strArray[SGK_i]);

	printf("\n\n");

	return(0);
}

int MyStrlen(char SGK_str[])
{
	//variable delcarations
	int SGK_j;
	int SGK_string_length = 0;

	//code
	// *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) ***
	for (SGK_j = 0; SGK_j < MAX_STRING_LENGTH; SGK_j++)
	{
		if (SGK_str[SGK_j] == '\0')
			break;
		else
			SGK_string_length++;
	}
	return(SGK_string_length);
}
