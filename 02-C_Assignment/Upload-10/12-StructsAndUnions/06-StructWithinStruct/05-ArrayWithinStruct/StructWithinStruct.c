#include<stdio.h>

struct MyNumber
{
	int SGK_num;
	int SGK_num_table[10];
};

struct NumTables
{
	struct MyNumber SGK_a;
	struct MyNumber SGK_b;
	struct MyNumber SGK_c;
};

int main(void)
{
	//variable declarations
	struct NumTables SGK_tables;
	int SGK_i;

	//code
	SGK_tables.SGK_a.SGK_num = 2;
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_tables.SGK_a.SGK_num_table[SGK_i] = SGK_tables.SGK_a.SGK_num * (SGK_i + 1);
	printf("\n\n");
	printf("Table Of %d : \n\n",SGK_tables.SGK_a.SGK_num);
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("%d * %d = %d\n", SGK_tables.SGK_a.SGK_num, (SGK_i + 1), SGK_tables.SGK_a.SGK_num_table[SGK_i]);

	SGK_tables.SGK_b.SGK_num = 3;
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_tables.SGK_b.SGK_num_table[SGK_i] = SGK_tables.SGK_b.SGK_num * (SGK_i + 1);
	printf("\n\n");
	printf("Table Of %d : \n\n", SGK_tables.SGK_b.SGK_num);
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("%d * %d = %d\n", SGK_tables.SGK_b.SGK_num, (SGK_i + 1), SGK_tables.SGK_b.SGK_num_table[SGK_i]);

	SGK_tables.SGK_c.SGK_num = 4;
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		SGK_tables.SGK_c.SGK_num_table[SGK_i] = SGK_tables.SGK_c.SGK_num * (SGK_i + 1);
	printf("\n\n");
	printf("Table Of %d : \n\n",SGK_tables.SGK_c.SGK_num);
	for (SGK_i = 0; SGK_i < 10; SGK_i++)
		printf("%d * %d = %d\n", SGK_tables.SGK_c.SGK_num, (SGK_i + 1), SGK_tables.SGK_c.SGK_num_table[SGK_i]);

	return(0);
}
