//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

//OpenGL Header
#include<gl/GL.h>
#include<GL/glu.h>

#define _USE_MATH_DEFINES
#include<math.h>

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

float OrangeColor[] = { 255.0f / 255.0f,103.0f / 255.0f, 31.0f / 255.0f };
float WhiteColor[] = { 255.0f / 255.0f, 255.0f / 255.0f , 255.0f / 255.0f };
float GreenColor[] = { 4.0f / 255.0f , 106.0f / 255.0f, 56.0f / 255.0f };

float JettBlueColor[] = { 186.0f / 255.0f, 226.0f / 255.0f, 238.0f / 255.0f };

GLfloat gfBLeft = -10.0f;

GLfloat gfHLeft = -12.25f;
GLfloat gfHTop = 10.0f;

GLfloat gfA1Left = -10.5f;
GLfloat gfA1Bottom = -10.0f;

GLfloat gfRRight = 10.75f;
GLfloat gfRTop = 10.0f;

GLfloat gfA2Right = 12.5f;
GLfloat gfA2Bottom = -10.0f;

GLfloat gfTRight = 14.0f;

GLfloat gfJettOneRight = -8.0f;

GLfloat gfJettTwoRight = -9.0f;
GLfloat gfJettTwoTop = 5.0f;

GLfloat gfJettThreeRight = -9.0f;
GLfloat gfJettThreeBottom = -5.0f;

GLfloat changeFactor = 0.05f;
GLfloat changeFactorJett = 0.0035f;

GLfloat gfJettTwoAngle = -45.0f;
GLfloat gfJettThreeAngle = 45.0f;

GLfloat gfFlag = -4.1f;
GLfloat gfFlag_B = -4.1f;
GLfloat gfFlag_H = -2.85f;
GLfloat gfFlag_A1 = -0.98f;
GLfloat gfFlag_R = 0.55f;
GLfloat gfFlag_A2 = 2.05f;
GLfloat gfFlag_T = 3.85f;

GLfloat Alpha_B = 0.0f;
GLfloat Alpha_H = 0.0f;
GLfloat Alpha_A1 = 0.0f;
GLfloat Alpha_R = 0.0f;
GLfloat Alpha_A2 = 0.0f;
GLfloat Alpha_T = 0.0f;



GLfloat gfFire_xAxisIncrement = 0.3f;
GLfloat gfFire_xAxisDecrement = 0.6f;

float jettTwoAnimation = 240.0f; 
float jettThreeAnimation = 300.0f;

BOOL IsArrived_B = FALSE;
BOOL IsArrived_H = FALSE;
BOOL IsArrived_A1 = FALSE;
BOOL IsArrived_A2 = FALSE;
BOOL IsArrived_R = FALSE;
BOOL IsArrived_T = FALSE;

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

	PlaySound(MAKEINTRESOURCE(MYMUSIC),GetModuleHandle(NULL),SND_RESOURCE|SND_ASYNC); // for play music

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
	void draw_BHARATLetters(void);
	void draw_BHARATLetters1(void);
	//function declaration
	void draw_jett(void);
	void jettScene(void);

	
	
	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f);

	/*draw_BHARATLetters1();
	draw_BHARATLetters();*/

	/*if (IsArrived_T)
	{
		jettScene();
	}*/
	draw_jett();

	SwapBuffers(ghdc);
}


