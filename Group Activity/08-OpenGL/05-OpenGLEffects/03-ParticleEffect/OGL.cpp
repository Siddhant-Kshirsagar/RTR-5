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

#pragma region Particle Effect

#define MAX_PARTICLES 1000 // number of particles to create
float slowDown = 2.0f; // slow down particles speed factor, low value means fast movement
float xSpeed; // base speed - which side the particle is moving left or right
float ySpeed; // base speed - which direction the particle is moving up or down
float zoom = -40.0f; // used for zooming out, start la particles la aat dhaklun mag te baher z axis var yenaret, so -40 start position

GLuint loop; // loop counter(iterative variables to used in loops)
GLuint col; // color counter (color counter - color chya array la iterate kartana use karaycha) - particles la different color denyasathi
GLuint delay; // cycle throught the color (array) - randomly color select karaila use karnar

typedef struct
{
	bool active; // decides whether the particle is to be used or not used(screen var particle dakhawaycha ka nahi he tharavto)
	float life; // kiti vel particle jagnar? mjanjech  particle chi alpha value in glColor4f(), different particle different time la alpha kami hot jail, mhanjech jevha alpha 0.0f hoil tyanch life sample as mhanta yeil
	float fade; // fade hi value present karto ki, particle chi life kitine kami hote in each iteration

	float r; // particle red color component
	float g; // particle green color component
	float b; // particle blue color component

	float x; // particle's x position
	float y; // particle's y position
	float z; // particle's z position

	float xi; // particle direction (left or right), increment factor (mhanje particle kiti speed ni x axis var halnar)
	float yi; // particle direction (up or down), increment factor(mhanje particle kiti speed ni y axis var halnar)
	float zi; // particle direction ( inward or outward), increment factor ( mhanje particle kiti speed ni z axis var halnar)

	float xg; // gravitational pull (left or right), particle chi movement slow karnar x axis var
	float yg; // gravitational pull (up or down), particle chi movement slow karnar y axis var
	float zg; // gravitational pull (inward or outward), particle chi movement slow karnar z axis var
}PARTICLES;

PARTICLES particle[MAX_PARTICLES];

// color array for particles - assigned random pair of rgb to particles
static GLfloat colors[12][3] = {
	{1.0f,0.5f,0.5f}, {1.0f,0.75f,0.5f}, {1.0f,1.0f,0.5f}, {0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f}, {0.5f,1.0f,0.75f}, {0.5f,1.0f,1.0f}, {1.0f,0.5f,0.5f},
	{0.5f,0.5f,1.0f}, {0.75f,0.5f,1.0f}, {1.0f,0.5f,1.0f}, {1.0f,0.5f,0.75f} 
};

//  global value for redisplay particle effect after one cycle completed
bool reinit = false;

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (loop = 0; loop < MAX_PARTICLES; loop++)
	{
		particle[loop].active = true; // jar ka particle active nasel tar screen var dakhwaycha nahi
		particle[loop].life = 1.0f; // particle life atta full ahe

		// particle life kitne ne kami karat jaychi, saglya particle la randomly different value assign kar ne
		//             random value 0 - 99 madhey aananyasathi | start lach life hi 1.0f ali tar particle chi lagech life sampel tar tya sathi thode increment waprl ahe
		particle[loop].fade = (float)(rand() % 100) / 1000.0f + 0.003f;

		// assigned color to particle (we are using white color to all element this time)
		particle[loop].r = 1.0f;
		particle[loop].g = 1.0f;
		particle[loop].b = 1.0f;

		//speed and direction in which the particle should move(trajectory of particle)
		particle[loop].xi = (float)((rand() % 50) - 26.0f) * 10.0f;
		particle[loop].yi = (float)((rand() % 50) - 25.0f) * 10.0f;
		particle[loop].zi = (float)((rand() % 50) - 25.0f) * 10.0f;

		// gravity - sadhya gravity effect nahi use karycha mhanun 0 set kela ahe
		particle[loop].xg = 0.0f;
		particle[loop].yg = 0.0f;
		particle[loop].zg = 0.0f;
	}

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
	// varible declaration
	bool reset = true; // flag for display particle component

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-12.0f);

	for (loop = 0; loop < MAX_PARTICLES; loop++)
	{
		if (particle[loop].active)
		{
			// temporary copy value of position of particles
			float x = particle[loop].x;
			float y = particle[loop].y;
			float z = particle[loop].z + zoom;

			// give color to the particle
			glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);

			glBegin(GL_QUADS);

			glVertex3f(x + 0.05f, y + 0.05f, z);
			glVertex3f(x - 0.05f, y + 0.05f, z);
			glVertex3f(x - 0.05f, y - 0.05f, z);
			glVertex3f(x + 0.05f, y - 0.05f, z);

			glEnd();

			// ***** animation part *****
			// slow down particle value 
			particle[loop].x = particle[loop].x + (particle[loop].xi / (slowDown * 1000));
			particle[loop].y = particle[loop].y + (particle[loop].yi / (slowDown * 1000));
			particle[loop].z = particle[loop].z + (particle[loop].zi / (slowDown * 1000));

			// add gravity(resistance) in movement of particle 
			particle[loop].xi = particle[loop].xi + particle[loop].xg;
			particle[loop].yi = particle[loop].yi + particle[loop].yg;
			particle[loop].zi = particle[loop].zi + particle[loop].zg;

			particle[loop].life = particle[loop].life - particle[loop].fade;

			reset = reset && (particle[loop].life < 0.0f);
		}
		reinit = reset;
	}
	
	SwapBuffers(ghdc);
}

void update(void)
{
	//code

	if (reinit) // after completing one cycle of particle effect reassigned values of particle
	{
		for (loop = 0; loop < MAX_PARTICLES; loop++)
		{
			particle[loop].life = 1.0f; 

			particle[loop].fade = (float)(rand() % 100) / 1000.0f + 0.003f;

			particle[loop].x = 0.0f;
			particle[loop].y = 0.0f;
			particle[loop].z = 0.0f;

			//speed and direction in which the particle should move(trajectory of particle)
			particle[loop].xi = (float)((rand() % 50) - 26.0f) * 10.0f;
			particle[loop].yi = (float)((rand() % 50) - 25.0f) * 10.0f;
			particle[loop].zi = (float)((rand() % 50) - 25.0f) * 10.0f;


			// assigned color to particle (we are using white color to all element this time)
			particle[loop].r = 1.0f;
			particle[loop].g = 1.0f;
			particle[loop].b = 1.0f;

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
	// Destroyf Window
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


