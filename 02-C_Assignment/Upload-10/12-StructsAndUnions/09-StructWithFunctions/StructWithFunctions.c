#include<stdio.h>

//DEFINING Struct 
struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

int main(void)
{
	//function prototype
	struct MyData AddStructMembers(struct MyData, struct MyData, struct MyData);

	//variable declarations
	struct MyData SGK_data1, SGK_data2, SGK_data3, SGK_answer_data;

	//code
	// *** data1 ***
	printf("\n\n\n\n");
	printf("***** DATA 1 *****\n\n");
	printf("Enter Integer Value For 'i' Of 'struct MyData data1' : ");
	scanf("%d", &SGK_data1.SGK_i);

	printf("\n\n");
	printf("Enter Floating-Point Value For 'f' Of 'struct MyData data1' : ");
	scanf("%f", &SGK_data1.SGK_f);

	printf("\n\n");
	printf("Enter 'Double' Value For 'd' Of 'struct MyData data1' : ");
	scanf("%lf", &SGK_data1.SGK_d);

	printf("\n\n");
	printf("Enter Character Value For 'c' Of 'struct MyData data1' : ");
	SGK_data1.SGK_c = getch();
	printf("%c", SGK_data1.SGK_c);

	// *** data2 ***
	printf("\n\n\n\n");
	printf("***** DATA 2 *****\n\n");
	printf("Enter Integer Value For 'i' Of 'struct MyData data2' : ");
	scanf("%d", &SGK_data2.SGK_i);

	printf("\n\n");
	printf("Enter Floating-Point Value For 'f' Of 'struct MyData data2' : ");
	scanf("%f", &SGK_data2.SGK_f);

	printf("\n\n");
	printf("Enter 'Double' Value For 'd' Of 'struct MyData data2' : ");
	scanf("%lf", &SGK_data2.SGK_d);

	printf("\n\n");
	printf("Enter Character Value For 'c' Of 'struct MyData data2' : ");
	SGK_data2.SGK_c = getch();
	printf("%c", SGK_data2.SGK_c);

	// *** data3 ***
	printf("\n\n\n\n");
	printf("***** DATA 3 *****\n\n");
	printf("Enter Integer Value For 'i' Of 'struct MyData data3' : ");
	scanf("%d", &SGK_data3.SGK_i);

	printf("\n\n");
	printf("Enter Floating-Point Value For 'f' Of 'struct MyData data3' : ");
	scanf("%f", &SGK_data3.SGK_f);

	printf("\n\n");
	printf("Enter 'Double' Value For 'd' Of 'struct MyData data3' : ");
	scanf("%lf", &SGK_data3.SGK_d);

	printf("\n\n");
	printf("Enter Character Value For 'c' Of 'struct MyData data1' : ");
	SGK_data3.SGK_c = getch();
	printf("%c", SGK_data3.SGK_c);

	// *** Calling Function AddStructMembers() Which Accepts Three Variables Of Type 'struct MyData' As Parameters And Adds UP The Respective Members And Returns The Answer In Another Struct Of Same Type ***
	SGK_answer_data = AddStructMembers(SGK_data1, SGK_data2, SGK_data3);

	printf("\n\n\n\n");
	printf("**********ANSWER***********\n\n");
	printf("answer_data.i = %d\n", SGK_answer_data.SGK_i);
	printf("answer_data.f = %f\n", SGK_answer_data.SGK_f);
	printf("answer_data.d = %lf\n", SGK_answer_data.SGK_d);

	SGK_answer_data.SGK_c = SGK_data1.SGK_c;
	printf("answer_data.c (from data1) = %c\n", SGK_answer_data.SGK_c);

	SGK_answer_data.SGK_c = SGK_data2.SGK_c;
	printf("answer_data.c (from data2) = %c\n", SGK_answer_data.SGK_c);

	SGK_answer_data.SGK_c = SGK_data3.SGK_c;
	printf("answer_data.c (from data3) = %c\n", SGK_answer_data.SGK_c);

	return(0);
}

struct MyData AddStructMembers(struct MyData SGK_md_one, struct MyData SGK_md_two, struct MyData SGK_md_three)
{
	//variable declarations
	struct MyData SGK_answer;

	//code
	SGK_answer.SGK_i = SGK_md_one.SGK_i + SGK_md_two.SGK_i + SGK_md_three.SGK_i;
	SGK_answer.SGK_f = SGK_md_one.SGK_f + SGK_md_two.SGK_f + SGK_md_three.SGK_f;
	SGK_answer.SGK_d = SGK_md_one.SGK_d + SGK_md_two.SGK_d + SGK_md_three.SGK_d;
	return(SGK_answer);
}
