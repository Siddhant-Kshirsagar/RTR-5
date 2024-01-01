//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

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

// for light
BOOL gbLight = FALSE;

//GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
//GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
//GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };
//
//GLfloat materialAmbient[] = {0.0f,0.0f,0.0f,1.0f};
//GLfloat materialDiffuse[] = { 0.5f,0.2f,0.7f,1.0f };
//GLfloat materialSpecular[] = { 0.7f, 0.7f, 0.7f,1.0f };
//GLfloat materialShininess[] = { 128.0f };

GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };

GLUquadric *quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;

GLuint keyPressed = 0;

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
		case 'L':
		case 'l':
		{
			if (gbLight == FALSE)
			{
				glEnable(GL_LIGHTING);
				gbLight = TRUE;
			}
			else
			{
				glDisable(GL_LIGHTING);
				gbLight = FALSE;
			}
		}
		break;

		case 'X':
		case 'x':
			keyPressed = 1;
			angleForXRotation = 0.0f; // reset
			break;

		case 'Y':
		case 'y':
			keyPressed = 2;
			angleForYRotation = 0.0f; // reset
			break;

		case 'Z':
		case 'z':
			keyPressed = 3;
			angleForZRotation = 0.0f; // reset
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

	// step 7 : - set clear color of window to dark grey (here OpenGL Start)
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	// light related initialization
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);

	// initialize quadric
	quadric = gluNewQuadric();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	if (width <= height)
	{
		glOrtho(0.0f,
			15.5f,
			0.0f * (((GLfloat)height) / ((GLfloat)width)),
			15.5f * (((GLfloat)height) / ((GLfloat)width)),
			-10.0f,
			10.0f);
	}
	else
	{
		glOrtho(0.0f * (((GLfloat)width) / ((GLfloat)height)),
			15.5f * (((GLfloat)width) / ((GLfloat)height)),
			0.0f,
			15.0f,
			-10.0f,
			10.0f);
	}
}

