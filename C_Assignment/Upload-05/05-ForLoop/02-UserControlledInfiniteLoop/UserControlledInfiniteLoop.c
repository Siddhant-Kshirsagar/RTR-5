#include<stdio.h>
int main(void)
{
	//variable declarations
	char SGK_option, SGK_ch = '\0';

	//code 
	printf("\n\n");
	printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The Infinite For Loop : \n\n");
	printf("Enter 'Y' or 'y' To Inititate User Controlled Infinite Loop : ");
	printf("\n\n");
	SGK_option = getch();
	if (SGK_option == 'Y' || SGK_option == 'y')
	{
		for (;;)//Infinite Loop
		{
			printf("In Loop... \n");
			SGK_ch = getch();
			if (SGK_ch == 'Q' || SGK_ch == 'q')
			{
				break; // User Controlled Exitting From Infinite Loop
			}
		}
	}

	printf("\n\n");
	printf("EXITING USER CONTROLLED INFINITE LOOP...");
	printf("\n\n");

	return(0);
}
