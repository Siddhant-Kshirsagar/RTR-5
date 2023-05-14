#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_a;
	int SGK_b;
	int SGK_x;

	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d",&SGK_a);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &SGK_b);

	printf("\n\n");

	//Since, In All The Following 5 Cases, The Operand on The Left 'a' Is Getting Repeated Immediately On The Right (e.g : SGK_a = SGK_a + SGK_b or SGK_a = SGK_a - SGK_b),
	//we are Using compound Assignment Operators +=, -=, *=,/= and %=

	//Since , 'SGK_a' Will Be Assigned The Value Of (SGK_a+SGK_b) At The Expression (SGK_a+=SGK_b) We Must Save The Original Value Of 'SGK_a' To Another Variable (SGK_x)
	SGK_x = SGK_a;
	SGK_a += SGK_b; // SGK_a = SGK_a + SGK_b
	printf("Addition Of SGK_A = %d And SGK_B = %d Gives %d. \n", SGK_x, SGK_b, SGK_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here ...
	//Since, 'SGK_a' Will Be Assigned The Value Of (SGK_a-SGK_b) At The Expression (SGK_a -= SGK_b), We Must Save The Original Value Of 'SGK_a' To Another Variable (SGK_x)

	SGK_x = SGK_a;
	SGK_a -= SGK_b; // SGK_a = SGK_a - SGK_b;
	printf("Subtration Of SGK_A = %d And SGK_B = %d  Gives %d . \n", SGK_x, SGK_b, SGK_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here ...
	//Since, 'SGK_a' Will Be Assigned The Value Of (SGK_a*SGK_b) At The Expression (SGK_a *= SGK_b), We Must Save The Original Value Of 'SGK_a' To Another Variable (SGK_x)
	
	SGK_x = SGK_a;
	SGK_a *= SGK_b; // SGK_a = SGK_a * SGK_b;
	printf("Multiplication Of SGK_A = %d And SGK_B = %d  Gives %d . \n", SGK_x, SGK_b, SGK_a);


	//Value Of 'a' Altered In The Above Expression Is Used Here ...
	//Since, 'SGK_a' Will Be Assigned The Value Of (SGK_a/SGK_b) At The Expression (SGK_a /= SGK_b), We Must Save The Original Value Of 'SGK_a' To Another Variable (SGK_x)

	SGK_x = SGK_a;
	SGK_a /= SGK_b; // SGK_a = SGK_a / SGK_b;
	printf("Division Of SGK_A = %d And SGK_B = %d  Gives  Quotient %d . \n", SGK_x, SGK_b, SGK_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here ...
//Since, 'SGK_a' Will Be Assigned The Value Of (SGK_a%SGK_b) At The Expression (SGK_a %= SGK_b), We Must Save The Original Value Of 'SGK_a' To Another Variable (SGK_x)

	SGK_x = SGK_a;
	SGK_a %= SGK_b; // SGK_a = SGK_a % SGK_b;
	printf("Division Of SGK_A = %d And SGK_B = %d  Gives Remainder %d  . \n", SGK_x, SGK_b, SGK_a);


	printf("\n\n");

	return(0);
}

