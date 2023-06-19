#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	//variable declarations
	int SGK_iArray[NUM_ELEMENTS];
	int SGK_i,SGK_j, SGK_num, SGK_count = 0;

	//code
	printf("\n\n");

	//***** ARRAY ELEMENTS INPUT *****
	printf("Enter Integer Elements For Array : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		scanf("%d",&SGK_num);
		//If 'num' is negative ( < 0), then convert it to positive (multiply by -1)
		if (SGK_num < 0)
			SGK_num = -1 * SGK_num;

		SGK_iArray[SGK_i] = SGK_num;
	}
	
	//***** PRINTING ENTIRE ARRAY *****
	printf("\n\n");
	printf("Array Elements Are : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
		printf("%d\n", SGK_iArray[SGK_i]);

	// ***** SEPARATING OUT EVEN NUMBERS FROM ARRAY ELEMENST *****
	printf("\n\n");
	printf("Prime Numbers Amongst The Array Elements Are : \n\n");
	for (SGK_i = 0; SGK_i < NUM_ELEMENTS; SGK_i++)
	{
		for (SGK_j = 1; SGK_j <= SGK_iArray[SGK_i]; SGK_j++)
		{
			if ((SGK_iArray[SGK_i] % SGK_j) == 0)
				SGK_count++;
		}
		
		//NUMBER 1 IS NEITHER A PRIME NUMBER NOR A CONSONANT
		//IF A NUMBER IS PRIME, IT IS ONLY DIVISIBLE BY 1 AND ITSELF.
		//HENCE, IF A NUMBER IS PRIME, THE VALUE OF 'count' WILL BE EXACTLY 2.
		//IF THE VALUE OF 'count'  IS GREATER THAN 2, THE NUMBER IS DIVISIBLE BY NUMBERS OTHER THANT 1 AND ITSELF AND HENCE, IT IS NOT PRIME 
		//THE VALUE OF 'count' WILL BE 1 ONLY IF iArray[i] is 1

		if (SGK_count == 2)
			printf("%d\n", SGK_iArray[SGK_i]);
			
		SGK_count = 0; // RESET 'count' TO 0 FOR CHECKING THE NEXT NUMBER...
	}
	return(0);
}