void display(void)
{
	// variable declaration
	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// animation
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}
	else if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}
	else if (keyPressed == 3)
	{
		glRotatef(angleForZRotation, 0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// first column of Precious Stone
	// emerad
	// ambient material
	materialAmbient[0] = 0.0215f; // r
	materialAmbient[1] = 0.1745f; // g
	materialAmbient[2] = 0.0215f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.07568f; // r
	materialDiffuse[1] = 0.61424f; // g
	materialDiffuse[2] = 0.07568f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.0633f; // r
	materialSpecular[1] = 0.727811f; // g
	materialSpecular[2] = 0.633f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f,14.0f,0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 1st column, jade
	// ambient material
	materialAmbient[0] = 0.135f; // r
	materialAmbient[1] = 0.2225f; // g
	materialAmbient[2] = 0.1575f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.54f; // r
	materialDiffuse[1] = 0.89f; // g
	materialDiffuse[2] = 0.63f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.316228f; // r
	materialSpecular[1] = 0.316228f; // g
	materialSpecular[2] = 0.316228f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f, 11.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 1st column, obsidian
	// ambient material
	materialAmbient[0] = 0.05375f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.06625f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.18275f; // r
	materialDiffuse[1] = 0.17f; // g
	materialDiffuse[2] = 0.22525f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.332741f; // r
	materialSpecular[1] = 0.328634f; // g
	materialSpecular[2] = 0.346435f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.3f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f, 9.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 1st column, pearl
	// ambient material
	materialAmbient[0] = 0.25f; // r
	materialAmbient[1] = 0.20725f; // g
	materialAmbient[2] = 0.20725f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 1.0f; // r
	materialDiffuse[1] = 0.829f; // g
	materialDiffuse[2] = 0.829f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.296648f; // r
	materialSpecular[1] = 0.296648f; // g
	materialSpecular[2] = 0.296648f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.088f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f, 6.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 1st column, ruby
	// ambient material
	materialAmbient[0] = 0.1745f; // r
	materialAmbient[1] = 0.01175f; // g
	materialAmbient[2] = 0.01175f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.61424f; // r
	materialDiffuse[1] = 0.04136f; // g
	materialDiffuse[2] = 0.04136f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.727811f; // r
	materialSpecular[1] = 0.626959f; // g
	materialSpecular[2] = 0.626959f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f, 4.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th shpere on 1st column, turquoise
	// ambient material
	materialAmbient[0] = 0.1f; // r
	materialAmbient[1] = 0.18725f; // g
	materialAmbient[2] = 0.1745f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.396f; // r
	materialDiffuse[1] = 0.74151f; // g
	materialDiffuse[2] = 0.69102f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.297254f; // r
	materialSpecular[1] = 0.30829f; // g
	materialSpecular[2] = 0.306678f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(1.5f, 1.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd column metal
	// 1st sphere on 2nd column, brass
	// ambient material
	materialAmbient[0] = 0.329412f; // r
	materialAmbient[1] = 0.223529f; // g
	materialAmbient[2] = 0.027451f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.780392f; // r
	materialDiffuse[1] = 0.568627f; // g
	materialDiffuse[2] = 0.113725f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.992157f; // r
	materialSpecular[1] = 0.941176f; // g
	materialSpecular[2] = 0.807843f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.21794872f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 14.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere
	// ambient material
	materialAmbient[0] = 0.2125f; // r
	materialAmbient[1] = 0.1275f; // g
	materialAmbient[2] = 0.054f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.714f; // r
	materialDiffuse[1] = 0.4284f; // g
	materialDiffuse[2] = 0.18144f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.393548f; // r
	materialSpecular[1] = 0.271906f; // g
	materialSpecular[2] = 0.166721f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.2 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 11.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 2nd column, chrome
	// ambient material
	materialAmbient[0] = 0.25f; // r
	materialAmbient[1] = 0.25f; // g
	materialAmbient[2] = 0.25f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.4f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.774597f; // r
	materialSpecular[1] = 0.774597f; // g
	materialSpecular[2] = 0.774597f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 9.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 2nd column, copper
	// ambient material
	materialAmbient[0] = 0.19125f; // r
	materialAmbient[1] = 0.0735f; // g
	materialAmbient[2] = 0.0225f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.7038f; // r
	materialDiffuse[1] = 0.27048f; // g
	materialDiffuse[2] = 0.0828f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.256777f; // r
	materialSpecular[1] = 0.137622f; // g
	materialSpecular[2] = 0.086014f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 6.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 2nd column, gold
	// ambient material
	materialAmbient[0] = 0.24725f; // r
	materialAmbient[1] = 0.1995f; // g
	materialAmbient[2] = 0.0745f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.75164f; // r
	materialDiffuse[1] = 0.60648f; // g
	materialDiffuse[2] = 0.22648f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.628281f; // r
	materialSpecular[1] = 0.555802f; // g
	materialSpecular[2] = 0.366065f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 4.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 2nd column, silver
		// ambient material
	materialAmbient[0] = 0.19225f; // r
	materialAmbient[1] = 0.19225f; // g
	materialAmbient[2] = 0.19225f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.50754f; // r
	materialDiffuse[1] = 0.50754f; // g
	materialDiffuse[2] = 0.50754f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.508273f; // r
	materialSpecular[1] = 0.508273f; // g
	materialSpecular[2] = 0.508273f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(5.5f, 1.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 1st sphere on 3rd column, black
		// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.01f; // r
	materialDiffuse[1] = 0.01f; // g
	materialDiffuse[2] = 0.01f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50f; // r
	materialSpecular[1] = 0.50f; // g
	materialSpecular[2] = 0.50f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 14.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 3rd column, cyan 
		// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.1f; // g
	materialAmbient[2] = 0.06f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.0f; // r
	materialDiffuse[1] = 0.50980392f; // g
	materialDiffuse[2] = 0.50980392f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50196078f; // r
	materialSpecular[1] = 0.50196078f; // g
	materialSpecular[2] = 0.50196078f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 11.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 2nd column, green
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.1f; // r
	materialDiffuse[1] = 0.35f; // g
	materialDiffuse[2] = 0.1f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.45f; // r
	materialSpecular[1] = 0.55f; // g
	materialSpecular[2] = 0.45f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 9.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 3rd column, red
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.0f; // g
	materialDiffuse[2] = 0.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.6f; // g
	materialSpecular[2] = 0.6f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 6.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 3rd column, white
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.55f; // r
	materialDiffuse[1] = 0.55f; // g
	materialDiffuse[2] = 0.55f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.70f; // r
	materialSpecular[1] = 0.70f; // g
	materialSpecular[2] = 0.70f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 4.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 3rd column, yellow
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.60f; // r
	materialSpecular[1] = 0.60f; // g
	materialSpecular[2] = 0.50f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(9.5f, 1.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 1st sphere on 4th column, black
	// ambient material
	materialAmbient[0] = 0.02f; // r
	materialAmbient[1] = 0.02f; // g
	materialAmbient[2] = 0.02f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.1f; // r
	materialDiffuse[1] = 0.1f; // g
	materialDiffuse[2] = 0.1f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.4f; // r
	materialSpecular[1] = 0.4f; // g
	materialSpecular[2] = 0.4f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 14.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 2nd sphere on 4th column, cyan
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.5f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.7f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 11.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 3rd sphere on 4th column, green
	// ambient material
	materialAmbient[0] = 0.0f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 9.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 4th sphere on 4th column, red
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.4f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.04f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 6.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 5th sphere on 4th column, white
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.05f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.5f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.7f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 4.0f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	// 6th sphere on 4th column, yellow
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.05f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5f; // r
	materialDiffuse[1] = 0.5f; // g
	materialDiffuse[2] = 0.4f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7f; // r
	materialSpecular[1] = 0.7f; // g
	materialSpecular[2] = 0.04f; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glLoadIdentity();

	glTranslatef(13.5f, 1.5f, 0.0f);

	gluSphere(quadric, 1.0f, 30, 30);

	SwapBuffers(ghdc);
}

void update(void)
{
	//code
	if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 1.0f;
		if (angleForXRotation >= 360.0f)
		{
			angleForXRotation = angleForXRotation - 360.0f;

		}
	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 1.0f;
		if (angleForYRotation >= 360.0f)
		{
			angleForYRotation = angleForYRotation - 360.0f;
		}
	}

	if (keyPressed == 3)
	{
		angleForZRotation = angleForZRotation + 1.0f;
		if (angleForZRotation >= 360.0f)
		{
			angleForZRotation = angleForZRotation - 360.0f;
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

	// uninitialize quadric
	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	// Close log file
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}


