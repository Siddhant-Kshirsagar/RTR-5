#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAX_STRING_LENGTH 1024

struct CharacterCount
{
	char SGK_ch;
	int SGK_ch_count;
}SGK_character_and_count[] = { {'A',0}, // character_and_count[0].ch = 'A' & character_and_count[0].ch_count = 0
							{'B',0},//character_and_count[1].ch = 'B' & character_and_count[1].ch_count = 0
							{'C',0},//character_and_count[2].ch = 'C' & character_and_count[2].ch_count = 0
							{'D',0},//character_and_count[3].ch = 'D' & character_and_count[3].ch_count = 0
							{'E',0},//character_and_count[4].ch = 'E' & character_and_count[4].ch_count = 0
							{'F',0},
							{'G',0},
							{'H',0},
							{'I',0},
							{'J',0},
							{'K',0},
							{'L',0},
							{'M',0},
							{'N',0},
							{'O',0},
							{'P',0},
							{'Q',0},
							{'R',0},
							{'S',0},
							{'T',0},
							{'U',0},
							{'V',0},
							{'W',0},
							{'X',0},
							{'Y',0},
							{'Z',0} };//character_and_count[25].ch = 'Z' & character_and_count[25].ch_count = 0

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(SGK_character_and_count)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(SGK_character_and_count[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS/SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)

//Entry Point Function
int main(void)
{
	//variable declarations
	char SGK_str[MAX_STRING_LENGTH];
	int SGK_i, SGK_j, SGK_actual_string_length = 0;

	//code
	printf("\n\n");
	printf("Enter A String : \n\n");
	gets_s(SGK_str, MAX_STRING_LENGTH);

	SGK_actual_string_length = strlen(SGK_str);

	printf("\n\n");
	printf("The String You Have Entered Is : \n\n");
	printf("%s\n\n",SGK_str);

	for (SGK_i = 0; SGK_i < SGK_actual_string_length; SGK_i++)
	{
		for (SGK_j = 0; SGK_j < NUM_ELEMENTS_IN_ARRAY; SGK_j++)// Run every character of the input string through the entire alphabet (A TO Z)
		{
			SGK_str[SGK_i] = toupper(SGK_str[SGK_i]);// If input character is in lower case, turn it to upper case for comparision

			if (SGK_str[SGK_i] == SGK_character_and_count[SGK_j].SGK_ch)//If Character is Present..
				SGK_character_and_count[SGK_j].SGK_ch_count++;// Increment its count by 1 ... 
		}
	}

	printf("\n\n");
	printf("The Number Of Occurences of All Characters From The Alphabet Are As Follows : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS_IN_ARRAY; SGK_i++)
	{
		printf("Character %c	= %d\n", SGK_character_and_count[SGK_i].SGK_ch, SGK_character_and_count[SGK_i].SGK_ch_count);
	}
	printf("\n\n");

	return(0);
}

