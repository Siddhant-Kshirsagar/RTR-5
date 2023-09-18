#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_i_num, SGK_num, SGK_i;

	//code 
	printf("\n\n");

	printf("Enter An Integer Value From Which Iteration Must Begin : ");
	scanf("%d", &SGK_i_num);

	printf("How Many Digits Do You Want To Print From %d Onwards ? : ", SGK_i_num);
	scanf("%d", &SGK_num);

	printf("Printing Digits %d to %d : \n\n", SGK_i_num, (SGK_i_num + SGK_num));

	SGK_i = SGK_i_num;
	while (SGK_i <= (SGK_i_num + SGK_num))
	{
		printf("\t%d\n", SGK_i);
		SGK_i++;
	}
	
	printf("\n\n");

	return(0);
}
