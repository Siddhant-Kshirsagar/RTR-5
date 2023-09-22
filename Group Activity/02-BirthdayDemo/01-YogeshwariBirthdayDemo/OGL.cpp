// Windows header files
// Common headers 
#include<windows.h> //win32 IO
#include<stdio.h> //for file IO
#include<stdlib.h>  //for exit
#include "OGL.h" // swatachi header file

// OpenGL header Files
#include<gl/GL.h>
#include<GL/glu.h>


// For Circle
#define _USE_MATH_DEFINES
#include <math.h>
#include "helper_timer.h"

// MACROS
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//scenes
#define SCENE_TIME1 12		// scene duration in SECONDS
#define SCENE_TIME2 20
#define SCENE_TIME3 33
#define SCENE_TIME4 41
#define SCENE_TIME5 51
#define SCENE_TIME6 80
#define SCENE_TIME7 95
#define SCENE_TIME8 103
#define SCENE_TIME9 110
//

// Link With OpenGL Library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib") 
#pragma comment(lib,"winmm.lib")


// Global Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations
//OpenGL Related Global Variables
HDC ghdc = NULL;
HGLRC  ghrc = NULL;       // handle to graphic rendering contex
FILE *gpFILE = NULL;
HWND ghwnd = NULL;        // global window handel 
BOOL gbActive = FALSE;    //means by default mazi window active nhi
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; //wpPrev = windowplacementPrevious
BOOL gbFullScreen = FALSE;

//for fadeIn effect 
BOOL enableFadeIn = FALSE;
GLfloat fadeInAlpha = 1.0f;
BOOL enableFadeOut = FALSE;
GLfloat fadeOutAlpha = 0.0f;

float elapseTime = 0.0f; // for timing 

#pragma region log messages related functions 
#define LOG_MESSAGE(msg) LogMessage(__FILE__, __func__, __LINE__, msg)
#define LOG_ERROR(msg) LogErrorMessage(__FILE__, __func__, __LINE__, msg, GetLastError())
#pragma endregion

#pragma region Shraddha Introduction Scene
BOOL gbSceneShraddhaIntro = FALSE;
BOOL gbStartYAnimation = FALSE;
BOOL gbLoadSaturn = FALSE;

GLfloat gbZoomOutFactor = 3.0f;
GLfloat gbYMovement = 0.0f;
GLfloat gbTextureXMovement = 0.0f;
GLfloat gbPresentsXMovement = 0.0f;
GLfloat starsOpacity = 0.0f;

#pragma endregion

#pragma region Shraddha KG scene

GLfloat dreamOpacity = 0.0f;
BOOL gbSceneKG = FALSE;
#pragma endregion

#pragma region Shraddha Teacher scene
BOOL gbSceneTeacher = FALSE;
#pragma endregion


void LogMessage(const char *, const char *, const int, const char *);
void LogErrorMessage(const char *, const char *, const int, const char *, int);


//scene global variables
BOOLEAN bScene1_Done = FALSE;
StopWatchInterface *SGD_timer;
float fadeOut_scene_1_alpha = 0.0f;

float fadeIn_scene_2_alpha = 1.0f;
float fadeOut_scene_2_alpha = 0.0f;

float fadeIn_scene_3_alpha = 1.0f;
float fadeOut_scene_3_alpha = 0.0f;

float fadeIn_scene_4_alpha = 1.0f;
float fadeOut_scene_4_alpha = 0.0f;

float fadeIn_scene_5_alpha = 1.0f;
float fadeOut_scene_5_alpha = 0.0f;

float fadeIn_scene_6_alpha = 1.0f;
float fadeOut_scene_6_alpha = 0.0f;

float fadeIn_scene_7_alpha = 1.0f;
float fadeOut_scene_7_alpha = 0.0f;

float fadeIn_scene_8_alpha = 1.0f;
float fadeOut_scene_8_alpha = 0.0f;

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declarations
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//Local Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("YYTWindow");
	int iResult = 0;
	BOOL bDone = FALSE; //SDK cha BOOL ahe

	// step : centering window sathi
	//int w_height = 600; //window height
	//int w_width = 800;  // window length or width
	int top_left_X = GetSystemMetrics(SM_CXSCREEN);   // (SM_CXSCREEN); <- its a macro...  SM_CX value gives width
	int top_left_Y = GetSystemMetrics(SM_CYSCREEN);   // (SM_CYSCREEN); <- its a macro...  SM_CY value gives height
	int y;
	int x;
	x = (top_left_X / 2) - WIN_WIDTH / 2;
	y = (top_left_Y / 2) - WIN_HEIGHT / 2;

	//Code
	gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully...\n");

	//WNDCLASSEX Class Initialization 
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW || CS_VREDRAW || CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	//wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register WNDCLASSEX 
	RegisterClassEx(&wndclass);

	//CreateWindow
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Yogeshwari B'day Demo"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,


		// Passing Calculated 4 Values in CreateWindow()
		x,            // x - user window top left x coordinate
		y,            // y - user window top left y coordinate
		WIN_WIDTH,     // width of window
		WIN_HEIGHT,    // height of window
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ghwnd = hwnd;


	// initialization
	iResult = initialize();

	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("Initialization Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);

		// unitialize();
		// exit (0);
		DestroyWindow(hwnd);
	}

	// Show The Window
	ShowWindow(hwnd, iCmdShow);

	// Set Foreground Window
	SetForegroundWindow(hwnd);

	SetFocus(hwnd);

	//PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
			//code
			if (gbActive == TRUE)
			{
				// Render
				display();

				// Update
				update();
			}
		}
	}


	// Uninitialization
	uninitialize();

	return((int)msg.wParam);
} //WinMain ends here


// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT imsg,
	WPARAM wParam,
	LPARAM lParam)

{
	// function declaration | FUNCTION PROTOTYPE
	void ToggleFullScreen(void);
	void resize(int, int);

	// Code
	switch (imsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:   //VK_ESCAPE has 27 ascii value
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE)
			{
				ToggleFullScreen();
				gbFullScreen = TRUE;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = FALSE;
			}
			break;
		}
		break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, imsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	// Local-Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbFullScreen == FALSE)
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

// User defined function defination
int initialize(void)
{
	//function declarations
	void resize(int, int);
	// Function Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormateIndex = 0;

	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// Initialization of PIXELFORMATDESCRIPTOR
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	//resize(WIN_WIDTH, WIN_HEIGHT);

	//Get The DC
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC Failed\n");
		return(-1);
	}

	iPixelFormateIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormateIndex == 0)
	{
		fprintf(gpFILE, "ChoosePixelFormat Failed\n");
		return(-2);
	}

	// Set obtained Pixel formate 
	if (SetPixelFormat(ghdc, iPixelFormateIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat Failed\n");
		return(-3);
	}

	// Step 4 Create OpenGl Context from Device Context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContext Failed\n");
		return(-4);
	}

	// Make Rendering Context Current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent Failed\n");
		return(-5);
	}

	//for enable depth
	glShadeModel(GL_SMOOTH); // optional (beautyfication color,light,texture shade)
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // optional (beautyfication for artifact e.g if we draw circle or sphere we see ellipse curve but we don't need that so we disable that)

	// Set The ClearColor Of Window To Black
	// Clear OpenGl Stars
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black Color
	resize(WIN_WIDTH, WIN_HEIGHT);

	//code

	sdkCreateTimer(&SGD_timer);
	sdkStartTimer(&SGD_timer);

	return(0);
}

void resize(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gluPerspective(45.0f, ((GLfloat)width / (GLfloat)height), 0.1f, 100.0f);


}

