#include<stdio.h>

int main(void)
{
	//DEFINING STRUCT
	struct MyData
	{
		int SGK_i;
		float SGK_f;
		double SGK_d;
	}SGK_data; //Declaring a Single Struct Variable of Type 'struct MyData' locally...

	//variable declaration
	int SGK_i_size;
	int SGK_f_size;
	int SGK_d_size;
	int SGK_struct_MyData_size;

	//code
	//Assigning Data Values To The Data Members Of 'struct MyData'
	SGK_data.SGK_i = 30;
	SGK_data.SGK_f = 11.45f;
	SGK_data.SGK_d = 1.2995;

	//Displaying Values Of The Data Members Of 'struct MyData' 
	printf("\n\n");
	printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
	printf("i = %d\n", SGK_data.SGK_i);
	printf("f = %f\n", SGK_data.SGK_f);
	printf("d = %lf\n", SGK_data.SGK_d);

	//Calculating Sizes (in Bytes) of The Data Members Of 'struct MyData'
	SGK_i_size = sizeof(SGK_data.SGK_i);
	SGK_f_size = sizeof(SGK_data.SGK_f);
	SGK_d_size = sizeof(SGK_data.SGK_d);

	//Displaying Sizes (in Bytes) of The Data Members Of 'struct MyData'
	printf("\n\n");
	printf("Sizes (in bytes) Of Data Members Of 'struct MyData' Are : \n\n");
	printf("Size of 'i' = %d bytes\n", SGK_i_size);
	printf("Size of 'f' = %d bytes\n", SGK_f_size);
	printf("Size of 'd' = %d bytes\n", SGK_d_size);
	
	//Calculating Size (in Bytes) Of the entire 'struct Mydata'
	SGK_struct_MyData_size = sizeof(struct MyData);//can also give struct name -> sizeof(MyData)

	//Displaying Sizes (in Bytes) Of The entire 'struct MyData'
	printf("\n\n");
	printf("Size of 'struct MyData' : %d bytes\n\n",SGK_struct_MyData_size);

	return(0);
}
