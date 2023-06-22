// header files
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include<math.h>// for sin, cos, M_PI
// global variable declarations
bool bIsFullScreen = false;
float x_axis = 0.0f;
float y_axis = 0.0f;
float movement = 0.01f;
bool reverse = false;

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
	glBegin(GL_LINES);
	float radianAngle;
	float radius = 0.5f;  // radius of a circle 

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.001f)
	{
		if (reverse == false)
		{
			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(x_axis, y_axis, 0.0f);
		}
		else
		{
			glColor3f(0.5f, 1.0f, 1.0f);
			glVertex3f(x_axis, y_axis, 0.0f);
		}

		//radianAngle = angle * (M_PI * 180.0f);
		 
		float x = radius * cos(angle); //
		float y = radius * sin(angle);

		
		x = x + x_axis;
		y = y + y_axis;

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}

	/*else if (y_axis == 0.8f)
	glColor3f(0.0f, 1.0f, 1.0f);
	else if (y_axis == -0.8f)*/
	/*else
	glColor3f(1.0f, 1.0f, 0.0f);*/
	

	if (reverse == false)
	{
			y_axis = y_axis - movement;
	
			x_axis = x_axis + movement;

			if (x_axis > 0.8f || y_axis >0.8f)
			reverse = true;
	}
	else{
		y_axis = y_axis + movement;

		x_axis = x_axis - movement;
		if (x_axis < -0.8f || y_axis < -0.8f)
			reverse = false;
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