void jettScene(void)
{
	//function declaration
	void draw_jett(void);

	//code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// for jett One 
	glPushMatrix();
	glTranslatef(gfJettOneRight, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.4f, 0.4f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfJettTwoRight, gfJettTwoTop, 0.0f);
	glPushMatrix();
	glRotatef(gfJettTwoAngle, 0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glScalef(0.35f, 0.35f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfJettThreeRight, gfJettThreeBottom, 0.0f);
	glPushMatrix();
	glRotatef(gfJettThreeAngle, 0.0f, 0.0f, 1.0f);
	glPushMatrix();
	glScalef(0.35f, 0.35f, 1.0f);
	draw_jett();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
}

void draw_BHARATLetters1(void)
{
	//function declaration
	void draw_B1(void);
	void draw_H1(void);
	void draw_A11(void);
	void draw_R1(void);
	void draw_T1(void);



	glPushMatrix();
	glTranslatef(gfBLeft, 0.0f, 0.0f);
	draw_B1();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(gfHLeft, gfHTop, 0.0f);
	draw_H1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA1Left, gfA1Bottom, 0.0f);
	draw_A11();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfRRight, gfRTop, 0.0f);
	draw_R1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA2Right, gfA2Bottom, 0.0f);
	draw_A11();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfTRight, 0.0f, 0.0f);
	draw_T1();
	glPopMatrix();





}
void draw_B1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left | bar
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 2 left | bar
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);


	// quad 3 right ) B upper curve
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.2f, 0.7f, 0.0f);

	// quad 4 mid part
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.9f, 0.5f, 0.0f);


	//  quad 5 right ) B upper curve bottom
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 6 mid part
	glVertex3f(0.9f, 0.5f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);



	// quad 7 right ) B lower curve bottom part
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.2f, -0.7f, 0.0f);

	// quad 8 mid part
	glVertex3f(0.65f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.0f);


	//  quad 9 right ) B lower curve  top part
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 10 mid part
	glVertex3f(0.9f, -0.5f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	glEnd();

}

void draw_H1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side
	glVertex3f(-0.3f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.0f);

	// quad 2
	glVertex3f(-0.3f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -1.0f, 0.0f);

	// quad 3  right side
	glVertex3f(0.3f, 1.0f, 0.0f);
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);

	// quad 4
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);


	// quad 5  mid part - 
	glVertex3f(0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glEnd();

}

void draw_A11(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side /
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);
	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
}

void draw_R1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 left side | top part
	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 2 left side | bottom part
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);

	// quad 3 right side ) upper curve top part
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);

	// quad 4 right side ) upper curve mid part
	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.8f, 0.4f, 0.0f);

	// quad 5 right side ) upper curve bottom part
	glVertex3f(0.8f, 0.4f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.1f, 0.25f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 6 right side \ 
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(0.1f, -0.3f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);




	glEnd();
}

void draw_T1(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	// quad 1 Mid part | top part
	glVertex3f(0.15f, 0.7f, 0.0f);
	glVertex3f(-0.15f, 0.7f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	// quad 2 mid part | bottom part
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, -1.0f, 0.0f);
	glVertex3f(0.15f, -1.0f, 0.0f);

	// quad 1 Mid part | top part
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 0.7f, 0.0f);
	glVertex3f(0.6f, 0.7f, 0.0f);

	glEnd();
}