void display(void)
{

	void sceneIntroduction(void);
	void sceneTextureGroupPresents(void);
	void sceneTitle(void);
	void sceneKinderGarden(void);
	void sceneTeacher(void);
	void fadeIn(void);
	void fadeOut(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	elapseTime = sdkGetTimerValue(&SGD_timer);
	elapseTime = elapseTime / 1000;		// CONVERT IN SECONDS


	if (bScene1_Done == FALSE)
	{
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		if (elapseTime < SCENE_TIME1)
		{
			LOG_MESSAGE("1");

			enableFadeIn = TRUE;

			gbSceneShraddhaIntro = TRUE;
			sceneIntroduction();			 // shraddha Introduction scene 1 astromedicomp and title
			if (elapseTime > SCENE_TIME1 - 2.0f)
			{
				//gbSceneShraddhaIntro = FALSE;

				if (fadeOut_scene_1_alpha <= 1.0f)
				{
					fadeOut_scene_1_alpha = fadeOut_scene_1_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_1_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);

			}
		}
		else if (elapseTime <= SCENE_TIME2 && elapseTime >= SCENE_TIME1)
		{
			LOG_MESSAGE("2");

			glLoadIdentity();
			glColor3f(1.0f, 0.5f, 0.0f);

			sceneTextureGroupPresents();					// hrishikesh Texture Group Present scene 2 astromedicomp and title

			if (elapseTime > SCENE_TIME2 - 2.0f)
			{

				if (fadeOut_scene_2_alpha <= 1.0f)
				{
					fadeOut_scene_2_alpha = fadeOut_scene_2_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_2_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);


			}
		}
		else if (elapseTime <= SCENE_TIME3 && elapseTime >= SCENE_TIME2)
		{
			LOG_MESSAGE("3");
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -3.0f);
			glColor3f(0.0f, 1.0f, 1.0f);

			sceneTitle();			// Theme Name  - Nurturing Dreams

			if (elapseTime >= SCENE_TIME3 - 2.0f)
			{
				gbSceneShraddhaIntro = FALSE;
				if (fadeOut_scene_3_alpha <= 1.0f)
				{
					fadeOut_scene_3_alpha = fadeOut_scene_3_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_3_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);


			}
		}
		else if (elapseTime <= SCENE_TIME4 && elapseTime >= SCENE_TIME3)
		{
			LOG_MESSAGE("4");

			gbSceneKG = TRUE;
			sceneKinderGarden();			// KG scene 

			if (elapseTime >= SCENE_TIME4 - 2.0f)
			{
				if (fadeOut_scene_4_alpha <= 1.0f)
				{
					fadeOut_scene_4_alpha = fadeOut_scene_4_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_4_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);

				gbSceneKG = TRUE;


			}
		}
		else if (elapseTime <= SCENE_TIME5 && elapseTime >= SCENE_TIME4)
		{
			gbSceneTeacher = TRUE;
			LOG_MESSAGE("5");

			sceneTeacher();			// Teacher scene 

			if (elapseTime >= SCENE_TIME5 - 2.0f)
			{
				if (fadeOut_scene_4_alpha <= 1.0f)
				{
					fadeOut_scene_4_alpha = fadeOut_scene_4_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_4_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);
				gbSceneTeacher = FALSE;

			}
		}

	}

	SwapBuffers(ghdc);
}

void update(void)
{
	//code3
	//only at time of change animation here code will come

	//code

	if (enableFadeIn)
	{
		LOG_MESSAGE("enableFadeIn");
		fadeInAlpha = fadeInAlpha - 0.008f;
	}

	// Introduction scene 
	// Animation code
	if (gbSceneShraddhaIntro)
	{
		if (gbZoomOutFactor >= 0.05f)
		{
			gbZoomOutFactor = gbZoomOutFactor - 0.01f;
		}
		if (gbZoomOutFactor <= 0.05f)
		{
			gbStartYAnimation = TRUE;
		}
		if (gbStartYAnimation == TRUE)
		{
			if (gbYMovement <= 5.0f)
				gbYMovement = gbYMovement + 0.02f;

		}
		if (gbYMovement >= 5.0f)
		{
			gbLoadSaturn = TRUE;
		}
		if (elapseTime <= SCENE_TIME2 && elapseTime >= SCENE_TIME1)
		{
			if (gbTextureXMovement <= 4.0)
				gbTextureXMovement = gbTextureXMovement + 0.01;

			if (gbTextureXMovement <= 4.0)
				gbPresentsXMovement = gbPresentsXMovement + 0.01;
		}
		if (elapseTime >= SCENE_TIME2)
		{
			if (starsOpacity <= 1.0f)
				starsOpacity = starsOpacity + 0.01f;
			else
				starsOpacity = 0.0f;
		}
	}


	if (gbSceneKG)
	{
		if (dreamOpacity <= 1.0f)
		{
			dreamOpacity = dreamOpacity + 0.004f;
		}
	}

}

void uninitialize(void)

{
	// Function Declarations
	void ToggleFullScreen(void);

	//code
	// if application is exiting in fullScreen then ha pudcha code follow kela jael
	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
		gbFullScreen = FALSE;
	}

	// Make the HDC as Current Context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// Destroy Rendering Context or Delete Rendering Context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// Release HDC 
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


	// DistroyWindow
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	//Close The Log File
	if (gpFILE)
	{
		fprintf(gpFILE, "Program Ended Sucessfully...\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}

}

// -------------------------------------------------- User Define Funtion -------------------------------------------------

void LogMessage(const char *file, const char *function, const int lineNumber, const char *logEntry)
{
	if (gpFILE)
	{
		fprintf(gpFILE, "[%s:%s][%s::%s(%d)] %s\n", __DATE__, __TIME__, file, function, lineNumber, logEntry);
		fflush(gpFILE);
	}
}

void LogErrorMessage(const char *file, const char *function, const int lineNumber, const char *logEntry, int errorCode)
{
	if (gpFILE)
	{
		fprintf(gpFILE, "[%s:%s][%s::%s(%d)] %s: %d\n", __DATE__, __TIME__, file, function, lineNumber, logEntry, errorCode);
		fflush(gpFILE);
	}
}

void fadeIn(void)
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, fadeInAlpha);

	glVertex3f(20.0f, 20.0f, 0.0f);
	glVertex3f(-20.0f, 20.0f, 0.0f);
	glVertex3f(-20.0f, -20.0f, 0.0f);
	glVertex3f(20.0f, -20.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);

}

void fadeOut(void)
{

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4f(0.0f, 0.0f, 0.0f, fadeOutAlpha);

	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(-50.0f, 50.0f, 0.0f);
	glVertex3f(-50.0f, -50.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.0f);

	glEnd();

	glDisable(GL_BLEND);
}


// ------------------------------ Scene - Introduction Astromedicomp and Saturn ------------------------
void sceneIntroduction(void)
{
	// ASTROMEDICOMP - 13 letter - 2 repeat
	void SGD_draw_A(void);
	void SGD_draw_S(void);
	void SGD_draw_T(void);
	void SGD_draw_R(void);
	void SGD_draw_O(void);
	void SGD_draw_M(void);
	void SGD_draw_E(void);
	void SGD_draw_D(void);
	void SGD_draw_I(void);
	void SGD_draw_C(void);
	//void SGD_draw_O(void);
	//void SGD_draw_M(void);
	void SGD_draw_P(void);
	void SGD_draw_Saturn(void);
	void SGD_draw_Saturn_ring(void);

	// code
	LOG_MESSAGE("Scene 1 Start\n");
	glColor3f(1.0f, 0.4f, 0.0f);			// ASTROMEDICOMP NAME COLOR
	glLoadIdentity();
	glTranslatef(-13.8f, -2.0f + gbYMovement, -20.5f + gbZoomOutFactor); // A -20.5f
	SGD_draw_A();

	glTranslatef(2.3f, 0.0f, 0.0f);    // S
	SGD_draw_S();

	glTranslatef(2.3f, 0.0f, 0.0f);    // T
	SGD_draw_T();

	glTranslatef(2.3f, 0.0f, 0.0f);    // R
	SGD_draw_R();

	glTranslatef(2.3f, 0.0f, 0.0f);    // O
	SGD_draw_O();

	glTranslatef(2.3f, 0.0f, 0.0f);    // M
	SGD_draw_M();

	glTranslatef(2.3f, 0.0f, 0.0f);    // E
	SGD_draw_E();

	glTranslatef(2.3f, 0.0f, 0.0f);    // D
	SGD_draw_D();

	glTranslatef(2.3f, 0.0f, 0.0f);    // I
	SGD_draw_I();

	glTranslatef(2.3f, 0.0f, 0.0f);    // C
	SGD_draw_C();

	glTranslatef(2.3f, 0.0f, 0.0f);    // O
	SGD_draw_O();

	glTranslatef(2.3f, 0.0f, 0.0f);    // M
	SGD_draw_M();

	glTranslatef(2.3f, 0.0f, 0.0f);    // P
	SGD_draw_P();

	if (gbLoadSaturn == TRUE)
	{
		glLoadIdentity();
		glTranslatef(0.0f, -0.5f, -3.0f);
		SGD_draw_Saturn();

		glLoadIdentity();
		glTranslatef(0.0f, -0.5f, -3.0f);
		glRotatef(-100.0f, 1.0f, 0.0f, 1.0f);
		SGD_draw_Saturn_ring();
	}
	LOG_MESSAGE("Scene 1 End\n");
	fadeIn();
}

