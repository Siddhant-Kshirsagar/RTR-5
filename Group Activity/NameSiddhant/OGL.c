//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit
#define _USE_MATH_DEFINES
#include<math.h>

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
#pragma comment(lib,"winmm.lib")

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // initialization of struct => this work on all type (if we want to initialize all value to 0)
BOOL gbFullscreen = FALSE;
FILE *gpFILE = NULL;

HWND ghwnd = NULL; // g = global handle of window
BOOL gbActive = FALSE; 

BOOL gbShowName = FALSE;

GLfloat fAngle = 0.0f;
GLfloat sTranslate = 0.0f;
GLfloat iTranslate = 0.0f;
GLfloat d1Translate = 0.0f;
GLfloat d2Translate = 0.0f;
GLfloat hTranslate = 0.0f;
GLfloat aTranslate = 0.0f;
GLfloat nTranslate = 0.0f;
GLfloat tTranslate = 0.0f;
GLfloat letterTranslate = -1.2f;
GLfloat zTransition = -10.0f;

float gfRadius = 1.4f;
float gfAngleLimit = 0.0f;
float gfPointVertex[] = { 0.0f,0.0f,0.0f };
float gfColor[] = { 0.0f,0.0f,0.0f };
float sceneTime = 0.0f;

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

	PlaySound(TEXT("MyMusic.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
		default:
			break;
		}
	}
	break;
	
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PlaySound(NULL, 0, 0);
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

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	resize(WIN_WIDTH,WIN_HEIGHT);

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
	// 1st param radius of circle	(float)
	// 2nd param x coordinate of center of circle (float)
	// 3rd param y coordinate of center of circle (float)
	// 4th param primitive of openGL (only GL_LINES or GL_POINTS allowed otherwise no result)(GLenum)
	void drawCircle(float, float, float,GLenum);

	// 1st param radius of circle	(float)
	// 2nd param x coordinate of center of circle (float)
	// 3rd param y coordinate of center of circle (float)
	// 4th param max angle limit (float)
	void circleAnimation(float, float, float,float); 

	// 1st param x coordinate of top right vertex 
	// 2nd param y coordinate of top right vertex
	// 3rd param length of quad
	// 4th param height of quad
	void drawQuad(float,float,float,float);

	void drawLetter_S(void);
	void drawLetter_I(void);
	void drawLetter_D(void);
	void drawLetter_H(void);
	void drawLetter_A(void);
	void drawLetter_N(void);
	void drawLetter_T(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (gbShowName)
	{
		
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, zTransition);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		circleAnimation(gfRadius, 0.0f, 0.0f, 360.0f);

		circleAnimation(gfRadius + 2.0f, 0.0f, 0.0f, 360.0f);
		//glRotatef(fAngle, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(sTranslate, 0.0f, zTransition);
		drawQuad(2.8f, 0.6f, 3.1f, 1.0f);
		drawQuad(3.0f, 0.8f, 3.5f, 1.4f);
		glColor3f(1.0f, 1.0f, 1.0f);
		drawLetter_S();

		glLoadIdentity();
		glTranslatef(iTranslate, 0.0f, zTransition);
		//glColor3f(gfColor[0]+0.1f, 0.0f, 0.0f);
		drawLetter_I();

		glLoadIdentity();
		glTranslatef(d1Translate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.2f, 0.0f, 0.0f);
		drawLetter_D();

		glLoadIdentity();
		glTranslatef(d2Translate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.3f, 0.0f, 0.0f);
		drawLetter_D();

		glLoadIdentity();
		glTranslatef(hTranslate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.4f, 0.0f, 0.0f);
		drawLetter_H();

		glLoadIdentity();
		glTranslatef(aTranslate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.5f, 0.0f, 0.0f);
		drawLetter_A();


		glLoadIdentity();
		glTranslatef(nTranslate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.6f, 0.0f, 0.0f);
		drawLetter_N();

		glLoadIdentity();
		glTranslatef(tTranslate, 0.0f, zTransition);
		//glColor3f(gfColor[0] + 0.7f, 0.0f, 0.0f);
		drawLetter_T();
		//drawCircle(0.1f, 0.5f, 0.5f, GL_LINES);

	/*	glLoadIdentity();
		glTranslatef(0.0f, 0.0f, zTransition+2.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		circleAnimation(1.5f, 0.0f, 0.0f, 360.0f);*/
	
	}
	else
	{
		if (sceneTime > 53.0f)
		{
			glColor3f(gfColor[0], gfColor[1], gfColor[2]);
		}
		else if(sceneTime > 54.0f)
		{
			
		}
		glTranslatef(sTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_S();

		glLoadIdentity();
		glTranslatef(iTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_I();

		glLoadIdentity();
		glTranslatef(d1Translate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_D();

		glLoadIdentity();
		glTranslatef(d2Translate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_D();

		glLoadIdentity();
		glTranslatef(hTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_H();

		glLoadIdentity();
		glTranslatef(aTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_A();


		glLoadIdentity();
		glTranslatef(nTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_N();

		glLoadIdentity();
		glTranslatef(tTranslate, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
		drawLetter_T();

		//drawCircle(gfRadius, 0.0f, 0.0f, GL_POINTS);

		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, zTransition);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		//circleAnimation(1.5f,0.0f,0.0f, 360.0f);
		drawCircle(1.5f, 0.0f, 0.0f, GL_POINTS);

		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, zTransition);
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		//circleAnimation(1.5f, 0.0f, 0.0f, 360.0f);
		drawCircle(1.5f, 0.0f, 0.0f, GL_POINTS);

		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, zTransition);
		glRotatef(fAngle, 0.0f, 1.0f, 1.0f);
		//circleAnimation(1.5f, 0.0f, 0.0f, 360.0f);
		drawCircle(1.5f, 0.0f, 0.0f, GL_POINTS);

		if (sceneTime > 53.0f)
		{
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, zTransition + 2.0f);
			glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
			circleAnimation(1.5f, 0.0f, 0.0f, 360.0f);
		}
		else if (sceneTime < 52.0f)
		{
		
		}


		//drawPoint(gfPointVertex[0], gfPointVertex[1]);	
	}

	SwapBuffers(ghdc);
}

void drawLetter_S(void)
{
	glBegin(GL_QUADS);
	//square 1 top left side
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);


	//square 2 middle left side
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);


	//square 3 middle right side
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);


	//square 4 bottom right side
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(0.0f, -0.3f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);
	//triangle 1 bottom side
	glVertex3f(0.0f, -0.2f, 0.0f);
	glVertex3f(-0.15f, -0.1f, 0.0f);
	glVertex3f(0.0f, -0.3f, 0.0f);

	//triangle 2 top side
	glVertex3f(0.0f, 0.4f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(0.15f, 0.2f, 0.0f);

	glEnd();
}

void drawLetter_I(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.04f, 0.3f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);
	glEnd();
}

