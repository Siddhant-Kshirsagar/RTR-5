#include<stdio.h>
int main(void)
{

	//code 
	printf("\n\n");

	printf("Size of 'int'			= %lld bytes\n",sizeof(int));
	printf("Size of 'unsigned int'  = %lld bytes\n",sizeof(unsigned int));
	printf("Size of 'long'			= %lld bytes\n",sizeof(long));
	printf("Size of 'long long'		= %lld bytes\n",sizeof(long long));

	printf("Size of 'float'			= %lld bytes\n",sizeof(float));
	printf("Size of 'double'		= %lld bytes\n",sizeof(double));
	printf("Size of 'long double'	= %lld bytes\n",sizeof(long double));

	printf("\n\n");
	
	return(0);
}