void SGD_draw_A(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 0.6f, 0.0f);
	glVertex3f(0.6f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.6f, 0.2f, 0.0f);
	glVertex3f(-0.6f, 0.2f, 0.0f);
	glVertex3f(-0.6f, -0.2f, 0.0f);
	glVertex3f(0.6f, -0.2f, 0.0f);
	glEnd();
}

void SGD_draw_S(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, -0.6f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.7f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-0.4f, 0.6f, 0.0f);
	glVertex3f(1.0f, -0.6f, 0.0f);
	glVertex3f(0.4f, -0.6f, 0.0f);
	glEnd();

}

void SGD_draw_T(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.20f, 0.6f, 0.0f);
	glVertex3f(0.20f, -1.0f, 0.0f);
	glVertex3f(-0.20f, -1.0f, 0.0f);
	glVertex3f(-0.20f, 0.6f, 0.0f);
	glEnd();
}

void SGD_draw_R(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.1f, 0.0f);
	glVertex3f(-0.65f, 0.1f, 0.0f);
	glVertex3f(-0.65f, -0.25f, 0.0f);
	glVertex3f(0.7f, -0.25f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, -0.24f, 0.0f);
	glVertex3f(-0.25f, -0.24f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);
	glEnd();
}

void SGD_draw_O(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.65f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, -0.6f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, 0.63f, 0.0f);
	glVertex3f(-1.0f, 0.63f, 0.0f);
	glVertex3f(-1.0f, -0.63, 0.0f);
	glVertex3f(-0.65f, -0.63f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.65f, 0.63f, 0.0f);
	glVertex3f(1.0f, 0.63f, 0.0f);
	glVertex3f(1.0f, -0.63, 0.0f);
	glVertex3f(0.65f, -0.63f, 0.0f);
	glEnd();
}

void SGD_draw_M(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, 1.0f, 0.0f);
	glVertex3f(-0.65f, 0.65f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.35f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(0.65f, 0.65f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.35f, 0.0f);
	glEnd();
}

void SGD_draw_E(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -0.65f, 0.0f);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, -0.15f, 0.0f);
	glVertex3f(0.05f, -0.15f, 0.0f);
	glVertex3f(0.05f, 0.15f, 0.0f);
	glVertex3f(-1.0f, 0.15f, 0.0f);

	glEnd();
}

void SGD_draw_D(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.65f, 0.6, 0.0f);
	glVertex3f(0.65f, -0.65f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -0.65f, 0.0f);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glEnd();
}

void SGD_draw_I(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.20f, 0.6f, 0.0f);
	glVertex3f(0.20f, -0.65f, 0.0f);
	glVertex3f(-0.20f, -0.65f, 0.0f);
	glVertex3f(-0.20f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, -0.6f, 0.0f);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.7f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -0.6f, 0.0f);
	glEnd();
}

void SGD_draw_C(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	// glBegin(GL_POLYGON);
	// 	glVertex3f(1.0f, -0.65f, 0.0f);
	// 	glVertex3f(1.0f, 0.6f, 0.0f);
	// 	glVertex3f(0.65f, 0.6, 0.0f);
	// 	glVertex3f(0.65f, -0.65f, 0.0f);
	// glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -0.65f, 0.0f);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glEnd();
}

void SGD_draw_P(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.1f, 0.0f);
	glVertex3f(0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.1f, 0.0f);
	glVertex3f(-0.65f, 0.1f, 0.0f);
	glVertex3f(-0.65f, -0.25f, 0.0f);
	glVertex3f(0.7f, -0.25f, 0.0f);
	glEnd();

	// glBegin(GL_POLYGON);
	// 	glVertex3f(-0.65f, -0.26f, 0.0f);
	// 	glVertex3f(-0.25f, -0.26f, 0.0f);
	// 	glVertex3f(1.0f, -1.0f, 0.0f);
	// 	glVertex3f(0.4f, -1.0f, 0.0f);
	// glEnd();
}

void SGD_draw_Saturn_ring(void)
{
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	float radius1 = 0.38f;
	for (float angle = 0.1f; angle < 360.0f; angle = angle + 0.1f)
	{
		if (!(angle <= 173.0f && angle >= 72.0f))
		{
			float x1 = (radius1 * cos((angle * M_PI) / 180.0f)) + (0.0);
			float y1 = (radius1 * sin((angle * M_PI) / 180.0f)) + (0.0);

			// Circle Origin
			glColor3f(0.8f, 0.8f, 0.9f);
			//glVertex3f(0.0, 0.0, -3.0f);

			// Circle cha parigh(Circumference) wrche points
			glVertex3f(x1, y1, 0.0f);
		}

	}
	glEnd();
	glPointSize(1.0f);
}

void SGD_draw_Saturn(void)
{
	glBegin(GL_LINES);
	float radius2 = 0.32f;
	for (float angle = 0.01f; angle < 360.0f; angle = angle + 0.01f)
	{
		float x1 = (radius2 * cos((angle * M_PI) / 180.0f)) + (0.0);
		float y1 = (radius2 * sin((angle * M_PI) / 180.0f)) + (0.0);

		// Circle Origin
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);

	}
	glEnd();

	glBegin(GL_LINES);
	float radius3 = 0.3f;
	for (float angle = 0.01f; angle < 360.0f; angle = angle + 0.01f)
	{
		float x1 = (radius3 * cos((angle * M_PI) / 180.0f)) + (0.0);
		float y1 = (radius3 * sin((angle * M_PI) / 180.0f)) + (0.0);

		// Circle Origin
		//glColor3f(0.91764f, 0.84705f, 0.67843f);
		glColor3f(0.5f, 0.5f, 0.8f);
		glVertex3f(0.0, 0.0, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);

	}
	glEnd();
}

