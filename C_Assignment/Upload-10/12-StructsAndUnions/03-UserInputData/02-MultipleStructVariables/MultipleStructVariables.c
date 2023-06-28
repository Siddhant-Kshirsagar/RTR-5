#include<stdio.h>

struct MyPoint
{
	int SGK_x;
	int SGK_y;
};

int main(void)
{
	//variable declarations
	struct MyPoint SGK_point_A, SGK_point_B, SGK_point_C, SGK_point_D, SGK_point_E; // declaring a 5 struct variables

	//code
	//User Input For The Data Members Of 'struct MyPoint' variable 'point_A'
	printf("\n\n");
	printf("Enter X-Coordinate For Point 'A' : ");
	scanf("%d", &SGK_point_A.SGK_x);	
	printf("Enter Y-Coordinate For Point 'A' : ");
	scanf("%d", &SGK_point_A.SGK_y);

	//User Input For The Data Members Of 'struct MyPoint' variable 'point_B'
	printf("\n\n");
	printf("Enter X-Coordinate For Point 'B' : ");
	scanf("%d", &SGK_point_B.SGK_x);
	printf("Enter Y-Coordinate For Point 'B' : ");
	scanf("%d", &SGK_point_B.SGK_y);

	//User Input For The Data Members Of 'struct MyPoint' variable 'point_C'
	printf("\n\n");
	printf("Enter X-Coordinate For Point 'C' : ");
	scanf("%d", &SGK_point_C.SGK_x);
	printf("Enter Y-Coordinate For Point 'C' : ");
	scanf("%d", &SGK_point_C.SGK_y);

	//User Input For The Data Members Of 'struct MyPoint' variable 'point_D'
	printf("\n\n");
	printf("Enter X-Coordinate For Point 'D' : ");
	scanf("%d", &SGK_point_D.SGK_x);
	printf("Enter Y-Coordinate For Point 'D' : ");
	scanf("%d", &SGK_point_D.SGK_y);

	//User Input For The Data Members Of 'struct MyPoint' variable 'point_E'
	printf("\n\n");
	printf("Enter X-Coordinate For Point 'E' : ");
	scanf("%d", &SGK_point_E.SGK_x);
	printf("Enter Y-Coordinate For Point 'E' : ");
	scanf("%d", &SGK_point_E.SGK_y);

	//Displaying Values Of The Data Members Of 'struct MyPoint' (all Variables)
	printf("\n\n");
	printf("Co-ordinates (x,y) of Point 'A' Are : (%d,%d)\n\n",SGK_point_A.SGK_x,SGK_point_A.SGK_y);
	printf("Co-ordinates (x,y) of Point 'B' Are : (%d,%d)\n\n", SGK_point_B.SGK_x, SGK_point_B.SGK_y);
	printf("Co-ordinates (x,y) of Point 'C' Are : (%d,%d)\n\n", SGK_point_C.SGK_x, SGK_point_C.SGK_y);
	printf("Co-ordinates (x,y) of Point 'D' Are : (%d,%d)\n\n", SGK_point_D.SGK_x,SGK_point_D.SGK_y);
	printf("Co-ordinates (x,y) of Point 'E' Are : (%d,%d)\n\n", SGK_point_E.SGK_x, SGK_point_E.SGK_y);

	return(0);
}