void draw_jett(void)
{
	//function declaration
	void draw_JettFire(float,float,float);

	// code
	// drawing core structure of jett
	glBegin(GL_TRIANGLES);
	// triangle 1 > center core 1st part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(1.7f, 0.0f, 0.0f);
	glVertex3f(1.28f, 0.12f, 0.0f);
	glVertex3f(1.28f, -0.12f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	// quad 1 > center core 2nd part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);

	glVertex3f(1.28f, 0.12f, 0.0f);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glVertex3f(0.9f, -0.2f, 0.0f);
	glVertex3f(1.28f, -0.12f, 0.0f);

	// quad 2 > center core 3rd part from right
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glVertex3f(0.3f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(0.9f, -0.2f, 0.0f);


	// quad 3 > center core 4th part from right
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glVertex3f(-1.5f, 0.5f, 0.0f);
	glVertex3f(-1.5f, -0.5f, 0.0f);
	glVertex3f(0.3f, -0.5f, 0.0f);



	// quad 4 > top side 1st wing
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, 0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-0.3f, 1.9f, 0.0f);
	glVertex3f(-0.9f, 1.9f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-0.7f, 0.5f, 0.0f);

	// quad 5 > top side 2nd wing
	
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.0f, 0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-1.7f, 1.3f, 0.0f);
	glVertex3f(-2.1f, 1.4f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.7f, 0.5f, 0.0f);

	// quad 6 > top side nozzle
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.49f, 0.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.65f, 0.4f, 0.0f);
	glVertex3f(-1.65f, 0.05f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.0f, 0.0f);

	// quad 7 > bottom side 1st wing


	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(0.3f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-0.3f, -1.9f, 0.0f);
	glVertex3f(-0.9f, -1.9f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-0.7f, -0.5f, 0.0f);

	// quad 8 > bottom side 2nd wing
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.0f, -0.5f, 0.0f);
	glColor3f(JettBlueColor[0], JettBlueColor[1], JettBlueColor[2]);
	glVertex3f(-1.7f, -1.3f, 0.0f);
	glVertex3f(-2.1f, -1.4f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.3f, JettBlueColor[1] - 0.3f, JettBlueColor[2] - 0.3f);
	glVertex3f(-1.7f, -0.5f, 0.0f);

	// quad 9 > bottom side nozzle
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f, 0.0f, 0.0f);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-1.65f, -0.05f, 0.0f);
	glVertex3f(-1.65f, -0.4f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-1.5f,-0.49f, 0.0f);

	glEnd();

	// for extra detailing

	glBegin(GL_TRIANGLES);

	// triangle 1 > cockpit from right
	glColor3f(0.3f, 0.3f,0.3f);
	glVertex3f(1.5f, 0.0f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex3f(1.28f, 0.1f, 0.0f);
	glVertex3f(1.28f, -0.1f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	// quad 1 > cockpit 2nd part from right
	glColor3f(0.25f, 0.25f, 0.25f);

	glVertex3f(1.28f, 0.1f, 0.0f);
	glVertex3f(0.9f, 0.18f, 0.0f);
	glVertex3f(0.9f, -0.18f, 0.0f);
	glVertex3f(1.28f, -0.1f, 0.0f);

	// quad 2 > cockpit 3rd part from right

	glVertex3f(0.9f, 0.18f, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.5f, 0.15f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex3f(0.9f, -0.18f, 0.0f);


	// quad 1 > for center spine design
	glColor3f(JettBlueColor[0] - 0.35f, JettBlueColor[1] - 0.35f, JettBlueColor[2] - 0.35f);
	glVertex3f(0.5f, 0.15f, 0.0f);
	glColor3f(JettBlueColor[0]-0.15f, JettBlueColor[1] - 0.15f, JettBlueColor[2] - 0.15f);
	glVertex3f(-1.5f, 0.01f, 0.0f);
	glVertex3f(-1.5f, -0.01f, 0.0f);
	glColor3f(JettBlueColor[0] - 0.25f, JettBlueColor[1] - 0.25f, JettBlueColor[2] - 0.25f);
	glVertex3f(0.5f, -0.15f, 0.0f);


	// quad 1 > top side  wing missile 1st

	glColor3f(0.3f,0.3f,0.3f);
	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.129f, 0.9f, 0.0f);
	glVertex3f(0.17f, 0.8f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);

	// quad 2 > top side  wing missile 1st
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.4f, 0.9f, 0.0f);
	glVertex3f(0.5f, 0.86f, 0.0f);
	glVertex3f(0.5f, 0.84f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);


	// quad 1 > top side wing missile 2nd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.3f, 1.1f, 0.0f);
	glVertex3f(0.04f, 1.1f, 0.0f);
	glVertex3f(0.08f, 1.0f, 0.0f);
	glVertex3f(0.3f, 1.0f, 0.0f);
	// quad 2 > top side wing missile 2nd 

	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.3f, 1.1f, 0.0f);
	glVertex3f(0.4f, 1.06f, 0.0f);
	glVertex3f(0.4f, 1.04f, 0.0f);
	glVertex3f(0.3f, 1.0f, 0.0f);

	// quad 1 > top side wing missile 3rd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.2f, 1.3f, 0.0f);
	glVertex3f(-0.05f, 1.3f, 0.0f);
	glVertex3f(-0.01f, 1.2f, 0.0f);
	glVertex3f(0.2f, 1.2f, 0.0f);
	// quad 2 > top side wing missile 3rd 
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.2f, 1.3f, 0.0f);
	glVertex3f(0.3f, 1.26f, 0.0f);
	glVertex3f(0.3f, 1.24f, 0.0f);
	glVertex3f(0.2f, 1.2f, 0.0f);

	// quad 1 > bottom side  wing missile 1st

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.4f, -0.9f, 0.0f);
	glVertex3f(0.129f, -0.9f, 0.0f);
	glVertex3f(0.17f, -0.8f, 0.0f);
	glVertex3f(0.4f, -0.8f, 0.0f);

	// quad 2 > bottom side  wing missile 1st
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.4f, -0.9f, 0.0f);
	glVertex3f(0.5f, -0.86f, 0.0f);
	glVertex3f(0.5f, -0.84f, 0.0f);
	glVertex3f(0.4f, -0.8f, 0.0f);


	// quad 1 > bottom side wing missile 2nd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.3f, -1.1f, 0.0f);
	glVertex3f(0.04f, -1.1f, 0.0f);
	glVertex3f(0.08f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);
	// quad 2 > bottom side wing missile 2nd 

	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.3f, -1.1f, 0.0f);
	glVertex3f(0.4f, -1.06f, 0.0f);
	glVertex3f(0.4f, -1.04f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);

	// quad 1 > bottom side wing missile 3rd 

	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(0.2f, -1.3f, 0.0f);
	glVertex3f(-0.05f, -1.3f, 0.0f);
	glVertex3f(-0.01f, -1.2f, 0.0f);
	glVertex3f(0.2f, -1.2f, 0.0f);

	// quad 2 > bottom side wing missile 3rd 
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex3f(0.2f, -1.3f, 0.0f);
	glVertex3f(0.3f, -1.26f, 0.0f);
	glVertex3f(0.3f, -1.24f, 0.0f);
	glVertex3f(0.2f, -1.2f, 0.0f);

	glEnd();

	draw_JettFire(-1.65, 0.4f, 0.0f);
	draw_JettFire(-1.65, -0.05f, 0.0f);

}

