#include<stdio.h>//for printf()
#include<conio.h>//for getch()

//ASCII Values For 'A' to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

//ASCII Values For 'a' to 'z' =>97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97 
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

//ASCII Values For '0' to '9' => 48 to 57
#define CHAR_DIGIT_BEGINNING 48 
#define CHAR_DIGIT_ENDING 57

int main(void)
{
	//variable declarations 
	char SGK_ch;
	int SGK_ch_value;

	//code 
	printf("\n\n");

	printf("Enter Character : ");
	SGK_ch = getch();

	printf("\n\n");
	if ((SGK_ch == 'A' || SGK_ch == 'a') || (SGK_ch == 'E' || SGK_ch == 'e') || (SGK_ch == 'I' || SGK_ch == 'i') || (SGK_ch == 'O' || SGK_ch == 'o') || (SGK_ch == 'U' || SGK_ch == 'u'))
	{
		printf("Character \'%c \' Entered By You, Is A Vowel CHARACTER From The English Alphabet !!! \n\n", SGK_ch);
	}
	else
	{
		SGK_ch_value = (int)SGK_ch;
		// If the character has ASCII value Between 65 and 90 Or Between 97 and 122 , it is still a letter of the alphabet , but It is A 'CONSONANT' , and Not a 'vowel' ...

		if ((SGK_ch_value >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && SGK_ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (SGK_ch_value >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && SGK_ch_value == CHAR_ALPHABET_LOWER_CASE_ENDING))
		{
			printf("Character \'%c\' Enter By You , Is A CONSONANT character From The English Alphabet !!! \n\n", SGK_ch);
		}
		else if (SGK_ch_value >= CHAR_DIGIT_BEGINNING && SGK_ch_value <= CHAR_DIGIT_ENDING)
		{
			printf("Character \'%c\' Entered By You , Is A DIGIT CHARACTER !!!\n\n", SGK_ch);
		}
		else
		{
			printf("Character \'%c\' Entered By You , Is A Special Character !!! \n\n", SGK_ch);
		}
	}

	return(0);
}
