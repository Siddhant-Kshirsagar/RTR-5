#include<stdio.h>

union MyUnion
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

int main(void)
{
	//variable declarations
	union MyUnion SGK_u1, SGK_u2;

	//code
	//*** MyUnion u1 ***
	printf("\n\n");
	printf("Members Of Union u1 Are : \n\n");

	SGK_u1.SGK_i = 6;
	SGK_u1.SGK_f = 1.2f;
	SGK_u1.SGK_d = 8.333333;
	SGK_u1.SGK_c = 'S';

	printf("u1.i = %d\n\n",SGK_u1.SGK_i);
	printf("u1.f = %f\n\n", SGK_u1.SGK_f);
	printf("u1.d = %lf\n\n", SGK_u1.SGK_d);
	printf("u1.c = %c\n\n", SGK_u1.SGK_c);

	printf("Addresses Of Members Of Union u1 Are : \n\n");
	printf("u1.i = %p\n\n", &SGK_u1.SGK_i);
	printf("u1.f = %p\n\n", &SGK_u1.SGK_f);
	printf("u1.d = %p\n\n", &SGK_u1.SGK_d);
	printf("u1.c = %p\n\n", &SGK_u1.SGK_c);

	printf("MyUnion u1 = %p\n\n", &SGK_u1);

	// *** MyUnion u2 ***
	printf("\n\n");
	printf("Members Of Union u2 Are : \n\n");

	SGK_u2.SGK_i = 3;
	printf("u2.i = %d\n\n", SGK_u2.SGK_i);

	SGK_u2.SGK_f = 4.5f;
	printf("u2.f = %f\n\n", SGK_u2.SGK_f);

	SGK_u2.SGK_d = 5.12764;
	printf("u2.d = %lf\n\n", SGK_u2.SGK_d);

	SGK_u2.SGK_c = 'A';
	printf("u2.c = %c\n\n", SGK_u2.SGK_c);

	printf("Addresses Of Members Of Union u2 Are : \n\n");
	printf("u2.i = %p\n\n", &SGK_u2.SGK_i);
	printf("u2.f = %p\n\n", &SGK_u2.SGK_f);
	printf("u2.d = %p\n\n", &SGK_u2.SGK_d);
	printf("u2.c = %p\n\n", &SGK_u2.SGK_c);

	printf("MyUnion u2 = %p\n\n", &SGK_u2);

	return(0);
}
