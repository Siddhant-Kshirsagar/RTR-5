#include<stdio.h>

struct MyStruct
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

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
	struct MyStruct SGK_s;
	union MyUnion SGK_u;

	//code
	printf("\n\n");
	printf("Members Of Struct Are : \n\n");

	SGK_s.SGK_i = 9;
	SGK_s.SGK_f = 8.7f;
	SGK_s.SGK_d = 1.233422;
	SGK_s.SGK_c = 'P';

	printf("s.i = %d\n\n", SGK_s.SGK_i);
	printf("s.f = %f\n\n", SGK_s.SGK_f);
	printf("s.d = %lf\n\n", SGK_s.SGK_d);
	printf("s.c = %c\n\n", SGK_s.SGK_c);

	printf("Addresses Of Members Of Struct Are : \n\n");
	printf("s.i = %p\n\n", &SGK_s.SGK_i);
	printf("s.f = %p\n\n", &SGK_s.SGK_f);
	printf("s.d = %p\n\n", &SGK_s.SGK_d);
	printf("s.c = %p\n\n", &SGK_s.SGK_c);

	printf("MyStruct s = %p\n\n", &SGK_s);

	printf("\n\n");
	printf("Members Os Union Are : \n\n");

	SGK_u.SGK_i = 3;
	printf("u.i = %d\n\n", SGK_u.SGK_i);

	SGK_u.SGK_f = 4.5f;
	printf("u.f = %f\n\n", SGK_u.SGK_f);

	SGK_u.SGK_d = 5.12764;
	printf("u.d = %lf\n\n", SGK_u.SGK_d);

	SGK_u.SGK_c = 'A';
	printf("u.c = %c\n\n", SGK_u.SGK_c);

	printf("Addresses Of Members Of Union Are : \n\n");
	printf("u.i = %p\n\n", &SGK_u.SGK_i);
	printf("u.f = %p\n\n", &SGK_u.SGK_f);
	printf("u.d = %p\n\n", &SGK_u.SGK_d);
	printf("u.c = %p\n\n", &SGK_u.SGK_c);

	printf("MyUnion u = %p\n\n", &SGK_u);

	return(0);
}
