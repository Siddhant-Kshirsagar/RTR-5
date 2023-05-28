#include<stdio.h>
int main(void)
{
	//variable declrations
	int SGK_num;

	//code 
	printf("\n\n");
	printf("Enter Value For 'num' : ");
	scanf("%d", &SGK_num);

	if (SGK_num < 0)//'if' - 01
	{
		printf("Num = %d Is Less Than 0 (NEGATIVE)!!!\n\n", SGK_num);
	}
	else // 'else ' - 01
	{
		if ((SGK_num > 0) && (SGK_num <= 100))//'if' - 02
		{
			printf("Num = %d Is Between 0 And 100 !!! \n\n", SGK_num);
		}
		else // 'else' - 02
		{
			if ((SGK_num > 100) && (SGK_num <= 200))//'if' - 03
			{
				printf("Num = %d Is Between 100 And 200 !!! \n\n", SGK_num);
			}
			else // 'else' - 03
			{
				if ((SGK_num > 200) && (SGK_num <= 300))// 'if' - 04
				{
					printf("Num = %d Is Between 200 And 300 !!! \n\n", SGK_num);
				}
				else // 'else' - 04
				{
					if ((SGK_num > 300) && (SGK_num <= 400))// 'else' - 05
					{
						printf("Num = %d Is Between 300 And 400 !!! \n\n", SGK_num);
					}
					else  // 'else' - 05
					{
						if ((SGK_num > 400) && (SGK_num <= 500)) // 'if' - 06
						{
							printf("Num = %d Is Between 400 And 500 !!! \n\n", SGK_num);
						}
						else  // 'else' - 06
						{
							printf("Num = %d Is Greater Than 500 !!! \n\n", SGK_num);
						}//closing brace of 'else' - 06
					} //closing brace of 'else' - 05

				}//closing brace of 'else' - 04
				
			}//closing brace of 'else' - 03
		}//closing brace of 'else' - 02
	}//closing brace of 'else' - 01

	return(0);
}
