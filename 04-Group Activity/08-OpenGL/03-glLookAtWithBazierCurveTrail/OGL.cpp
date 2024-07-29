//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

#define _USE_MATH_DEFINES
#include<math.h>

#include<vector>

//OpenGL Header
#include<gl/GL.h>
#include<GL/glu.h>

#include"OGL.h"

//OpenGl Related Global variable
HDC ghdc = NULL;
HGLRC ghrc = NULL;

//Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // initialization of struct => this work on all type (if we want to initialize all value to 0)
BOOL gbFullscreen = FALSE;
FILE *gpFILE = NULL;

HWND ghwnd = NULL; // g = global handle of window
BOOL gbActive = FALSE; 

GLfloat pAngle = 0.0f;

//for camera moment
GLfloat objx = 0.0f;
GLfloat objy = 0.0f;
GLfloat objz = 5.0f;
GLfloat incrementFactor = 0.1f;
BOOL cameraReached = FALSE;

//for snow animation
GLfloat snowX = -2.0f;
GLfloat snowY = 2.0f;
GLfloat snowZ = -5.0f;
BOOL snowAnimation = FALSE;


int timer = 0;
bool bEnableBazierCamera = false;
std::vector<float> BazierPoints;
float controlPoints[7][3] = {
								{0.0f,2.0f,3.0f},
								{-20.0f,1.0f,-7.0f},
								{20.0f,5.0f,-18.0f},
								{0.0f,2.5f,-34.0f},
								{-20.0f,0.0f,-50.0f},
								{-18.0f,5.0f,-18.0f},
								{20.0f,0.0f,-29.0f}
							};

//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//Function declaration
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//local variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SGKWindow");

	int iResult = 0;

	BOOL bDone = FALSE; // For Game Loop

	int SystemWindowWidth, SystemWindowHeight;
	int x_Coordinate, y_Coordinate;

	// get system window width and height
	SystemWindowWidth = GetSystemMetrics(SM_CXSCREEN);
	SystemWindowHeight = GetSystemMetrics(SM_CYSCREEN);
	// get x and y coordinate for show window in center on screen
	x_Coordinate = (SystemWindowWidth / 2) - (WIN_WIDTH / 2);
	y_Coordinate = (SystemWindowHeight / 2) - (WIN_HEIGHT / 2);

	//code
	gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log file cannot be open"), TEXT("Error"), MB_OK|MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully\n");

	//WNDCLASSEX Initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register WNDCLASSEX
	RegisterClassEx(&wndclass);

	//Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Siddhant Ganesh Kshirsagar"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x_Coordinate,
		y_Coordinate,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	//Initialization 
	iResult = initialize();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() failed."), TEXT("Error"), MB_OK|MB_ICONERROR);
		DestroyWindow(hwnd);
	}

	//show window
	ShowWindow(hwnd, iCmdShow);

	// set window first in z-order (foreground)
	SetForegroundWindow(hwnd);

	// set focus on this window(internally send 'WM_SETFOCUS' message to WndProc())
	SetFocus(hwnd);

	//Game Loop
	while(bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// code
			if (gbActive == TRUE) // If Window is in focus then 'WM_SETFOCUS' is send to WndProc() and We Handled this message and set gbActive = true 
			{
				//Render
				display();

				//Update
				update();
			}

		}
	}

	// Uninitialization
	uninitialize();

	return((int)msg.wParam);
}

//Callback function defination
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function declarations
	void ToggleFullScreen(void);
	void resize(int, int);

	switch (iMsg)
	{
	case WM_SETFOCUS: // when window is in focus (window is activate)
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS: // when window is not in focus(window is deactivate)
		gbActive = FALSE;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam),HIWORD(lParam));
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		default:
			break;
		}
	}
	break;
	case WM_CHAR:
	{
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullscreen == FALSE)
			{
				ToggleFullScreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullScreen();
				gbFullscreen = FALSE;
			}
			break;
		case 'W':
		case 'w':
			objz += incrementFactor;
			break;
		case 'S':
		case 's':
			objz -= incrementFactor;
			break;
		case 'A':
		case 'a':
			objx -= incrementFactor;
			break;
		case 'D':
		case 'd':
			objx += incrementFactor;
			break;
		case 'Q':
		case 'q':
			objy -= incrementFactor;
			break;
		case 'E':
		case 'e':
			objy += incrementFactor;
			break;
		case 'C':
		case 'c':
			bEnableBazierCamera = !bEnableBazierCamera;
			break;
		default:
			break;
		}
	}
	break;
	
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	//local variable declaration
	MONITORINFO mi = { sizeof(MONITORINFO) };

	//code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

