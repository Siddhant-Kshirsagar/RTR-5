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
	printf("Size Of MyStruct = %zu\n", sizeof(SGK_s));
	printf("\n\n");
	printf("Size Of MyUnion = %zu\n",sizeof(SGK_u));
	return(0);
}
