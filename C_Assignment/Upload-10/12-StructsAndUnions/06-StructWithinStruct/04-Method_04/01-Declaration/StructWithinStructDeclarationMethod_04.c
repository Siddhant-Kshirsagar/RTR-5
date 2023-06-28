#include<stdio.h>

int main(void)
{
	//variable declarations
	int SGK_length, SGK_breadth, SGK_area;

	struct MyPoint
	{
		int SGK_x;
		int SGK_y;
	};

	struct Rectangle
	{
		struct MyPoint SGK_point_01;
		struct MyPoint SGK_point_02;
	};

	struct Rectangle SGK_rect;

	//code 
	printf("\n\n");
	printf("Enter Leftmost X-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_01.SGK_x);

	printf("\n\n");
	printf("Enter Bottommost Y-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_01.SGK_y);

	printf("\n\n");
	printf("Enter Rightmost X-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_02.SGK_x);

	printf("\n\n");
	printf("Enter Topmost Y-Coordinate Of Rectangle : ");
	scanf("%d", &SGK_rect.SGK_point_02.SGK_y);

	SGK_length = SGK_rect.SGK_point_02.SGK_y - SGK_rect.SGK_point_01.SGK_y;
	if (SGK_length < 0)
		SGK_length = SGK_length * -1;

	SGK_breadth = SGK_rect.SGK_point_02.SGK_x - SGK_rect.SGK_point_01.SGK_x;
	if (SGK_breadth < 0)
		SGK_breadth = SGK_breadth * -1;

	SGK_area = SGK_length * SGK_breadth;

	printf("\n\n");
	printf("Length Of Rectangle = %d\n\n",SGK_length);
	printf("Breadth Of Rectangle = %d\n\n",SGK_breadth);
	printf("Area Of Rectangle = %d\n\n",SGK_area);

	return(0);
}