void draw_JettFire(float xAxis, float yAxis, float zAxis)
{
	int i = 0;
	float y = 0.0f;

	glBegin(GL_LINES);
	
	for (y = yAxis, i = 0; y >= yAxis - 0.34f; y = y - 0.02f, i++)
	{
		
		glColor3f(0.0f, 174.0f / 255.0f, 1.0f);
		glVertex3f(xAxis, y, zAxis);

		if (i < 8) // manually counted value for keep y axis minimum or maximum
		{
			if (i % 2 == 0)
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisIncrement, y-0.05f, zAxis);
			}
			else
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisDecrement, y - 0.05f, zAxis);
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisIncrement, y + 0.05f, zAxis);
			}
			else
			{
				glColor3f(1.0f, 97.0f / 255.0f, 0.0f);
				glVertex3f(xAxis - gfFire_xAxisDecrement, y + 0.05f , zAxis);
			}
		}
		
	
	}
	glEnd();
}


void draw_BHARATLetters(void)
{
	//function declaration
	void draw_B(void);
	void draw_H(void);
	void draw_A1(void);
	void draw_R(void);
	void draw_A2(void);
	void draw_T(void);


	
	glPushMatrix();
	glTranslatef(gfBLeft, 0.0f, 0.0f);
	draw_B();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(gfHLeft, gfHTop, 0.0f);
	draw_H();		
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA1Left, gfA1Bottom, 0.0f);
	draw_A1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfRRight, gfRTop, 0.0f);
	draw_R();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(gfA2Right, gfA2Bottom, 0.0f);
	draw_A2();
	glPopMatrix();
				
	glPushMatrix();
	glTranslatef(gfTRight, 0.0f, 0.0f);
	draw_T();
	glPopMatrix();


	


}
void draw_B(void)
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	// quad 1 left | bar
	
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(-0.1f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 2 left | bar
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(-0.1f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);


	// quad 3 right ) B upper curve
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.65f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 148.0f / 255.0f, 97.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.7f, 0.0f);
	glVertex3f(0.2f, 0.7f, 0.0f);

	// quad 4 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 148.0f / 255.0f, 97.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 181.0f / 255.0f, 146.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.5f, 0.0f);
	glVertex3f(0.9f, 0.5f, 0.0f);


	//  quad 5 right ) B upper curve bottom
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 233.0f / 255.0f, 222.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 6 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 181.0f / 255.0f, 146.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.9f, 0.5f, 0.0f);
	glVertex3f(0.65f, 0.5f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 233.0f / 255.0f, 222.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, 0.1f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.0f, 0.0f);



	// quad 7 right ) B lower curve bottom part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(79.0f / 255.0f, 150.0f / 255.0f, 115.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.7f, 0.0f);
	glVertex3f(0.2f, -0.7f, 0.0f);

	// quad 8 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, -1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(79.0f / 255.0f, 150.0 / 255.0f, 115.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(125.0f / 255.0f, 177.0f / 255.0f, 152.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, -0.5f, 0.0f);
	glVertex3f(0.9f, -0.5f, 0.0f);


	//  quad 9 right ) B lower curve  top part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(217.0f / 255.0f, 232.0f / 255.0f, 225.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	// quad 10 mid part
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(125.0f / 255.0f, 177.0f / 255.0f, 152.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.9f, -0.5f, 0.0f);
	glVertex3f(0.65f, -0.5f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(217.0f / 255.0f, 232.0f / 255.0f, 225.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.5f, -0.1f, 0.0f);
	if ((!IsArrived_T) || gfFlag_B <= -4.0f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_B);
	}
	
	glVertex3f(0.65f, 0.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void draw_H(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	// quad 1 left side
	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.3f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.6f, 0.0f, 0.0f);
	glVertex3f(-0.3f, 0.0f, 0.0f);

	// quad 2
	glVertex3f(-0.3f, 0.0f, 0.0f);
	glVertex3f(-0.6f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -1.0f, 0.0f);

	// quad 3  right side
	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 103.0f / 255.0f, 31.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.3f, 1.0f, 0.0f);
	glVertex3f(0.6f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);

	// quad 4
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(4.0f / 255.0f, 106.0f / 255.0f, 56.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);


	// quad 5  mid part - 

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 241.0f / 255.0f, 234.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(0.3f, 0.1f, 0.0f);
	glVertex3f(-0.3f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_H <= -2.75f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(228.0f / 255.0f, 229.0f / 255.0f, 234.0f / 255.0f, Alpha_H);
	}
	
	glVertex3f(-0.3f, -0.1f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

}

