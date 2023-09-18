#include<stdio.h>

//DEFINING STRUCT 
struct MyData
{
	int SGK_i;
	float SGK_f;
	double SGK_d;
	char SGK_c;
};

int main(void)
{
	//variable declaration

	// 35 will be assigned to 'i' of 'data_one'
	// 3.9 will be assigned to 'f' of 'data_one'
	// 1.23765 will be assigned to 'd' of 'data_one'
	// 'A' will be assinged to 'c' of data_one
	struct MyData SGK_data_one = { 35,3.9,1.23765,'A' }; // InLine Initialization

	// 'P' will be assigned to 'i' of 'data_two' ... but 'P' is a character (char) and 'i' is an integer ... so 'P' is converted into it decimal integer ASCII value (80) and 80 is assigned to 'i' of 'data_two'
	// 6.2 will be assigned to 'f' of 'data_two' 
	// 12.199523 will be assigned to 'd' of 'data_two'
	// 68 will be assigned to 'c' of 'data_two'... but 68 is an integer (int) and 'C' is a 'char' ... so 68 is considered as a decimal ASCII value and its corresponding character ('D') is Assigned to 'c' of 'data_two'
	struct MyData SGK_data_two = { 'P', 6.2f, 12.199523, 68 }; //Inline Initialization

	// 36 will be assigned to 'i' of 'data_three'
	// 'G' is 'char', but 'f' of 'data_three' is 'float' ... hence, 'G' is converted to its decimal integer ASCII value(71) and this in turn is converted to 'float' (71.000000) and then it will be assinged to 'f' of 'data_three'
	// 0.0000000 will be assigned to 'd' of 'data_three'
	// No  Character will be assigned to 'c' of 'data_three'
	struct MyData SGK_data_three = { 36, 'G' }; // Inline Initialization

	// 79 will be assinged to 'i' of 'data_four'
	// 0.000000 will be assigned to 'f' of 'data_four'
	// 0.000000 wiil be assigned to 'd' of 'data_four'
	// No character will be assigned to 'c' of 'data_four'
	struct MyData SGK_data_four = { 79 };// Inline Initialization

	//code
	//Displaying Values Of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("DATA MEMBERS Of 'struct MyData data_one' ARE : \n\n");
	printf("i = %d\n", SGK_data_one.SGK_i);
	printf("f = %f\n", SGK_data_one.SGK_f);
	printf("d = %lf\n", SGK_data_one.SGK_d);
	printf("c = %c\n", SGK_data_one.SGK_c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData data_two' ARE : \n\n");
	printf("i = %d\n", SGK_data_two.SGK_i);
	printf("f = %f\n", SGK_data_two.SGK_f);
	printf("d = %lf\n", SGK_data_two.SGK_d);
	printf("c = %c\n", SGK_data_two.SGK_c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData data_three' ARE : \n\n");
	printf("i = %d\n", SGK_data_three.SGK_i);
	printf("f = %f\n", SGK_data_three.SGK_f);
	printf("d = %lf\n", SGK_data_three.SGK_d);
	printf("c = %c\n", SGK_data_three.SGK_c);

	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData data_four' ARE : \n\n");
	printf("i = %d\n", SGK_data_four.SGK_i);
	printf("f = %f\n", SGK_data_four.SGK_f);
	printf("d = %lf\n", SGK_data_four.SGK_d);
	printf("c = %c\n", SGK_data_four.SGK_c);

	return(0);
}
