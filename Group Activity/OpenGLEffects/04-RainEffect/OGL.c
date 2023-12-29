//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

//OpenGL Header
#include<gl/GL.h>
#include<GL/glu.h>

#include"OGL.h"

#pragma region PNG Loading
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

GLuint texture = 0;
GLuint starTexture = 0;
#pragma endregion

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

#pragma region Rain Effect

GLuint keydown = 0;


#pragma endregion

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
		case VK_NUMPAD1:
		case 0x31:
			keydown = 1;
			break;
		case VK_NUMPAD2:
		case 0x32:
			keydown = 2;
			break;
		case VK_NUMPAD3:
		case 0x33:
			keydown = 3;
			break;
		case VK_NUMPAD4:
		case 0x34:
			keydown = 4;
			break;
		case VK_NUMPAD5:
		case 0x35:
			keydown = 5;
			break;
		case VK_NUMPAD6:
		case 0x36:
			keydown = 6;
			break;
		default:
			keydown = 0;
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
	BOOL loadGLPNGTexture(GLuint * texture, char *image);
	
	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	BOOL bResult = FALSE;

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

	bResult = loadGLPNGTexture(&texture, "WaterDrop.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of water texture is Failed.\n");
		return(-6);
	}

	bResult = loadGLPNGTexture(&starTexture, "StarImage.png");
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of star texture is Failed.\n");
		return(-7);
	}

	// enable texture
	glEnable(GL_TEXTURE_2D);

	resize(WIN_WIDTH,WIN_HEIGHT);

	return(0);
}

BOOL loadGLPNGTexture(GLuint *texture, char *image)
{
	//local variable declaration
	int iWidth, iHeight, iComponents;

	unsigned char *Data = stbi_load(image, &iWidth, &iHeight, &iComponents, 0);

	if (!Data)
	{
		fprintf(gpFILE, "LoadPNGTexture Failed For %s : \n", image);
		stbi_image_free(Data);
	}
	else
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		// set texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (iComponents == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		}
		else if (iComponents == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		}

		glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

		//Delete Object
		stbi_image_free(Data);

		return(TRUE);
	}

	return(FALSE);
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
	void rainTypeOne(void);
	void rainTypeTwo(void);
	void rainTypeThree(void);
	void rainTypeFour(void);
	void rainTypeStar(void);
	void rainTypeStarTexture(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);

	if (keydown == 1)
	{
		rainTypeOne();
	}
	else if (keydown == 2)
	{
		rainTypeTwo();
	}
	else if (keydown == 3)
	{
		rainTypeThree();
	}
	else if (keydown == 4)
	{
		rainTypeFour();
	}
	else if (keydown == 5)
	{
		rainTypeStar();
	}
	else if (keydown == 6)
	{
		rainTypeStarTexture();
	}

	SwapBuffers(ghdc);
}

void update(void)
{
	//code

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
	// Destroy texture
	if (texture)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}

	// Destroy texture
	if (starTexture)
	{
		glDeleteTextures(1, &starTexture);
		starTexture = 0;
	}

	// Close log file
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}

void rainTypeOne(void)
{
	glPointSize(5.0f);
	glEnable(GL_POINT_SMOOTH);
	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glBegin(GL_POINTS);

		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex3f(x, y, z);

		glEnd();
	}
	glDisable(GL_POINT_SMOOTH);
	glPointSize(1.0f);
}

void rainTypeTwo(void)
{

	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glBegin(GL_LINES);

		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex3f(x, y, z);
		glVertex3f(x-0.01f, y-0.02f, z + 0.01f);

		glEnd();
	}
}

void rainTypeThree(void)
{
	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glBegin(GL_LINES);

		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex3f(x, y, z);
		glVertex3f(x, y - 0.02f, z);

		glEnd();
	}
}

void rainTypeFour(void)
{
	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, texture); 
		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y, z);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x- 0.02f, y, z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x - 0.02f, y - 0.02f, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y - 0.02f, z);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}
}

void rainTypeStar(void)
{
	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glBegin(GL_LINES);

		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex3f(x, y, z);
		glVertex3f(x, y - 0.02f, z);


		glVertex3f(x- 0.015f, y, z);
		glVertex3f(x + 0.015f, y - 0.02f, z);


		glVertex3f(x + 0.015f, y, z);
		glVertex3f(x - 0.015f, y - 0.02f, z);


		glEnd();
	}
}

void rainTypeStarTexture(void)
{
	for (GLint i = 0; i < 100; i++)
	{
		GLfloat x = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat y = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);
		GLfloat z = -2.0f + 4.0f * ((GLfloat)rand() / RAND_MAX);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, starTexture);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y, z);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x - 0.02f, y, z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x - 0.02f, y - 0.02f, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y - 0.02f, z);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}
}