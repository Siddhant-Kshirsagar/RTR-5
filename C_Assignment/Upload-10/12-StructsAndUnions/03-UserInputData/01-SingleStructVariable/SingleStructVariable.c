#include<stdio.h>
#include<conio.h>

struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_ch;
};

int main(void)
{
	//variable declarations
	struct MyData SGK_data; //declaring a single struct variable
	
	//code 
	//User Input For values Of Data Members of 'struct MyData'
	printf("\n\n");

	printf("Enter Integer Value For Data Member 'i' Of 'struct MyData' : \n");
	scanf("%d", &SGK_data.SGK_i);

	printf("Enter Floating-Point Value For Data Member 'f' Of 'struct MyData' : \n");
	scanf("%f", &SGK_data.SGK_f);

	printf("Enter 'Double' Value For Data Member 'd' Of 'struct MyData' : \n");
	scanf("%lf", &SGK_data.SGK_d);

	printf("Enter Character Value For Data member 'c' of 'struct MyData' : \n");
	SGK_data.SGK_ch = getch();

	//Display Values Data Members Of 'struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS of 'struct MyData' ARE : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);
	printf("c = %c\n\n", SGK_data.SGK_ch);

	return(0);
}