int initialize(void)
{
	//function declarations
	void resize(int width, int height);
	
	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//step 1 :- Initialization of PIXELFORMATDESCRIPTOR
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// step 2 :- Get DC 
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC() Failed.\n");
		return(-1);
	}

	// step 3 :- Os will return pixel format index which is closest match to our pixel format descriptor
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0) // ChoosePixelFormat() return non zero positive on success , on failure return 0
	{
		fprintf(gpFILE, "ChoosePixelFormat() Failed.\n");
		return(-2);
	}

	// step 4 :- Set obtained pixel format
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() Failed.\n");
		return(-3);
	}

	// step 5 :- Create OpenGL Context From Device Context(ghdc)
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContext() Failed.\n");
		return(-4);
	}

	// step 6 :- Make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE,"wglMakeCurrent() Failed.\n");
		return(-5);
	}
	
	//for enable depth
	glShadeModel(GL_SMOOTH); // optional (beautyfication color,light,texture shade)
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // optional (beautyfication for artifact e.g if we draw circle or sphere we see ellipse curve but we don't need that so we disable that)

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	resize(WIN_WIDTH,WIN_HEIGHT);
	//for camera
	float x, y, z;
	for (int i = 0; i < 2; i++)
	{
		for (float t = 0.0f; t <= 1.0f; t = t + 0.003f)
		{

			if (cameraReached == FALSE)
			{
				x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
				y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
				z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

			}
			else
			{
				x = pow((1 - t), 3) * controlPoints[3][0] + 3 * pow((1 - t), 2) * t * controlPoints[4][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[5][0] + pow(t, 3) * controlPoints[6][0];
				y = pow((1 - t), 3) * controlPoints[3][1] + 3 * pow((1 - t), 2) * t * controlPoints[4][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[5][1] + pow(t, 3) * controlPoints[6][1];
				z = pow((1 - t), 3) * controlPoints[3][2] + 3 * pow((1 - t), 2) * t * controlPoints[4][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[5][2] + pow(t, 3) * controlPoints[6][2];

			}

			BazierPoints.push_back(x);
			BazierPoints.push_back(y);
			BazierPoints.push_back(z);
		}

		if (cameraReached)
		{
			cameraReached = FALSE;
		}
		else
		{
			cameraReached = TRUE;
		}
	}
	

	return(0);
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void display(void)
{
	//function declaration
	//1st param x coordinate of center of tree
	//2nd param y coordinate of center of tree
	//3rd param z coordinate of center of tree
	void drawTree(float, float, float);
	//1st param x coordinate of center of point
	//2nd param y coordinate of center of point
	//3rd param z coordinate of center of point
	void drawPoint(float, float, float);
	

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*if (cameraReached == FALSE)
	{*/
		if ((timer + 3) + 2 < BazierPoints.size())
		{
			timer = timer + 3;
		}
		else
		{
			cameraReached = TRUE;
			//timer = 0;
		}
	//}
	//else
	//{
	//	if ((timer + 6) + 2 < BazierPoints.size())
	//	{
	//		timer = timer + 6;
	//	}
	//	else
	//	{
	//		cameraReached = FALSE;
	//		//timer = 0;
	//	}
	//}


	if (bEnableBazierCamera)
	{
		gluLookAt(objx, objy, objz, 0.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
			gluLookAt(BazierPoints[timer], BazierPoints[timer + 1], BazierPoints[timer + 2], BazierPoints[(timer + 3)], BazierPoints[(timer + 3) + 1], BazierPoints[(timer + 3) + 2], 0.0f, 1.0f, 0.0f);	
	}
	
	glEnable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	// Display Bazier Points
	//glBegin(GL_POINTS);
	/*for (float t = 0.0f; t <= 1.0f; t = t + 0.001f)
	{
		float x = pow((1 - t), 3) * controlPoints[0][0] + 3 * pow((1 - t), 2) * t * controlPoints[1][0] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][0] + pow(t, 3) * controlPoints[3][0];
		float y = pow((1 - t), 3) * controlPoints[0][1] + 3 * pow((1 - t), 2) * t * controlPoints[1][1] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][1] + pow(t, 3) * controlPoints[3][1];
		float z = pow((1 - t), 3) * controlPoints[0][2] + 3 * pow((1 - t), 2) * t * controlPoints[1][2] + 3 * (1 - t) * pow(t, 2) * controlPoints[2][2] + pow(t, 3) * controlPoints[3][2];

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, z);
	}*/
	/*for (int i = 0; i < BazierPoints.size(); i = i + 3)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(BazierPoints[i], BazierPoints[i+1], BazierPoints[i + 2]);
	}
	glEnd();*/

	//display control points
	//glPointSize(4.0f);
	//glBegin(GL_POINTS);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(controlPoints[0][0], controlPoints[0][1], controlPoints[0][2]);
	//glVertex3f(controlPoints[1][0], controlPoints[1][1], controlPoints[1][2]);
	//glVertex3f(controlPoints[2][0], controlPoints[2][1], controlPoints[2][2]);
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(controlPoints[3][0], controlPoints[3][1], controlPoints[3][2]);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(controlPoints[4][0], controlPoints[4][1], controlPoints[4][2]);
	//glVertex3f(controlPoints[5][0], controlPoints[5][1], controlPoints[5][2]);
	//glVertex3f(controlPoints[6][0], controlPoints[6][1], controlPoints[6][2]);
	////glVertex3f(controlPoints[3][0], controlPoints[3][1], controlPoints[3][2]);
	//glEnd();

	glBegin(GL_QUADS);
	//bottom face
	glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
	glVertex3f(20.0f, -1.0f, -50.0f); // right top
	glVertex3f(-20.0f, -1.0f, -50.0f);// left top
	/*glColor3f(213.0f / 255.0f, 216.0f / 255.0f, 220.0f / 255.0f);*/
	glVertex3f(-20.0f, -1.0f, 5.0f);//left bottom
	glVertex3f(20.0f, -1.0f, 5.0f);// right bottom

	//back face
	glColor3f(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	glVertex3f(-20.0f, 12.0f, -50.0f); // right top
	glVertex3f(20.0f, 12.0f, -50.0f);// left top
	glColor3f(236.0f/255.0f, 240.0f/255.0f, 241.0f/255.0f);
	glVertex3f(20.0f, -1.0f, -50.0f);//left bottom
	glVertex3f(-20.0f, -1.0f, -50.0f);// right bottom

	//right face
	glColor3f(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	glVertex3f(20.0f, 12.0f, -50.0f); // right top
	glVertex3f(20.0f, 12.0f, 5.0f);// left top
	glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
	glVertex3f(20.0f, -1.0f, 5.0f);//left bottom
	glVertex3f(20.0f, -1.0f, -50.0f);// right bottom

	//left face
	glColor3f(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	glVertex3f(-20.0f, 12.0f, 5.0f); // right top
	glVertex3f(-20.0f, 12.0f, -50.0f);// left top
	glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
	glVertex3f(-20.0f, -1.0f, -50.0f);//left bottom
	glVertex3f(-20.0f, -1.0f, 5.0f);// right bottom

	//top face
	glColor3f(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	glVertex3f(20.0f, 12.0f, -50.0f); // right top
	glVertex3f(-20.0f, 12.0f, -50.0f);// left top
	//glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
	glVertex3f(-20.0f, 12.0f, 5.0f);//left bottom
	glVertex3f(20.0f, 12.0f, 5.0f);// right bottom

	//front face
	glColor3f(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f);
	glVertex3f(20.0f, 12.0f, 5.0f); // right top
	glVertex3f(-20.0f, 12.0f, 5.0f);// left top
	glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
	glVertex3f(-20.0f, -1.0f, 5.0f);//left bottom
	glVertex3f(20.0f, -1.0f, 5.0f);// right bottom

	glEnd();
	

	for (float x = -12.0f; x <= 12.0f; x = x + 3.0f)
	{
		for (float z = 0.0f; z >= -41.0f; z = z - 4.9f)
		{
			/*for (float y = (0.0f + 0.6f); y <= 2.4f; y = y + 0.4f)
			{*/
				drawTree(x, 0.0f, z);

			//}
		}
	}

	for (float x = -20.0f; x <= 20.0f; x = x + 4.0f)
	{
		//glTranslatef(0.0f, 0.0f, snowZ);
		for (float z = -50.0f; z <= 5.0f; z = z + 5.0f)
		{
			
			for (float y = 12.0f; y >= -1.0f; y = y - 2.5f)
			{
				drawPoint(x+snowX, y+snowY, z+snowZ);

			}
		}
	}

	SwapBuffers(ghdc);
}

void update(void)
{
	//code
	/*pAngle = pAngle + 1.0f;
	if (pAngle >= 360.0f)
	{
		pAngle = pAngle - 360.0f;
	}*/
	snowY = snowY - 0.01f;
	if (snowY <= -5.0f)
	{
		snowY = 2.0f;
	}
	
	if (snowAnimation)
	{
		if (snowX <= 2.0f)
		{
			snowX = snowX + 0.05f;
		}
		else
		{
			snowAnimation = FALSE;
		}
		
		if (snowZ <= 5.0f)
		{
			snowZ = snowZ + 0.03f;
		}
	}
	else
	{

		if (snowX >= -2.0f)
		{
			snowX = snowX - 0.05f;
		}
		else
		{
			snowAnimation = TRUE;
		}
		if (snowZ >= -5.0f)
		{
			snowZ = snowZ - 0.03f;
		}
	}
	
}

void uninitialize(void)
{
	//function declarations
	void ToggleFullScreen(void);
	//code

	// If Application is exitting in fullscreen mode
	if (gbFullscreen == TRUE)
	{
		ToggleFullScreen();
		gbFullscreen = FALSE;
	}

	//Make ghdc as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	//Delete Rendering Context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// Release the ghdc
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	// Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	// Close log file
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}

void drawPoint(float xAxis, float yAxis, float zAxis)
{
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.99f, 0.98f);
	glVertex3f(xAxis, yAxis, zAxis);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}

void drawTree(float xAxis, float yAxis, float zAxis)
{
	// 1st param x axis of center of circle
	// 2nd param y axis of center of circle
	// 3rd param z axis of center of circle
	// 4th param minimum angle of circle arc
	// 5th param maximum angle of circle arc
	// 6th param thickness of circle for 3d effect
	// 7th param radius of circle
	void SGK_drawCylinder(float, float, float, float, float, float, float);
	// 1st param x axis of center of circle
	// 2nd param y axis of center of circle
	// 3rd param z axis of center of circle
	// 4th param minimum angle of circle arc
	// 5th param maximum angle of circle arc
	// 6th param thickness of circle for 3d effect
	// 7th param radius of circle
	void SGK_drawCone(float, float, float, float, float, float, float);

	float radius = 0.8f;
	/*for (float x = 0.0f; x <= 5.0f; x = x + 2.0f)
	{
		for (float z = 0.0f; z >= -5.0f; z = z - 2.0f)
		{*/
			for (float y = (yAxis + 0.6f),radius = 0.8f; y <= 2.4f; y = y + 0.5f,radius= radius - 0.1f)
			{
				SGK_drawCone(xAxis, y, zAxis, 0.0f, 360.0f, 0.8f, radius);
				
			}
		//}
	
	//}
	
	
	SGK_drawCylinder(xAxis, yAxis, zAxis, 0.0f, 360.0f, 1.5f, 0.1f);
	
}

void SGK_drawCone(float xAxis, float yAxis, float zAxis, float startAngle, float endAngle, float fDepth, float fRadius)
{
	glBegin(GL_LINES);
	for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle = fAngle + 0.1f)
	{
		float x = xAxis + fRadius * sin((fAngle * M_PI) / 180.0f);
		float y = zAxis + fRadius * cos((fAngle * M_PI) / 180.0f);

		//front face
		glColor3f(236.0f / 255.0f, 240.0f / 255.0f, 241.0f / 255.0f);
		glVertex3f(xAxis, yAxis, zAxis);
		glColor3f(1.0f / 255.0f, 50.0f / 255.0f, 32.0f / 255.0f);
		glVertex3f(x, yAxis -0.8f, y);
		glVertex3f(xAxis,yAxis - 0.5f, zAxis);
		glVertex3f(x, yAxis - 0.8f, y);
	}
	glEnd();
}

void SGK_drawCylinder(float xAxis, float yAxis, float zAxis, float startAngle, float endAngle, float fDepth, float fRadius)
{
	float x, y;
	if (startAngle < endAngle)
	{
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINES);
		for (float fAngle = startAngle; fAngle <= endAngle; fAngle = fAngle + 0.1f)
		{
			x = xAxis + fRadius * sin((fAngle * M_PI) / 180.0f);
			y = zAxis + fRadius * cos((fAngle * M_PI) / 180.0f);

			// up face
			glColor3f(147.0f/255.0f, 81.0f/255.0f, 22.0f/255.0f);
			glVertex3f(xAxis, yAxis, zAxis);
			glVertex3f(x, yAxis, y);

			//down face
			glVertex3f(xAxis, yAxis-1.0f, y);
			glVertex3f(x, yAxis - 1.0f, y);

			// circumference
			glVertex3f(x, yAxis, y + 0.01f);
			glColor3f(127.0f / 255.0f, 61.0f / 255.0f, 2.0f / 255.0f);
			glVertex3f(x, yAxis - 1.0f, y - 0.01f);
		}
		glEnd();
		glDisable(GL_LINE_SMOOTH);
	}
}
