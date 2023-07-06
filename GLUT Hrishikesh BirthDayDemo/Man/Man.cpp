// header files
#include <GL/freeglut.h>
#include<math.h>
// global variable declarations
bool bIsFullScreen = false;

bool bWalk = false;
bool bLegOne = false;
bool bLegTwo = true;
bool bRightHand = false;
bool bLeftHand = true;

float vertexLegOneUpperSide[] = { 0.15f,-0.3f,0.0f };
float vertexLegOneBottomSide[] = { 0.2f,-0.6f,0.0f };

float vertexLegTwoUpperSide[] = { 0.05f,-0.3f,0.0f };
float vertexLegTwoBottomSide[] = { 0.0f,-0.6f,0.0f };

float rightArmElbow[] = {-0.1f,0.15f,0.0f};
float rightArmWrist[] = { -0.1f,-0.05f,0.0f };

float leftArmElbow[] = { 0.15f,0.15f,0.0f };
float leftArmWrist[] = { 0.2f,-0.05f,0.0f };

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

	glBegin(GL_QUADS);

	// left side of pant 
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.03f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.01f, -0.27f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, -0.27f, 0.0f);

	// right side of pant
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.03f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.11f, -0.27f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.2f, -0.27f, 0.0f);

	// upper body abdoman and chest
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.03f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.06f, 0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.14f, 0.32f, 0.0f);

	// upper body sholder
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.14f, 0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.06f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.06f, 0.32f, 0.0f);

	// upper body neck
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.06f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.06f, 0.45f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.45f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.4f, 0.0f);

	// face jaw
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.06f, 0.45f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.55f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.04f, 0.55f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.45f, 0.0f);

	// face hair
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.12f, 0.62f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.08f, 0.68f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.05f, 0.65f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.05f, 0.57f, 0.0f);

	glEnd();

	glBegin(GL_LINES); // face circle

	float radius = 0.08f;
	for (float angle = 0.0f; angle <= 360.0f; angle += 0.0001f)
	{
		float x = 0.03f + (radius * cos(angle));
		float y = 0.57f + (radius * sin(angle));

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.03f, 0.57f, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
	
	glLineWidth(10.0f);
	glBegin(GL_LINES);

	//1st leg upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegOneUpperSide[0], vertexLegOneUpperSide[1], vertexLegOneUpperSide[2]);


	//1st leg down part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegOneUpperSide[0], vertexLegOneUpperSide[1], vertexLegOneUpperSide[2]);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegOneBottomSide[0], vertexLegOneBottomSide[1], vertexLegOneBottomSide[2]);

	//2nd leg upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegTwoUpperSide[0], vertexLegTwoUpperSide[1], vertexLegTwoUpperSide[2]);

	//2nd leg down part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegTwoUpperSide[0], vertexLegTwoUpperSide[1], vertexLegTwoUpperSide[2]);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(vertexLegTwoBottomSide[0], vertexLegTwoBottomSide[1], vertexLegTwoBottomSide[2]);

	//right arm upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.051f, 0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(rightArmElbow[0], rightArmElbow[1], rightArmElbow[2]);

	//right arm lower part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(rightArmElbow[0], rightArmElbow[1], rightArmElbow[2]);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(rightArmWrist[0], rightArmWrist[1], rightArmWrist[2]);

	//right arm upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.051f, 0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(rightArmElbow[0], rightArmElbow[1], rightArmElbow[2]);

	//right arm upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.051f, 0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(rightArmElbow[0], rightArmElbow[1], rightArmElbow[2]);

	//left arm upper part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.13f,0.32f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(leftArmElbow[0], leftArmElbow[1], leftArmElbow[2]);

	//left arm lower part
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(leftArmElbow[0], leftArmElbow[1], leftArmElbow[2]);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(leftArmWrist[0], leftArmWrist[1], leftArmWrist[2]);

	glEnd();

	if (bWalk == true)
	{
		if (bLegOne == false)
		{
			if (vertexLegOneBottomSide[0] < 0.2f)
			{
				vertexLegOneUpperSide[0] = vertexLegOneUpperSide[0] + 0.001f;
				vertexLegOneBottomSide[0] = vertexLegOneBottomSide[0] + 0.006f;
			}
			else
			{
				bLegOne= true;
			}
		}
		else
		{
			if (vertexLegOneBottomSide[0] > 0.1f)
			{
				vertexLegOneUpperSide[0] = vertexLegOneUpperSide[0] - 0.001f;
				vertexLegOneBottomSide[0] = vertexLegOneBottomSide[0] - 0.006f;
			}
			else
			{
				bLegOne = false;
			}
		}

		if (bLegTwo == true)
		{
			if (vertexLegTwoBottomSide[0] > 0.0f)
			{
				vertexLegTwoUpperSide[0] = vertexLegTwoUpperSide[0] - 0.001f;
				vertexLegTwoBottomSide[0] = vertexLegTwoBottomSide[0] - 0.006f;
			}
			else
			{
				bLegTwo = false;
			}
		}
		else
		{
			if (vertexLegTwoBottomSide[0] < 0.1f)
			{
				vertexLegTwoUpperSide[0] = vertexLegTwoUpperSide[0] + 0.001f;
				vertexLegTwoBottomSide[0] = vertexLegTwoBottomSide[0] + 0.006f;
			}
			else
			{
				bLegTwo = true;
			}
		}

		if (bRightHand == true)
		{
			if (rightArmWrist[0] < 0.0f)
			{
				rightArmElbow[0] = rightArmElbow[0] + 0.001f;
				rightArmWrist[0] = rightArmWrist[0] + 0.006f;
			}
			else
			{
				bRightHand = false;
			}
		}
		else
		{
			if (rightArmWrist[0] > -0.1f)
			{
				rightArmElbow[0] = rightArmElbow[0] - 0.001f;
				rightArmWrist[0] = rightArmWrist[0] - 0.006f;
			}
			else
			{
				bRightHand = true;
			}
		}

		if (bLeftHand == true)
		{
			if (leftArmWrist[0] > 0.1f)
			{
				leftArmElbow[0] = leftArmElbow[0] - 0.001f;
				leftArmWrist[0] = leftArmWrist[0] - 0.006f;
			}
			else
			{
				bLeftHand= false;
			}
		}
		else
		{
			if (leftArmWrist[0] < 0.2f)
			{
				leftArmElbow[0] = leftArmElbow[0] + 0.001f;
				leftArmWrist[0] = leftArmWrist[0] + 0.006f;
			}
			else
			{
				bLeftHand = true;
			}
		}
	}
	

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
	{
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
	}
	break;
	case 'W':
	case 'w':
	{
		if (bWalk == false)
		{
			bWalk = true;
		}
		else
		{
			bWalk = false;
		}
	}
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

