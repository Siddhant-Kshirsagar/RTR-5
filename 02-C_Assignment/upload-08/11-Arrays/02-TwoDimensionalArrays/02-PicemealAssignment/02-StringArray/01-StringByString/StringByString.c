#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	void MyStrcpy(char[], char[]);

	//variable declarations

	// *** A 'STRING' Is An Array Of Characters ... So char[] Is A char Array And Hence, char[] Is A 'STRING' ***
	// *** An Array Of Char Arrays Is An Array Of Strings !!! ***
	// *** Hence, char[] Is One char Array AND Hence, Is One String ***
	// *** Hence, char[][] Is An Array Of char Arrays And Hence, Is AN Array Of Strings *** 
	
	// Here , the string array can allow a maximum number of 5 strings (5 rows) and each of these 5 strings can have only upto 10 charactes maximun (10 columns) 
	char SGK_strArray[5][10]; // 5 ROWS (0,1,2,3,4) -> 5 STRINGS (EACH STRING CAN HAVE A MAXIMUM OF 10 CHARACTERS)
	int SGK_char_size;
	int SGK_strArray_size;
	int SGK_strArray_num_elements, SGK_strArray_num_rows, SGK_strArray_num_columns;
	int SGK_i;
	
	//code
	printf("\n\n");

	SGK_char_size = sizeof(char);

	SGK_strArray_size = sizeof(SGK_strArray);
	printf("Size Of Two Dimensional (2D) Character Array (String Array) Is = %d \n\n", SGK_strArray_size);

	SGK_strArray_num_rows = SGK_strArray_size / sizeof(SGK_strArray[0]);
	printf("Number of Rows (Strings) In Two Dimensional (2D) Character Array (String Array) Is = %d\n\n",SGK_strArray_num_rows);

	SGK_strArray_num_columns = sizeof(SGK_strArray[0]) / SGK_char_size;
	printf("Number of columns In Two Dimensional ( 2D) Character Array (String Array) Is = %d\n\n", SGK_strArray_num_columns);

	SGK_strArray_num_elements = SGK_strArray_num_rows * SGK_strArray_num_columns;
	printf("Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", SGK_strArray_num_elements);

	// *** PIECE-MEAL ASSIGNEMENT ***
	MyStrcpy(SGK_strArray[0], "My");
	MyStrcpy(SGK_strArray[1], "Name");
	MyStrcpy(SGK_strArray[2], "Is");
	MyStrcpy(SGK_strArray[3], "Siddhant");
	MyStrcpy(SGK_strArray[4], "Kshirsagar");

	printf("\n\n");
	printf("The Strings In The 2D Character Array Are : \n\n");

	for (SGK_i = 0; SGK_i < SGK_strArray_num_rows; SGK_i++)
		printf("%s ",SGK_strArray[SGK_i]);

	printf("\n\n");

	return(0);
}

void MyStrcpy(char SGK_str_destination[], char SGK_str_source[])
{
	//function prototype
	int MyStrlen(char[]);

	//variable declarations
	int SGK_iStringLength = 0;
	int SGK_j;

	//code 
	SGK_iStringLength = MyStrlen(SGK_str_source);
	for (SGK_j = 0; SGK_j < SGK_iStringLength; SGK_j++)
		SGK_str_destination[SGK_j] = SGK_str_source[SGK_j];

	SGK_str_destination[SGK_j] = '\0';
}

int MyStrlen(char SGK_str[])
{
	//variable declarations
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
