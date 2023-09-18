#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a;
	int SGK_b;
	int SGK_c;
	int SGK_result;

	//code 
	printf("\n\n");
	printf("Enter First Integer : ");
	scanf("%d", &SGK_a);

	printf("\n\n");
	printf("Enter Second Integer : ");
	scanf("%d", &SGK_b);

	printf("\n\n");
	printf("Enter Third Integer : ");
	scanf("%d", &SGK_c);

	printf("\n\n");
	printf("If Answer = 0, It Is 'FALSE' . \n");
	printf("If Answer = 1, It Is 'TRUE' .\n\n");

	SGK_result = (SGK_a <= SGK_b) && (SGK_b != SGK_c);
	printf("LOGICAL AND (&&) : ANSWER is TRUE (1) If And Only If Both Conditions Are True. The Answer is FALSE (0), If Any One or Both Conditions Are False.\n\n");
	printf("SGK_A = %d Is Less Than Or Equal To SGK_B = %d AND SGK_B = %d Is Not Equal To SGK_C = %d \t Answer = %d \n\n", SGK_a, SGK_b, SGK_b, SGK_c, SGK_result);

	SGK_result = (SGK_b >= SGK_a) || (SGK_a == SGK_c);
	printf("LOGICAL OR (||) : Answer is FALSE (0) If And Only If Both Conditions Are False. The Answer Is TRUE(1) , If Any One Or Both Condition Are TRUE. \n\n");
	printf("Either SGK_=%d Is Greater Than Or Equal To SGK_A = %d OR SGK_A = %d Is Equal To SGK_C = %d \t Answer = %d \n\n", SGK_b, SGK_a, SGK_a, SGK_c, SGK_result);

	SGK_result = !SGK_a;
	printf("SGK_A = %d And Using Logical NOT (!) Operator on SGK_A Gives SGK_Result = %d \n\n ", SGK_a, SGK_result);

	SGK_result = !SGK_b;
	printf("SGK_B = %d And Using Logical NOT(!) Operator on SGK_B Gives SGK_Result = %d \n\n", SGK_b, SGK_result);

	SGK_result = !SGK_c;
	printf("SGK_C =%d And Using Logical NOT(!) Operator On SGK_C Gives SGK_Result = %d \n\n", SGK_c, SGK_result);

	SGK_result = (!(SGK_a <= SGK_b) && !(SGK_b != SGK_c));
	printf("Using Logical NOT (!) On (SGK_a <= SGK_b) And Also On (SGK_b != SGK_c) And then AND-ing Them Afterwards Gives Result = %d \n", SGK_result);

	printf("\n\n");

	SGK_result = !((SGK_b >= SGK_a) || (SGK_a == SGK_c));
	printf("Using Logical NOT(!) On Entire Logical Expression (SGK_b >= SGK_a) And then AND-ing Them Afterwards Gives Result = %d \n", SGK_result);

	printf("\n\n");

	return(0);
}
