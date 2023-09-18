#include<stdio.h>
#include<string.h> // for toupper()

int main(void)
{
	//variable declarations
	char SGK_ch, SGK_ch_i;
	unsigned int SGK_ascii_ch = 0;

	//code
	printf("\n\n");
	printf("Enter The First Character Of First Name : ");
	SGK_ch = getch();

	SGK_ch = toupper(SGK_ch);

	for (SGK_ch_i = 'A'; SGK_ch_i <= 'z'; SGK_ch_i++)
	{
		if (SGK_ch == SGK_ch_i)
		{
			SGK_ascii_ch = (unsigned int)SGK_ch;
			goto result_output; // program flow jumps directly to label "result_output"
		}
	}

	printf("\n\n");
	printf("Goto Statement not executed, so printing \"Hello, World !!!\".\n"); //will be omiited if 'goto' statement is executed 

result_output: // Label itself does not alter flow of program. Following code is executed regardless of whether goto statement is executed or not.
	printf("\n\n");

	if (SGK_ascii_ch == 0)
	{
		printf("You Must have a strange name! Could not find the character '%c' in the entire English Alphabet!\n", SGK_ch);
	}
	else
	{
		printf("Character '%c' found. It Has ASCII value %u.\n", SGK_ch, SGK_ascii_ch);
	}

	printf("\n\n");
	return(0);
}
