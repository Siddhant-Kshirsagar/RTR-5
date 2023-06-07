#include<stdio.h>
#include<conio.h>
int main(void)
{
	//variable declartions
	int SGK_i;
	char SGK_ch;

	//code 
	printf("\n\n");

	printf("Printing Even Numbers From 1 to 100 For Every User Input. EXITTING the Loop When User Enters Character 'Q' or 'q' : \n\n");
	printf("Enter Character 'Q' or 'q' To Exit Loop : \n\n");

	for (SGK_i = 1; SGK_i <= 100; SGK_i++)
	{
		printf("\t%d\n", SGK_i);
		SGK_ch = getch();
		if (SGK_ch == 'Q' || SGK_ch == 'q')
		{
			break;
		}
	}

	printf("\n\n");
	printf("EXITTING LOOP...");
	printf("\n\n");

	return(0);
}
