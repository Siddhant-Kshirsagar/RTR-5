#include<stdio.h>

//Global typedef
typedef int MY_INT; // "type" int has been re"def"ined as MY_INT ... Now, "MY_INT" can be treated just like "int"

int main(void)
{
	//Function prototype
	MY_INT Add(MY_INT, MY_INT);

	//Typedefs
	typedef int MY_INT;//"type" int has been re"def"ined  as MY_INT ... Now, "MY_INT" can be treated just like "int"
	typedef float SGK_FLOAT; // "type" float has been re"def"ined as SGK_FLOAT ... Now, "SGK_FLOAT" can be treated just like "float"
	typedef char CHARACTER; // "type" char has been re"def"ined as "CHARACTER" ... Now, "CHARACTER" can be treated just like "char"
	typedef double MY_DOUBLE; // "type" double has been re"def"ined as MY_DOUBLE ... Now, "MY_DOUBLE" can be treated just like "double"

	// ***** Just Like In Win32SDK !!! *****
	typedef unsigned int UINT;
	typedef UINT HANDLE;
	typedef HANDLE HWND;
	typedef HANDLE HINSTANCE;

	//variable declarations
	MY_INT SGK_a = 10, SGK_i;
	MY_INT SGK_iArray[] = { 9,18,27,36,45,54,63,72,81,90 };

	SGK_FLOAT SGK_f_sgk = 30.9f;
	const SGK_FLOAT SGK_f_sgk_pi = 3.14f;

	CHARACTER SGK_ch = '*';
	CHARACTER SGK_chArray_01[] = "Hello";
	CHARACTER SGK_chArray_02[][10] = { "RTR","Batch","2023-2024" };

	MY_DOUBLE SGK_d = 8.041997;

	// ***** JUST RANDOM NUMBERS - THEY HAVE NOTHING TO DO WITH ANY WINDOW'S HANDLE OR INSTANCE HANDLE !!! THIS IS JUST FOR UNDERSTANDING *****
	UINT SGK_uint = 3456;
	HANDLE SGK_handle = 987;
	HWND SGK_hwnd = 9876;
	HINSTANCE SGK_hInstance = 14466;

	//code 
	printf("\n\n");
	printf("Type MY_INT Variable a = %d\n",SGK_a);

	printf("\n\n");
	for (SGK_i = 0; SGK_i < (sizeof(SGK_iArray) / sizeof(int)); SGK_i++)
	{
		printf("Type MY_INT array variable iArray [%d] = %d \n", SGK_i, SGK_iArray[SGK_i]);
	}
	printf("\n\n");

	printf("\n\n");
	printf("Type SGK_FLOAT variable f = %f \n", SGK_f_sgk);
	printf("Type SGK_FLOAT constance f_sgk_pi = %f \n", SGK_f_sgk_pi);

	printf("\n\n");
	printf("Type MY_DOUBLE variable d = %lf \n",SGK_d);

	printf("\n\n");
	printf("Type CHARACTER variable ch = %c \n",SGK_ch);

	printf("\n\n");
	printf("Type CHARACTER array variable chArray_01 = %s \n", SGK_chArray_01);
	
	printf("\n\n");
	for (SGK_i = 0; SGK_i < (sizeof(SGK_chArray_02) / sizeof(SGK_chArray_02[0])); SGK_i++)
	{
		printf("%s\t",SGK_chArray_02);
	}
	printf("\n\n");

	printf("\n\n");
	printf("Type UINT variable uint = %u \n\n", SGK_uint);
	printf("Type HANDLE variable handle = %u \n\n", SGK_handle);
	printf("Type HWND variable hwnd = %u\n\n", SGK_hwnd);
	printf("Type HINSTANCE variable hInstance = %u\n\n",SGK_hInstance);
	printf("\n\n");

	MY_INT SGK_x = 90;
	MY_INT SGK_y = 30;
	MY_INT SGK_ret;

	SGK_ret = Add(SGK_x, SGK_y);
	printf("ret = %d \n\n",SGK_ret);

	return(0);
}

MY_INT Add(MY_INT SGK_a, MY_INT SGK_b)
{
	//code 
	MY_INT SGK_c;
	SGK_c = SGK_a + SGK_b;
	return(SGK_c);
}