// -------------------------- Scene - Texture Group Presents ----------------------
void sceneTextureGroupPresents(void)
{
	// Draw Texture
	//Function declarations
	void hms_drawT(void);
	void hms_drawE(void);
	void hms_drawX(void);
	void hms_drawU(void);
	void hms_drawR(void);
	void hms_drawG(void);
	void hms_drawO(void);
	void hms_drawP(void);
	void hms_drawN(void);
	void hms_drawS(void);

	LOG_MESSAGE("scene 2 start \n");
	//T
	glLoadIdentity();
	glTranslatef(-12.0f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawT();

	//E
	glLoadIdentity();
	glTranslatef(-11.2f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawE();

	//X
	glLoadIdentity();
	glTranslatef(-9.6f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawX();

	//T
	glLoadIdentity();
	glTranslatef(-8.4f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawT();

	//U
	glLoadIdentity();
	glTranslatef(-7.0f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawU();

	//R
	glLoadIdentity();
	glTranslatef(-6.1f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawR();

	//E
	glLoadIdentity();
	glTranslatef(-4.8f + gbTextureXMovement, 4.0f, -20.0f);
	hms_drawE();

	//G
	glLoadIdentity();
	glTranslatef(-3.2f, 0.0f, -20.0f);
	hms_drawG();

	//R
	glLoadIdentity();
	glTranslatef(-1.9f, 0.0f, -20.0f);
	hms_drawR();

	//O
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	hms_drawO();

	//U
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -20.0f);
	hms_drawU();


	//P
	glLoadIdentity();
	glTranslatef(2.5f, 0.0f, -20.0f);
	hms_drawP();

	//P
	glLoadIdentity();
	glTranslatef(4.2f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawP();

	//R
	glLoadIdentity();
	glTranslatef(5.5f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawR();

	//E
	glLoadIdentity();
	glTranslatef(6.8f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawE();

	//S
	glLoadIdentity();
	glTranslatef(8.0f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawS();

	//E
	glLoadIdentity();
	glTranslatef(9.1f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawE();

	//N
	glLoadIdentity();
	glTranslatef(10.2f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawN();

	//T
	glLoadIdentity();
	glTranslatef(11.8f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawT();

	//S
	glLoadIdentity();
	glTranslatef(12.6f - gbPresentsXMovement, -4.0f, -20.0f);
	hms_drawS();
	LOG_MESSAGE("scene 2 End \n");

}

void hms_drawT(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(0.6f, 0.8f, 0.0f);

	glEnd();
}

void hms_drawE(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.8f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);

	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.2f, -0.8f, 0.0f);
	glVertex3f(0.8f, -0.8f, 0.0f);

	glEnd();
}

void hms_drawX(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);

	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawU(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);

	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);

	glVertex3f(0.4f, -0.8f, 0.0f);
	glVertex3f(-0.4f, -0.8f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawR(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.8f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);
	glVertex3f(1.0f, -0.1f, 0.0f);

	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

}

void hms_drawG(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);
	glVertex3f(0.2f, -0.8f, 0.0f);
	glVertex3f(0.8f, -0.8f, 0.0f);

	glVertex3f(0.8f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.6f, -0.8f, 0.0f);
	glVertex3f(0.8f, -0.8f, 0.0f);

	glVertex3f(1.0f, 0.2f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(0.4f, 0.0f, 0.0f);
	glVertex3f(1.0, 0.0f, 0.0f);
	glEnd();

}

void hms_drawO(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.6f, 1.0f, 0.0f);
	glVertex3f(-0.6f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);

	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);

	glVertex3f(0.4f, -0.8f, 0.0f);
	glVertex3f(-0.4f, -0.8f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glVertex3f(0.4f, -1.0f, 0.0f);

	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);

	glEnd();
}

void hms_drawP(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.8f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);
	glVertex3f(1.0f, -0.1f, 0.0f);

	glEnd();
}

void hms_drawN(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.6f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(0.8f, -0.6f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}

void hms_drawS(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.2f, 0.8f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.2f, 0.1f, 0.0f);

	glVertex3f(0.8f, 0.1f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.0f, -0.1f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);

	glVertex3f(0.8f, -0.1f, 0.0f);
	glVertex3f(0.6f, -0.1f, 0.0f);
	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);

	glVertex3f(0.8f, -0.8f, 0.0f);
	glVertex3f(0.0f, -0.8f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glEnd();
}

// --------------------------- Scene - Theme Name Nurturing Dreams ------------------
void sceneTitle(void)
{
	void SGD_Cloud(float);
	void SGD_Stars(void);
	void SGD_Nurturing(void);
	void SGD_Dreams(void);
	void SGD_Bottle(void);

	LOG_MESSAGE("scene 3 start \n");
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -50.0f);
	// Purple background
	glBegin(GL_QUADS);
	glColor3f(0.2f, 0.0f, 0.3f);
	glVertex3f(40.0f, 22.0f, 0.0f);
	glVertex3f(-40.0f, 22.0f, 0.0f);
	//glColor3f(0.9f, 0.8f, 0.5f);
	glColor3f(0.3f, 0.0f, 0.4f);
	glVertex3f(-40.0f, 10.0f, 0.0f);
	glVertex3f(40.0f, 10.0f, 0.0f);

	//glColor3f(0.2f, 0.0f, 0.3f);
	glVertex3f(40.0f, 10.0f, 0.0f);
	glVertex3f(-40.0f, 10.0f, 0.0f);
	glColor3f(0.9f, 0.8f, 0.5f);
	glVertex3f(-40.0f, -22.0f, 0.0f);
	glVertex3f(40.0f, -22.0f, 0.0f);
	glEnd();

	// Cloud 1 from left
	glLoadIdentity();
	glTranslatef(-3.5f, 1.6f, -6.0f);
	SGD_Cloud(0.5f);
	glTranslatef(0.5f, 0.2f, 0.0f);
	SGD_Cloud(0.6f);
	glTranslatef(0.6f, -0.3f, 0.0f);
	SGD_Cloud(0.5f);

	// glTranslatef(-1.5f, 3.5f, -10.0f);

	// SGD_Stars();

	// Cloud 2
	glLoadIdentity();
	glTranslatef(-2.5f, 3.2f, -12.0f);
	SGD_Cloud(0.5f);
	glTranslatef(0.5f, 0.2f, 0.0f);
	SGD_Cloud(0.6f);
	glTranslatef(0.6f, -0.3f, 0.0f);
	SGD_Cloud(0.5f);

	// Cloud 3
	glLoadIdentity();
	glTranslatef(1.5f, 3.8f, -12.0f);
	SGD_Cloud(0.5f);
	glTranslatef(0.5f, 0.2f, 0.0f);
	SGD_Cloud(0.6f);
	glTranslatef(0.6f, -0.3f, 0.0f);
	SGD_Cloud(0.5f);

	// Cloud 4
	glLoadIdentity();
	glTranslatef(7.0f, 4.6f, -13.0f);
	SGD_Cloud(0.5f);
	glTranslatef(0.5f, 0.2f, 0.0f);
	SGD_Cloud(0.6f);
	glTranslatef(0.6f, -0.3f, 0.0f);
	SGD_Cloud(0.5f);

	glLoadIdentity();
	glTranslatef(4.0f, -2.5f, -20.0f);
	SGD_Nurturing();

	glLoadIdentity();
	glTranslatef(6.0f, -5.0f, -20.0f);
	SGD_Dreams();

	glLoadIdentity();
	glTranslatef(-5.0f, -3.5f, -14.0f);
	SGD_Bottle();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 0.0f, starsOpacity);
	glTranslatef(-5.0f, -4.5f, -16.0f);
	SGD_Stars();
	glTranslatef(-1.0f, -0.8f, 0.0f);
	SGD_Stars();
	glTranslatef(-1.0f, 0.5f, 0.0f);
	SGD_Stars();
	glTranslatef(0.4f, 1.0f, 0.0f);
	SGD_Stars();
	glTranslatef(0.1f, 1.5f, -6.0f);
	SGD_Stars();
	glTranslatef(-3.0f, 2.0f, -6.0f);
	SGD_Stars();
	glTranslatef(0.5f, 2.5f, -6.0f);
	SGD_Stars();
	glTranslatef(-3.0f, 3.0f, -7.0f);
	SGD_Stars();
	glDisable(GL_BLEND);

	LOG_MESSAGE("scene 3 end \n");
}

void SGD_Cloud(float radiusC)
{
	// ---------------------------------------- Cloud starts
	glBegin(GL_LINES);
	//float radiusC = 0.45f;
	for (float angle = 0.01f; angle < 360.0f; angle = angle + 0.01f)
	{
		float x1 = (radiusC * cos((angle * M_PI) / 180.0f)) + (0.0);
		float y1 = (radiusC * sin((angle * M_PI) / 180.0f)) + (0.0);

		// Circle Origin
		// glColor3f(0.0f, 0.9f, 0.8f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);

	}
	glEnd();
	// ---------------------------------------- Cloud ends
}

void SGD_Stars(void)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.75f, 0.0f);
	glVertex3f(-0.50f, -0.50f, 0.0f);
	glVertex3f(0.50f, -0.50f, 0.0f);

	glVertex3f(0.0f, -0.75f, 0.0f);
	glVertex3f(-0.50f, 0.25f, 0.0f);
	glVertex3f(0.50f, 0.25f, 0.0f);
	glEnd();
}

void SGD_Nurturing(void)
{
	void hms_drawU(void);
	void hms_drawR(void);
	void hms_drawT(void);
	void hms_drawI(void);
	void hms_drawN(void);
	void hms_drawG(void);


	// Draw N
	glColor3f(1.0, 0.0f, 0.0f);
	glColor3f(1.0, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.65f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0, 0.0f);
	glVertex3f(0.65f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.65f, 1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);

	glVertex3f(0.65f, -1.0f, 0.0f);
	glVertex3f(0.65f, -0.6f, 0.0f);
	glEnd();

	glTranslatef(9.5f, -5.3f, -30.0f);
	glColor3f(1.0, 1.0f, 1.0f);
	hms_drawU();
	glTranslatef(1.1f, 0.0f, 0.0f);
	hms_drawR();
	glTranslatef(1.6f, 0.0f, 0.0f);
	hms_drawT();
	glTranslatef(1.5f, 0.0f, 0.0f);
	hms_drawU();
	glTranslatef(1.1f, 0.0f, 0.0f);
	hms_drawR();
	glTranslatef(1.7f, 0.0f, 0.0f);
	hms_drawI();
	glTranslatef(1.1f, 0.0f, 0.0f);
	hms_drawN();
	glTranslatef(1.5f, 0.0f, 0.0f);
	hms_drawG();

}

void hms_drawI(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.6f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(0.6f, 0.8f, 0.0f);

	glVertex3f(0.6f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -1.0f, 0.0f);
	glVertex3f(-0.4f, -0.8f, 0.0f);
	glVertex3f(0.6f, -0.8f, 0.0f);

	glEnd();
}

void SGD_Dreams(void)
{
	void hms_drawR(void);
	void hms_drawE(void);
	void hms_drawA(void);
	void hms_drawM(void);
	void hms_drawS(void);

	// draw D
	glColor3f(1.0, 0.0f, 0.0f);
	glColor3f(1.0, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, 0.6f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glVertex3f(1.0f, 0.6f, 0.0f);
	glVertex3f(0.65f, 0.6, 0.0f);
	glVertex3f(0.65f, -0.65f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.7f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -1.0f, 0.0f);
	glVertex3f(-0.65f, -0.65f, 0.0f);
	glVertex3f(1.0f, -0.65f, 0.0f);
	glEnd();

	glTranslatef(12.0f, -9.0f, -30.0f);
	glColor3f(1.0, 1.0f, 1.0f);
	hms_drawR();
	glTranslatef(1.5f, 0.0f, 0.0f);
	hms_drawE();
	glTranslatef(1.4f, 0.0f, 0.0f);
	hms_drawA();
	glTranslatef(1.5f, 0.0f, 0.0f);
	hms_drawM();
	glTranslatef(1.5f, 0.0f, 0.0f);
	hms_drawS();
}

void hms_drawA(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.3f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glVertex3f(1.0f, 0.8f, 0.0f);

	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(1.0f, 0.2f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}

void hms_drawM(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.6f, 0.0f);
	glVertex3f(0.5f, -0.1f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glVertex3f(0.4f, -0.1f, 0.0f);
	glVertex3f(0.4, 0.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, 0.6f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}

void SGD_Bottle(void)
{
	glLineWidth(5.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.75f, 1.9f, 0.0f);
	glVertex3f(-0.7f, 1.8f, 0.0f);
	glVertex3f(-0.7f, 1.5f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.7f, 1.5f, 0.0f);
	glVertex3f(0.7f, 1.8f, 0.0f);
	glVertex3f(0.75f, 1.9f, 0.0f);
	glEnd();
	glLineWidth(1.0f);
}

// --------------------------- Scene - I want to be a Teacher ------------------

void sceneKinderGarden(void)
{
	// function prototypes	
	void SGD_drawClassroom(void);
	void SGD_Board(void);
	void SGD_BoradText(void);
	void SGD_drawBoy(float, float, float, float, float);
	void SGD_drawGirl(void);
	void SGD_Dream(void);
	void SGD_Banner(void);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);    // -z mhnje z axis atlya bajula sarkel mhnje chota honare // ya ult +ve dilaz tr baher alyasarkha kiva motha object disel

	SGD_drawClassroom();
	SGD_Board();
	SGD_BoradText();

	glTranslatef(-1.6f, 1.0f, 0.0f);
	glScalef(2.2f, 2.2f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 1.0f, 1.0f, 0.1f);

	glTranslatef(0.3f, 0.1f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.0f, 1.0f, 1.0f);
	SGD_drawGirl();

	glTranslatef(0.3f, -0.1f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.8f, 0.9f, 0.3f);

	glTranslatef(0.3f, 0.1f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.9f, 0.3f, 0.5f);
	SGD_drawGirl();

	glTranslatef(0.3f, -0.1f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.8f, 0.7f, 0.1f);
	SGD_drawGirl();


	glLoadIdentity();
	glTranslatef(0.5f, -0.3f, -3.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SGD_Dream();

	glDisable(GL_BLEND);


}

void SGD_drawClassroom(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.94862f, 0.91372f, 0.82784f);        // bhint
	glVertex3f(1.2f, 1.3f, 0.0f);
	glVertex3f(-3.0f, 1.3f, 0.0f);
	glVertex3f(-3.0f, -0.5f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.98862f, 0.90372f, 0.80784f);        // right side bhint
	glVertex3f(1.2f, 1.3f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(2.2f, 1.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.87843f, 0.67451f, 0.38823f);        // front Skirting 
	glVertex3f(-3.0f, -0.5f, 0.0f);
	glVertex3f(1.15f, -0.5f, 0.0f);
	glVertex3f(1.20f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -0.58f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.87843f, 0.67451f, 0.38823f);        // right side Skirting
	glVertex3f(1.1f, -0.5f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(2.1f, -1.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.60784f, 0.37647f, 0.27451f);        // Floor
	glVertex3f(1.20f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -1.3f, 0.0f);
	glVertex3f(2.1f, -1.3f, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);        // right side Skirting LINE	
	glVertex3f(-3.0f, -0.58f, 0.0f);
	glVertex3f(1.20f, -0.58f, 0.0f);
	glVertex3f(2.1f, -1.3f, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0.6f, 0.6f, 0.6f);        // Front Banner Line        
	glVertex3f(-2.2f, 1.3f, 0.0f);
	glVertex3f(-2.02f, 1.15f, 0.0f);
	glVertex3f(1.0f, 1.15f, 0.0f);
	glVertex3f(1.2f, 1.3f, 0.0f);
	glVertex3f(1.4f, 1.15f, 0.0f);
	glVertex3f(2.5f, 1.15f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);				// Front Banner   
	glColor3f(0.5f, 0.1f, 0.4f);
	for (float i = 0.0f; i <= 2.8f; i = i + 0.24f)
	{
		glVertex3f(-2.0f + i, 1.14f, 0.0f);
		glVertex3f(-1.7f + i, 1.14f, 0.0f);
		glVertex3f(-1.85f + i, 1.0f, 0.0f);
	}
	glEnd();

	glBegin(GL_TRIANGLES);				// Right side Banner 
	glColor3f(0.5f, 0.1f, 0.4f);
	for (float i = -0.3f; i <= 0.45f; i = i + 0.24f)
	{
		glVertex3f(2.0f + i, 1.14f, 0.0f);
		glVertex3f(1.7f + i, 1.14f, 0.0f);
		glVertex3f(1.85f + i, 1.0f, 0.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.8f, 0.8f);        // right side bhint painting
	glVertex3f(1.6f, 0.3f, 0.0f);
	glVertex3f(1.6f, -0.5f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.4f, 0.4f, 0.4f);        // right side bhint painting border
	glVertex3f(1.6f, 0.3f, 0.0f);
	glVertex3f(1.6f, -0.5f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, 0.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.2f, 0.4f);        // right side bhint painting - SQUARE
	glVertex3f(1.7f, -0.3f, 0.0f);
	glVertex3f(1.7f, -0.5f, 0.0f);
	glVertex3f(1.8f, -0.55f, 0.0f);
	glVertex3f(1.8f, -0.35f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.4f, 0.4f);        // right side bhint painting - Triangle
	glVertex3f(1.8f, -0.26f, 0.0f);
	glVertex3f(1.95f, -0.3f, 0.0f);
	glVertex3f(1.9f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 0.0f);        // right side bhint painting - Rhombus
	glVertex3f(1.7f, 0.05f, 0.0f);
	glVertex3f(1.8f, 0.05f, 0.0f);
	glVertex3f(1.75f, -0.1f, 0.0f);
	glVertex3f(1.65f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINES);					// right side bhint painting - circle
	float radius = 0.07f;
	for (float angle = 0.01f; angle < 360; angle = angle + 0.01f)
	{

		float x1 = (radius * cos((angle * M_PI) / 180.0f)) + (1.9);
		float y1 = (radius * sin((angle * M_PI) / 180.0f)) + (0.05);

		y1 = y1 + 0.1;

		// Circle Origin
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.9f, 0.1f, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();

}

void SGD_Board(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.3f, 0.2f);        // Board Outline
	glVertex3f(-2.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.2f, 0.0f);
	glVertex3f(-2.0f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.5f, 0.3f);        // Board
	glVertex3f(-1.9f, 0.4f, 0.0f);
	glVertex3f(-0.6f, 0.4f, 0.0f);
	glVertex3f(-0.6f, -0.1f, 0.0f);
	glVertex3f(-1.9f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);        // Board lines
	glVertex3f(-1.9f, 0.4f, 0.0f);
	glVertex3f(-0.6f, 0.4f, 0.0f);

	glVertex3f(-0.6f, -0.1f, 0.0f);
	glVertex3f(-1.9f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);        // Board lines
	glVertex3f(-2.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);

	glVertex3f(-0.5f, -0.2f, 0.0f);
	glVertex3f(-2.0f, -0.2f, 0.0f);
	glEnd();

}

void SGD_BoradText(void)
{
	// create bitmaps for the device context font's first 256 glyphs
	wglUseFontBitmaps(ghdc, 0, 256, 1000);

	// set up for a string-drawing display list call
	glListBase(1000);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(-1.7f, 0.15f);
	glCallLists(41, GL_UNSIGNED_BYTE, "What do you want to be when you grow up?");


	glRasterPos2f(-0.88f, 0.34f);
	glCallLists(11, GL_UNSIGNED_BYTE, "18-09-2007");

	// get all those commands to execute
	glFlush();

	// delete our 256 glyph display lists
	glDeleteLists(1000, 256);
}

void SGD_drawBoy(float Boy_Xaxis, float Boy_Yaxis, float red, float green, float blue)
{

	// kid - 2 -boy - kaan
	glColor3f(1.0f, 0.9f, 0.7f);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(-0.16f, -0.84f, 0.0f);
	glVertex3f(-0.04f, -0.84f, 0.0f);
	glEnd();
	glPointSize(1.0);


	// kid 2 - boy - haath
	glColor3f(1.0f, 0.9f, 0.7f);
	glLineWidth(10.0);
	glBegin(GL_LINES);
	glVertex3f(-0.165f, -0.97f, 0.0f);
	glVertex3f(-0.17f, -1.0f, 0.0f);

	glVertex3f(-0.04f, -0.95f, 0.0f);
	glVertex3f(-0.03f, -1.0f, 0.0f);
	glEnd();
	glLineWidth(1.0);

	// kid 2 - boy - dress		
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);

	glVertex3f(-0.15f, -0.89f, 0.0f);
	glVertex3f(-0.02f, -0.97f, 0.0f);
	glVertex3f(-0.05f, -0.89f, 0.0f);
	glVertex3f(-0.05f, -1.0f, 0.0f);
	glVertex3f(-0.15f, -1.0f, 0.0f);
	glVertex3f(-0.15f, -0.97f, 0.0f);
	glVertex3f(-0.18f, -0.97f, 0.0f);
	glEnd();


	// kid 2 - boy - head
	glBegin(GL_LINES);
	float radius = 0.06f;
	for (float angle = 0.01f; angle < 360; angle = angle + 0.01f)
	{

		float x1 = (radius * cos((angle * M_PI) / 180.0f)) + (Boy_Xaxis);
		float y1 = (radius * sin((angle * M_PI) / 180.0f)) + (Boy_Yaxis - 0.06);

		y1 = y1 + 0.1;

		// Circle Origin
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex3f(Boy_Xaxis, Boy_Yaxis, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();
}

void SGD_drawGirl(void)
{

	// kid - 2 -girl - kes
	glColor3f(0.2f, 0.2f, 0.2f);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(-0.16f, -0.86f, 0.0f);
	glVertex3f(-0.04f, -0.86f, 0.0f);

	glVertex3f(-0.16f, -0.875f, 0.0f);
	glVertex3f(-0.04f, -0.875f, 0.0f);

	glVertex3f(-0.16f, -0.89f, 0.0f);
	glVertex3f(-0.04f, -0.89f, 0.0f);
	glEnd();
	glPointSize(1.0);
}

void SGD_Dream(void)
{
	glBegin(GL_TRIANGLES);
	glColor4f(0.5f, 0.5f, 0.7f, dreamOpacity);
	glVertex3f(0.0f, -0.12f, 0.0f);
	glVertex3f(-0.15f, -0.22f, 0.0f);
	glVertex3f(-0.15f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	float radius = 0.25f;
	for (float angle = 0.01f; angle < 360; angle = angle + 0.01f)
	{

		float x1 = (radius * cos((angle * M_PI) / 180.0f)) + (0.0);
		float y1 = (radius * sin((angle * M_PI) / 180.0f)) + (0.0);

		y1 = y1 + 0.1;

		// Circle Origin
		glColor4f(1.0f, 1.0f, 1.0f, dreamOpacity);
		glVertex3f(0.0f, 0.0f, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glColor4f(0.5f, 0.5f, 0.7f, dreamOpacity);
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();

	// create bitmaps for the device context font's first 256 glyphs
	wglUseFontBitmaps(ghdc, 0, 256, 1000);

	// set up for a string-drawing display list call
	glListBase(1000);

	glColor4f(0.0f, 0.0f, 0.0f, dreamOpacity);

	glRasterPos2f(-0.13f, 0.08f);
	//TCHAR dream[] = TEXT("मला आई सारखं शिक्षिका व्हायचंय .... ");
	glCallLists(16, GL_UNSIGNED_BYTE, "Teacher.....!!!");

	// get all those commands to execute
	glFlush();

	// delete our 256 glyph display lists
	glDeleteLists(1000, 256);

}

// ------------------------------------- Scene Teacher -------------------------------

void sceneTeacher(void)
{
	// function prototypes	
	void SGD_drawCollageClassroom(void);
	void SGD_drawBookshelf(void);
	void SGD_drawVase(void);
	void SGD_drawBooks(void);
	void SGD_CollegeBoard(void);
	void SGD_CollegeBoardText(void);
	void SGD_drawBoy(float, float, float, float, float);
	void SGD_drawGirl(void);
	void SGD_drawTeacher(void);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);    // -z mhnje z axis atlya bajula sarkel mhnje chota honare // ya ult +ve dilaz tr baher alyasarkha kiva motha object disel

	SGD_drawCollageClassroom();

	glLoadIdentity();
	glTranslatef(0.0f, 0.2f, -3.0f);
	glScalef(1.2f, 1.2f, 1.2f);
	SGD_CollegeBoard();
	SGD_CollegeBoardText();

	glLoadIdentity();
	glTranslatef(-3.45f, 0.5f, -3.0f);
	glScalef(2.0f, 1.5f, 2.0f);
	SGD_drawBookshelf();

	glLoadIdentity();
	glTranslatef(-3.5f, 0.5f, -3.0f);
	glScalef(2.0f, 1.5f, 2.0f);
	SGD_drawVase();
	SGD_drawBooks();

	glLoadIdentity();
	glTranslatef(0.9f, -0.2f, -3.0f);
	SGD_drawTeacher();

	glLoadIdentity();
	glTranslatef(-0.8f, 0.25f, -3.0f);
	glScalef(1.6f, 1.6f, 1.6f);
	SGD_drawBoy(-0.1, -0.88, 0.7f, 0.8f, 0.0f);

	glTranslatef(0.3f, 0.05f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.9f, 0.4f, 0.0f);
	SGD_drawGirl();

	glTranslatef(0.3f, 0.0f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.6f, 0.0f, 0.5f);
	SGD_drawGirl();

	glTranslatef(0.3f, -0.04f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.5f, 0.4f, 0.8f);

	glTranslatef(0.25f, 0.05f, 0.0f);
	SGD_drawBoy(-0.1, -0.88, 0.9f, 0.3f, 0.5f);
	SGD_drawGirl();
}

void SGD_drawCollageClassroom(void)
{

	glBegin(GL_QUADS);
	glColor3f(0.94862f, 0.91372f, 0.82784f);        // bhint
	glColor3f(0.9f, 0.9f, 0.7f);        // bhint
	glVertex3f(1.2f, 1.3f, 0.0f);
	glVertex3f(-3.0f, 1.3f, 0.0f);
	glVertex3f(-3.0f, -0.5f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.98862f, 0.90372f, 0.80784f);        // right side bhint
	glColor3f(0.9f, 0.9f, 0.7f);        // right side bhint
	glVertex3f(1.2f, 1.3f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(2.2f, 1.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.4f, 0.4f);        // front Skirting 
	glVertex3f(-3.0f, -0.5f, 0.0f);
	glVertex3f(1.15f, -0.5f, 0.0f);
	glVertex3f(1.20f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -0.58f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.4f, 0.4f);        // right side Skirting
	glVertex3f(1.1f, -0.5f, 0.0f);
	glVertex3f(1.2f, -0.5f, 0.0f);
	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(2.1f, -1.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.7f, 0.7f);        // Floor
	glVertex3f(1.20f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -0.58f, 0.0f);
	glVertex3f(-3.0f, -1.3f, 0.0f);
	glVertex3f(2.1f, -1.3f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.8f, 0.9f);        // right side window
	glVertex3f(2.0f, 0.3f, 0.0f);
	glVertex3f(1.6f, 0.3f, 0.0f);
	glColor3f(0.6f, 0.8f, 0.3f);
	glVertex3f(1.6f, -0.5f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);

	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.4f, 0.4f, 0.4f);        // right side bhint painting border
	glVertex3f(1.6f, 0.3f, 0.0f);
	glVertex3f(1.6f, -0.5f, 0.0f);
	glVertex3f(2.0f, -0.8f, 0.0f);
	glVertex3f(2.0f, 0.3f, 0.0f);
	glEnd();

}

void SGD_CollegeBoard(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.3f, 0.2f);        // Board Outline
	glVertex3f(-2.0f + 1.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f + 1.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f + 1.0f, -0.2f, 0.0f);
	glVertex3f(-2.0f + 1.0f, -0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.5f, 0.3f);        // Board
	glVertex3f(-1.9f + 1.0f, 0.4f, 0.0f);
	glVertex3f(-0.6f + 1.0f, 0.4f, 0.0f);
	glVertex3f(-0.6f + 1.0f, -0.1f, 0.0f);
	glVertex3f(-1.9f + 1.0f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);        // Board lines
	glVertex3f(-1.9f + 1.0f, 0.4f, 0.0f);
	glVertex3f(-0.6f + 1.0f, 0.4f, 0.0f);

	glVertex3f(-0.6f + 1.0f, -0.1f, 0.0f);
	glVertex3f(-1.9f + 1.0f, -0.1f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 0.0f, 0.0f);        // Board lines
	glVertex3f(-2.0f + 1.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f + 1.0f, 0.5f, 0.0f);

	glVertex3f(-0.5f + 1.0f, -0.2f, 0.0f);
	glVertex3f(-2.0f + 1.0f, -0.2f, 0.0f);
	glEnd();

}

void SGD_CollegeBoardText(void)
{
	// create bitmaps for the device context font's first 256 glyphs
	wglUseFontBitmaps(ghdc, 0, 256, 1000);

	// set up for a string-drawing display list call
	glListBase(1000);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(-1.7f + 1.15f, 0.15f);
	glCallLists(20, GL_UNSIGNED_BYTE, "HAPPY TEACHER'S DAY");

	glRasterPos2f(-1.7f + 1.355f, 0.05f);
	glCallLists(5, GL_UNSIGNED_BYTE, "2027");

	glRasterPos2f(-0.88f + 0.99f, 0.34f);
	glCallLists(11, GL_UNSIGNED_BYTE, "5-09-2027");

	// get all those commands to execute
	glFlush();

	// delete our 256 glyph display lists
	glDeleteLists(1000, 256);
}

// user defined function : draw bookshelf
void SGD_drawBookshelf(void)
{
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.5f, 0.3f, 0.2f);
	glPointSize(12.0f);		//set point size to X pixels    
	glBegin(GL_POINTS); 	//starts drawing of points	
	glVertex3f(0.64f, -0.88f, 0.0f);
	glVertex3f(0.998f, -0.885f, 0.0f);

	glVertex3f(0.8f, -0.67f, 0.0f);
	glVertex3f(0.84f, -0.67f, 0.0f);
	glEnd();
	glPointSize(1.0f);		//reset point size to X pixels  

	// outer border of bookShelf
	glColor3f(0.5f, 0.3f, 0.2f);
	glBegin(GL_POLYGON);
	glVertex3f(0.63f, -0.88f, 0.0f);
	glVertex3f(0.998f, -0.88f, 0.0f);
	glVertex3f(0.998f, -0.3f, 0.0f);
	glVertex3f(0.63f, -0.3f, 0.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex3f(0.64f, -0.85f, 0.0f);
	glVertex3f(0.993f, -0.85f, 0.0f);
	glVertex3f(0.993f, -0.32f, 0.0f);
	glVertex3f(0.64f, -0.32f, 0.0f);
	glEnd();

	glLineWidth(3.0f);	// set line width	
	glBegin(GL_LINES);
	glColor3f(0.5f, 0.3f, 0.2f);			// bookshelf partition line
	glVertex3f(0.82f, -0.85f, 0.0f);
	glVertex3f(0.82f, -0.32f, 0.0f);

	glColor3f(0.4f, 0.2f, 0.0f);			// handle
	glVertex3f(0.8f, -0.65f, 0.0f);
	glVertex3f(0.8f, -0.55f, 0.0f);

	glVertex3f(0.84f, -0.65f, 0.0f);
	glVertex3f(0.84f, -0.55f, 0.0f);
	glEnd();
	glLineWidth(1.0f);	// reset line width	

	// handle points
	glEnable(GL_POINT_SMOOTH);
	glColor3f(0.4f, 0.2f, 0.0f);
	glPointSize(6.0f);		//set point size to X pixels    
	glBegin(GL_POINTS); 	//starts drawing of points	
	glVertex3f(0.8f, -0.53f, 0.0f);
	glVertex3f(0.84f, -0.53f, 0.0f);

	glVertex3f(0.8f, -0.67f, 0.0f);
	glVertex3f(0.84f, -0.67f, 0.0f);
	glEnd();
	glPointSize(1.0f);		//reset point size to X pixels  


	glLoadIdentity();
	glTranslatef(-1.35f, 0.2f, -3.0f);
	// create bitmaps for the device context font's first 256 glyphs
	wglUseFontBitmaps(ghdc, 0, 256, 1000);

	// set up for a string-drawing display list call
	glListBase(1000);

	glColor3f(1.0f, 0.0f, 0.0f);

	glRasterPos2f(-0.4f, -0.25f);
	glCallLists(11, GL_UNSIGNED_BYTE, "Class - SY");

	// get all those commands to execute
	glFlush();

	// delete our 256 glyph display lists
	glDeleteLists(1000, 256);

}

// draw vase on bookshelf
void SGD_drawVase(void)
{
	glColor3f(0.1f, 0.2f, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(0.75f, -0.295f, 0.0f);
	glVertex3f(0.837f, -0.295f, 0.0f);
	glVertex3f(0.815f, -0.27f, 0.0f);
	glVertex3f(0.775f, -0.27f, 0.0f);
	glEnd();

	glColor3f(0.1f, 0.2f, 0.3f);
	glBegin(GL_LINES);
	glVertex3f(0.814f, -0.27f, 0.0f);
	glVertex3f(0.805f, -0.1f, 0.0f);

	glVertex3f(0.776f, -0.27f, 0.0f);
	glVertex3f(0.785f, -0.1f, 0.0f);

	glVertex3f(0.785f, -0.12f, 0.0f);
	glVertex3f(0.805f, -0.12f, 0.0f);

	glVertex3f(0.785f, -0.124f, 0.0f);
	glVertex3f(0.805f, -0.124f, 0.0f);

	glVertex3f(0.785f, -0.126f, 0.0f);
	glVertex3f(0.805f, -0.126f, 0.0f);

	glVertex3f(0.785f, -0.13f, 0.0f);
	glVertex3f(0.805f, -0.13f, 0.0f);

	glVertex3f(0.785f, -0.132f, 0.0f);
	glVertex3f(0.805f, -0.132f, 0.0f);
	glEnd();

	// flower sticks
	glColor3f(0.4f, 0.2f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.79f, -0.12f, 0.0f);
	glVertex3f(0.78f, 0.02f, 0.0f);

	glVertex3f(0.795f, -0.12f, 0.0f);
	glVertex3f(0.782f, 0.115f, 0.0f);

	glVertex3f(0.8f, -0.12f, 0.0f);
	glVertex3f(0.792f, 0.15f, 0.0f);

	glVertex3f(0.805f, -0.12f, 0.0f);
	glVertex3f(0.815f, 0.19f, 0.0f);

	glVertex3f(0.801f, -0.12f, 0.0f);
	glVertex3f(0.825f, 0.15f, 0.0f);
	glEnd();

	// flowers

	glPointSize(7.0f);		//set point size	
	glBegin(GL_POINTS); 	//starts drawing of points	
	glColor3f(0.8235f, 0.0157f, 0.1765f);
	glVertex3f(0.78f, 0.02f, 0.0f);
	glVertex3f(0.782f, 0.115f, 0.0f);
	glVertex3f(0.792f, 0.15f, 0.0f);
	glVertex3f(0.8f, 0.09f, 0.0f);
	glVertex3f(0.815f, 0.19f, 0.0f);
	glVertex3f(0.825f, 0.15f, 0.0f);
	glColor3f(1.7f, 0.5f, 0.5f);
	glVertex3f(0.775f, 0.04f, 0.0f);
	glVertex3f(0.78f, 0.08f, 0.0f);
	glVertex3f(0.8f, 0.12f, 0.0f);
	glVertex3f(0.8f, 0.06f, 0.0f);
	glVertex3f(0.813f, 0.15f, 0.0f);
	glVertex3f(0.83f, 0.17f, 0.0f);
	glEnd();
	glPointSize(1.0f);		//reset point size
}

void SGD_drawBooks(void)
{

	glLineWidth(12.0f);	// set line width	
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.85f, -0.287f, 0.0f);
	glVertex3f(0.955f, -0.287f, 0.0f);

	glColor3f(0.0f, 0.6f, 0.8f);
	glVertex3f(0.835f, -0.25f, 0.0f);
	glVertex3f(0.936f, -0.25f, 0.0f);

	glColor3f(0.9f, 0.4f, 0.0f);
	glVertex3f(0.846f, -0.22f, 0.0f);
	glVertex3f(0.946f, -0.22f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.4f);
	glVertex3f(0.967f, -0.3f, 0.0f);
	glVertex3f(0.942f, -0.18f, 0.0f);

	glColor3f(1.0f, 0.9f, 0.4f);
	glVertex3f(0.98f, -0.3f, 0.0f);
	glVertex3f(0.975f, -0.16f, 0.0f);

	glEnd();

	glLineWidth(1.0f);	// reset line width	
}

void SGD_drawTeacher(void)
{
	void SGD_drawFace(float, float, float, float, float, float);

	// code


	SGD_drawFace(0.2f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f);		// hair

															// neck
	glColor3f(1.0f, 0.9f, 0.7f);
	glColor3f(0.9f, 0.7f, 0.5f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(-0.1f, 0.35f, 0.0f);
	glVertex3f(0.1f, 0.35f, 0.0f);
	glEnd();

	//SGD_drawFace(0.15f, 0.0f, 0.45f, 1.0f, 0.9f, 0.7f);		// face
	SGD_drawFace(0.15f, 0.0f, 0.45f, 0.9f, 0.7f, 0.5f);		// face

															// shirt
	glColor3f(1.0f, 0.5f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.22f, 0.35f, 0.0f);
	glVertex3f(0.22f, 0.35f, 0.0f);
	glVertex3f(0.17f, 0.0f, 0.0f);
	glVertex3f(-0.17f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.38f, 0.1f, 0.0f);
	glVertex3f(-0.22f, 0.35f, 0.0f);
	glVertex3f(-0.17f, 0.15f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.38f, 0.1f, 0.0f);
	glVertex3f(0.22f, 0.35f, 0.0f);
	glVertex3f(0.17f, 0.15f, 0.0f);
	glEnd();

	// Skirt
	glColor3f(0.2f, 0.0f, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(0.17f, 0.0f, 0.0f);
	glVertex3f(-0.17f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.6f, 0.0f);
	glVertex3f(0.3f, -0.6f, 0.0f);
	glEnd();

	// legs
	glColor3f(1.0f, 0.9f, 0.7f);
	glColor3f(0.9f, 0.7f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.05f, -0.6f, 0.0f);
	glVertex3f(-0.15f, -0.6f, 0.0f);
	glVertex3f(-0.15f, -0.75f, 0.0f);
	glVertex3f(-0.05f, -0.75f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(0.0f, -0.6f, 0.0f);
	glVertex3f(0.1f, -0.6f, 0.0f);
	glVertex3f(0.1f, -0.75f, 0.0f);
	glVertex3f(0.0f, -0.75f, 0.0f);
	glEnd();

	glPointSize(10.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	// eyes
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.05f, 0.57f, 0.0f);
	glVertex3f(0.05f, 0.57f, 0.0f);

	// lips
	glVertex3f(-0.01f, 0.5f, 0.0f);
	glVertex3f(0.01f, 0.5f, 0.0f);
	glColor3f(1.0f, 0.9f, 0.7f);
	glColor3f(0.9f, 0.7f, 0.5f);
	glVertex3f(-0.01f, 0.51f, 0.0f);
	glVertex3f(0.01f, 0.51f, 0.0f);

	// shirt buttons
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.25f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.15f, 0.0f);

	// skirt buttons
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.03f, -0.03f, 0.0f);
	glVertex3f(0.0f, -0.03f, 0.0f);
	glVertex3f(0.03f, -0.03f, 0.0f);
	glEnd();

	// hands
	glColor3f(1.0f, 0.9f, 0.7f);
	glColor3f(0.9f, 0.7f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.35f, 0.11f, 0.0f);
	glVertex3f(-0.28f, -0.05f, 0.0f);
	glVertex3f(-0.22f, -0.05f, 0.0f);
	glVertex3f(-0.2f, 0.15f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.35f, 0.11f, 0.0f);
	glVertex3f(0.28f, -0.05f, 0.0f);
	glVertex3f(0.22f, -0.05f, 0.0f);
	glVertex3f(0.2f, 0.15f, 0.0f);
	glEnd();
}

void SGD_drawFace(float radius, float teacher_Xaxis, float teacher_Yaxis, float red, float green, float blue)
{

	glBegin(GL_LINES);
	for (float angle = 0.01f; angle < 360; angle = angle + 0.01f)
	{

		float x1 = (radius * cos((angle * M_PI) / 180.0f)) + (teacher_Xaxis);
		float y1 = (radius * sin((angle * M_PI) / 180.0f)) + (teacher_Yaxis);

		y1 = y1 + 0.1;

		// Circle Origin
		glColor3f(red, green, blue);
		glVertex3f(teacher_Xaxis, teacher_Yaxis, 0.0f);

		// Circle cha parigh(Circumference) wrche points
		glVertex3f(x1, y1, 0.0f);
	}
	glEnd();

}

