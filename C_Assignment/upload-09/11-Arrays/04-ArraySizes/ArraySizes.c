#include<stdio.h>
int main(void)
{
	//variable declarations
	int SGK_iArray_One[5];
	int SGK_iArray_Two[5][3];
	int SGK_iArray_Three[100][100][5];

	int SGK_num_rows_2D;
	int SGK_num_columns_2D;

	int SGK_num_rows_3D;
	int SGK_num_columns_3D;
	int SGK_depth_3D;

	//code
	printf("\n\n");
	printf("Size of 1-D Integer Array iArray_One = %zu \n",sizeof(SGK_iArray_One));
	printf("Number of elements in 1-D integer array iArray_One = %zu\n",(sizeof(SGK_iArray_One)/sizeof(int)));

	printf("\n\n");
	printf("Size of 2-D integer array iArray_Two = %zu\n",sizeof(SGK_iArray_Two));

	printf("Number rows in 2-D integer array iArray_Two = %zu\n",(sizeof(SGK_iArray_Two)/sizeof(SGK_iArray_Two[0])));

	SGK_num_rows_2D = (sizeof(SGK_iArray_Two)/sizeof(SGK_iArray_Two[0]));

	printf("Number Of elements (columns) in each row in 2-D integer array iArray_Two = %zu\n",(sizeof(SGK_iArray_Two[0])/sizeof(SGK_iArray_Two[0][0])));

	SGK_num_columns_2D = (sizeof(SGK_iArray_Two[0])/sizeof(SGK_iArray_Two[0][0]));
	printf("Number of elements in total in 2-D Array iArray_Two = %d\n",(SGK_num_rows_2D * SGK_num_columns_2D));

	printf("\n\n");

	printf("\n\n");
	printf("Size of 3-D integer array iArray_Three = %zu\n",sizeof(SGK_iArray_Three));

	printf("Number rows in 3-D integer array iArray_Three = %zu\n",(sizeof(SGK_iArray_Three)/sizeof(SGK_iArray_Three[0])));
	SGK_num_rows_3D = (sizeof(SGK_iArray_Three)/sizeof(SGK_iArray_Three[0]));

	printf("Number of elements (columns) in one row in 3-D integer array iArray_Three = %zu\n",(sizeof(SGK_iArray_Three[0])/sizeof(SGK_iArray_Three[0][0])));

	SGK_num_columns_3D = (sizeof(SGK_iArray_Three[0])/sizeof(SGK_iArray_Three[0][0]));
	printf("Number of elements (depth) in one column in one row in 3-D integer array iArray_Three = %zu\n",(sizeof(SGK_iArray_Three[0][0])/sizeof(SGK_iArray_Three[0][0][0])));
	SGK_depth_3D = (sizeof(SGK_iArray_Three[0][0]))/sizeof(SGK_iArray_Three[0][0][0]);

	printf("Number of elements in total in 3-D Array iArray_Three = %d\n",(SGK_num_rows_3D * SGK_num_columns_3D * SGK_depth_3D));

	printf("\n\n");
	return(0);
}
