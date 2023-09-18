#include<stdio.h>
int main(void)
{
	//variable declarations
	char SGK_option, SGK_ch = '\0';

	//code 
	printf("\n\n");
	printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The Infinite For Loop : \n\n");

	do
	{
		do
		{
			printf("\n");
			printf("In Loop...\n");
			SGK_ch = getch(); //control flow waits for character input...
		} while (SGK_ch != 'Q' && SGK_ch != 'q');
		printf("\n\n");
		printf("EXITTING USER CONTROLLED INFINITE LOOP...");

		printf("\n\n");
		printf("DO YOU WANT TO BEGIN USER CONTROLLED INFINITE LOOP AGAIN?...(Y/y - Yes, Any Other key - No) : ");
		SGK_option = getch();
	} while (SGK_option == 'Y' || SGK_option == 'y');

	return(0);
}
