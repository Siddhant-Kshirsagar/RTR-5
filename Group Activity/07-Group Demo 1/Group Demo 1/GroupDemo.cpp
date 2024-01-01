// header files
#include <GL/freeglut.h>
#include<math.h>//for sin and cos

// global variable declarations
bool bIsFullScreen = false;

char* c_gl_Points = "GL_POINTS";
char* c_gl_Lines = "GL_LINES";
char* c_gl_Polygon = "GL_POLYGON";
char* c_gl_Triangle ="GL_TRIANGLES";

// Entry-point function
int main(int argc, char* argv[])
{
	// function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	// code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First RTR5 Program : Siddhant Ganesh Kshirsagar");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return(0);

}

void initialize(void)
{
	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	// code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.9f, 0.5f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.9f, -0.5f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, -0.5f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.5f, 0.0f);

	glEnd();

	//bottom star upward triangle 
	glBegin(GL_TRIANGLES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.4f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -0.4f, 0.0f);

	glEnd();
	//bottom star downward triangle 
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.35f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.45f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -0.35f, 0.0f);

	glEnd();

	glLineWidth(2.5);
	glBegin(GL_LINES);

	//right side square 1
	//left vertical line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, 0.07f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, -0.07f, 0.0f);

	//right vertical line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.01f, -0.07f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.01f, 0.07f, 0.0f);
	
	//bottom line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, -0.07f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.01f, -0.07f, 0.0f);

	//top line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, 0.07f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.01f, 0.07f, 0.0f);

	//Inside square
	//vertical line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.04f, 0.07f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.04f, -0.07f, 0.0f);

	//horizontal line
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.01f, 0.0f, 0.0f);

	glEnd();

	glLineWidth(2.5f);
	glBegin(GL_LINES);

	// left vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.95f, 0.57f, 0.0f);
	

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.95f, -0.57f, 0.0f);
	
	//top horrizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.95f, 0.57f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.57f, 0.0f);

	//Right vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.57f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	// right horizontal
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.18f, 0.00f, 0.0f);

	// right slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.18f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, -0.05f, 0.0f);

	//bottom vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, -0.05f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, -0.57f, 0.0f);
	//bottom horrizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.95f, -0.57f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, -0.57f, 0.0f);

	glEnd();
	// GL_LINES text for box 1
	glRasterPos2f(0.40f, 0.45f);
	char* c;
	for (c = c_gl_Lines; *c !='\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}

	//Right side blocks
	glLineWidth(2.5f);
	
	glBegin(GL_LINES);
	// 1 BOX
	// Start left slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, 0.57f, 0.0f);


	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.53f, 0.0f);

	//left vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.53f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.35f, 0.0f);

	//bottom horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, 0.35f, 0.0f);

	// bottom right slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, 0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.39f, 0.0f);

	// right vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.39f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.57f, 0.0f);

	//top horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.57f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, 0.57f, 0.0f);

	glEnd();


	glRasterPos2f(0.40f, 0.13f);
	for (c = c_gl_Points; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glBegin(GL_LINES);
	// 2 BOX
	// Start left slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, 0.25f, 0.0f);


	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.21f, 0.0f);

	//left vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.21f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.05f, 0.0f);

	//bottom horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, 0.05f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, 0.05f, 0.0f);

	// bottom right slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, 0.05f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.09f, 0.0f);

	// right vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.09f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.25f, 0.0f);

	//top horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, 0.25f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, 0.25f, 0.0f);

	glEnd();

	//text for polygon
	glRasterPos2f(0.38f, -0.16f);
	for (c = c_gl_Polygon; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glBegin(GL_LINES);
	// 3 BOX
	// Start left slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, -0.04f, 0.0f);


	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.08f, 0.0f);

	//left vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.08f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);

	//bottom horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, -0.25f, 0.0f);

	// bottom right slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, -0.25f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.21f, 0.0f);

	// right vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.21f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.04f, 0.0f);

	//top horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.04f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, -0.04f, 0.0f);

	glEnd();
	

	//text for triangle
	glRasterPos2f(0.38f, -0.46f);
	for (c = c_gl_Triangle; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glBegin(GL_LINES);
	// 4 BOX
	// Start left slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, -0.35f, 0.0f);


	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.39f, 0.0f);

	//left vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.39f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.55f, 0.0f);

	//bottom horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.25f, -0.55f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, -0.55f, 0.0f);

	// bottom right slope line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.85f, -0.55f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.51f, 0.0f);

	// right vertical line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.51f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.35f, 0.0f);

	//top horizontal line
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.88f, -0.35f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.28f, -0.35f, 0.0f);

	glEnd();


	//lines inside circle
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.53f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.47f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.03f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.03f, 0.0f);

	glEnd();

	//hexagon
	glBegin(GL_LINES);

	//left top slope 
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, 0.25f, 0.0f);

	//left vertical
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, 0.25f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, 0.15f, 0.0f);

	//left bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, 0.15f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.10f, 0.0f);

	//right bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.10f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.15f, 0.0f);

	//right vertical
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.15f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.25f, 0.0f);

	//right top slope 
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.25f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.3f, 0.0f);

	//lines inside hexagon
	// top vertical 
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);

	//right slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.15f, 0.0f);

	//left slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.8f, 0.15f, 0.0f);

	glEnd();


	glBegin(GL_LINES);

	//right top slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, 0.15f, 0.0f);

	//right bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, 0.15f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.05f, 0.0f);

	//left bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.05f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.15f, 0.0f);

	//left top slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.15f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.2f, 0.0f);

	//left bottom slope of kite
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.05f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.35f, -0.0f, 0.0f);

	// bottom horrizontal line 
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.35f, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.25f, -0.0f, 0.0f);

	//left top slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.25f, -0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.05f, 0.0f);

	//vertical line inside kite
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.05f, 0.0f);
	//horrizontal line inside kite
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, 0.15f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.15f, 0.0f);

	glEnd();

	//bottom diamond
	glLineWidth(10.5f);
	glBegin(GL_LINES);
	//right top slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.3f, 0.0f);

	//right bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.4f, 0.0f);

	//left bottom slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.4f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);

	//left top slope
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.2f, 0.0f);

	glEnd();

	glPointSize(3.0f);
	glBegin(GL_POINTS);
	float radianAngle;
	float radius = 0.05f;  // radius of a circle 
	float center_x = -0.5f;
	float center_y = 0.0f;
	float radius_1 = 0.02f;
	float center_x_1 = -0.2f;
	float center_y_1 = 0.3f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.001f)
	{
		//radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(angle) + center_x; 
		float y = radius * sin(angle) + center_y;

		float x_1 = radius_1 * cos(angle) + center_x_1;
		float y_1 = radius_1 * sin(angle) + center_y_1;

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x_1, y_1, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glLineWidth(0.50f);
	glBegin(GL_LINES);
	float radius_2 = 0.1f;
	float center_x_2 = -0.75f;
	float center_y_2 = -0.3f;

	for (float angle = 0.0f; angle <= 180.0f; angle = angle + 5.0f)
	{

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(center_x_2, center_y_2, 0.0f);

		float x_2 = radius_2 * cos(angle) + center_x_2;
		float y_2 = radius_2 * sin(angle) + center_y_2;

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x_2, y_2, 0.0f);
	}

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
	// code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;

		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	// code 
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void)
{
	// code
}