void draw_A1(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side /
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_A1);
	}

	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A1);
	}

	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_A1);
	}

	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A1);
	}

	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A1 <= -0.88f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A1);
	}

	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
	glDisable(GL_BLEND);
}

void draw_A2(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side /
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2],Alpha_A2);
	}
	
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}

	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	// quad 2 left side /
	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2],Alpha_A2);
	}
	
	glVertex3f(-0.8f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	// quad 3 right side /
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2],Alpha_A2);
	}
	
	glVertex3f(0.0f, 0.7f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}
	
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	// quad 4 right side /
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_A2);
	}
	
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);


	// quad 5 mid part -
	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_A2);
	}
	
	glVertex3f(0.18f, 0.1f, 0.0f);
	glVertex3f(-0.18f, 0.1f, 0.0f);

	if ((!IsArrived_T) || gfFlag_A2 <= 2.15f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2],Alpha_A2);
	}
	
	glVertex3f(-0.23f, -0.1f, 0.0f);
	glVertex3f(0.23f, -0.1f, 0.0f);


	glEnd();
	glDisable(GL_BLEND);

}


void draw_R(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 left side | top part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.1f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 2 left side | bottom part
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(-0.2f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_R);
	}

	glVertex3f(-0.2f, -1.0f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);

	// quad 3 right side ) upper curve top part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.1f, 0.7f, 0.0f);
	glVertex3f(0.4f, 0.7f, 0.0f);

	// quad 4 right side ) upper curve mid part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_R);
	}

	glVertex3f(0.5f, 1.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_R);
	}

	glVertex3f(0.4f, 0.7f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 154.0f / 255.0f, 106.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.8f, 0.4f, 0.0f);

	// quad 5 right side ) upper curve bottom part
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 154.0f / 255.0f, 106.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.8f, 0.4f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 208.0f / 255.0f, 186.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.1f, 0.25f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}
	
	glVertex3f(0.1f, 0.0f, 0.0f);

	// quad 6 right side \ 
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_R);
	}
	
	glVertex3f(0.1f, 0.0f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(179.0f / 255.0f, 210.0f / 255.0f, 195.0f / 255.0f, Alpha_R);
	}
	
	glVertex3f(0.1f, -0.3f, 0.0f);
	if ((!IsArrived_T) || gfFlag_R <= 0.65f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_R);
	}
	
	glVertex3f(0.5f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void draw_T(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	// quad 1 Mid part | top part

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_T);
	}
	
	glVertex3f(0.15f, 0.7f, 0.0f);

	glVertex3f(-0.15f, 0.7f, 0.0f);

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(WhiteColor[0], WhiteColor[1], WhiteColor[2], Alpha_T);
	}
	
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glVertex3f(0.15f, 0.0f, 0.0f);

	// quad 2 mid part | bottom part
	glVertex3f(0.15f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(GreenColor[0], GreenColor[1], GreenColor[2], Alpha_T);
	}
	
	glVertex3f(-0.15f, -1.0f, 0.0f);
	glVertex3f(0.15f, -1.0f, 0.0f);

	// quad 1 Mid part | top part
	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(OrangeColor[0], OrangeColor[1], OrangeColor[2], Alpha_T);
	}
	
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);

	if ((!IsArrived_T) || gfFlag_T <= 3.95f)
	{
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glColor4f(255.0f / 255.0f, 145.0f / 255.0f, 92.0f / 255.0f, Alpha_T);
	}
	
	glVertex3f(-0.6f, 0.7f, 0.0f);
	glVertex3f(0.6f, 0.7f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}

