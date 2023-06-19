#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArrayOne[10];
	int SGK_iArrayTwo[10];

	//code

	//***** iArrayOne[] *****
	SGK_iArrayOne[0] = 3;
	SGK_iArrayOne[1] = 6;
	SGK_iArrayOne[2] = 9;
	SGK_iArrayOne[3] = 12;
	SGK_iArrayOne[4] = 15;
	SGK_iArrayOne[5] = 18;
	SGK_iArrayOne[6] = 21;
	SGK_iArrayOne[7] = 24;
	SGK_iArrayOne[8] = 27;
	SGK_iArrayOne[9] = 30;

	printf("\n\n");
	printf("Piece-meal (Hard-coded) Assignment And Display Of Elements to Array 'iArrayOne[]' : \n\n");
	printf("1st Element Of Array 'iArrayOne[]' Or Element At 0th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[0]);
	printf("2nd Element Of Array 'iArrayOne[]' Or Element At 1st Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[1]);
	printf("3rd Element Of Array 'iArrayOne[]' Or Element At 2nd Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[2]);
	printf("4th Element Of Array 'iArrayOne[]' Or Element At 3rd Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[3]);
	printf("5th Element Of Array 'iArrayOne[]' Or Element At 4th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[4]);
	printf("6th Element Of Array 'iArrayOne[]' Or Element At 5th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[5]);
	printf("7th Element Of Array 'iArrayOne[]' Or Element At 6th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[6]);
	printf("8th Element Of Array 'iArrayOne[]' Or Element At 7th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[7]);
	printf("9th Element Of Array 'iArrayOne[]' Or Element At 8th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[8]);
	printf("10th Element Of Array 'iArrayOne[]' Or Element At 9th Index Of Array 'iArrayOne[]' = %d\n", SGK_iArrayOne[9]);

	//***** iArrayTwo[] *****
	printf("\n\n");

	printf("Enter 1st Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[0]);
	printf("\nEnter 2nd Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[1]);
	printf("\nEnter 3rd Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[2]);
	printf("\nEnter 4th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[3]);
	printf("\nEnter 5th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[4]);
	printf("\nEnter 6th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[5]);
	printf("\nEnter 7th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[6]);
	printf("\nEnter 8th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[7]);
	printf("\nEnter 9th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[8]);
	printf("\nEnter 10th Element Of Array 'iArrayTwo[]' : ");
	scanf("%d", &SGK_iArrayTwo[9]);

	printf("\n\n");
	printf("Piece-meal (User Input) Assignment And Display Of Elements to Array 'iArrayTwo[]' : \n\n");
	printf("1st Element Of Array 'iArrayTwo[]' or Element At 0th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[0]);
	printf("2nd Element Of Array 'iArrayTwo[]' or Element At 1st Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[1]);
	printf("3rd Element Of Array 'iArrayTwo[]' or Element At 2nd Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[2]);
	printf("4th Element Of Array 'iArrayTwo[]' or Element At 3rd Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[3]);
	printf("5th Element Of Array 'iArrayTwo[]' or Element At 4th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[4]);
	printf("6th Element Of Array 'iArrayTwo[]' or Element At 5th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[5]);
	printf("7th Element Of Array 'iArrayTwo[]' or Element At 6th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[6]);
	printf("8th Element Of Array 'iArrayTwo[]' or Element At 7th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[7]);
	printf("9th Element Of Array 'iArrayTwo[]' or Element At 8th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[8]);
	printf("10th Element Of Array 'iArrayTwo[]' or Element At 9th Index Of Array 'iArrayTwo[]' = %d\n", SGK_iArrayTwo[9]);

	return(0);
}
