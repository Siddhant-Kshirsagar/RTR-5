#include<stdio.h>

struct MyNumber
{
	int SGK_num;
	int SGK_num_table[10];
};

struct NumTables
{
	struct MyNumber SGK_n;
};

int main(void)
{
	//variable declarations
	struct NumTables SGK_tables[10]; // an array of 10 'struct NumTables'
	int SGK_i, SGK_j;

	//code
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
	{
		SGK_tables[SGK_i].SGK_n.SGK_num = (SGK_i + 1);
	}

	for (SGK_i = 0; SGK_i < 10; SGK_i++)
	{
		printf("\n\n");
		printf("Table Of %d : \n\n", SGK_tables[SGK_i].SGK_n.SGK_num);
		for (SGK_j = 0; SGK_j < 10; SGK_j++)
		{
			SGK_tables[SGK_i].SGK_n.SGK_num_table[SGK_j] = SGK_tables[SGK_i].SGK_n.SGK_num * (SGK_j + 1);
			printf("%d * %d = %d\n", SGK_tables[SGK_i].SGK_n.SGK_num, (SGK_j + 1), SGK_tables[SGK_i].SGK_n.SGK_num_table[SGK_j]);
		}
	}
	return(0);
}
