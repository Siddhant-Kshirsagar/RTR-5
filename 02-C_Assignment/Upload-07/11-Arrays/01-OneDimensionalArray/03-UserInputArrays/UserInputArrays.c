#include<stdio.h>

//MACRO CONSTANT USED AS ARRAY SIZE IN SUBSCRIPT AND AS ARRAY LENGTH.
//HENCE, THIS PROGRAM'S ARRAYS' SIZES CAN BE SIMPLY CHANGED BY CHANGING THESE FOLLOWING 3 GLOBAL MACRO CONSTANT VALUES, BEFORE COMPILING, LINKING AND EXECUTING THE PROGRAM !!!
	
#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(void)
{
	//variable declarations 
	int SGK_iArray[INT_ARRAY_NUM_ELEMENTS];
	float SGK_fArray[FLOAT_ARRAY_NUM_ELEMENTS];
	char SGK_cArray[CHAR_ARRAY_NUM_ELEMENTS];
	int SGK_i, SGK_num;

	//code 

	//***** ARRAY ELEMENTS INPUT *****
	printf("\n\n");
	printf("Enter Elements For 'Integer' Array : \n");
	for (SGK_i = 0; SGK_i < INT_ARRAY_NUM_ELEMENTS; SGK_i++)
		scanf("%d", &SGK_iArray[SGK_i]);

	printf("\n\n");
	printf("Enter Elements For 'Floating-Point' Array : \n");
	for (SGK_i = 0; SGK_i < FLOAT_ARRAY_NUM_ELEMENTS; SGK_i++)
		scanf("%f", &SGK_fArray[SGK_i]);

	printf("\n\n");
	printf("Enter Elements For 'Character' Array : \n");
	for (SGK_i = 0; SGK_i < CHAR_ARRAY_NUM_ELEMENTS; SGK_i++)
	{
		SGK_cArray[SGK_i] = getch();
		printf("%c\n",SGK_cArray[SGK_i]);
	}

	// ***** ARRAY ELEMENTS OUTPUT *****
	printf("\n\n");
	printf("Integer Array Entered By You : \n\n");
	for (SGK_i = 0; SGK_i < INT_ARRAY_NUM_ELEMENTS; SGK_i++)
		printf("%d\n", SGK_iArray[SGK_i]);

	printf("\n\n");
	printf("Float Array Enterd By You : \n\n");
	for (SGK_i = 0; SGK_i < FLOAT_ARRAY_NUM_ELEMENTS; SGK_i++)
		printf("%f\n", SGK_fArray[SGK_i]);

	printf("Character Array Entered By You : \n\n");
	for (SGK_i = 0; SGK_i < CHAR_ARRAY_NUM_ELEMENTS; SGK_i++)
		printf("%c\n", SGK_cArray[SGK_i]);

	return(0);
}