void drawLetter_D(void)
{
	glBegin(GL_QUADS);
	// left side bar
	glVertex3f(0.04f, 0.3f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);

	// right bottom / bar
	glVertex3f(0.2f, -0.15f, 0.0f);
	glVertex3f(0.04f,-0.25f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);

	//right vertical bar
	glVertex3f(0.2f, 0.2f, 0.0f);
	glVertex3f(0.15f, 0.25f, 0.0f);
	glVertex3f(0.15f, -0.185f, 0.0f);
	glVertex3f(0.2f, -0.15f, 0.0f);

	//right top \ bar
	glVertex3f(0.2f, 0.25f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(0.04f, 0.3f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);

	glEnd();
}

void drawLetter_H(void)
{
	glBegin(GL_QUADS);
	// left side bar
	glVertex3f(0.04f, 0.3f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);

	// center horizontal bar
	glVertex3f(0.22f, 0.04f, 0.0f);
	glVertex3f(0.04f, 0.04f, 0.0f);
	glVertex3f(0.04f, -0.04f, 0.0f);
	glVertex3f(0.22f, -0.04f, 0.0f);

	//right side bar
	glVertex3f(0.3f, 0.3f, 0.0f);
	glVertex3f(0.22f, 0.4f, 0.0f);
	glVertex3f(0.22f, -0.3f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);

	glEnd();
}

void drawLetter_A(void)
{
	glBegin(GL_QUADS);
	// left side bar
	glVertex3f(0.15f, 0.3f, 0.0f);
	glVertex3f(0.1f, 0.4f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);

	// center horizontal bar
	glVertex3f(0.22f, 0.04f, 0.0f);
	glVertex3f(0.04f, 0.04f, 0.0f);
	glVertex3f(0.04f, -0.04f, 0.0f);
	glVertex3f(0.22f, -0.04f, 0.0f);

	//right side bar
	glVertex3f(0.18f, 0.3f, 0.0f);
	glVertex3f(0.1f, 0.4f, 0.0f);
	glVertex3f(0.22f, -0.3f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);

	glEnd();
}

void drawLetter_N(void)
{
	glBegin(GL_QUADS);
	// left side bar
	glVertex3f(0.04f, 0.3f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);

	// center horizontal bar
	glVertex3f(0.08f, 0.3f, 0.0f);
	glVertex3f(-0.04f, 0.4f, 0.0f);
	glVertex3f(0.22f, -0.3f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);

	//right side bar
	glVertex3f(0.3f, 0.3f, 0.0f);
	glVertex3f(0.22f, 0.4f, 0.0f);
	glVertex3f(0.22f, -0.3f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);

	glEnd();
}

void drawLetter_T(void)
{
	glBegin(GL_QUADS);
	// vertical bar
	glVertex3f(0.04f, 0.32f, 0.0f);
	glVertex3f(-0.04f, 0.32f, 0.0f);
	glVertex3f(-0.04f, -0.3f, 0.0f);
	glVertex3f(0.04f, -0.3f, 0.0f);

	// center horizontal bar
	glVertex3f(0.2f, 0.4f, 0.0f);
	glVertex3f(-0.2f, 0.4f, 0.0f);
	glVertex3f(-0.2f, 0.32f, 0.0f);
	glVertex3f(0.25f, 0.32f, 0.0f);

	glEnd();
}

void drawQuad(float xAxis, float yAxis,float length,float height)
{
	glBegin(GL_LINE_LOOP);

	glVertex3f(xAxis, yAxis, 0.0f);
	glVertex3f(xAxis- length, yAxis, 0.0f);
	glVertex3f(xAxis- length, yAxis- height, 0.0f);
	glVertex3f(xAxis, yAxis- height, 0.0f);

	glEnd();
}

void circleAnimation(float fRadius, float xAxis, float yAxis,float angleLimit)
{
	float x, y;
	/*glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);*/
	glBegin(GL_LINE_LOOP);
	for (float fAngle = 0.0f; fAngle <= angleLimit; fAngle = fAngle + 1.0f)
	{
		/*gfPointVertex[0] = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
		gfPointVertex[1] = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));*/

		if (fAngle >= 45.0f && fAngle <= 90.0f)
		{
			x = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
			y = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));

			glVertex3f(x, y, 0.0f);
			//glVertex3f(xAxis, yAxis, 0.0f);
		}
		else if (fAngle >= 135.0f && fAngle <= 180.0f)
		{
			x = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
			y = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));

			glVertex3f(x, y, 0.0f);
			//glVertex3f(xAxis, yAxis, 0.0f);
		}
		else if(fAngle >= 225.0f && fAngle <= 270.0f)
		{
			x = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
			y = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));

			glVertex3f(x, y, 0.0f);
			//glVertex3f(xAxis, yAxis, 0.0f);
		}
		else if (fAngle >= 315.0f && fAngle <= 360.0f)
		{
			x = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
			y = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));

			glVertex3f(x, y, 0.0f);
			//glVertex3f(xAxis, yAxis, 0.0f);
		}
		
	
	}
	glEnd();
}