void update(void)
{
	//code
	// for fire animation
	gfFire_xAxisIncrement = gfFire_xAxisIncrement + 0.02f;
	if (gfFire_xAxisIncrement >= 0.6f)
	{
		gfFire_xAxisIncrement = 0.3f;
	}

	gfFire_xAxisDecrement = gfFire_xAxisDecrement - 0.02f;
	if (gfFire_xAxisDecrement <= 0.3f)
	{
		gfFire_xAxisDecrement = 0.6f;
	}

	// for letter animation
	if (IsArrived_B == FALSE)
	{
		if (gfBLeft <= -4.0f)
		{
			gfBLeft = gfBLeft + changeFactor;
		}
		else
		{
			IsArrived_B = TRUE;
		}

	}
	

	if (IsArrived_B == TRUE)
	{
		if (gfHLeft <= -2.25f)
		{
			gfHLeft = gfHLeft + changeFactor;
		}
		if (gfHTop >= 0.0f)
		{
			gfHTop = gfHTop - changeFactor;
		}
		else
		{
			IsArrived_H = TRUE;
		}
	}

	if (IsArrived_H == TRUE)
	{
		if (gfA1Left <= -0.5f)
		{
			gfA1Left = gfA1Left + changeFactor;
		}
		if (gfA1Bottom <= 0.0f)
		{
			gfA1Bottom = gfA1Bottom + changeFactor;
		}
		else
		{
			IsArrived_A1 = TRUE;
		}
	}

	if (IsArrived_A1 == TRUE)
	{
		if (gfRRight >= 0.75f)
		{
			gfRRight = gfRRight - changeFactor;
		}
		if (gfRTop >= 0.0f)
		{
			gfRTop = gfRTop - changeFactor;
		}
		else
		{
			IsArrived_R = TRUE;
		}
	}

	if (IsArrived_R == TRUE)
	{
		if (gfA2Right >= 2.5f)
		{
			gfA2Right = gfA2Right - changeFactor;
		}
		if (gfA2Bottom <= 0.0f)
		{
			gfA2Bottom = gfA2Bottom + changeFactor;
		}
		else
		{
			IsArrived_A2 = TRUE;
		}
	}

	if (IsArrived_A2 == TRUE)
	{
		if (gfTRight >= 4.0f)
		{
			gfTRight = gfTRight - changeFactor;
		}
		else
		{
			IsArrived_T = TRUE;
		}
	}

	// for jett animation
	if (IsArrived_T)
	{
		if (gfJettOneRight <= 10.0f)
		{
			gfJettOneRight = gfJettOneRight + changeFactorJett;
		}

		if (gfJettOneRight >= -4.1f && gfJettOneRight <= 4.16f)
		{
			gfFlag = gfJettOneRight; // for flag
		}

		if (gfJettOneRight >= -4.1f && gfJettOneRight <= -3.32f)
		{
			gfFlag_B = gfJettOneRight;
		}

		if (gfFlag_B >= -4.0f)
		{
			if (Alpha_B <= 1.0f)
			{
				Alpha_B = Alpha_B + 0.001f;
			}
			
		}

		if (gfJettOneRight >= -2.85f && gfJettOneRight <= -1.659f)
		{
			gfFlag_H = gfJettOneRight;
		}

		if (gfFlag_H >= -2.75f)
		{
			if (Alpha_H <= 1.0f)
			{
				Alpha_H = Alpha_H + 0.001f;
			}
			
		}

		if (gfJettOneRight >= -1.0f && gfJettOneRight <= 0.0f)
		{
			gfFlag_A1 = gfJettOneRight;
		}

		if (gfFlag_A1 >= -0.88f)
		{
			if (Alpha_A1 <= 1.0f)
			{
				Alpha_A1 = Alpha_A1 + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 0.55f && gfJettOneRight <= 0.88f)
		{
			gfFlag_R = gfJettOneRight;
		}

		if (gfFlag_R >= 0.65f)
		{
			if (Alpha_R <= 1.0f)
			{
				Alpha_R = Alpha_R + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 2.05f && gfJettOneRight <= 2.98f)
		{
			gfFlag_A2 = gfJettOneRight;
		}

		if (gfFlag_A2 >= 2.15f)
		{
			if (Alpha_A2 <= 1.0f)
			{
				Alpha_A2 = Alpha_A2 + 0.001f;
			}
				
		}

		if (gfJettOneRight >= 3.85f && gfJettOneRight <= 4.165f)
		{
			gfFlag_T = gfJettOneRight;
		}

		if (gfFlag_T >= 3.95f)
		{
			if (Alpha_T <= 1.0f)
			{
				Alpha_T = Alpha_T + 0.001f;
			}
				
		}

		// for jett two animation
		if (gfJettTwoRight <= 8.0f)
		{
			gfJettTwoRight = gfJettTwoRight + changeFactorJett;
		}
		
		if (gfJettTwoRight >= -9.0f && gfJettTwoRight <= -4.0f)
		{
			if (gfJettTwoTop >= 1.5f)
			{
				gfJettTwoTop = gfJettTwoTop - changeFactorJett;
			}
			if (gfJettTwoAngle <= 0.0f)
			{
				gfJettTwoAngle = gfJettTwoAngle + 0.0555f;
			}
		}
		else if (gfJettTwoRight >= 4.5f && gfJettTwoRight <= 8.0f)
		{
			if (gfJettTwoTop <= 8.0f)
			{
				gfJettTwoTop = gfJettTwoTop + changeFactorJett;
			}
			if (gfJettTwoAngle >= -45.0f)
			{
				gfJettTwoAngle = gfJettTwoAngle + 0.0555f;
			}
		}

		if (gfJettThreeRight <= 8.0f)
		{
			gfJettThreeRight = gfJettThreeRight + changeFactorJett;
		}

		if (gfJettThreeRight >= -9.0f && gfJettThreeRight <= -4.5f)
		{
			if (gfJettThreeBottom <= -1.5f)
			{
				gfJettThreeBottom = gfJettThreeBottom + changeFactorJett;
			}
			if (gfJettThreeAngle >= 0.0f)
			{
				gfJettThreeAngle = gfJettThreeAngle - 0.0555f;
			}
		}
		else if (gfJettThreeRight >= 4.5f && gfJettThreeRight <= 8.0f)
		{
			if (gfJettThreeBottom >= -8.0f)
			{
				gfJettThreeBottom = gfJettThreeBottom - changeFactorJett;
			}
			if (gfJettThreeAngle >= -45.0f)
			{
				gfJettThreeAngle = gfJettThreeAngle - 0.0555f;
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


