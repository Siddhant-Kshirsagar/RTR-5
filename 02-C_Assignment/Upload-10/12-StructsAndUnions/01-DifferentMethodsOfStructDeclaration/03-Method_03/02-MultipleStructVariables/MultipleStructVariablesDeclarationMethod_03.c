#include<stdio.h>

int main(void)
{
	struct MyPoint
	{
		int SGK_x;
		int SGK_y;
	}SGK_point_A,SGK_point_B,SGK_point_C,SGK_point_D,SGK_point_E; // Declararing 5 Struct variable of type 'struct MyPoint' locally...

	//code 
	//Assigning Data Values To The Data Members Of 'struct MyPoint' variable 'point_A'
	SGK_point_A.SGK_x = 3;
	SGK_point_A.SGK_y = 0;

	//Assigning Data Values To The Data Members Of 'struct MyPoint' variable 'point_B'
	SGK_point_B.SGK_x = 1;
	SGK_point_B.SGK_y = 2;

	//Assigning Data Values To The Data Members Of 'struct MyPoint' variable 'point_C'
	SGK_point_C.SGK_x = 9;
	SGK_point_C.SGK_y = 6;

	//Assigning Data values To The Data Member Of 'struct MyPoint' variable 'point_D' 
	SGK_point_D.SGK_x = 8;
	SGK_point_D.SGK_y = 2;

	//Assigning Data values To The Data Member Of 'struct MyPoint' variable 'point_E'
	SGK_point_E.SGK_x = 11;
	SGK_point_E.SGK_y = 8;

	//Displaying Values Of The Data Members Of 'struct MyPoint' (all variables) 
	printf("\n\n");
	printf("Co-ordinates (x,y) Of Point 'A' Are : (%d,%d)\n\n", SGK_point_A.SGK_x, SGK_point_A.SGK_y);
	printf("Co-ordinates (x,y) Of Point 'B' Are : (%d,%d)\n\n", SGK_point_B.SGK_x, SGK_point_B.SGK_y);
	printf("Co-ordinates (x,y) Of Point 'C' Are : (%d,%d)\n\n", SGK_point_C.SGK_x, SGK_point_C.SGK_y);
	printf("Co-ordinates (x,y) Of Point 'D' Are : (%d,%d)\n\n", SGK_point_D.SGK_x, SGK_point_D.SGK_y);
	printf("Co-ordinates (x,y) Of Point 'E' Are : (%d,%d)\n\n", SGK_point_E.SGK_x, SGK_point_E.SGK_y);

	return(0);
}