void drawCircle(float fRadius, float xAxis, float yAxis,GLenum mode)
{
	//code
	if (mode == GL_LINES || mode == GL_POINTS)
	{
		float x, y;
		//glLoadIdentity();
		//glTranslatef(0.0f, 0.0f, -3.0f);

		if (mode == GL_LINES)
		{
			glEnable(GL_LINE_SMOOTH);
		}
		else
		{
			glEnable(GL_POINT_SMOOTH);
		}
		glBegin(mode);
		for (float fAngle = 0.0f; fAngle <= 360.0f; fAngle = fAngle + 1.0f)
		{
			x = xAxis + (fRadius * cos((fAngle * M_PI) / 180.0f));
			y = yAxis + (fRadius * sin((fAngle * M_PI) / 180.0f));

			if (mode == GL_LINES)
			{
				glVertex3f(xAxis, yAxis, 0.0f);
				glVertex3f(x, y, 0.0f);
			}
			else
			{
				glVertex3f(x, y, 0.0f);
			}
		}
		glEnd();

		if (mode == GL_LINES)
		{
			glDisable(GL_LINE_SMOOTH);
		}
		else
		{
			glDisable(GL_POINT_SMOOTH);
		}
	}

}
void update(void)
{
	//code
	if (sceneTime < 100000.0f)
	{
		sceneTime = sceneTime + 0.1f;
	}

	// for continuosly rotation
	fAngle = fAngle + 10.0f;
	if (fAngle >= 360.0f)
	{
		fAngle = 0.0f;
	}

	// circle animation
	gfAngleLimit = gfAngleLimit + 10.0f;
	if (gfAngleLimit >= 360.0f)
	{
		gfAngleLimit = 0.0f;
	}

	/*if (gfColor[0] < 1.0f)
	{*/
		gfColor[0] = gfColor[0] + 0.01f;
		if (gfColor[0] >= 1.0f)
		{
			gfColor[0] = 0.0f;
		}
	//}

	if (gbShowName)
	{
		if (gfRadius <= 1.5f)
		{
			gfRadius = gfRadius - 0.01f;
			if (gfRadius < 0.1f)
			{
				gfRadius = 1.5f;
			}
		}
		

		if (zTransition < 1.0f)
		{
			zTransition = zTransition + 0.01f;
		}
		// show name in correct sequence sequence
		sTranslate = -1.2f;
		iTranslate = -0.9f;
		d1Translate = -0.7f;
		d2Translate = -0.4f;
		hTranslate = -0.1f;
		aTranslate = 0.3f;
		nTranslate = 0.7f;
		tTranslate = 1.2f;
	}
	else
	{

		gfRadius = gfRadius - 0.01f;
		if (gfRadius < 0.1f)
		{
			gfRadius = 1.0f;
		}

		zTransition = zTransition + 0.01f;
		if (zTransition > -3.0f)
		{
			gbShowName = TRUE;
			zTransition = -10.0f;
		}
		sTranslate = letterTranslate;
		iTranslate = letterTranslate + 0.3f;
		d1Translate = letterTranslate + 0.5f;
		d2Translate = letterTranslate + 0.8f;
		hTranslate = letterTranslate + 1.1f;
		aTranslate = letterTranslate + 1.5f;
		nTranslate = letterTranslate + 1.9f;
		tTranslate = letterTranslate + 2.4f;

		if (gfColor[0] > 0.4f)
		{
			gfColor[1] = gfColor[1] + 0.1f;
			if (gfColor[1] >= 0.4f)
			{
				gfColor[1] = 0.0f;
			}
		}

		if (gfColor[0] > 0.2f)
		{
			gfColor[2] = gfColor[2] + 0.1f;
			if (gfColor[2] <= 1.0f)
			{
				gfColor[2] = 0.0f;
			}
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


