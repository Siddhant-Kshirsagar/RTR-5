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
GLfloat cAngle = 0.0f;

//GLuint texture_smiley = 0;

//GLuint texture_water = 0;
GLuint texture_flag = 0;

#pragma region FLAG EFFECT

float points[45][45][3];                    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;                       // Counter Used To Control How Fast Flag Waves
GLfloat hold;

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;

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
		MessageBox(NULL, TEXT("Log file cannot be open"), TEXT("Error"), MB_OK | MB_ICONERROR);
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
		MessageBox(hwnd, TEXT("initialize() failed."), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}

	//show window
	ShowWindow(hwnd, iCmdShow);

	// set window first in z-order (foreground)
	SetForegroundWindow(hwnd);

	// set focus on this window(internally send 'WM_SETFOCUS' message to WndProc())
	SetFocus(hwnd);

	//Game Loop
	while (bDone == FALSE)
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
		resize(LOWORD(lParam), HIWORD(lParam));
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
	BOOL loadGLTexture(GLuint *, TCHAR[]);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	BOOL bResult;

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
		fprintf(gpFILE, "wglMakeCurrent() Failed.\n");
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

	/*bResult = loadGLTexture(&texture_smiley, MAKEINTRESOURCE(MY_BITMAP_SMILEY));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of smiley texture is Failed.\n");
		return(-7);
	}*/

	/*bResult = loadGLTexture(&texture_water, MAKEINTRESOURCE(MY_BITMAP_WATER));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of water texture is Failed.\n");
		return(-7);
	}*/

	bResult = loadGLTexture(&texture_flag, MAKEINTRESOURCE(MY_BITMAP_FLAG));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of flag texture is Failed.\n");
		return(-7);
	}
	// enable texture
	glEnable(GL_TEXTURE_2D);

	//glPolygonMode(GL_BACK, GL_FILL);          // Back Face Is Filled In
	//glPolygonMode(GL_FRONT, GL_LINE);         // Front Face Is Drawn With Lines

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// flag effect
	// Loop Through The X Plane
	fprintf(gpFILE, "\n\t\t***************** Loop Start Here ****************** \n");
	for (int x = 0; x < 45; x++)
	{
		fprintf(gpFILE, "\n\t\t***************** x = % d ****************** \n",x);
		// Loop Through The Y Plane
		for (int y = 0; y < 45; y++)
		{
			// Apply The Wave To Our Mesh
			points[x][y][0] = (float)((x / 5.0f) - 4.5f);
			points[x][y][1] = (float)((y / 5.0f) - 4.5f);
			points[x][y][2] = (float)(sin((((x / 5.0f) * 40.0f) / 360.0f) * 3.141592654 * 2.0f));
			fprintf(gpFILE, "\n\t\t***************** y = % d ****************** \n", y);
			fprintf(gpFILE, "\n { x = %f y = %f  z = %f }\n", points[x][y][0], points[x][y][1],points[x][y][2]);
		}
	}
	fprintf(gpFILE, "\n\t\t***************** Loop End Here ****************** \n");


	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

BOOL loadGLTexture(GLuint *texture, TCHAR imageResourceID[])
{
	//local variable declaration
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == NULL)
	{
		fprintf(gpFILE, "LoadImage() Failed.\n");
		return(FALSE);
	}

	// get Image Data
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	// create openGL texture
	glGenTextures(1, texture);

	// bind to the generated texture
	glBindTexture(GL_TEXTURE_2D, *texture);

	// decide image pixel alignment and unpacking 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// set texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// create multiple mipmap images
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

	// unBind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// delete bitmap object
	DeleteObject(hBitmap);

	hBitmap = NULL;

	return(TRUE);
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

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	// variable declaration 
	
	int x, y;                       // Loop Variables
	float float_x, float_y, float_xb, float_yb;     // Used To Break The Flag Into Tiny Quads

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	// cube
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);             // Translate 12 Units Into The Screen

	//glRotatef(xrot, 1.0f, 0.0f, 0.0f);             // Rotate On The X Axis
	//glRotatef(yrot, 0.0f, 1.0f, 0.0f);             // Rotate On The Y Axis 
	//glRotatef(zrot, 0.0f, 0.0f, 1.0f);             // Rotate On The Z Axis

	glBindTexture(GL_TEXTURE_2D, texture_flag);       // Select Our Texture

	glBegin(GL_QUADS);                  // Start Drawing Our Quads
	for (x = 0; x < 44; x++)                // Loop Through The X Plane (44 Points)
	{
		for (y = 0; y < 44; y++)            // Loop Through The Y Plane (44 Points)
		{
			float_x = (float)(x) / 44.0f;       // Create A Floating Point X Value
			float_y = (float)(y) / 44.0f;       // Create A Floating Point Y Value
			float_xb = (float)(x + 1) / 44.0f;        // Create A Floating Point Y Value+0.0227f
			float_yb = (float)(y + 1) / 44.0f;        // Create A Floating Point Y Value+0.0227f
			glTexCoord2f(float_x, float_y);    // First Texture Coordinate (Bottom Left)
			glVertex3f(points[x][y][0], points[x][y][1], points[x][y][2]);

			glTexCoord2f(float_x, float_yb);  // Second Texture Coordinate (Top Left)
			glVertex3f(points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2]);

			glTexCoord2f(float_xb, float_yb); // Third Texture Coordinate (Top Right)
			glVertex3f(points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2]);

			glTexCoord2f(float_xb, float_y);  // Fourth Texture Coordinate (Bottom Right)
			glVertex3f(points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2]);
		}
	}
	glEnd();                        // Done Drawing Our Quads
	glBindTexture(GL_TEXTURE_2D, 0);

	if (wiggle_count == 2)                 // Used To Slow Down The Wave (Every 2nd Frame Only)(Every time when display is called than each time here check value of wiggle_count if we increment value of check than animation is slow down because after n number of display call only once this if block will be executed. e.g.  if we check wiggle_count == 2 this block executed once when display called twice, if we check wiggle_count == 4 this block executed once when display called 4 times)
	{
		for (y = 0; y < 45; y++)            // Loop Through The Y Plane
		{
			hold = points[0][y][2];           // Store Current Value One Left Side Of Wave
			for (x = 0; x < 44; x++)     // Loop Through The X Plane
			{
				// Current Wave Value Equals Value To The Right
				points[x][y][2] = points[x + 1][y][2];
			}
			points[44][y][2] = hold;          // Last Value Becomes The Far Left Stored Value
		}
		wiggle_count = 0;               // Set Counter Back To Zero
	}
	wiggle_count++;

	SwapBuffers(ghdc);
}

void update(void)
{
	//code
	//pAngle = pAngle + 1.0f;
	//if (pAngle >= 360.0f)
	//{
	//	pAngle = pAngle - 360.0f;
	//}

	//cAngle = cAngle + 1.0f;
	//if (cAngle >= 360.0f)
	//{
	//	cAngle = cAngle - 360.0f;
	//}

	xrot += 0.3f;                     // Increase The X Rotation Variable
	yrot += 0.2f;                     // Increase The Y Rotation Variable
	zrot += 0.4f;                     // Increase The Z Rotation Variable
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
	// delete texture
	if (texture_flag)
	{
		glDeleteTextures(1, &texture_flag);
		texture_flag = 0;
	}

	// Close log file
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}


