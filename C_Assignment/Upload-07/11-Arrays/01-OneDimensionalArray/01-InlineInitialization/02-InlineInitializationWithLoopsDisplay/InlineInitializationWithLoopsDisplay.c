#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray[] = { 9,30,6,12,98,95,20,23,2,45 };
	int SGK_int_size;
	int SGK_iArray_size;
	int SGK_iArray_num_elements;

	float SGK_fArray[] = { 1.2f,2.3f,3.4f,4.5f,5.6f,6.7f,7.8f,8.9f };
	int SGK_float_size;
	int SGK_fArray_size;
	int SGK_fArray_num_elements;

	char SGK_cArray[] = { 'A','S','T','R','O','M','E','D','I','C','O','M','P' };
	int SGK_char_size;
	int SGK_cArray_size;
	int SGK_cArray_num_elements;

	int SGK_i;

	//code 

	// ***** iArray[] *****
	printf("\n\n");
	printf("In-line Initialization And Loop (for) Display Of Elements of Array 'iArray[]' : \n\n");
	
	SGK_int_size = sizeof(int);
	SGK_iArray_size = sizeof(SGK_iArray);
	SGK_iArray_num_elements = SGK_iArray_size / SGK_int_size;

	for (SGK_i = 0; SGK_i < SGK_iArray_num_elements; SGK_i++)
	{
		printf("iArray[%d] (Element %d) = %d\n", SGK_i, (SGK_i + 1), SGK_iArray[SGK_i]);
	}

	printf("\n\n");
	printf("Size Of Data type 'int'										=%d bytes\n",SGK_int_size);
	printf("Number Of Element In 'int' Array 'iArray[]'					=%d Elements\n", SGK_iArray_num_elements);
	printf("Size Of Array 'iArray[]' (%d Elements * %d bytes			=%d bytes\n\n",SGK_iArray_num_elements, SGK_int_size, SGK_iArray_size);

	//***** fArray[] *****
	printf("\n\n");
	printf("In-Line Initialization And Loop (while) Display Of Elemenets of Array 'fArray[]' : \n\n");

	SGK_float_size = sizeof(float);
	SGK_fArray_size = sizeof(SGK_fArray);
	SGK_fArray_num_elements = SGK_fArray_size / SGK_float_size;

	SGK_i = 0;
	while(SGK_i < SGK_fArray_num_elements)
	{
		printf("fArray[%d] (Element %d) = %f\n", SGK_i, (SGK_i + 1), SGK_fArray[SGK_i]);
		SGK_i++;
	}

	printf("\n\n");
	printf("Size Of Data type 'float'								= %d bytes\n", SGK_float_size);
	printf("Number Of Elements in 'float' Array 'fArray[]'			= %d Elements\n", SGK_fArray_num_elements);
	printf("Size of Array 'fArray[]' (%d Elements * %d bytes)		= %d bytes\n\n", SGK_fArray_num_elements, SGK_float_size, SGK_fArray_size);

	// ***** cArray[] *****
	printf("\n\n");
	printf("In-line Initialization And Loop (do-while) Display Of Elements Of Array 'cArray[]' : \n\n");

	SGK_char_size = sizeof(char);
	SGK_cArray_size = sizeof(SGK_cArray);
	SGK_cArray_num_elements = SGK_cArray_size / SGK_char_size;

	SGK_i = 0;
	do
	{
		printf("cArray[%d] (Element %d) = %c \n", SGK_i, (SGK_i + 1), SGK_cArray[SGK_i]);
		SGK_i++;
	} while (SGK_i < SGK_cArray_num_elements);

	printf("\n\n");
	printf("Size Of Data type 'char'							= %d bytes\n", SGK_char_size);
	printf("Number of Elements In 'char' Array 'cArray[]'		= %d Elemenst\n", SGK_cArray_num_elements);
	printf("Size of Array 'cArray[]' (%d Element * %d bytes)	= %d bytes\n", SGK_cArray_num_elements, SGK_char_size, SGK_cArray_size);

	return(0);
}
