#include<stdio.h>

//DEFINING STRUCT 
struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
}SGK_data = {30,4.5f,11.451995, 'A'}; //InLine Initialization of struct variables 'data' of type 'struct MyData'

int main(void)
{
	//code 
	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("Data Members Of 'struct MyData' Are : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);
	printf("c = %c\n", SGK_data.SGK_c);

	return(0);
}
