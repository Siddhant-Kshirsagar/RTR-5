// Windows header files
// Common headers 
#include<windows.h> //win32 IO
#include<stdio.h> //for file IO
#include<stdlib.h>  //for exit
#include "OGL.h" // swatachi header file
#include "SukrutScene.h" // sukrut code header file

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
#define SCENE_TIME1 11		// scene duration in SECONDS
#define SCENE_TIME2 21
#define SCENE_TIME3 33
#define SCENE_TIME4 40
#define SCENE_TIME5 52
#define SCENE_TIME6 66
#define SCENE_TIME7 77
#define SCENE_TIME8 82
#define SCENE_TIME9 92
#define SCENE_TIME10 108
#define SCENE_TIME11 121


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

GLfloat gfStarYAxis = 2.5f;

#pragma endregion

#pragma region Shraddha KG scene

GLfloat dreamOpacity = 0.0f;
BOOL gbSceneKG = FALSE;
GLfloat gfSceneKG_ZAxis = 0.0f;
#pragma endregion

#pragma region Shraddha Teacher scene
BOOL gbSceneTeacher = FALSE;
#pragma endregion

#pragma region Siddhant Elephant scene

//1st param radius of circle (float)
//2nd param x coordinate of center of circle (float)
//3rd param y coordinate of center of circle (float)
//4th param starting angle of circle arc(float)
//5th param ending angle of circle arc(float)
void SGK_drawCircle(float, float, float, float, float);

//1st param radius of circle (float)
//2nd param x coordinate of center of circle (float)
//3rd param y coordinate of center of circle (float)
//4th param starting angle of circle arc(float)
//5th param ending angle of circle arc(float)
void SGK_drawHollowCircle(float, float, float, float, float);

//1st param x coordinate of bottom of grass
//2nd param y coordinate of bottom of grass
//3rd param height of the grass
void SGK_DrawGrass(float, float, float);

GLfloat SGK_cAngle = 0.0f;
GLfloat SGK_eyeVertex[] = { 0.0f,0.0f,0.0f };
GLfloat SGK_eyeVertex_Y = 10.0f;
GLfloat SGK_upVertex_Z = -1;
BOOL SGK_gbGrassRight = FALSE; // for grass animation
BOOL SGK_gbEarRight = FALSE; //for ear animation
float SGK_gfEarMove = 0.0f;
float SGK_gfGrassMove = 0.0f; // for grass animation
float SGK_sceneAlpha = 1.0f;
float SGK_sceneAlphaIncrement = 0.0f;

struct SGK_MYCOLOR
{
	float rColor;
	float gColor;
	float bColor;
};

BOOL gbSceneElephant = FALSE;
BOOL gbSceneThinking = TRUE;
#pragma endregion

#pragma region Hrishikesh End Credit scene

// function declaration
void hms_drawAEndCredit(void);
void hms_drawA(void);
void hms_drawD(void);
void hms_drawE(void);
void hms_drawG(void);
void hms_drawH(void);
void hms_drawI(void);
void hms_drawJ(void);
void hms_drawK(void);
void hms_drawL(void);
void hms_drawM(void);
void hms_drawMEndCredit(void);
void hms_drawN(void);
void hms_drawO(void);
void hms_drawP(void);
void hms_drawR(void);
void hms_drawS(void);
void hms_drawT(void);
void hms_drawU(void);
void hms_drawV(void);
void hms_drawX(void);
void hms_drawY(void);
void hms_drawDash(void);

//for animation
float y_Texture = -15.0f;
float x_Texture = -8.0f;
float y_Pratik = -32.0f;
float x_Pratik = -15.0f;
float y_Siddhant = -36.0f;
float x_Siddhant = -15.0f;
float y_Hrishikesh = -40.0f;
float x_Hrishikesh = -14.5f;
float y_Prathamesh = -44.0f;
float x_Prathamesh = -15.0f;
float y_Pushkar = -48.0f;
float x_Pushkar = -15.0f;
float y_Shraddha = -52.0f;
float x_Shraddha = -15.0f;
float y_Sukrut = -56.0f;
float x_Sukrut = -15.0f;
float y_Vinay = -60.0f;
float x_Vinay = -14.0f;

BOOL gbEndCredit = FALSE;

#pragma endregion

#pragma region Vinay Drawing Scene
BOOL gbDrawingScene = FALSE;
GLfloat xRotationf = 0.0f;
#pragma endregion

#pragma region Pushkar reading scene

BOOL gbReadingScene = FALSE;
#pragma endregion

#pragma region Prathamesh Kitchen Scene
GLfloat Translate_Line1_Y = 0.35f;
GLfloat Translate_Line2_Y = 0.0f;
GLfloat Translate_Line3_Y = 0.0f;
GLfloat Translate_Line4_Y = 0.0f;

BOOL gbKitchenScene = FALSE;
#pragma endregion

#pragma region Sukrut Study Scene

BOOL gbStudyScene = FALSE;

#pragma endregion

void LogMessage(const char *, const char *, const int, const char *);
void LogErrorMessage(const char *, const char *, const int, const char *, int);


//scene global variables
BOOLEAN bScene1_Done = FALSE;
StopWatchInterface *SGD_timer;

float fadeOut_scene_1_alpha = 0.0f;
float fadeOut_scene_2_alpha = 0.0f;
float fadeOut_scene_3_alpha = 0.0f;
float fadeOut_scene_4_alpha = 0.0f;
float fadeOut_scene_5_alpha = 0.0f;
float fadeOut_scene_6_alpha = 0.0f;
float fadeOut_scene_7_alpha = 0.0f;
float fadeOut_scene_8_alpha = 0.0f;
float fadeOut_scene_9_alpha = 0.0f;
float fadeOut_scene_10_alpha = 0.0f;
float fadeOut_scene_11_alpha = 0.0f;

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

	PlaySound(MAKEINTRESOURCE(MYMUSIC), NULL, SND_RESOURCE | SND_ASYNC);

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
		PlaySound(NULL, 0, 0);
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
	pfd.cDepthBits = 32;

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
	void sceneElephant(void);
	void sceneEndCredit(void);
	void sceneDrawing(void);
	void sceneReading(void);
	void sceneKitchen(void);
	void sceneStudy(void);

	void fadeIn(void);
	void fadeOut(void);
	

	//code
	if (!gbKitchenScene)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	elapseTime = sdkGetTimerValue(&SGD_timer);
	elapseTime = elapseTime / 1000;		// CONVERT IN SECONDS

	if (bScene1_Done == FALSE)
	{
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
			LOG_MESSAGE("5");

			gbDrawingScene = TRUE;
			sceneDrawing();			// Drawing Scene

			if (elapseTime >= SCENE_TIME5 - 2.0f)
			{
				if (fadeOut_scene_5_alpha <= 1.0f)
				{
					fadeOut_scene_5_alpha = fadeOut_scene_5_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_5_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);

				gbDrawingScene = FALSE;


			}
		}
		else if (elapseTime <= SCENE_TIME6 && elapseTime >= SCENE_TIME5)
		{
			gbSceneElephant = TRUE;
			LOG_MESSAGE("6");

			sceneElephant();			// Elephant scene 

			if (elapseTime >= SCENE_TIME6 - 2.0f)
			{
				if (fadeOut_scene_6_alpha <= 1.0f)
				{
					fadeOut_scene_6_alpha = fadeOut_scene_6_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_6_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);
				gbSceneElephant = FALSE;

			}
		}
		else if (elapseTime <= SCENE_TIME7 && elapseTime >= SCENE_TIME6)
		{
			gbReadingScene = TRUE;
			LOG_MESSAGE("7");

			sceneReading();			// Reading scene 

			if (elapseTime >= SCENE_TIME7 - 2.0f)
			{
				if (fadeOut_scene_7_alpha <= 1.0f)
				{
					fadeOut_scene_7_alpha = fadeOut_scene_7_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_7_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);
				gbReadingScene = FALSE;

			}
		}
		else if (elapseTime <= SCENE_TIME8 && elapseTime >= SCENE_TIME7)
		{
	
			glClearColor(1.0f, 1.0f, 0.97f, 1.0f);
			gbKitchenScene = TRUE;
			glDisable(GL_DEPTH_TEST);
			LOG_MESSAGE("8");

			sceneKitchen();			// kitchen scene 

			if (elapseTime >= SCENE_TIME8 - 2.0f)
			{
				if (fadeOut_scene_8_alpha <= 1.0f)
				{
					fadeOut_scene_8_alpha = fadeOut_scene_8_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_8_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);
				gbKitchenScene = FALSE;
				glEnable(GL_DEPTH_TEST);

			}
		}
		else if (elapseTime <= SCENE_TIME9 && elapseTime >= SCENE_TIME8)
		{
			glClearColor(0.4f, 0.4f, 0.5f, 0.4f); //Black Color
			//gbSceneTeacher = TRUE;
			LOG_MESSAGE("9");
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			gbStudyScene = TRUE;

			sceneStudy();			// Study scene 

			glDisable(GL_BLEND);
			if (elapseTime >= SCENE_TIME9 - 2.0f)
			{
				if (fadeOut_scene_9_alpha <= 1.0f)
				{
					fadeOut_scene_9_alpha = fadeOut_scene_9_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_9_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);
				gbStudyScene = FALSE;

			}
		}
		else if (elapseTime <= SCENE_TIME10 && elapseTime >= SCENE_TIME9)
		{
			gbSceneTeacher = TRUE;
			LOG_MESSAGE("10");

			sceneTeacher();			// Teacher scene 

			if (elapseTime >= SCENE_TIME10 - 2.0f)
			{
				if (fadeOut_scene_10_alpha <= 1.0f)
				{
					fadeOut_scene_10_alpha = fadeOut_scene_10_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_10_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();



				glDisable(GL_BLEND);
				gbSceneTeacher = FALSE;

			}
		}
		else if (elapseTime <= SCENE_TIME11 && elapseTime >= SCENE_TIME10)
		{
			gbEndCredit = TRUE;
			LOG_MESSAGE("11");
			sceneEndCredit();			// End credit scene 

			if (elapseTime >= SCENE_TIME11 - 2.0f)
			{
				if (fadeOut_scene_11_alpha <= 1.0f)
				{
					fadeOut_scene_11_alpha = fadeOut_scene_11_alpha + 0.008f;
				}

				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.0f);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, fadeOut_scene_11_alpha);

				glVertex3f(6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, 6.0f, 0.0f);
				glVertex3f(-6.0f, -6.0f, 0.0f);
				glVertex3f(6.0f, -6.0f, 0.0f);

				glEnd();

				glDisable(GL_BLEND);
				gbEndCredit = FALSE;
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
		if (gfStarYAxis <= 3.2f)
		{
			gfStarYAxis = gfStarYAxis + 0.0003f;
		}
	}


	if (gbSceneKG)
	{
		if (dreamOpacity <= 1.0f)
		{
			dreamOpacity = dreamOpacity + 0.004f;
		}
	}

	if (gbSceneElephant)
	{

		if(gbSceneThinking == FALSE)
		{
			if (SGK_sceneAlphaIncrement >= 1.0f)
			{
				//decrementing alpha
				if (SGK_sceneAlpha >= 0.0f)
				{
					SGK_sceneAlpha = SGK_sceneAlpha - 0.005f;
				}
			}
		}
		

		if (SGK_sceneAlphaIncrement <= 1.0f)
		{
			SGK_sceneAlphaIncrement = SGK_sceneAlphaIncrement + 0.003f;
		}

		//for grass
		if (SGK_gbGrassRight == FALSE)
		{
			if (SGK_gfGrassMove < 0.03f)
			{
				SGK_gfGrassMove = SGK_gfGrassMove + 0.0005f;
			}
			else
			{
				SGK_gbGrassRight = TRUE;
			}
		}
		else
		{
			if (SGK_gfGrassMove > 0.0f)
			{
				SGK_gfGrassMove = SGK_gfGrassMove - 0.0005f;
			}
			else
			{
				SGK_gbGrassRight = FALSE;
			}
		}
	}

	if (gbEndCredit)
	{
		if (y_Texture <= 8.0f)
			y_Texture += 0.056f;

		if (y_Pratik <= 9.0f)
			y_Pratik += 0.1f;

		if (y_Siddhant <= 5.0f)
			y_Siddhant += 0.1f;

		if (y_Hrishikesh <= 1.0f)
			y_Hrishikesh += 0.1f;

		if (y_Prathamesh <= -3.0f)
			y_Prathamesh += 0.1f;

		if (y_Pushkar <= -7.0f)
			y_Pushkar += 0.1f;

		if (y_Shraddha <= -11.0f)
			y_Shraddha += 0.1f;

		if (y_Sukrut <= -15.0f)
			y_Sukrut += 0.1f;

		if (y_Vinay <= -19.0f)
			y_Vinay += 0.1f;
	}

	if (gbKitchenScene)
	{
		if (Translate_Line1_Y <= 0.65f)
		{
			Translate_Line1_Y = Translate_Line1_Y + 0.001f;
		}
		else
		{
			Translate_Line1_Y = 0.35f;
		}
	}
	if (gbStudyScene)
	{
		updateScene();
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
	glColor4f(0.0f, 0.0f, 0.0f, fadeInAlpha);

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
	/*glTranslatef(-5.0f, -4.5f, -16.0f);
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
	SGD_Stars();*/
	glTranslatef(-5.0f, gfStarYAxis - 7.2f, -16.0f);
	SGD_Stars();
	glTranslatef(-1.0f, gfStarYAxis - 3.8f, 0.0f);
	SGD_Stars();
	glTranslatef(-1.0f, gfStarYAxis - 2.5f, 0.0f);
	SGD_Stars();
	glTranslatef(0.4f, gfStarYAxis - 2.0f, 0.0f);
	SGD_Stars();
	glTranslatef(0.1f, gfStarYAxis - 1.5f, -6.0f);
	SGD_Stars();
	glTranslatef(-3.0f, gfStarYAxis - 1.0f, -6.0f);
	SGD_Stars();
	glTranslatef(0.5f, gfStarYAxis-0.5f, -6.0f);
	SGD_Stars();
	glTranslatef(-3.0f, gfStarYAxis, -7.0f);
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


	//glLoadIdentity();
	//glTranslatef(-1.35f, 0.2f, -3.0f);
	//// create bitmaps for the device context font's first 256 glyphs
	//wglUseFontBitmaps(ghdc, 0, 256, 1000);

	//// set up for a string-drawing display list call
	//glListBase(1000);

	//glColor3f(1.0f, 0.0f, 0.0f);

	//glRasterPos2f(-0.4f, -0.25f);
	//glCallLists(11, GL_UNSIGNED_BYTE, "Class - SY");

	//// get all those commands to execute
	//glFlush();

	//// delete our 256 glyph display lists
	//glDeleteLists(1000, 256);

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

	glLineWidth(10.0f);	// set line width	
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.85f, -0.287f, 0.0f);
	glVertex3f(0.955f, -0.287f, 0.0f);

	glColor3f(0.0f, 0.6f, 0.8f);
	glVertex3f(0.835f, -0.27f, 0.0f);
	glVertex3f(0.936f, -0.27f, 0.0f);

	glColor3f(0.9f, 0.4f, 0.0f);
	glVertex3f(0.846f, -0.26f, 0.0f);
	glVertex3f(0.946f, -0.26f, 0.0f);

	glColor3f(0.0f, 0.6f, 0.8f);
	glVertex3f(0.967f, -0.3f, 0.0f);
	glVertex3f(0.942f, -0.18f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
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

	// lips
	glColor3f(1.0f,0.0f,0.0f);
	SGK_drawHollowCircle(0.03f, 0.0f, 0.5f, 210.0f, 330.0f);

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

	// PALMS
	glBegin(GL_POLYGON);
	glVertex3f(-0.27f, -0.05f, 0.0f);
	glVertex3f(-0.235f, -0.05f, 0.0f);
	glVertex3f(-0.25f, -0.15f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.27f, -0.05f, 0.0f);
	glVertex3f(0.235f, -0.05f, 0.0f);
	glVertex3f(0.25f, -0.15f, 0.0f);
	glEnd();
}

void SGD_drawFace(float radius, float teacher_Xaxis, float teacher_Yaxis, float red, float green, float blue)
{

	glBegin(GL_LINES);
	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
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

// ------------------------------------ Scene Elephant --------------

void sceneElephant(void)
{
	//function declaration
	void SGK_drawElephant(void);
	void SGK_drawRoom(void);
	void SGK_drawBed(void);

	//drawing Room
	glLoadIdentity();
	glTranslatef(0.0f, -2.0f, -7.0f);
	SGK_drawRoom();

	//drawing bed and girl
	glLoadIdentity();
	glTranslatef(-0.5f, -2.0f, -5.0f);
	SGK_drawBed();

	//for dream effect
	glLoadIdentity();
	glTranslatef(0.6f, 0.9f, -7.0f);
	glScalef(0.7f, 0.7f, 0.0f);

	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, SGK_sceneAlphaIncrement);
	SGK_drawCircle(0.1f, -2.8f, -3.0f, 0.0f, 360.0f); // 1st circle 
	SGK_drawCircle(0.2f, -2.4f, -2.6f, 0.0f, 360.0f); // 2nd circle
	SGK_drawCircle(0.4f, -1.8f, -2.0f, 0.0f, 360.0f); // 3rd circle
	SGK_drawCircle(2.0f, 0.0f, 0.0f, 0.0f, 360.0f); // 4th circle

	if (elapseTime >= SCENE_TIME5 + 5.0f)
	{
		gbSceneThinking = FALSE;
	}
	if (gbSceneThinking)
	{
		// create bitmaps for the device context font's first 256 glyphs
		wglUseFontBitmaps(ghdc, 0, 256, 1000);

		// set up for a string-drawing display list call
		glListBase(1000);

		glColor4f(0.0f, 0.0f, 0.0f,SGK_sceneAlphaIncrement); // color for text

		glRasterPos2f(0.0f, 0.8f);
		glCallLists(strlen("I Want a Pet ...!!!"), GL_UNSIGNED_BYTE, "I Want a Pet ...!!!");

		glRasterPos2f(-0.5f, 0.3f);
		glCallLists(strlen("Maybe A Cat ?"), GL_UNSIGNED_BYTE, "Maybe A Cat ?");

		glRasterPos2f(0.5f, -0.35f);
		glCallLists(strlen("Maybe A Parrot ?"), GL_UNSIGNED_BYTE, "Maybe A Parrot ?");

		glRasterPos2f(0.0f, 0.0);
		glCallLists(strlen("Maybe A Dog ?"), GL_UNSIGNED_BYTE, "Maybe A Dog ?");

		glRasterPos2f(-0.7f, -0.53f);
		glCallLists(strlen("Or A Baby Elephant ?"), GL_UNSIGNED_BYTE, "Or A Baby Elephant ?");
	
		// get all those commands to execute
		glFlush();

		// delete our 256 glyph display lists
		glDeleteLists(1000, 256);

		glDisable(GL_BLEND);
	}
	else
	{
		if (SGK_sceneAlphaIncrement >= 1.0f)
		{
			//down side of background
			glColor3f(239.0f / 255.0f, 222.0f / 255.0f, 189.0f / 255.0f);
			SGK_drawCircle(2.0f, 0.0f, 0.0f, 180.0f, 360.0f);
			//upside of background
			glColor3f(189.0f / 255.0f, 233.0f / 255.0f, 239.0f / 255.0f);
			SGK_drawCircle(2.0f, 0.0f, 0.0f, 0.0f, 180.0f);

			//drawing grass
			for (float xAxis = -1.5f; xAxis <= 1.5f; xAxis = xAxis + 0.03f)
			{
				for (float yAxis = -0.2f; yAxis >= -1.3f; yAxis = yAxis - 0.05f)
				{
					SGK_DrawGrass(xAxis, yAxis, 0.1f);
				}

			}

			// drawing Elephant
			SGK_drawElephant();

			glLoadIdentity();
			glTranslatef(0.6f, 0.9f, -7.0f);
			glScalef(0.7f, 0.7f, 0.0f);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, SGK_sceneAlpha);

			SGK_drawCircle(2.0f, 0.0f, 0.0f, 0.0f, 360.0f);

			glDisable(GL_BLEND);
		}
	}
	

}

void SGK_drawBed(void)
{
	glBegin(GL_QUADS);

	//base of bed
	glColor3f(37.0f / 255.0f, 48.0f / 255.0f, 59.0f / 255.0f);
	glVertex3f(1.0f, 0.5f, 0.0f);//right top
	glVertex3f(-1.0f, 0.5f, 0.0f);//left top
	glColor3f(27.0f / 255.0f, 38.0f / 255.0f, 49.0f / 255.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);//left bottom
	glVertex3f(1.0f, -0.2f, 0.0f);//right bottom

	//base of bed
	glColor3f(232.0f / 255.0f, 218.0f / 255.0f, 239.0f / 255.0f);
	glVertex3f(1.05f, 0.6f, 0.0f);//right top
	glVertex3f(-1.05f, 0.6f, 0.0f);//left top
	glVertex3f(-1.05f, 0.5f, 0.0f);//left bottom
	glVertex3f(1.05f, 0.5f, 0.0f);//right bottom

	glEnd();

	//face part
	//hair
	glColor3f(0.0f, 0.0f, 0.0f);
	SGK_drawCircle(0.21f, -0.77f, 0.85f, 0.0f, 360.0f);
	//face
	glColor3f(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
	SGK_drawCircle(0.15f, -0.8f, 0.85f, 0.0f, 360.0f);
	//hair on face
	glColor3f(0.0f, 0.0f, 0.0f);

	SGK_drawHollowCircle(0.135f, -0.82f, 0.845f, 180.0f, 10.0f);
	SGK_drawHollowCircle(0.139f, -0.81f, 0.845f, 180.0f, 10.0f);
	SGK_drawHollowCircle(0.138f, -0.812f, 0.85f, 180.0f, 10.0f);
	SGK_drawHollowCircle(0.139f, -0.815f, 0.849f, 183.0f, 10.0f);
	SGK_drawHollowCircle(0.139f, -0.81f, 0.853f, 183.0f, 10.0f);
	SGK_drawHollowCircle(0.137f, -0.82f, 0.86f, 180.0f, 270.0f);
	SGK_drawHollowCircle(0.14f, -0.8f, 0.85f, 180.0f, 270.0f);

	//right eye
	SGK_drawHollowCircle(0.04f, -0.87f, 0.9f, 320.0f, 360.0f);
	SGK_drawHollowCircle(0.04f, -0.87f, 0.9f, 0.0f, 40.0f);

	//left eye
	SGK_drawHollowCircle(0.04f, -0.87f, 0.8f, 320.0f, 360.0f);
	SGK_drawHollowCircle(0.04f, -0.87f, 0.8f, 0.0f, 40.0f);

	//lips
	SGK_drawHollowCircle(0.04f, -0.77f, 0.85f, 300.0f, 360.0f);
	SGK_drawHollowCircle(0.04f, -0.77f, 0.85f, 0.0f, 60.0f);

	//nose
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.78f, 0.85f, 0.0f);
	glVertex3f(-0.78f, 0.84f, 0.0f);
	glVertex3f(-0.81f, 0.85f, 0.0f);
	glEnd();

	//neck
	glBegin(GL_QUADS);


	glColor3f(255.0f / 255.0f, 219.0f / 255.0f, 172.0f / 255.0f);
	glVertex3f(-0.5f, 0.88f, 0.0f);//right top
	glColor3f(241.0f / 255.0f, 194.0f / 255.0f, 125.0f / 255.0f);
	glVertex3f(-0.66f, 0.89f, 0.0f);//left top
	glVertex3f(-0.66f, 0.81f, 0.0f);//left bottom
	glColor3f(255.0f / 255.0f, 219.0f / 255.0f, 172.0f / 255.0f);
	glVertex3f(-0.5f, 0.84f, 0.0f);//right bottom

	glEnd();

	//pillow
	glBegin(GL_POLYGON);

	glColor3f(46.0f / 255.0f, 92.0f / 255.0f, 128.0f / 255.0f);
	glVertex3f(-0.6f, 0.73f, 0.0f);
	glVertex3f(-0.7f, 0.72f, 0.0f);
	glVertex3f(-0.8f, 0.71f, 0.0f);
	glVertex3f(-0.9f, 0.72f, 0.0f);
	glVertex3f(-0.98f, 0.75f, 0.0f);
	glVertex3f(-1.02f, 0.72f, 0.0f);
	glVertex3f(-1.05f, 0.68f, 0.0f);
	glVertex3f(-1.04f, 0.64f, 0.0f);
	glVertex3f(-1.0f, 0.6f, 0.0f);
	glVertex3f(-0.6f, 0.6f, 0.0f);

	glEnd();


	//blanket
	glBegin(GL_POLYGON);

	glColor3f(46.0f / 255.0f, 92.0f / 255.0f, 128.0f / 255.0f);

	glVertex3f(1.1f, 0.6f, 0.0f);
	glVertex3f(1.0f, 0.7f, 0.0f);
	glVertex3f(0.9f, 0.72f, 0.0f);
	glVertex3f(0.8f, 0.71f, 0.0f);
	glVertex3f(0.7f, 0.73f, 0.0f);
	glVertex3f(0.5f, 0.74f, 0.0f);
	glVertex3f(0.4f, 0.78f, 0.0f);
	glVertex3f(0.3f, 0.82f, 0.0f);
	glVertex3f(0.2f, 0.88f, 0.0f);
	glVertex3f(-0.1f, 0.92f, 0.0f);
	glVertex3f(-0.3f, 0.98f, 0.0f);
	glVertex3f(-0.5f, 0.99f, 0.0f);
	glVertex3f(-0.6f, 0.95f, 0.0f);
	glColor3f(26.0f / 255.0f, 82.0f / 255.0f, 118.0f / 255.0f);

	glVertex3f(-0.62f, 0.85f, 0.0f);
	glVertex3f(-0.68f, 0.7f, 0.0f);
	glVertex3f(-0.62f, 0.5f, 0.0f);
	glVertex3f(1.1f, 0.5f, 0.0f);

	glEnd();
}

void SGK_drawRoom(void)
{
	//1st param left most x coordinate of start of line
	// 2nd param right most x coordinate of start of line
	//3rd param y coordinate of start of line
	//4th param length of line
	void SGK_drawLine(float, float, float, float);

	//1st param right most x coordinate of cube 
	//2nd param right most y coordinate of cube
	//3rd param depth of cube
	//4th param lenght of side
	//5th param struct MYCOLOR variable to pass color value for cube 
	//6th param struct MYCOLOR variable to pass color value for lines on cube 
	void SGK_drawCube(float, float, float, float, struct SGK_MYCOLOR, struct SGK_MYCOLOR);

	void SGD_drawVase(void);

	void SGK_drawBooks(void);

	void SGK_drawPainting(void);

	//variable declaration

	struct SGK_MYCOLOR cColor;
	struct SGK_MYCOLOR lColor;
	glBegin(GL_QUADS);

	//back Face
	glColor3f(133.0f / 255.0f, 193.0f / 255.0f, 233.0f / 255.0f);
	glVertex3f(3.0f, 5.0f, 0.0f);//right top
	glVertex3f(-3.0f, 5.0f, 0.0f);//left top
	glVertex3f(-3.0f, -0.2f, 0.0f);//left bottom
	glVertex3f(3.0f, -0.2f, 0.0f);//right bottom

	//right side
	glColor3f(123.0f / 255.0f, 183.0f / 255.0f, 223.0f / 255.0f);
	glVertex3f(3.0f, 5.0f, 0.0f);//right top
	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);
	glVertex3f(3.0f, 5.0f, 3.0f);//left top
	glVertex3f(3.0f, -0.2f, 3.0f);//left bottom
	glColor3f(123.0f / 255.0f, 183.0f / 255.0f, 223.0f / 255.0f);
	glVertex3f(3.0f, -0.2f, 0.0f);//right bottom

	//left side Face
	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);
	glVertex3f(-3.0f, 5.0f, 3.0f);//right top
	glColor3f(123.0f / 255.0f, 183.0f / 255.0f, 223.0f / 255.0f);
	glVertex3f(-3.0f, 5.0f, 0.0f);//left top
	glVertex3f(-3.0f, -0.2f, 0.0f);//left bottom
	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);
	glVertex3f(-3.0f, -0.2f, 3.0f);//right bottom

	//bottom side Face
	glColor3f(166.0f / 255.0f, 172.0f / 255.0f, 175.0f / 255.0f);
	glVertex3f(3.0f, -0.2f, 0.0f);//right top
	glVertex3f(-3.0f, -0.2f, 0.0f);//left top
	glColor3f(86.0f / 255.0f, 101.0f / 255.0f, 115.0f / 255.0f);
	glVertex3f(-3.0f, -0.2f, 3.0f);//left bottom
	glVertex3f(3.0f, -0.2f, 3.0f);//right bottom

	//top side Face
	glColor3f(210.0f / 255.0f, 180.0f / 255.0f, 222.0f / 255.0f);
	glVertex3f(3.0f, 5.0f, 0.0f);//right top
	glVertex3f(-3.0f, 5.0f, 0.0f);//left top
	glVertex3f(-3.0f, 5.0f, 3.0f);//left bottom
	glVertex3f(3.0f, 5.0f, 3.0f);//right bottom

	glEnd();

	// tiles on ground floor 
	SGK_drawLine(-3.0f, 3.0f, -0.199f, 3.0f);

	cColor.rColor = 27.0f / 255.0f;
	cColor.gColor = 38.0f / 255.0f;
	cColor.bColor = 49.0f / 255.0f;

	lColor.rColor = 133.0f / 255.0f;
	lColor.gColor = 146.0f / 255.0f;
	lColor.bColor = 158.0f / 255.0f;

	//right side cube
	SGK_drawCube(2.99f, 0.8f, 0.5f, 1.0f, cColor, lColor);

	//left side cube 
	SGK_drawCube(-2.4f, 2.8f, 1.0f, 0.6f, cColor, lColor);

	SGK_drawCube(-2.4f, 2.2f, 1.5f, 0.6f, cColor, lColor);

	SGK_drawCube(-2.4f, 1.6f, 2.0f, 0.6f, cColor, lColor);

	SGK_drawCube(-2.4f, 1.0f, 2.5f, 0.6f, cColor, lColor);

	SGK_drawCube(-2.4f, 0.4f, 3.0f, 0.6f, cColor, lColor);

	glLoadIdentity();
	glTranslatef(-1.3f, -0.15f, -5.0f);
	glScalef(4.0f, 2.5f, 0.0f);
	SGD_drawVase();

	//for clock
	glLoadIdentity();
	glTranslatef(-2.4f, -1.5f, -5.9f);
	glRotatef(80.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	SGK_drawCircle(0.1f, -1.9f, 1.5f, 0.0f, 360.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(3.0f);
	SGK_drawHollowCircle(0.102f, -1.9f, 1.5f, 0.0f, 360.0f);

	glLineWidth(2.0f);

	//clock needle
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	// 12 clock
	glVertex3f(-1.9f, 1.58f, 0.02f);
	glVertex3f(-1.9f, 1.59f, 0.02f);
	// 6 clock
	glVertex3f(-1.9f, 1.41f, 0.02f);
	glVertex3f(-1.9f, 1.42f, 0.02f);

	// 3 clock
	glVertex3f(-1.84f, 1.5f, 0.02f);
	glVertex3f(-1.82f, 1.5f, 0.02f);

	// 9 clock
	glVertex3f(-1.98f, 1.5f, 0.02f);
	glVertex3f(-2.0f, 1.5f, 0.02f);

	glEnd();
	//clock needle
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	// hour hand
	glVertex3f(-1.9f, 1.5f, 0.02f);
	glVertex3f(-1.86f, 1.5f, 0.02f);

	// second hand
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.9f, 1.5f, 0.02f);
	glVertex3f(-1.97f, 1.53f, 0.02f);

	// minute hand
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.9f, 1.5f, 0.02f);
	glVertex3f(-1.89f, 1.43f, 0.02f);
	glEnd();

	glLineWidth(1.0f);

	glLoadIdentity();
	glTranslatef(-2.3f, -0.27f, -5.8f);
	glRotatef(3.0f, 0.0f, 1.0f, 0.0f);
	SGK_drawBooks();

	glLoadIdentity();
	glTranslatef(-1.2f, -0.4f, -6.5f);
	SGK_drawPainting();

}

void SGK_drawPainting(void)
{
	//drawing page
	glBegin(GL_QUADS);

	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);
	glVertex3f(0.5f, 0.5f, 2.0f);//right top
	glVertex3f(-0.2f, 0.5f, 2.0f);//left top
	glVertex3f(-0.2f, 0.0f, 2.0f);//left bottom
	glVertex3f(0.5f, 0.0f, 2.0f);//right bottom

	glEnd();



	// cello tape
	glLineWidth(3.0f);
	glBegin(GL_LINES);

	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);

	glVertex3f(0.485f, 0.515f, 2.0f);//right top
	glVertex3f(0.515f, 0.48f, 2.0f);//right top

	glVertex3f(-0.185f, 0.515f, 2.0f);//left top
	glVertex3f(-0.215f, 0.48f, 2.0f);//left top

	glVertex3f(-0.195f, -0.02f, 2.0f);//left bottom
	glVertex3f(-0.215f, 0.015f, 2.0f);//left bottom

	glVertex3f(0.48f, -0.02f, 2.0f);//right bottom
	glVertex3f(0.52f, 0.015f, 2.0f);//right bottom

	glEnd();
	glLineWidth(3.0f);

	// drawing
	glBegin(GL_LINE_LOOP);

	glColor3f(220.0f / 255.0f, 118.0f / 255.0f, 51.0f / 255.0f);

	glVertex3f(0.35f, 0.3f, 2.0f);//right top
	glVertex3f(-0.05f, 0.3f, 2.0f);//left top
	glVertex3f(-0.05f, 0.1f, 2.0f);//left bottom
	glVertex3f(0.35f, 0.1f, 2.0f);//right bottom

	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(120.0f / 255.0f, 40.0f / 255.0f, 31.0f / 255.0f);

	glVertex3f(0.4f, 0.3f, 2.0f);//right top
	glVertex3f(0.25f, 0.4f, 2.0f);//left top
	glVertex3f(0.05f, 0.4f, 2.0f);//left bottom
	glVertex3f(-0.1f, 0.3f, 2.0f);//right bottom

	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(120.0f / 255.0f, 40.0f / 255.0f, 31.0f / 255.0f);

	glVertex3f(0.25f, 0.25f, 2.0f);//right top
	glVertex3f(0.15f, 0.25f, 2.0f);//left top
	glVertex3f(0.15f, 0.1f, 2.0f);//left bottom
	glVertex3f(0.25f, 0.1f, 2.0f);//right bottom

	glEnd();

	glLineWidth(1.0f);
}

void SGK_drawBooks(void)
{
	glBegin(GL_QUADS);
	//0 base book
	glColor3f(208.0f / 255.0f, 211.0f / 255.0f, 212.0f / 255.0f);
	glVertex3f(0.39f, 0.03f, 2.0f);//right top
	glVertex3f(0.01f, 0.03f, 2.0f);//left top
	glVertex3f(0.02f, -0.02f, 2.0f);//left bottom
	glVertex3f(0.4f, -0.02f, 2.0f);//right bottom

	//1 book
	glVertex3f(0.38f, 0.08f, 2.0f);//right top
	glVertex3f(0.02f, 0.08f, 2.0f);//left top
	glVertex3f(0.03f, 0.035f, 2.0f);//left bottom
	glVertex3f(0.39f, 0.035f, 2.0f);//right bottom

	//2 book
	glVertex3f(0.39f, 0.13f, 2.0f);//right top
	glVertex3f(0.03f, 0.13f, 2.0f);//left top
	glVertex3f(0.02f, 0.085f, 2.0f);//left bottom
	glVertex3f(0.38f, 0.085f, 2.0f);//right bottom

	//3 book
	glVertex3f(0.37f, 0.18f, 2.0f);//right top
	glVertex3f(0.03f, 0.18f, 2.0f);//left top
	glVertex3f(0.02f, 0.135f, 2.0f);//left bottom
	glVertex3f(0.38f, 0.135f, 2.0f);//right bottom
	glEnd();

	glLineWidth(2.0f);
	//book cover
	glBegin(GL_LINES);
	//0 base book
	glColor3f(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.39f, 0.03f, 2.0f);//right top
	glVertex3f(0.01f, 0.03f, 2.0f);//left top

	glVertex3f(0.01f, 0.03f, 2.0f);//left top
	glVertex3f(0.02f, -0.02f, 2.0f);//left bottom

	glVertex3f(0.02f, -0.02f, 2.0f);//left bottom
	glVertex3f(0.4f, -0.02f, 2.0f);//right bottom

	//1 book
	glColor3f(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.38f, 0.08f, 2.0f);//right top
	glVertex3f(0.02f, 0.08f, 2.0f);//left top

	glVertex3f(0.02f, 0.08f, 2.0f);//left top
	glVertex3f(0.03f, 0.035f, 2.0f);//left bottom

	glVertex3f(0.03f, 0.035f, 2.0f);//left bottom
	glVertex3f(0.39f, 0.035f, 2.0f);//right bottom

	//2 book
	glColor3f(255.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f);
	glVertex3f(0.39f, 0.13f, 2.0f);//right top
	glVertex3f(0.03f, 0.13f, 2.0f);//left top

	glVertex3f(0.03f, 0.13f, 2.0f);//left top
	glVertex3f(0.02f, 0.085f, 2.0f);//left bottom

	glVertex3f(0.02f, 0.085f, 2.0f);//left bottom
	glVertex3f(0.38f, 0.085f, 2.0f);//right bottom

	//3 book
	glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	glVertex3f(0.37f, 0.18f, 2.0f);//right top
	glVertex3f(0.03f, 0.18f, 2.0f);//left top 

	glVertex3f(0.03f, 0.18f, 2.0f);//left top 
	glVertex3f(0.02f, 0.135f, 2.0f);//left bottom

	glVertex3f(0.02f, 0.135f, 2.0f);//left bottom
	glVertex3f(0.38f, 0.135f, 2.0f);//right bottom

	glEnd();
	glLineWidth(1.0f);
}

void SGK_drawCube(float xAxis, float yAxis, float depth, float length, struct SGK_MYCOLOR cubeColor, struct SGK_MYCOLOR lineColor)
{
	glBegin(GL_QUADS);
	//front Face
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis, yAxis, depth);//right top
	glVertex3f(xAxis - length, yAxis, depth);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis - length, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, depth);//right bottom

	//back Face
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left bottom
	glVertex3f(xAxis, yAxis - length, 0.0f);//right bottom

	//right side
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis, yAxis, depth);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, 0.0f);//right bottom

	//left side Face
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis - length, yAxis, depth);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left bottom
	glVertex3f(xAxis - length, yAxis - length, depth);//right bottom

	//bottom side Face
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis, yAxis - length, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis - length, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, depth);//right bottom

	//top side Face
	glColor3f(cubeColor.rColor + 0.1f, cubeColor.gColor + 0.1f, cubeColor.bColor + 0.1f);
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glColor3f(cubeColor.rColor, cubeColor.gColor, cubeColor.bColor);
	glVertex3f(xAxis - length, yAxis, depth);//left bottom
	glVertex3f(xAxis, yAxis, depth);//right bottom

	glEnd();

	//draw line on cube 
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);

	glColor3f(lineColor.rColor, lineColor.gColor, lineColor.bColor);
	//front Face
	glVertex3f(xAxis, yAxis, depth);//right top
	glVertex3f(xAxis - length, yAxis, depth);//left top
	glVertex3f(xAxis - length, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, depth);//right bottom

	glEnd();

	glBegin(GL_LINE_STRIP);
	//back Face
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left bottom
	glVertex3f(xAxis, yAxis - length, 0.0f);//right bottom

	glEnd();

	glBegin(GL_LINE_STRIP);
	//right side
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis, yAxis, depth);//left top
	glVertex3f(xAxis, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, 0.0f);//right bottom

	glEnd();

	glBegin(GL_LINE_STRIP);
	//left side Face
	glVertex3f(xAxis - length, yAxis, depth);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left bottom
	glVertex3f(xAxis - length, yAxis - length, depth);//right bottom

	glEnd();

	glBegin(GL_LINE_STRIP);
	//bottom side Face
	glVertex3f(xAxis, yAxis - length, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis - length, 0.0f);//left top
	glVertex3f(xAxis - length, yAxis - length, depth);//left bottom
	glVertex3f(xAxis, yAxis - length, depth);//right bottom

	glEnd();

	glBegin(GL_LINE_STRIP);
	//top side Face
	glVertex3f(xAxis, yAxis, 0.0f);//right top
	glVertex3f(xAxis - length, yAxis, 0.0f);//left top
	glVertex3f(xAxis - length, yAxis, depth);//left bottom
	glVertex3f(xAxis, yAxis, depth);//right bottom

	glEnd();

	glLineWidth(1.0f);
}

void SGK_drawLine(float xAxis, float xAxisLimit, float yAxis, float lineLength)
{
	glLineWidth(3.0f);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	glColor3f(251.0f / 255.0f, 252.0f / 255.0f, 252.0f / 255.0f);

	for (float x = xAxis; x <= xAxisLimit; x = x + 0.5f)
	{
		glVertex3f(x, yAxis, 0.0f);
		glVertex3f(x, yAxis, lineLength);
	}

	for (float z = yAxis; z <= lineLength; z = z + 0.5f)
	{
		glVertex3f(xAxis, yAxis, z);
		glVertex3f(xAxisLimit, yAxis, z);
	}

	glEnd();
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
}

void SGK_drawElephant(void)
{
	//function declarations
	void SGK_drawElephantTooth(void);
	void SGK_drawElephantEye(void);
	void SGK_drawElephantMidBody(void);
	void SGK_drawFace(void);

	//draw mid body of the elephant
	SGK_drawElephantMidBody();

	//face of elephant
	SGK_drawFace();

	glLoadIdentity();
	glTranslatef(0.6f, 0.9f, -7.0f);
	glScalef(0.7f, 0.7f, 0.0f);
	glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
	SGK_drawElephantEye();

	glLoadIdentity();
	glTranslatef(0.1f, 1.1f, -7.0f);
	glScalef(0.7f, 0.7f, 0.0f);
	SGK_drawElephantTooth();
}

void SGK_drawFace(void)
{
	//base of head
	glBegin(GL_POLYGON);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	// face start vertex from right side
	glVertex3f(-0.4, 0.8f, 0.0f);
	glVertex3f(-0.47, 0.9f, 0.0f);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(-0.49, 0.94f, 0.0f);
	glVertex3f(-0.51, 0.95f, 0.0f);
	glVertex3f(-0.55, 0.96f, 0.0f);
	glVertex3f(-0.58, 0.96f, 0.0f);
	glVertex3f(-0.6, 0.965f, 0.0f);
	glVertex3f(-0.65, 0.96f, 0.0f);
	glVertex3f(-0.7, 0.95f, 0.0f);
	glVertex3f(-0.8, 0.93f, 0.0f);
	glVertex3f(-0.85, 0.91f, 0.0f);
	glVertex3f(-1.0, 0.73f, 0.0f);
	glVertex3f(-1.0, 0.1f, 0.0f);
	glColor3f(74.0f / 255.0f, 71.0f / 255.0f, 67.0f / 255.0f);
	glVertex3f(-0.8, 0.05f, 0.0f);
	glVertex3f(-0.65, 0.08f, 0.0f);
	glVertex3f(-0.55, 0.12f, 0.0f);
	//face end vertex from right side
	glVertex3f(-0.4, 0.18f, 0.0f);
	glEnd();

	//trunk top part
	glBegin(GL_POLYGON);
	// trunk start vertex from right side
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(-1.0f, 0.7f, 0.0f);
	glVertex3f(-1.04f, 0.65f, 0.0f);
	glVertex3f(-1.08f, 0.6f, 0.0f);
	glVertex3f(-1.12f, 0.55f, 0.0f);
	glVertex3f(-1.15f, 0.5f, 0.0f);
	glVertex3f(-1.18f, 0.45f, 0.0f);
	glVertex3f(-1.2f, 0.4f, 0.0f);
	glVertex3f(-1.22f, 0.35f, 0.0f);
	glVertex3f(-1.24f, 0.3f, 0.0f);
	glVertex3f(-1.25f, 0.25f, 0.0f);
	glVertex3f(-1.25f, 0.2f, 0.0f);
	glVertex3f(-1.26f, 0.15f, 0.0f);
	glVertex3f(-1.26f, 0.1f, 0.0f);
	glVertex3f(-1.27f, 0.05f, 0.0f);
	glVertex3f(-1.275f, 0.0f, 0.0f);
	glVertex3f(-1.28f, -0.05f, 0.0f);
	glVertex3f(-1.285f, -0.1f, 0.0f);
	glVertex3f(-1.29f, -0.15f, 0.0f);
	glVertex3f(-1.285f, -0.2f, 0.0f);
	glVertex3f(-1.28f, -0.25f, 0.0f);
	glVertex3f(-1.275f, -0.3f, 0.0f);
	glVertex3f(-1.27f, -0.4f, 0.0f);
	glVertex3f(-1.05f, -0.4f, 0.0f);
	glVertex3f(-1.0, 0.1f, 0.0f);
	glEnd();

	//trunk mid part
	glBegin(GL_POLYGON);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	// trunk start vertex from right side
	glVertex3f(-1.05f, -0.4f, 0.0f);
	glVertex3f(-1.27f, -0.4f, 0.0f);
	glVertex3f(-1.26f, -0.45f, 0.0f);
	glVertex3f(-1.25f, -0.5f, 0.0f);
	glVertex3f(-1.24f, -0.55f, 0.0f);
	glVertex3f(-1.22f, -0.6f, 0.0f);
	glVertex3f(-1.2f, -0.65f, 0.0f);
	glVertex3f(-1.18f, -0.7f, 0.0f);
	glVertex3f(-1.15f, -0.75f, 0.0f);
	glVertex3f(-1.1f, -0.8f, 0.0f);

	glEnd();

	// trunk bottom part
	glBegin(GL_POLYGON);
	glColor3f(54.0f / 255.0f, 51.0f / 255.0f, 47.0f / 255.0f);
	// trunk start vertex from right side
	glVertex3f(-0.95f, -0.62f, 0.0f);
	glVertex3f(-0.97f, -0.61f, 0.0f);
	glVertex3f(-0.99f, -0.60f, 0.0f);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(-1.1f, -0.68f, 0.0f);
	glVertex3f(-1.1f, -0.8f, 0.0f);
	glVertex3f(-1.05f, -0.815f, 0.0f);
	glVertex3f(-1.02f, -0.82f, 0.0f);
	glVertex3f(-0.98f, -0.81f, 0.0f);
	glColor3f(54.0f / 255.0f, 51.0f / 255.0f, 47.0f / 255.0f);
	glVertex3f(-0.95f, -0.78f, 0.0f);
	glVertex3f(-1.0f, -0.73f, 0.0f);

	glEnd();
}

void SGK_drawElephantTooth(void)
{
	glBegin(GL_QUADS);
	//base of the tooth

	glColor3f(84.0f / 255.0f, 81.0f / 255.0f, 77.0f / 255.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.1f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(-0.15f, -0.2f, 0.0f);
	glVertex3f(0.0f, -0.15f, 0.0f);

	//middle part of the tooth
	glColor3f(225.0f / 255.0f, 216.0f / 255.0f, 183.0f / 255.0f);
	glVertex3f(-0.2f, -0.1f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);
	glColor3f(225.0f / 255.0f, 216.0f / 255.0f, 183.0f / 255.0f);
	glVertex3f(-0.15f, -0.2f, 0.0f);

	//sharp end of the tooth

	glColor3f(225.0f / 255.0f, 216.0f / 255.0f, 183.0f / 255.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, -0.23f, 0.0f);
	glVertex3f(-0.55f, -0.18f, 0.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);

	glEnd();
}

void SGK_drawElephantEye(void)
{
	glBegin(GL_QUADS);
	//eye white part
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex3f(-0.5f, 0.68f, 0.0f);
	glVertex3f(-0.52f, 0.72f, 0.0f);
	glVertex3f(-0.57f, 0.74f, 0.0f);
	glVertex3f(-0.6f, 0.7f, 0.0f);

	glEnd();

	// eye pupil
	glPointSize(5.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.55f, 0.71f, 0.0f);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	// eye brow
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-0.63f, 0.72f, 0.0f);
	glVertex3f(-0.57f, 0.76f, 0.0f);
	glVertex3f(-0.5f, 0.74f, 0.0f);
	glEnd();
	glLineWidth(1.0f);
}

void SGK_drawElephantMidBody(void)
{

	glBegin(GL_POLYGON);	// front leg 1
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(-0.5f, 0.18f, 0.0f);
	glColor3f(74.0f / 255.0f, 71.0f / 255.0f, 67.0f / 255.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(-0.2f, -0.9f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(0.1f, -0.9f, 0.0f);
	glVertex3f(-0.1f, -0.3f, 0.0f);
	glVertex3f(0.1f, 0.22f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);	// front leg 2

	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(-0.4f, 0.18f, 0.0f);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(-0.2f, -0.3f, 0.0f);
	glVertex3f(0.2f, -0.9f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(0.5f, -0.8f, 0.0f);
	glVertex3f(0.2f, -0.3f, 0.0f);
	glVertex3f(0.25f, 0.18f, 0.0f);

	glEnd();

	glBegin(GL_POINTS);

	glVertex3f(1.35f, 0.6f, 0.0f);

	glEnd();


	glBegin(GL_POLYGON);	// back leg 1

	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(0.5f, 0.18f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(0.6f, -0.3f, 0.0f);
	glVertex3f(0.8f, -0.9f, 0.0f);
	glVertex3f(1.1f, -0.9f, 0.0f);
	glVertex3f(0.9f, -0.3f, 0.0f);
	glVertex3f(1.0f, 0.18f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);	// back leg 2

	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(0.7f, 0.18f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(0.8f, -0.3f, 0.0f);
	glVertex3f(1.0f, -0.9f, 0.0f);
	glVertex3f(1.3f, -0.9f, 0.0f);
	glVertex3f(1.1f, -0.3f, 0.0f);
	glVertex3f(1.2f, 0.18f, 0.0f);

	glEnd();

	glPointSize(8.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	//back leg 2 nails
	glColor3f(225.0f / 255.0f, 216.0f / 255.0f, 183.0f / 255.0f);
	glVertex3f(1.02f, -0.88f, 0.0f);
	glVertex3f(1.08f, -0.88f, 0.0f);
	glVertex3f(1.14f, -0.88f, 0.0f);
	glVertex3f(1.2f, -0.88f, 0.0f);

	//back leg 1 nails
	glVertex3f(0.82f, -0.88f, 0.0f);
	glVertex3f(0.88f, -0.88f, 0.0f);
	glVertex3f(0.94f, -0.88f, 0.0f);

	//front leg 2 nails
	glVertex3f(0.22f, -0.88f, 0.0f);
	glVertex3f(0.28f, -0.86f, 0.0f);
	glVertex3f(0.34f, -0.84f, 0.0f);
	glVertex3f(0.4f, -0.82f, 0.0f);

	//front leg 1 nails
	glVertex3f(-0.18f, -0.88f, 0.0f);
	glVertex3f(-0.12f, -0.88f, 0.0f);
	glVertex3f(-0.06f, -0.88f, 0.0f);
	glVertex3f(0.0f, -0.88f, 0.0f);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
	glPointSize(1.0f);

	glBegin(GL_POLYGON);	// mid body

	glColor3f(84.0f / 255.0f, 81.0f / 255.0f, 67.0f / 255.0f);
	// upper part
	glVertex3f(1.35f, 0.6f, 0.0f);
	glVertex3f(1.3f, 0.8f, 0.0f);
	glColor3f(123.0f / 255.0f, 117.0f / 255.0f, 110.0f / 255.0f);
	glVertex3f(1.2f, 0.9f, 0.0f);
	glVertex3f(1.1f, 0.95f, 0.0f);
	glVertex3f(0.9f, 1.0f, 0.0f);
	glVertex3f(0.7f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.9f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	//lower part
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(-0.4f, 0.18f, 0.0f);
	glVertex3f(-0.3f, 0.12f, 0.0f);
	glVertex3f(-0.25f, 0.08f, 0.0f);
	glVertex3f(-0.2f, 0.04f, 0.0f);
	glVertex3f(0.0f, -0.08f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(0.2f, -0.13f, 0.0f);
	glVertex3f(0.3f, -0.15f, 0.0f);
	glVertex3f(0.4f, -0.17f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glVertex3f(0.6f, -0.13f, 0.0f);
	glVertex3f(0.7f, -0.11f, 0.0f);
	glVertex3f(0.8f, -0.1f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(1.2f, 0.18f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);	// tail 

	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(1.33f, 0.6f, 0.0f);
	glVertex3f(1.32f, 0.4f, 0.0f);
	glVertex3f(1.31f, 0.2f, 0.0f);
	glVertex3f(1.31f, 0.0f, 0.0f);
	glVertex3f(1.32f, -0.2f, 0.0f);
	glVertex3f(1.35f, -0.2f, 0.0f);
	glVertex3f(1.35f, 0.0f, 0.0f);
	glVertex3f(1.35f, 0.2f, 0.0f);
	glVertex3f(1.35f, 0.6f, 0.0f);

	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.32f, -0.15f, 0.0f);
	glVertex3f(1.32f, -0.25f, 0.0f);
	glVertex3f(1.32f, -0.18f, 0.0f);
	glVertex3f(1.33f, -0.2f, 0.0f);
	glVertex3f(1.33f, -0.25f, 0.0f);
	glVertex3f(1.34f, -0.18f, 0.0f);
	glVertex3f(1.34f, -0.25f, 0.0f);
	glVertex3f(1.35f, -0.15f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_POLYGON);
	// ear
	glColor3f(94.0f / 255.0f, 91.0f / 255.0f, 87.0f / 255.0f);
	glVertex3f(0.1f, 0.9f, 0.0f);
	glColor3f(64.0f / 255.0f, 61.0f / 255.0f, 57.0f / 255.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.2f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(-0.4f, 0.3f, 0.0f);
	glVertex3f(-0.2f, 0.2f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.0f);

	glEnd();
}

void SGK_DrawGrass(float xAxis, float yAxis, float height)
{
	glBegin(GL_LINES);
	for (float fXAxis = xAxis - 0.03f; fXAxis <= xAxis + 0.03f; fXAxis = fXAxis + 0.02f)
	{
		glColor3f(8.0f / 255.0f, 78.0f / 255.0f, 14.0f / 255.0f);

		glVertex3f(fXAxis + SGK_gfGrassMove, (yAxis + height), 0.0f);
		glColor3f(189.0f / 255.0f, 239.0f / 255.0f, 193.0f / 255.0f);
		glColor3f(0.0f, 0.8f, 0.2f);
		glVertex3f(xAxis, yAxis, 0.0f);
	}
	glEnd();
}

void SGK_drawCircle(float fRadius, float xAxis, float yAxis, float startAngle, float endAngle)
{
	glBegin(GL_LINES);
	float x, y;
	if (startAngle < endAngle)
	{
		for (float fAngle = startAngle; fAngle <= endAngle; fAngle = fAngle + 0.1f)
		{
			x = xAxis + fRadius * cos((fAngle * M_PI) / 180.0f);
			y = yAxis + fRadius * sin((fAngle * M_PI) / 180.0f);

			glVertex3f(xAxis, yAxis, 0.0f);
			glVertex3f(x, y, 0.0f);
		}
	}
	else
	{
		for (float fAngle = startAngle; fAngle >= endAngle; fAngle = fAngle - 0.1f)
		{
			x = xAxis + fRadius * cos((fAngle * M_PI) / 180.0f);
			y = yAxis + fRadius * sin((fAngle * M_PI) / 180.0f);

			glVertex3f(xAxis, yAxis, 0.0f);
			glVertex3f(x, y, 0.0f);
		}
	}
	glEnd();
}

void SGK_drawHollowCircle(float fRadius, float xAxis, float yAxis, float startAngle, float endAngle)
{
	glBegin(GL_LINE_STRIP);
	float x, y;
	if (startAngle < endAngle)
	{
		for (float fAngle = startAngle; fAngle <= endAngle; fAngle = fAngle + 0.1f)
		{
			x = xAxis + fRadius * cos((fAngle * M_PI) / 180.0f);
			y = yAxis + fRadius * sin((fAngle * M_PI) / 180.0f);

			glVertex3f(x, y, 0.0f);
		}
	}
	else
	{
		for (float fAngle = startAngle; fAngle >= endAngle; fAngle = fAngle - 0.1f)
		{
			x = xAxis + fRadius * cos((fAngle * M_PI) / 180.0f);
			y = yAxis + fRadius * sin((fAngle * M_PI) / 180.0f);

			glVertex3f(x, y, 0.0f);
		}
	}
	glEnd();
}

// ------------------------------------- Scene End Creadit ----------------------

void sceneEndCredit(void)
{
	//function declarations
	void hms_Texture(void);
	void hms_Pratik(void);
	void hms_Siddhant(void);
	void hms_Hrishikesh(void);
	void hms_Prathamesh(void);
	void hms_Pushkar(void);
	void hms_Shraddha(void);
	void hms_Sukrut(void);
	void hms_Vinay(void);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -50.0f);
	// Purple background
	glBegin(GL_QUADS);
	glColor3f(0.2f, 0.0f, 0.3f);
	glVertex3f(40.0f, 22.0f, 0.0f);
	glVertex3f(-40.0f, 22.0f, 0.0f);

	glColor3f(0.3f, 0.0f, 0.4f);

	glVertex3f(-40.0f, 10.0f, 0.0f);
	glVertex3f(40.0f, 10.0f, 0.0f);
	
	glVertex3f(40.0f, 10.0f, 0.0f);
	glVertex3f(-40.0f, 10.0f, 0.0f);

	glColor3f(0.9f, 0.8f, 0.5f);

	glVertex3f(-40.0f, -22.0f, 0.0f);
	glVertex3f(40.0f, -22.0f, 0.0f);
	glEnd();


	glColor3f(1.0f, 1.0f, 1.0f);
	hms_Texture();
	hms_Pratik();
	hms_Siddhant();
	hms_Hrishikesh();
	hms_Prathamesh();
	hms_Pushkar();
	hms_Shraddha();
	hms_Sukrut();
	hms_Vinay();
}



void hms_Texture(void)
{
	//T
	glLoadIdentity();
	glTranslatef(x_Texture, y_Texture, -25.0f);
	hms_drawT();

	//E
	glLoadIdentity();
	glTranslatef(x_Texture + 0.8f, y_Texture, -25.0f);
	hms_drawE();

	//X
	glLoadIdentity();
	glTranslatef(x_Texture + 2.4f, y_Texture, -25.0f);
	hms_drawX();

	//T
	glLoadIdentity();
	glTranslatef(x_Texture + 3.6f, y_Texture, -25.0f);
	hms_drawT();

	//U
	glLoadIdentity();
	glTranslatef(x_Texture + 5.0f, y_Texture, -25.0f);
	hms_drawU();

	//R
	glLoadIdentity();
	glTranslatef(x_Texture + 5.9f, y_Texture, -25.0f);
	hms_drawR();

	//E
	glLoadIdentity();
	glTranslatef(x_Texture + 7.2f, y_Texture, -25.0f);
	hms_drawE();

	//G
	glLoadIdentity();
	glTranslatef(x_Texture + 9.0f, y_Texture, -25.0f);
	hms_drawG();

	//R
	glLoadIdentity();
	glTranslatef(x_Texture + 10.2f, y_Texture, -25.0f);
	hms_drawR();

	//O
	glLoadIdentity();
	glTranslatef(x_Texture + 12.0f, y_Texture, -25.0f);
	hms_drawO();

	//U
	glLoadIdentity();
	glTranslatef(x_Texture + 13.4f, y_Texture, -25.0f);
	hms_drawU();

	//P
	glLoadIdentity();
	glTranslatef(x_Texture + 14.3f, y_Texture, -25.0f);
	hms_drawP();
}

void hms_Hrishikesh(void)
{
	//H
	glLoadIdentity();
	glTranslatef(x_Hrishikesh, y_Hrishikesh, -50.0f);
	hms_drawH();

	//R
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 0.9f, y_Hrishikesh, -50.0f);
	hms_drawR();

	//I
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 2.6f, y_Hrishikesh, -50.0f);
	hms_drawI();

	//S
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 3.5f, y_Hrishikesh, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 5.4f, y_Hrishikesh, -50.0f);
	hms_drawH();

	//I
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 6.8f, y_Hrishikesh, -50.0f);
	hms_drawI();

	//K
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 7.8f, y_Hrishikesh, -50.0f);
	hms_drawK();

	//E
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 9.0f, y_Hrishikesh, -50.0f);
	hms_drawE();

	//S
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 10.2f, y_Hrishikesh, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 12.0f, y_Hrishikesh, -50.0f);
	hms_drawH();

	//S
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 14.0f, y_Hrishikesh, -50.0f);
	hms_drawS();

	//A
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 15.8f, y_Hrishikesh, -50.0f);
	hms_drawAEndCredit();

	//T
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 17.0f, y_Hrishikesh, -50.0f);
	hms_drawT();

	//K
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 18.0f, y_Hrishikesh, -50.0f);
	hms_drawK();

	//A
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 20.0f, y_Hrishikesh, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Hrishikesh + 21.0f, y_Hrishikesh, -50.0f);
	hms_drawR();

}

void hms_Siddhant(void)
{
	//S
	glLoadIdentity();
	glTranslatef(x_Siddhant, y_Siddhant, -50.0f);
	hms_drawS();

	//I
	glLoadIdentity();
	glTranslatef(x_Siddhant + 1.5f, y_Siddhant, -50.0f);
	hms_drawI();

	//D
	glLoadIdentity();
	glTranslatef(x_Siddhant + 2.7f, y_Siddhant, -50.0f);
	hms_drawD();

	//D
	glLoadIdentity();
	glTranslatef(x_Siddhant + 4.2f, y_Siddhant, -50.0f);
	hms_drawD();

	//H
	glLoadIdentity();
	glTranslatef(x_Siddhant + 6.2f, y_Siddhant, -50.0f);
	hms_drawH();

	//A
	glLoadIdentity();
	glTranslatef(x_Siddhant + 7.8f, y_Siddhant, -50.0f);
	hms_drawAEndCredit();

	//N
	glLoadIdentity();
	glTranslatef(x_Siddhant + 8.8f, y_Siddhant, -50.0f);
	hms_drawN();

	//T
	glLoadIdentity();
	glTranslatef(x_Siddhant + 10.5f, y_Siddhant, -50.0f);
	hms_drawT();

	//K
	glLoadIdentity();
	glTranslatef(x_Siddhant + 12.5f, y_Siddhant, -50.0f);
	hms_drawK();

	//S
	glLoadIdentity();
	glTranslatef(x_Siddhant + 13.7f, y_Siddhant, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Siddhant + 15.5f, y_Siddhant, -50.0f);
	hms_drawH();

	//I
	glLoadIdentity();
	glTranslatef(x_Siddhant + 16.8f, y_Siddhant, -50.0f);
	hms_drawI();

	//R
	glLoadIdentity();
	glTranslatef(x_Siddhant + 17.8f, y_Siddhant, -50.0f);
	hms_drawR();

	//S
	glLoadIdentity();
	glTranslatef(x_Siddhant + 19.2f, y_Siddhant, -50.0f);
	hms_drawS();

	//A
	glLoadIdentity();
	glTranslatef(x_Siddhant + 21.0f, y_Siddhant, -50.0f);
	hms_drawAEndCredit();

	//G
	glLoadIdentity();
	glTranslatef(x_Siddhant + 22.0f, y_Siddhant, -50.0f);
	hms_drawG();

	//A
	glLoadIdentity();
	glTranslatef(x_Siddhant + 23.8f, y_Siddhant, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Siddhant + 24.8f, y_Siddhant, -50.0f);
	hms_drawR();

	//-
	glLoadIdentity();
	glTranslatef(x_Siddhant + 27.0f, y_Siddhant, -50.0f);
	hms_drawDash();

	//P
	glLoadIdentity();
	glTranslatef(x_Siddhant + 28.5f, y_Siddhant, -50.0f);
	hms_drawP();

	//L
	glLoadIdentity();
	glTranslatef(x_Siddhant + 30.0f, y_Siddhant, -50.0f);
	hms_drawL();
}

void hms_Pratik(void)
{
	//P
	glLoadIdentity();
	glTranslatef(x_Pratik, y_Pratik, -50.0f);
	hms_drawP();

	//R
	glLoadIdentity();
	glTranslatef(x_Pratik + 1.3f, y_Pratik, -50.0f);
	hms_drawR();

	//A
	glLoadIdentity();
	glTranslatef(x_Pratik + 3.2f, y_Pratik, -50.0f);
	hms_drawAEndCredit();

	//T
	glLoadIdentity();
	glTranslatef(x_Pratik + 4.6f, y_Pratik, -50.0f);
	hms_drawT();

	//I
	glLoadIdentity();
	glTranslatef(x_Pratik + 6.0f, y_Pratik, -50.0f);
	hms_drawI();

	//K
	glLoadIdentity();
	glTranslatef(x_Pratik + 7.0f, y_Pratik, -50.0f);
	hms_drawK();

	//J
	glLoadIdentity();
	glTranslatef(x_Pratik + 10.0f, y_Pratik, -50.0f);
	hms_drawJ();

	//A
	glLoadIdentity();
	glTranslatef(x_Pratik + 11.4, y_Pratik, -50.0f);
	hms_drawAEndCredit();

	//G
	glLoadIdentity();
	glTranslatef(x_Pratik + 12.4f, y_Pratik, -50.0f);
	hms_drawG();

	//A
	glLoadIdentity();
	glTranslatef(x_Pratik + 14.4f, y_Pratik, -50.0f);
	hms_drawAEndCredit();

	//D
	glLoadIdentity();
	glTranslatef(x_Pratik + 15.8f, y_Pratik, -50.0f);
	hms_drawD();

	//A
	glLoadIdentity();
	glTranslatef(x_Pratik + 17.7f, y_Pratik, -50.0f);
	hms_drawAEndCredit();

	//L
	glLoadIdentity();
	glTranslatef(x_Pratik + 18.7f, y_Pratik, -50.0f);
	hms_drawL();

	//E
	glLoadIdentity();
	glTranslatef(x_Pratik + 20.0f, y_Pratik, -50.0f);
	hms_drawE();

	//-
	glLoadIdentity();
	glTranslatef(x_Pratik + 22.0f, y_Pratik, -50.0f);
	hms_drawDash();

	//G
	glLoadIdentity();
	glTranslatef(x_Pratik + 23.5f, y_Pratik, -50.0f);
	hms_drawG();

	//L
	glLoadIdentity();
	glTranslatef(x_Pratik + 25.0f, y_Pratik, -50.0f);
	hms_drawL();
}

void hms_Prathamesh(void)
{
	//P
	glLoadIdentity();
	glTranslatef(x_Prathamesh, y_Prathamesh, -50.0f);
	hms_drawP();

	//R
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 1.3f, y_Prathamesh, -50.0f);
	hms_drawR();

	//A
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 3.2f, y_Prathamesh, -50.0f);
	hms_drawAEndCredit();

	//T
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 4.6f, y_Prathamesh, -50.0f);
	hms_drawT();

	//H
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 6.0f, y_Prathamesh, -50.0f);
	hms_drawH();

	//A
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 7.6f, y_Prathamesh, -50.0f);
	hms_drawAEndCredit();

	//M
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 9.5f, y_Prathamesh, -50.0f);
	hms_drawMEndCredit();

	//E
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 10.8f, y_Prathamesh, -50.0f);
	hms_drawE();

	//S
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 12.0f, y_Prathamesh, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 13.8f, y_Prathamesh, -50.0f);
	hms_drawH();

	//P
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 15.8f, y_Prathamesh, -50.0f);
	hms_drawP();

	//A
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 17.9f, y_Prathamesh, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 18.9, y_Prathamesh, -50.0f);
	hms_drawR();

	//O
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 20.8f, y_Prathamesh, -50.0f);
	hms_drawO();

	//P
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 21.8f, y_Prathamesh, -50.0f);
	hms_drawP();

	//K
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 23.2f, y_Prathamesh, -50.0f);
	hms_drawK();

	//A
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 25.2f, y_Prathamesh, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 26.2f, y_Prathamesh, -50.0f);
	hms_drawR();

	//I
	glLoadIdentity();
	glTranslatef(x_Prathamesh + 28.0f, y_Prathamesh, -50.0f);
	hms_drawI();
}

void hms_Pushkar(void)
{
	//P
	glLoadIdentity();
	glTranslatef(x_Pushkar, y_Pushkar, -50.0f);
	hms_drawP();

	//U
	glLoadIdentity();
	glTranslatef(x_Pushkar + 2.0f, y_Pushkar, -50.0f);
	hms_drawU();

	//S
	glLoadIdentity();
	glTranslatef(x_Pushkar + 3.0f, y_Pushkar, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Pushkar + 4.7f, y_Pushkar, -50.0f);
	hms_drawH();

	//K
	glLoadIdentity();
	glTranslatef(x_Pushkar + 5.7f, y_Pushkar, -50.0f);
	hms_drawK();

	//A
	glLoadIdentity();
	glTranslatef(x_Pushkar + 7.5f, y_Pushkar, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Pushkar + 8.5f, y_Pushkar, -50.0f);
	hms_drawR();

	//S
	glLoadIdentity();
	glTranslatef(x_Pushkar + 11.0f, y_Pushkar, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Pushkar + 12.8f, y_Pushkar, -50.0f);
	hms_drawH();

	//I
	glLoadIdentity();
	glTranslatef(x_Pushkar + 14.1f, y_Pushkar, -50.0f);
	hms_drawI();

	//R
	glLoadIdentity();
	glTranslatef(x_Pushkar + 15.1f, y_Pushkar, -50.0f);
	hms_drawR();

	//O
	glLoadIdentity();
	glTranslatef(x_Pushkar + 17.1f, y_Pushkar, -50.0f);
	hms_drawO();

	//R
	glLoadIdentity();
	glTranslatef(x_Pushkar + 18.1f, y_Pushkar, -50.0f);
	hms_drawR();

	//E
	glLoadIdentity();
	glTranslatef(x_Pushkar + 19.4f, y_Pushkar, -50.0f);
	hms_drawE();
}

void hms_Shraddha(void)
{
	//S
	glLoadIdentity();
	glTranslatef(x_Shraddha, y_Shraddha, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Shraddha + 1.7f, y_Shraddha, -50.0f);
	hms_drawH();

	//R
	glLoadIdentity();
	glTranslatef(x_Shraddha + 2.6f, y_Shraddha, -50.0f);
	hms_drawR();

	//A
	glLoadIdentity();
	glTranslatef(x_Shraddha + 4.5f, y_Shraddha, -50.0f);
	hms_drawAEndCredit();

	//D
	glLoadIdentity();
	glTranslatef(x_Shraddha + 5.8f, y_Shraddha, -50.0f);
	hms_drawD();

	//D
	glLoadIdentity();
	glTranslatef(x_Shraddha + 7.4f, y_Shraddha, -50.0f);
	hms_drawD();

	//H
	glLoadIdentity();
	glTranslatef(x_Shraddha + 9.3f, y_Shraddha, -50.0f);
	hms_drawH();

	//A
	glLoadIdentity();
	glTranslatef(x_Shraddha + 11.0f, y_Shraddha, -50.0f);
	hms_drawAEndCredit();

	//D
	glLoadIdentity();
	glTranslatef(x_Shraddha + 13.0f, y_Shraddha, -50.0f);
	hms_drawD();

	//E
	glLoadIdentity();
	glTranslatef(x_Shraddha + 14.4f, y_Shraddha, -50.0f);
	hms_drawE();

	//S
	glLoadIdentity();
	glTranslatef(x_Shraddha + 15.5, y_Shraddha, -50.0f);
	hms_drawS();

	//H
	glLoadIdentity();
	glTranslatef(x_Shraddha + 17.2f, y_Shraddha, -50.0f);
	hms_drawH();

	//M
	glLoadIdentity();
	glTranslatef(x_Shraddha + 19.2f, y_Shraddha, -50.0f);
	hms_drawMEndCredit();

	//U
	glLoadIdentity();
	glTranslatef(x_Shraddha + 21.1f, y_Shraddha, -50.0f);
	hms_drawU();

	//K
	glLoadIdentity();
	glTranslatef(x_Shraddha + 22.1f, y_Shraddha, -50.0f);
	hms_drawK();

	//H
	glLoadIdentity();
	glTranslatef(x_Shraddha + 23.9f, y_Shraddha, -50.0f);
	hms_drawH();
}

void hms_Sukrut(void)
{
	//S
	glLoadIdentity();
	glTranslatef(x_Sukrut, y_Sukrut, -50.0f);
	hms_drawS();

	//U
	glLoadIdentity();
	glTranslatef(x_Sukrut + 1.8f, y_Sukrut, -50.0f);
	hms_drawU();

	//K
	glLoadIdentity();
	glTranslatef(x_Sukrut + 2.8f, y_Sukrut, -50.0f);
	hms_drawK();

	//R
	glLoadIdentity();
	glTranslatef(x_Sukrut + 4.0f, y_Sukrut, -50.0f);
	hms_drawR();

	//U
	glLoadIdentity();
	glTranslatef(x_Sukrut + 6.0f, y_Sukrut, -50.0f);
	hms_drawU();

	//T
	glLoadIdentity();
	glTranslatef(x_Sukrut + 7.4f, y_Sukrut, -50.0f);
	hms_drawT();

	//D
	glLoadIdentity();
	glTranslatef(x_Sukrut + 9.4, y_Sukrut, -50.0f);
	hms_drawD();

	//A
	glLoadIdentity();
	glTranslatef(x_Sukrut + 11.4f, y_Sukrut, -50.0f);
	hms_drawAEndCredit();

	//T
	glLoadIdentity();
	glTranslatef(x_Sukrut + 12.8f, y_Sukrut, -50.0f);
	hms_drawT();

	//A
	glLoadIdentity();
	glTranslatef(x_Sukrut + 14.2f, y_Sukrut, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Sukrut + 15.2f, y_Sukrut, -50.0f);
	hms_drawR();
}

void hms_Vinay(void)
{
	//V
	glLoadIdentity();
	glTranslatef(x_Vinay, y_Vinay, -50.0f);
	hms_drawV();

	//I
	glLoadIdentity();
	glTranslatef(x_Vinay + 1.5f, y_Vinay, -50.0f);
	hms_drawI();

	//N
	glLoadIdentity();
	glTranslatef(x_Vinay + 2.6f, y_Vinay, -50.0f);
	hms_drawN();

	//A
	glLoadIdentity();
	glTranslatef(x_Vinay + 4.6f, y_Vinay, -50.0f);
	hms_drawAEndCredit();

	//Y
	glLoadIdentity();
	glTranslatef(x_Vinay + 6.4f, y_Vinay, -50.0f);
	hms_drawY();

	//R
	glLoadIdentity();
	glTranslatef(x_Vinay + 8.4f, y_Vinay, -50.0f);
	hms_drawR();

	//E
	glLoadIdentity();
	glTranslatef(x_Vinay + 9.9f, y_Vinay, -50.0f);
	hms_drawE();

	//V
	glLoadIdentity();
	glTranslatef(x_Vinay + 11.8f, y_Vinay, -50.0f);
	hms_drawV();

	//A
	glLoadIdentity();
	glTranslatef(x_Vinay + 13.6f, y_Vinay, -50.0f);
	hms_drawAEndCredit();

	//R
	glLoadIdentity();
	glTranslatef(x_Vinay + 14.6f, y_Vinay, -50.0f);
	hms_drawR();
}

void hms_drawH(void)
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

	glVertex3f(0.4f, 0.1f, 0.0f);
	glVertex3f(-0.4f, 0.1f, 0.0f);
	glVertex3f(-0.4f, -0.1f, 0.0f);
	glVertex3f(0.4f, -0.1f, 0.0f);

	glEnd();
}


void hms_drawK(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.2f, 0.2f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawD(void)
{
	glBegin(GL_QUADS);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.3f, 1.0f, 0.0f);
	glVertex3f(-0.3f, 0.8f, 0.0f);
	glVertex3f(1.0f, 0.8f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -0.8f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -1.0f, 0.0f);
	glVertex3f(-0.3f, -0.8f, 0.0f);
	glVertex3f(1.0f, -0.8f, 0.0f);

	glVertex3f(0.1f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 1.0f, 0.0f);
	glVertex3f(-0.1f, -0.8f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawJ(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(0.0f, -0.8f, 0.0f);
	glVertex3f(-1.0f, -0.8f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glVertex3f(-0.8f, -0.4f, 0.0f);
	glVertex3f(-1.0f, -0.4f, 0.0f);
	glVertex3f(-1.0f, -0.8f, 0.0f);
	glVertex3f(-0.8f, -0.8f, 0.0f);

	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);
	glEnd();
}

void hms_drawL(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.2f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.2f, -1.0f, 0.0f);

	glVertex3f(1.0f, -0.8f, 0.0f);
	glVertex3f(0.0f, -0.8f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();
}

void hms_drawV(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.8f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawY(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.8f, 1.0f, 0.0f);
	glVertex3f(-0.8f, 0.8f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, -1.0f, 0.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);
	glEnd();
}

void hms_drawDash(void)
{
	glBegin(GL_QUADS);
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(-0.5f, 0.1f, 0.0f);
	glVertex3f(-0.5f, -0.1f, 0.0f);
	glVertex3f(0.5f, -0.1f, 0.0f);
	glEnd();
}


void hms_drawAEndCredit(void)
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

	glVertex3f(0.4f, 0.1f, 0.0f);
	glVertex3f(-0.4f, 0.1f, 0.0f);
	glVertex3f(-0.4f, -0.1f, 0.0f);
	glVertex3f(0.4f, -0.1f, 0.0f);

	glVertex3f(0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 1.0f, 0.0f);
	glVertex3f(-0.4f, 0.8f, 0.0f);
	glVertex3f(0.4f, 0.8f, 0.0f);

	glEnd();
}

void hms_drawMEndCredit(void)
{
	glBegin(GL_QUADS);
	glVertex3f(-0.8f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.8f, -1.0f, 0.0f);

	glVertex3f(-0.8f, 1.0f, 0.0f);
	glVertex3f(-0.8f, 0.5f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.8f, 0.5f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.8f, 1.0f, 0.0f);
	glVertex3f(0.8f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}

// -------------------------------------- Drawing Scene -----------------------------

void sceneDrawing(void)
{
	void drawWalls(void);
	void drawBoard(void);
	void drawCat(void);
	void drawGirl(void);
	void drawFan(void);

	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);

	drawWalls();

	drawBoard();

	drawCat();

	drawGirl();

	drawFan();

}

void drawWalls(void)
{
	glBegin(GL_QUADS);


	//sides
	glColor3f(0.57f, 0.77f, 0.44f);
	glVertex3f(-2.0f, 1.3f, -3.0f);
	glVertex3f(-2.2f, 1.3f, 0.0f);
	glColor3f(0.23f, 0.33f, 0.22f);
	glVertex3f(-2.2f, -1.3f, 0.0f);
	glVertex3f(-2.0f, -1.3f, -3.0f);

	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(-2.0f, -0.8f, -3.0f);
	glVertex3f(-2.2f, -0.8f, 0.01f);
	glVertex3f(-2.2f, -1.3f, 0.01f);
	glVertex3f(-2.0f, -1.3f, -3.0f);

	glColor3f(0.57f, 0.77f, 0.44f);
	glVertex3f(2.0f, 1.3f, -3.0f);
	glVertex3f(2.2f, 1.3f, 0.0f);
	glColor3f(0.23f, 0.33f, 0.22f);
	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(2.0f, -1.3f, -3.0f);

	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(2.0f, -0.8f, -3.0f);
	glVertex3f(2.2f, -0.8f, 0.01f);
	glVertex3f(2.2f, -1.3f, 0.01f);
	glVertex3f(2.0f, -1.3f, -3.0f);

	//

	//back wall



	glColor3f(0.57f, 0.77f, 0.44f);
	glVertex3f(2.0f, 1.3f, -3.0f);
	glVertex3f(-2.0f, 1.3f, -3.0f);
	glColor3f(0.23f, 0.33f, 0.22f);
	glVertex3f(-2.0f, -1.3f, -3.0f);
	glVertex3f(2.0f, -1.3f, -3.0f);

	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(2.0f, -0.8f, -3.0f);
	glVertex3f(-2.0f, -0.8f, -3.0f);
	glVertex3f(-2.0f, -1.3f, -3.0f);
	glVertex3f(2.0f, -1.3f, -3.0f);


	//
	glEnd();

	//ceiling

	glBegin(GL_QUADS);
	glColor3f(0.9f, 0.9f, 0.8f);

	glVertex3f(2.2f, 1.3f, 0.0f);
	glVertex3f(-2.2f, 1.3f, 0.0f);
	glVertex3f(-2.0f, 1.0f, -3.0f);
	glVertex3f(2.0f, 1.0f, -3.0f);
	glEnd();


	//floor

	glBegin(GL_QUADS);
	glColor3f(0.74f, 0.39f, 0.11f);

	glVertex3f(2.2f, -1.3f, 0.0f);
	glVertex3f(-2.2f, -1.3f, 0.0f);
	glVertex3f(-2.0f, -1.0f, -3.0f);
	glVertex3f(2.0f, -1.0f, -3.0f);
	glEnd();

	//



}

void drawBoard(void)
{
	glBegin(GL_QUADS);
	glColor3f(0.9f, 0.9f, 0.8f);
	glVertex3f(-0.5f, 0.1f, -2.0f);
	glVertex3f(0.3f, 0.1f, -2.0f);
	glVertex3f(0.3f, -0.7f, -2.0f);
	glVertex3f(-0.5f, -0.7f, -2.0f);
	glEnd();
	
	glLoadIdentity();
	glTranslatef(-0.1f, -0.3f, -3.0f);
	glScalef(0.5f, 0.5f, 0.0f);
	SGK_drawPainting();

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	//stands
	glLineWidth(10.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.09f, -0.7f, -2.0f);
	glVertex3f(-0.09f, -1.0f, -2.0f);

	glVertex3f(-0.1f, -0.7f, -2.0f);
	glVertex3f(-0.3f, -1.0f, -2.0f);

	glVertex3f(-0.09f, -0.7f, -2.0f);
	glVertex3f(0.1f, -1.0f, -2.0f);
	glEnd();
	//
}

void drawCat(void)
{
	glTranslatef(-0.6f, -0.5f, 0.5f);
	glScalef(0.5f, 0.5f, -0.0f);


	//draw tail
	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.5f, 0.5f);


	glVertex3f(0.0f, -0.1f, 0.0f);

	glVertex3f(-0.1f, -1.0f, 0.0f);

	glVertex3f(0.1f, -1.0f, 0.0f);

	glEnd();
	//

	//draw body
	glBegin(GL_LINES);

	float radius = 0.2f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(x, -(y + 0.5f), 0.0f);
	}

	glEnd();
	//


	//draw head
	glBegin(GL_LINES);

	radius = 0.15f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();

	//


	//draw ears
	glTranslatef(-0.07f, 0.16f, -1.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.5f, 0.5f);


	glVertex3f(0.0f, 0.05f, 0.0f);

	glVertex3f(-0.05f, -0.05f, 0.0f);

	glVertex3f(0.05f, -0.05f, 0.0f);

	glEnd();


	glTranslatef(0.14f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);

	glVertex3f(0.0f, 0.05f, 0.0f);

	glVertex3f(-0.05f, -0.05f, 0.0f);

	glVertex3f(0.05f, -0.05f, 0.0f);

	glEnd();

	//
}

void drawFan(void)
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.5f, -1.7f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	glLineWidth(10.0f);

	glBegin(GL_LINES);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -0.0f, 0.0f);
	glEnd();

	glTranslatef(0.0f, 0.0f, 0.01f);
	xRotationf = xRotationf + 4.0f;
	glRotatef(xRotationf, 0.0f, 0.0f, 1.0f);

	//draw fan
	glBegin(GL_LINES);

	float radius = 0.15f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		glColor3f(0.74f, 0.39f, 0.11f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		glColor3f(0.74f, 0.39f, 0.11f);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();



	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.2f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.15f, -0.4f, 0.0f);
	glVertex3f(0.25f, -0.4f, 0.0f);
	glVertex3f(0.25f, 0.4f, 0.0f);
	glVertex3f(0.15f, 0.4f, 0.0f);

	glEnd();


	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -0.2f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-0.05f, -0.4f, 0.0f);
	glVertex3f(0.05f, -0.4f, 0.0f);
	glVertex3f(0.05f, 0.4f, 0.0f);
	glVertex3f(-0.05f, 0.4f, 0.0f);
	glEnd();

	//
}
void drawGirl()
{
	glLoadIdentity();
	glTranslatef(0.2f, 0.0f, -2.5f);

	//draw legs
	glBegin(GL_QUADS);
	//glColor3f(1.0f, 0.85f, 0.67f);
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(-0.0f, -0.55f, 0.0f);
	glVertex3f(-0.10f, -0.55f, 0.0f);
	glVertex3f(-0.13f, -0.75f, 0.0f);
	glVertex3f(-0.10f, -0.75f, 0.0f);

	glVertex3f(0.10f, -0.55f, 0.0f);
	glVertex3f(0.0f, -0.55f, 0.0f);
	glVertex3f(0.07f, -0.75f, 0.0f);
	glVertex3f(0.1f, -0.75f, 0.0f);

	glEnd();


	// leg wrist
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.2f, 0.2f);

	glVertex3f(0.11f, -0.75f, 0.0f);
	glVertex3f(0.06f, -0.75f, 0.0f);
	glVertex3f(0.07f, -0.80f, 0.0f);
	glVertex3f(0.10f, -0.80f, 0.0f);

	glVertex3f(-0.14f, -0.75f, 0.0f);
	glVertex3f(-0.09f, -0.75f, 0.0f);
	glVertex3f(-0.10f, -0.80f, 0.0f);
	glVertex3f(-0.13f, -0.80f, 0.0f);

	glEnd();
	//



	//draw body
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.2f, -0.15f, 0.0f);
	glVertex3f(-0.2f, -0.15f, 0.0f);
	glVertex3f(-0.1f, -0.25f, 0.0f);
	glVertex3f(0.1f, -0.25f, 0.0f);

	glVertex3f(0.1f, -0.25f, 0.0f);
	glVertex3f(-0.1f, -0.25f, 0.0f);
	glVertex3f(-0.1f, -0.35f, 0.0f);
	glVertex3f(0.1f, -0.35f, 0.0f);

	glVertex3f(0.1f, -0.35f, 0.0f);
	glVertex3f(-0.1f, -0.35f, 0.0f);
	glVertex3f(-0.2f, -0.60f, 0.0f);
	glVertex3f(0.2f, -0.60f, 0.0f);

	glEnd();
	//

   //draw hands
	glBegin(GL_QUADS);
	/* glColor3f(1.0f, 0.85f, 0.67f);*/
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(0.18f, -0.17f, 0.0f);
	glVertex3f(0.12f, -0.23f, 0.0f);
	glVertex3f(0.15f, -0.4f, 0.0f);
	glVertex3f(0.21f, -0.4f, 0.0f);


	glVertex3f(-0.18f, -0.17f, 0.0f);
	glVertex3f(-0.12f, -0.23f, 0.0f);
	glVertex3f(-0.15f, -0.33f, 0.0f);
	glVertex3f(-0.21f, -0.33f, 0.0f);

	glVertex3f(-0.15f, -0.33f, 0.0f);
	glVertex3f(-0.21f, -0.33f, 0.0f);
	glVertex3f(-0.30f, -0.2f, 0.0f);
	glVertex3f(-0.25f, -0.2f, 0.0f);

	glEnd();

	//


	// hand wrist

	glBegin(GL_LINES);

	float radius = 0.03f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		//glColor3f(1.0f, 0.85f, 0.67f);
		glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
		glVertex3f(-0.28f, -0.2f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		//glColor3f(1.0f, 0.85f, 0.67f);
		glVertex3f(x + (-0.28f), y - 0.2f, 0.0f);
	}

	glEnd();

	//draw pen
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	
	glColor3f(220.0f / 255.0f, 118.0f / 255.0f, 51.0f / 255.0f);
	glVertex3f(-0.28f, -0.2f, 0.0f);
	glVertex3f(-0.28f, -0.15f, 0.0f);
	glEnd();

	//draw crayons
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.25f, -0.85f, -1.5f);
	glVertex3f(-0.30f, -0.95f, -1.5f);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.26f, -0.87f, -1.5f);
	glVertex3f(-0.29f, -0.93f, -1.5f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.35f, -0.85f, -1.5f);
	glVertex3f(-0.40f, -0.95f, -1.5f);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.36f, -0.87f, -1.5f);
	glVertex3f(-0.39f, -0.93f, -1.5f);


	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.50f, -0.85f, -1.5f);
	glVertex3f(-0.45f, -0.95f, -1.5f);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.49f, -0.87f, -1.5f);
	glVertex3f(-0.46f, -0.93f, -1.5f);

	glEnd();

	//



	glBegin(GL_LINES);

	radius = 0.03f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		//glColor3f(1.0f, 0.85f, 0.67f);
		glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
		glVertex3f(0.18f, -0.4f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		//glColor3f(1.0f, 0.85f, 0.67f);
		glVertex3f(x + (0.18f), y - 0.4f, 0.0f);
	}

	glEnd();


	glLineWidth(1.5f);
	glBegin(GL_LINES);
	//glColor3f(0.0f,0.0f,0.0f);
	glColor3f(204.0f / 255.0f, 152.0f / 255.0f, 85.0f / 255.0f);
	glVertex3f(0.18f, -0.4f, 0.0f);
	glVertex3f(0.18f, -0.43f, 0.0f);

	glVertex3f(0.16f, -0.4f, 0.0f);
	glVertex3f(0.16f, -0.43f, 0.0f);

	glVertex3f(0.20f, -0.4f, 0.0f);
	glVertex3f(0.20f, -0.43f, 0.0f);

	glVertex3f(0.14f, -0.38f, 0.0f);
	glVertex3f(0.21f, -0.38f, 0.0f);

	glEnd();
	//

	//draw head
	glBegin(GL_LINES);

	radius = 0.10f;

	for (float angle = 0.0f; angle < 360.0f; angle = angle + 0.1f)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		float radianAngle = angle * (M_PI * 180.0f);

		float x = radius * cos(radianAngle);
		float y = radius * sin(radianAngle);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();




	//

	//draw head
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, -0.2f, 0.0f);
	glVertex3f(0.1f, -0.2f, 0.0f);
	glEnd();
	//


	//draw pen
	glLineWidth(5.0f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.05f, -0.1f, 0.0f);
	glVertex3f(0.05f, -0.1f, 0.0f);

	glEnd();


	//




}

// -------------------------------------  Scene Reading ----------------------------

void sceneReading(void)
{
	// for Upper
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);
	glScalef(1.9f, 1.05f, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.9f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);
	glVertex3f(1.0f, -0.2f, 0.0f);
	glEnd();

	//for Lower

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(1.0f, -0.2f, 0.0f);
	glVertex3f(-1.0f, -0.2f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	// LINES 1
	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.1f, -0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.990f, -1.0f, 0.0f);
	glVertex3f(0.110f, -0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// LINES 2

	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -0.4f, 0.0f);
	glVertex3f(-0.2f, -1.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.210f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -0.410f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// LINE 3
	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, -0.2f, 0.0f);
	glVertex3f(0.310f, -1.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(6.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.510f, -0.2f, 0.0f);
	glVertex3f(0.3f, -1.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// LINE 4

	glLineWidth(10.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, -0.2f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// LINE 5
	glLineWidth(10.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.1f, -1.0f, 0.0f);
	glVertex3f(1.0f, -0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// LEFT CUBORD
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(-0.980f, 0.0f, 0.0f);
	glVertex3f(-0.980f, -0.2f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);
	glVertex3f(-0.4f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(-0.920f, 0.0f, 0.0f);
	glVertex3f(-0.470f, 0.0f, 0.0f);
	glVertex3f(-0.470f, 0.7f, 0.0f);
	glVertex3f(-0.920f, 0.7f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(-0.985f, 0.7f, 0.0f);
	glVertex3f(-0.4f, 0.7f, 0.0f);
	glVertex3f(-0.4f, 0.760f, 0.0f);
	glVertex3f(-0.985f, 0.760f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.9f, 0.660f, 0.0f);
	glVertex3f(-0.720f, 0.660f, 0.0f);
	glVertex3f(-0.720f, 0.430f, 0.0f);
	glVertex3f(-0.9f, 0.430f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.9f, 0.4f, 0.0f);
	glVertex3f(-0.720f, 0.4f, 0.0f);
	glVertex3f(-0.720f, 0.2f, 0.0f);
	glVertex3f(-0.9f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.9f, 0.18f, 0.0f);
	glVertex3f(-0.720f, 0.18f, 0.0f);
	glVertex3f(-0.720f, 0.0f, 0.0f);
	glVertex3f(-0.9f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.480f, 0.660f, 0.0f);
	glVertex3f(-0.7f, 0.660f, 0.0f);
	glVertex3f(-0.7f, 0.430f, 0.0f);
	glVertex3f(-0.480f, 0.430f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.480f, 0.4f, 0.0f);
	glVertex3f(-0.7f, 0.4f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);
	glVertex3f(-0.480f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.480f, 0.18f, 0.0f);
	glVertex3f(-0.7f, 0.18f, 0.0f);
	glVertex3f(-0.7f, 0.0f, 0.0f);
	glVertex3f(-0.480f, 0.0f, 0.0f);
	glEnd();


	// RIGHT CUBORD
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(0.980f, -0.2f, 0.0f);
	glVertex3f(0.310f, -0.2f, 0.0f);
	glVertex3f(0.310f, 0.0f, 0.0f);
	glVertex3f(0.980f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(0.920f, 0.0f, 0.0f);
	glVertex3f(0.380f, 0.0f, 0.0f);
	glVertex3f(0.380f, 0.7f, 0.0f);
	glVertex3f(0.920f, 0.7f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f(0.980f, 0.7f, 0.0f);
	glVertex3f(0.330f, 0.7f, 0.0f);
	glVertex3f(0.330f, 0.770f, 0.0f);
	glVertex3f(0.980f, 0.770f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.4f, 0.660f, 0.0f);
	glVertex3f(0.6f, 0.660f, 0.0f);
	glVertex3f(0.6f, 0.450f, 0.0f);
	glVertex3f(0.4f, 0.450f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.4f, 0.420f, 0.0f);
	glVertex3f(0.6f, 0.420f, 0.0f);
	glVertex3f(0.6f, 0.220f, 0.0f);
	glVertex3f(0.4f, 0.220f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);
	glVertex3f(0.6f, 0.2f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.4f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.9f, 0.660f, 0.0f);
	glVertex3f(0.630f, 0.660f, 0.0f);
	glVertex3f(0.630f, 0.450f, 0.0f);
	glVertex3f(0.9f, 0.450f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.9f, 0.430f, 0.0f);
	glVertex3f(0.630f, 0.430f, 0.0f);
	glVertex3f(0.630f, 0.220f, 0.0f);
	glVertex3f(0.9f, 0.220f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glVertex3f(0.630f, 0.2f, 0.0f);
	glVertex3f(0.630f, 0.0f, 0.0f);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glEnd();

	// WINDOW
	glBegin(GL_QUADS);
	glColor3f(0.3f, 0.9f, 1.0f);
	glVertex3f(-0.310f, 0.3f, 0.0f);
	glVertex3f(-0.310f, 0.9f, 0.0f);
	glVertex3f(0.230f, 0.9f, 0.0f);
	glVertex3f(0.230f, 0.3f, 0.0f);
	glEnd();

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(-0.315f, 0.890f, 0.0f);
	glVertex3f(0.235f, 0.890f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(-0.315f, 0.310f, 0.0f);
	glVertex3f(0.235f, 0.310f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(-0.315f, 0.9f, 0.0f);
	glVertex3f(-0.315f, 0.3f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(0.235f, 0.9f, 0.0f);
	glVertex3f(0.235f, 0.3f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(-0.035f, 0.9f, 0.0f);
	glVertex3f(-0.035f, 0.3f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(15.0f);
	glBegin(GL_LINES);
	glColor3f(0.7f, 0.4f, 0.1f);
	glVertex3f(-0.315f, 0.6f, 0.0f);
	glVertex3f(0.235f, 0.6f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);
	glScalef(1.7f, 1.05f, 0.0f);
	// TABLE
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.4f, -0.580f, 0.0f);
	glVertex3f(-0.4f, -0.640f, 0.0f);
	glVertex3f(0.3f, -0.640f, 0.0f);
	glVertex3f(0.3f, -0.580f, 0.0f);
	glEnd();

	glLineWidth(14.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.250f, -0.640f, 0.0f);
	glVertex3f(0.250f, -0.9f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(14.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.350f, -0.640f, 0.0f);
	glVertex3f(-0.350f, -0.90f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(14.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.290f, -0.640f, 0.0f);
	glVertex3f(-0.290f, -0.9f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(14.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.190f, -0.640f, 0.0f);
	glVertex3f(0.190f, -0.9f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// BOOKS ON TABLE

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-0.4f, -0.540f, 0.0f);
	glVertex3f(-0.4f, -0.590f, 0.0f);
	glVertex3f(-0.230f, -0.590f, 0.0f);
	glVertex3f(-0.230f, -0.540f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.540f, 0.0f);
	glVertex3f(-0.230f, -0.540f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.540f, 0.0f);
	glVertex3f(-0.4f, -0.490f, 0.0f);
	glVertex3f(-0.230f, -0.490f, 0.0f);
	glVertex3f(-0.230f, -0.540f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.4f, -0.440f, 0.0f);
	glVertex3f(-0.4f, -0.490f, 0.0f);
	glVertex3f(-0.230f, -0.490f, 0.0f);
	glVertex3f(-0.230f, -0.440f, 0.0f);
	glEnd();

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.490f, 0.0f);
	glVertex3f(-0.230f, -0.490f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, -0.540f, 0.0f);
	glVertex3f(0.3f, -0.590f, 0.0f);
	glVertex3f(0.15f, -0.590f, 0.0f);
	glVertex3f(0.15f, -0.540f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.540f, 0.0f);
	glVertex3f(0.15f, -0.540f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, -0.540f, 0.0f);
	glVertex3f(0.3f, -0.490f, 0.0f);
	glVertex3f(0.15f, -0.490f, 0.0f);
	glVertex3f(0.15f, -0.540f, 0.0f);
	glEnd();

	// CHAIR
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.15f, -0.72f, 0.0f);
	glVertex3f(-0.15f, -0.78f, 0.0f);
	glVertex3f(0.05f, -0.78f, 0.0f);
	glVertex3f(0.05f, -0.72f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(-0.15f, -0.75f, 0.0f);
	glVertex3f(-0.15f, -0.92f, 0.0f);
	glVertex3f(-0.12f, -0.92f, 0.0f);
	glVertex3f(-0.12f, -0.75f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.3f, 0.0f);
	glVertex3f(0.05f, -0.75f, 0.0f);
	glVertex3f(0.05f, -0.92f, 0.0f);
	glVertex3f(0.02f, -0.92f, 0.0f);
	glVertex3f(0.02f, -0.75f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.4f, 1.0f);
	glVertex3f(-0.11f, -0.785f, 0.0f);
	glColor3f(0.0f, 0.4f, 0.5f);
	glVertex3f(-0.11f, -0.640f, 0.0f);
	glVertex3f(0.01f, -0.640f, 0.0f);
	glColor3f(0.0f, 0.4f, 1.0f);
	glVertex3f(0.01f, -0.785f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(-0.1f, -0.785f, 0.0f);
	glVertex3f(-0.1f, -0.92f, 0.0f);
	glVertex3f(-0.075f, -0.92f, 0.0f);
	glVertex3f(-0.075f, -0.785f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(0.0f, -0.785f, 0.0f);
	glVertex3f(0.0f, -0.92f, 0.0f);
	glVertex3f(-0.025f, -0.92f, 0.0f);
	glVertex3f(-0.025f, -0.785f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.105f, -0.89f, 0.0f);
	glVertex3f(-0.105f, -0.94f, 0.0f);
	glVertex3f(-0.07f, -0.94f, 0.0f);
	glVertex3f(-0.07f, -0.89f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.005f, -0.89f, 0.0f);
	glVertex3f(0.005f, -0.94f, 0.0f);
	glVertex3f(-0.03f, -0.94f, 0.0f);
	glVertex3f(-0.03f, -0.89f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);
	glScalef(1.9f, 1.05f, 0.0f);

	// Face
	glLineWidth(1.0f);
	float radius1 = 0.080f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius1 * cos((angle * M_PI) / 180.0f)) + (-0.05);
		float y = (radius1 * sin((angle * M_PI) / 180.0f)) + (-0.31);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.05f, -0.31f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	float radius2 = 0.060f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius2 * cos((angle * M_PI) / 180.0f)) + (-0.05);
		float y = (radius2 * sin((angle * M_PI) / 180.0f)) + (-0.33);

		glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
		glVertex3f(-0.05f, -0.33f, 0.0f);

		glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	float radius5 = 0.015f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius5 * cos((angle * M_PI) / 180.0f)) + (-0.08);
		float y = (radius5 * sin((angle * M_PI) / 180.0f)) + (-0.33);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.08f, -0.33f, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	float radius6 = 0.015f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius6 * cos((angle * M_PI) / 180.0f)) + (-0.02);
		float y = (radius6 * sin((angle * M_PI) / 180.0f)) + (-0.33);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.02f, -0.33f, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();


	float radius8 = 0.005f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius8 * cos((angle * M_PI) / 180.0f)) + (-0.02);
		float y = (radius8 * sin((angle * M_PI) / 180.0f)) + (-0.341);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.02f, -0.341f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();


	float radius9 = 0.005f;
	glBegin(GL_LINES);
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius9 * cos((angle * M_PI) / 180.0f)) + (-0.08);
		float y = (radius9 * sin((angle * M_PI) / 180.0f)) + (-0.341);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.08f, -0.341f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	float radius7 = 0.020f;
	glBegin(GL_LINES);
	for (float angle = 180.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		// float radiangle = angle * (M_PI * 180.0f)
		float x = (radius7 * cos((angle * M_PI) / 180.0f)) + (-0.05);
		float y = (radius7 * sin((angle * M_PI) / 180.0f)) + (-0.365);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.05f, -0.365f, 0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	// READING BOOK

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.8f);
	glVertex3f(-0.04f, -0.39f, 0.0f);
	glVertex3f(-0.04f, -0.58f, 0.0f);
	glColor3f(0.0f, 0.6f, 0.6f);
	glVertex3f(0.07f, -0.58f, 0.0f);
	glVertex3f(0.07f, -0.39f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.9f, 0.9f);
	glVertex3f(-0.04f, -0.39f, 0.0f);
	glVertex3f(-0.06f, -0.39f, 0.0f);
	glVertex3f(-0.06f, -0.58f, 0.0f);
	glVertex3f(-0.04f, -0.58f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.8f);
	glVertex3f(-0.06f, -0.39f, 0.0f);
	glVertex3f(-0.06f, -0.58f, 0.0f);
	glColor3f(0.0f, 0.6f, 0.6f);
	glVertex3f(-0.17f, -0.58f, 0.0f);
	glVertex3f(-0.17f, -0.39f, 0.0f);
	glEnd();

	// HANDS

	glBegin(GL_QUADS);
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(0.09f, -0.48f, 0.0f);
	glVertex3f(0.09f, -0.54f, 0.0f);
	glVertex3f(0.05f, -0.54f, 0.0f);
	glVertex3f(0.05f, -0.48f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(224.0f / 255.0f, 172.0f / 255.0f, 105.0f / 255.0f);
	glVertex3f(-0.15f, -0.48f, 0.0f);
	glVertex3f(-0.19f, -0.48f, 0.0f);
	glVertex3f(-0.19f, -0.54f, 0.0f);
	glVertex3f(-0.15f, -0.54f, 0.0f);
	glEnd();


	// FINGERS

	glLineWidth(1.5f);
	glBegin(GL_LINES);
	glColor3f(194.0f / 255.0f, 134.0f / 255.0f, 66.0f / 255.0f);
	glVertex3f(-0.15f, -0.50f, 0.0f);
	glVertex3f(-0.19f, -0.50f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-0.15f, -0.52f, 0.0f);
	glVertex3f(-0.19f, -0.52f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.05f, -0.50f, 0.0f);
	glVertex3f(0.09f, -0.50f, 0.0f);
	glEnd();
	glBegin(GL_LINES);

	glVertex3f(0.05f, -0.52f, 0.0f);
	glVertex3f(0.09f, -0.52f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	// BOOKS IN RIGHT COBORD

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.40f, 0.600f, 0.0f);
	glVertex3f(0.43f, 0.600f, 0.0f);
	glVertex3f(0.43f, 0.450f, 0.0f);
	glVertex3f(0.40f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.43f, 0.600f, 0.0f);
	glVertex3f(0.43f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.600f, 0.0f);
	glVertex3f(0.43f, 0.600f, 0.0f);
	glVertex3f(0.43f, 0.450f, 0.0f);
	glVertex3f(0.46f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.600f, 0.0f);
	glVertex3f(0.46f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.600f, 0.0f);
	glVertex3f(0.49f, 0.600f, 0.0f);
	glVertex3f(0.49f, 0.450f, 0.0f);
	glVertex3f(0.46f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.49f, 0.600f, 0.0f);
	glVertex3f(0.49f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.52f, 0.600f, 0.0f);
	glVertex3f(0.49f, 0.600f, 0.0f);
	glVertex3f(0.49f, 0.450f, 0.0f);
	glVertex3f(0.52f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.600f, 0.0f);
	glVertex3f(0.52f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.52f, 0.600f, 0.0f);
	glVertex3f(0.55f, 0.600f, 0.0f);
	glVertex3f(0.55f, 0.450f, 0.0f);
	glVertex3f(0.52f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.600f, 0.0f);
	glVertex3f(0.52f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.58f, 0.600f, 0.0f);
	glVertex3f(0.55f, 0.600f, 0.0f);
	glVertex3f(0.55f, 0.450f, 0.0f);
	glVertex3f(0.58f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.55f, 0.600f, 0.0f);
	glVertex3f(0.55f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.600f, 0.0f);
	glVertex3f(0.52f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.4f, 0.360f, 0.0f);
	glVertex3f(0.43f, 0.360f, 0.0f);
	glVertex3f(0.43f, 0.220f, 0.0f);
	glVertex3f(0.4f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.43f, 0.360f, 0.0f);
	glVertex3f(0.43f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.46f, 0.360f, 0.0f);
	glVertex3f(0.43f, 0.360f, 0.0f);
	glVertex3f(0.43f, 0.220f, 0.0f);
	glVertex3f(0.46f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.360f, 0.0f);
	glVertex3f(0.46f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.46f, 0.360f, 0.0f);
	glVertex3f(0.49f, 0.360f, 0.0f);
	glVertex3f(0.49f, 0.220f, 0.0f);
	glVertex3f(0.46f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.49f, 0.360f, 0.0f);
	glVertex3f(0.49f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.52f, 0.360f, 0.0f);
	glVertex3f(0.49f, 0.360f, 0.0f);
	glVertex3f(0.49f, 0.220f, 0.0f);
	glVertex3f(0.52f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.360f, 0.0f);
	glVertex3f(0.52f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.52f, 0.360f, 0.0f);
	glVertex3f(0.55f, 0.360f, 0.0f);
	glVertex3f(0.55f, 0.220f, 0.0f);
	glVertex3f(0.52f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.55f, 0.360f, 0.0f);
	glVertex3f(0.55f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.58f, 0.360f, 0.0f);
	glVertex3f(0.55f, 0.360f, 0.0f);
	glVertex3f(0.55f, 0.220f, 0.0f);
	glVertex3f(0.58f, 0.220f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.14f, 0.0f);
	glVertex3f(0.43f, 0.14f, 0.0f);
	glVertex3f(0.43f, 0.0f, 0.0f);
	glVertex3f(0.4f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.43f, 0.14f, 0.0f);
	glVertex3f(0.43f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.46f, 0.14f, 0.0f);
	glVertex3f(0.43f, 0.14f, 0.0f);
	glVertex3f(0.43f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.14f, 0.0f);
	glVertex3f(0.46f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.46f, 0.14f, 0.0f);
	glVertex3f(0.49f, 0.14f, 0.0f);
	glVertex3f(0.49f, 0.0f, 0.0f);
	glVertex3f(0.46f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.49f, 0.14f, 0.0f);
	glVertex3f(0.49f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.52f, 0.14f, 0.0f);
	glVertex3f(0.49f, 0.14f, 0.0f);
	glVertex3f(0.49f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.14f, 0.0f);
	glVertex3f(0.52f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.52f, 0.14f, 0.0f);
	glVertex3f(0.55f, 0.14f, 0.0f);
	glVertex3f(0.55f, 0.0f, 0.0f);
	glVertex3f(0.52f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.55f, 0.14f, 0.0f);
	glVertex3f(0.55f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.58f, 0.14f, 0.0f);
	glVertex3f(0.55f, 0.14f, 0.0f);
	glVertex3f(0.55f, 0.0f, 0.0f);
	glVertex3f(0.58f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.630f, 0.600f, 0.0f);
	glVertex3f(0.660f, 0.600f, 0.0f);
	glVertex3f(0.660f, 0.450f, 0.0f);
	glVertex3f(0.630f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.660f, 0.600f, 0.0f);
	glVertex3f(0.660f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.690f, 0.600f, 0.0f);
	glVertex3f(0.660f, 0.600f, 0.0f);
	glVertex3f(0.660f, 0.450f, 0.0f);
	glVertex3f(0.690f, 0.450f, 0.0f);
	glEnd();


	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.600f, 0.0f);
	glVertex3f(0.690f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.690f, 0.600f, 0.0f);
	glVertex3f(0.720f, 0.600f, 0.0f);
	glVertex3f(0.720f, 0.450f, 0.0f);
	glVertex3f(0.690f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.720f, 0.600f, 0.0f);
	glVertex3f(0.720f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.750f, 0.600f, 0.0f);
	glVertex3f(0.720f, 0.600f, 0.0f);
	glVertex3f(0.720f, 0.450f, 0.0f);
	glVertex3f(0.750f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.600f, 0.0f);
	glVertex3f(0.750f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.750f, 0.600f, 0.0f);
	glVertex3f(0.780f, 0.600f, 0.0f);
	glVertex3f(0.780f, 0.450f, 0.0f);
	glVertex3f(0.750f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.780f, 0.600f, 0.0f);
	glVertex3f(0.780f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.4f);
	glVertex3f(0.810f, 0.620f, 0.0f);
	glVertex3f(0.780f, 0.620f, 0.0f);
	glVertex3f(0.780f, 0.450f, 0.0f);
	glVertex3f(0.810f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.620f, 0.0f);
	glVertex3f(0.810f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.4f);
	glVertex3f(0.810f, 0.620f, 0.0f);
	glVertex3f(0.840f, 0.620f, 0.0f);
	glVertex3f(0.840f, 0.450f, 0.0f);
	glVertex3f(0.810f, 0.450f, 0.0f);
	glEnd();


	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.840f, 0.620f, 0.0f);
	glVertex3f(0.840f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.4f);
	glVertex3f(0.870f, 0.620f, 0.0f);
	glVertex3f(0.840f, 0.620f, 0.0f);
	glVertex3f(0.840f, 0.450f, 0.0f);
	glVertex3f(0.870f, 0.450f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.620f, 0.0f);
	glVertex3f(0.870f, 0.450f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.4f);
	glVertex3f(0.870f, 0.620f, 0.0f);
	glVertex3f(0.9f, 0.620f, 0.0f);
	glVertex3f(0.9f, 0.450f, 0.0f);
	glVertex3f(0.870f, 0.450f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.2f, 0.4f);
	glVertex3f(0.630f, 0.430f, 0.0f);
	glVertex3f(0.660f, 0.430f, 0.0f);
	glVertex3f(0.660f, 0.220f, 0.0f);
	glVertex3f(0.630f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.660f, 0.430f, 0.0f);
	glVertex3f(0.660f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.2f, 0.4f);
	glVertex3f(0.690f, 0.430f, 0.0f);
	glVertex3f(0.660f, 0.430f, 0.0f);
	glVertex3f(0.660f, 0.220f, 0.0f);
	glVertex3f(0.690f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.430f, 0.0f);
	glVertex3f(0.690f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);



	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.2f, 0.4f);
	glVertex3f(0.690f, 0.430f, 0.0f);
	glVertex3f(0.720f, 0.430f, 0.0f);
	glVertex3f(0.720f, 0.220f, 0.0f);
	glVertex3f(0.690f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.720f, 0.430f, 0.0f);
	glVertex3f(0.720f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.380f, 0.0f);
	glVertex3f(0.720f, 0.380f, 0.0f);
	glVertex3f(0.720f, 0.220f, 0.0f);
	glVertex3f(0.750f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.380f, 0.0f);
	glVertex3f(0.750f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.380f, 0.0f);
	glVertex3f(0.780f, 0.380f, 0.0f);
	glVertex3f(0.780f, 0.220f, 0.0f);
	glVertex3f(0.750f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.780f, 0.380f, 0.0f);
	glVertex3f(0.780f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.380f, 0.0f);
	glVertex3f(0.780f, 0.380f, 0.0f);
	glVertex3f(0.780f, 0.220f, 0.0f);
	glVertex3f(0.810f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.380f, 0.0f);
	glVertex3f(0.810f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 0.0f);
	glVertex3f(0.810f, 0.430f, 0.0f);
	glVertex3f(0.840f, 0.430f, 0.0f);
	glVertex3f(0.840f, 0.220f, 0.0f);
	glVertex3f(0.810f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.840f, 0.430f, 0.0f);
	glVertex3f(0.840f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 0.0f);
	glVertex3f(0.870f, 0.430f, 0.0f);
	glVertex3f(0.840f, 0.430f, 0.0f);
	glVertex3f(0.840f, 0.220f, 0.0f);
	glVertex3f(0.870f, 0.220f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.430f, 0.0f);
	glVertex3f(0.870f, 0.220f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 0.0f);
	glVertex3f(0.870f, 0.430f, 0.0f);
	glVertex3f(0.900f, 0.430f, 0.0f);
	glVertex3f(0.900f, 0.220f, 0.0f);
	glVertex3f(0.870f, 0.220f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 1.0f);
	glVertex3f(0.660f, 0.0f, 0.0f);
	glVertex3f(0.630f, 0.0f, 0.0f);
	glVertex3f(0.630f, 0.200f, 0.0f);
	glVertex3f(0.660f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.660f, 0.200f, 0.0f);
	glVertex3f(0.660f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 1.0f);
	glVertex3f(0.660f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.200f, 0.0f);
	glVertex3f(0.660f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.200f, 0.0f);
	glVertex3f(0.690f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.f, 0.4f, 1.0f);
	glVertex3f(0.720f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.0f, 0.0f);
	glVertex3f(0.690f, 0.200f, 0.0f);
	glVertex3f(0.720f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.720f, 0.200f, 0.0f);
	glVertex3f(0.720f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.1f, 0.6f);
	glVertex3f(0.720f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.200f, 0.0f);
	glVertex3f(0.720f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.200f, 0.0f);
	glVertex3f(0.750f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.1f, 0.6f);
	glVertex3f(0.780f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.0f, 0.0f);
	glVertex3f(0.750f, 0.200f, 0.0f);
	glVertex3f(0.780f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.780f, 0.200f, 0.0f);
	glVertex3f(0.780f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.1f, 0.6f);
	glVertex3f(0.780f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.200f, 0.0f);
	glVertex3f(0.780f, 0.200f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.150f, 0.0f);
	glVertex3f(0.810f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 0.6f);
	glVertex3f(0.840f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.0f, 0.0f);
	glVertex3f(0.810f, 0.150f, 0.0f);
	glVertex3f(0.840f, 0.150f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.840f, 0.150f, 0.0f);
	glVertex3f(0.840f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 0.6f);
	glVertex3f(0.840f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.150f, 0.0f);
	glVertex3f(0.840f, 0.150f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.150f, 0.0f);
	glVertex3f(0.870f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 0.6f);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.0f, 0.0f);
	glVertex3f(0.870f, 0.150f, 0.0f);
	glVertex3f(0.9f, 0.150f, 0.0f);
	glEnd();

	// BOOKS IN LEFT CUBORD

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.0f, 1.0f);
	glVertex3f(-0.9f, 0.600f, 0.0f);
	glVertex3f(-0.870f, 0.600f, 0.0f);
	glVertex3f(-0.870f, 0.430f, 0.0f);
	glVertex3f(-0.9f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.600f, 0.0f);
	glVertex3f(-0.870f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.0f, 1.0f);
	glVertex3f(-0.850f, 0.600f, 0.0f);
	glVertex3f(-0.870f, 0.600f, 0.0f);
	glVertex3f(-0.870f, 0.430f, 0.0f);
	glVertex3f(-0.850f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.850f, 0.600f, 0.0f);
	glVertex3f(-0.850f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.0f, 1.0f);
	glVertex3f(-0.850f, 0.600f, 0.0f);
	glVertex3f(-0.820f, 0.600f, 0.0f);
	glVertex3f(-0.820f, 0.430f, 0.0f);
	glVertex3f(-0.850f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.820f, 0.600f, 0.0f);
	glVertex3f(-0.820f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 1.0f);
	glVertex3f(-0.790f, 0.600f, 0.0f);
	glVertex3f(-0.820f, 0.600f, 0.0f);
	glVertex3f(-0.820f, 0.430f, 0.0f);
	glVertex3f(-0.790f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.790f, 0.600f, 0.0f);
	glVertex3f(-0.790f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 1.0f);
	glVertex3f(-0.790f, 0.600f, 0.0f);
	glVertex3f(-0.760f, 0.600f, 0.0f);
	glVertex3f(-0.760f, 0.430f, 0.0f);
	glVertex3f(-0.790f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.760f, 0.600f, 0.0f);
	glVertex3f(-0.760f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.6f, 1.0f);
	glVertex3f(-0.730f, 0.600f, 0.0f);
	glVertex3f(-0.760f, 0.600f, 0.0f);
	glVertex3f(-0.760f, 0.430f, 0.0f);
	glVertex3f(-0.730f, 0.430f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 1.0f);
	glVertex3f(-0.9f, 0.35f, 0.0f);
	glVertex3f(-0.870f, 0.35f, 0.0f);
	glVertex3f(-0.870f, 0.2f, 0.0f);
	glVertex3f(-0.9f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.35f, 0.0f);
	glVertex3f(-0.870f, 0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 1.0f);
	glVertex3f(-0.840f, 0.35f, 0.0f);
	glVertex3f(-0.870f, 0.35f, 0.0f);
	glVertex3f(-0.870f, 0.2f, 0.0f);
	glVertex3f(-0.840f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.840f, 0.35f, 0.0f);
	glVertex3f(-0.840f, 0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.8f, 1.0f);
	glVertex3f(-0.840f, 0.35f, 0.0f);
	glVertex3f(-0.810f, 0.35f, 0.0f);
	glVertex3f(-0.810f, 0.2f, 0.0f);
	glVertex3f(-0.840f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.35f, 0.0f);
	glVertex3f(-0.810f, 0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.6f);
	glVertex3f(-0.780f, 0.35f, 0.0f);
	glVertex3f(-0.810f, 0.35f, 0.0f);
	glVertex3f(-0.810f, 0.2f, 0.0f);
	glVertex3f(-0.780f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.780f, 0.35f, 0.0f);
	glVertex3f(-0.780f, 0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.6f);
	glVertex3f(-0.780f, 0.35f, 0.0f);
	glVertex3f(-0.740f, 0.35f, 0.0f);
	glVertex3f(-0.740f, 0.2f, 0.0f);
	glVertex3f(-0.780f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.740f, 0.35f, 0.0f);
	glVertex3f(-0.740f, 0.2f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.6f);
	glVertex3f(-0.720f, 0.35f, 0.0f);
	glVertex3f(-0.740f, 0.35f, 0.0f);
	glVertex3f(-0.740f, 0.2f, 0.0f);
	glVertex3f(-0.720f, 0.2f, 0.0f);
	glEnd();


	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.4f, 0.4f);
	glVertex3f(-0.9f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.15f, 0.0f);
	glVertex3f(-0.9f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.15f, 0.0f);
	glVertex3f(-0.870f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.4f, 0.4f);
	glVertex3f(-0.840f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.0f, 0.0f);
	glVertex3f(-0.870f, 0.15f, 0.0f);
	glVertex3f(-0.840f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.840f, 0.15f, 0.0f);
	glVertex3f(-0.840f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.4f, 0.4f);
	glVertex3f(-0.840f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.15f, 0.0f);
	glVertex3f(-0.840f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.15f, 0.0f);
	glVertex3f(-0.810f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.4f, 0.4f);
	glVertex3f(-0.780f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.0f, 0.0f);
	glVertex3f(-0.810f, 0.15f, 0.0f);
	glVertex3f(-0.780f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.780f, 0.15f, 0.0f);
	glVertex3f(-0.780f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.4f, 0.4f);
	glVertex3f(-0.780f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.15f, 0.0f);
	glVertex3f(-0.780f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.15f, 0.0f);
	glVertex3f(-0.750f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.4f, 0.4f);
	glVertex3f(-0.720f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.15f, 0.0f);
	glVertex3f(-0.720f, 0.15f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.720f, 0.15f, 0.0f);
	glVertex3f(-0.720f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1.0f);


	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.4f, 0.4f);
	glVertex3f(-0.720f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.0f, 0.0f);
	glVertex3f(-0.750f, 0.15f, 0.0f);
	glVertex3f(-0.720f, 0.15f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.0f);
	glVertex3f(-0.670f, 0.600f, 0.0f);
	glVertex3f(-0.7f, 0.600f, 0.0f);
	glVertex3f(-0.7f, 0.430f, 0.0f);
	glVertex3f(-0.670f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.670f, 0.600f, 0.0f);
	glVertex3f(-0.670f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.0f);
	glVertex3f(-0.670f, 0.600f, 0.0f);
	glVertex3f(-0.640f, 0.600f, 0.0f);
	glVertex3f(-0.640f, 0.430f, 0.0f);
	glVertex3f(-0.670f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.640f, 0.600f, 0.0f);
	glVertex3f(-0.640f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.0f);
	glVertex3f(-0.610f, 0.600f, 0.0f);
	glVertex3f(-0.640f, 0.600f, 0.0f);
	glVertex3f(-0.640f, 0.430f, 0.0f);
	glVertex3f(-0.610f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.610f, 0.600f, 0.0f);
	glVertex3f(-0.610f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.6f);
	glVertex3f(-0.610f, 0.600f, 0.0f);
	glVertex3f(-0.580f, 0.600f, 0.0f);
	glVertex3f(-0.580f, 0.430f, 0.0f);
	glVertex3f(-0.610f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.580f, 0.600f, 0.0f);
	glVertex3f(-0.580f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.6f);
	glVertex3f(-0.550f, 0.600f, 0.0f);
	glVertex3f(-0.580f, 0.600f, 0.0f);
	glVertex3f(-0.580f, 0.430f, 0.0f);
	glVertex3f(-0.550f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.550f, 0.600f, 0.0f);
	glVertex3f(-0.550f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.6f);
	glVertex3f(-0.550f, 0.600f, 0.0f);
	glVertex3f(-0.520f, 0.600f, 0.0f);
	glVertex3f(-0.520f, 0.430f, 0.0f);
	glVertex3f(-0.550f, 0.430f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.520f, 0.600f, 0.0f);
	glVertex3f(-0.520f, 0.430f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.4f, 0.6f);
	glVertex3f(-0.490f, 0.600f, 0.0f);
	glVertex3f(-0.520f, 0.600f, 0.0f);
	glVertex3f(-0.520f, 0.430f, 0.0f);
	glVertex3f(-0.490f, 0.430f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.9f, 0.1f);
	glVertex3f(-0.7f, 0.35f, 0.0f);
	glVertex3f(-0.670f, 0.35f, 0.0f);
	glVertex3f(-0.670f, 0.2f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.670f, 0.35f, 0.0f);
	glVertex3f(-0.670f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.9f, 0.1f);
	glVertex3f(-0.640f, 0.35f, 0.0f);
	glVertex3f(-0.670f, 0.35f, 0.0f);
	glVertex3f(-0.670f, 0.2f, 0.0f);
	glVertex3f(-0.640f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.640f, 0.35f, 0.0f);
	glVertex3f(-0.640f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.9f, 0.1f);
	glVertex3f(-0.640f, 0.35f, 0.0f);
	glVertex3f(-0.610f, 0.35f, 0.0f);
	glVertex3f(-0.610f, 0.2f, 0.0f);
	glVertex3f(-0.640f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.610f, 0.35f, 0.0f);
	glVertex3f(-0.610f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.9f);
	glVertex3f(-0.580f, 0.35f, 0.0f);
	glVertex3f(-0.610f, 0.35f, 0.0f);
	glVertex3f(-0.610f, 0.2f, 0.0f);
	glVertex3f(-0.580f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.580f, 0.35f, 0.0f);
	glVertex3f(-0.580f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.9f);
	glVertex3f(-0.580f, 0.35f, 0.0f);
	glVertex3f(-0.550f, 0.35f, 0.0f);
	glVertex3f(-0.550f, 0.2f, 0.0f);
	glVertex3f(-0.580f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.550f, 0.35f, 0.0f);
	glVertex3f(-0.550f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.9f);
	glVertex3f(-0.520f, 0.35f, 0.0f);
	glVertex3f(-0.550f, 0.35f, 0.0f);
	glVertex3f(-0.550f, 0.2f, 0.0f);
	glVertex3f(-0.520f, 0.2f, 0.0f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.520f, 0.35f, 0.0f);
	glVertex3f(-0.520f, 0.20f, 0.0f);
	glEnd();
	glLineWidth(1.0f);

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.5f, 0.9f);
	glVertex3f(-0.670f, 0.35f, 0.0f);
	glVertex3f(-0.7f, 0.35f, 0.0f);
	glVertex3f(-0.7f, 0.2f, 0.0f);
	glVertex3f(-0.670f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.4f);
	glVertex3f(-0.670f, 0.15f, 0.0f);
	glVertex3f(-0.7f, 0.15f, 0.0f);
	glVertex3f(-0.7f, 0.0f, 0.0f);
	glVertex3f(-0.670f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.670f, 0.15f, 0.0f);
	glVertex3f(-0.670f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.4f);
	glVertex3f(-0.670f, 0.15f, 0.0f);
	glVertex3f(-0.640f, 0.15f, 0.0f);
	glVertex3f(-0.640f, 0.0f, 0.0f);
	glVertex3f(-0.670f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.640f, 0.15f, 0.0f);
	glVertex3f(-0.640f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.4f);
	glVertex3f(-0.610f, 0.15f, 0.0f);
	glVertex3f(-0.640f, 0.15f, 0.0f);
	glVertex3f(-0.640f, 0.0f, 0.0f);
	glVertex3f(-0.610f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.610f, 0.15f, 0.0f);
	glVertex3f(-0.610f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.610f, 0.15f, 0.0f);
	glVertex3f(-0.580f, 0.15f, 0.0f);
	glVertex3f(-0.580f, 0.0f, 0.0f);
	glVertex3f(-0.610f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.580f, 0.15f, 0.0f);
	glVertex3f(-0.580f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.550f, 0.15f, 0.0f);
	glVertex3f(-0.580f, 0.15f, 0.0f);
	glVertex3f(-0.580f, 0.0f, 0.0f);
	glVertex3f(-0.550f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.550f, 0.15f, 0.0f);
	glVertex3f(-0.550f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.550f, 0.15f, 0.0f);
	glVertex3f(-0.520f, 0.15f, 0.0f);
	glVertex3f(-0.520f, 0.0f, 0.0f);
	glVertex3f(-0.550f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.520f, 0.15f, 0.0f);
	glVertex3f(-0.520f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.4f, 0.0f);
	glVertex3f(-0.490f, 0.15f, 0.0f);
	glVertex3f(-0.520f, 0.15f, 0.0f);
	glVertex3f(-0.520f, 0.0f, 0.0f);
	glVertex3f(-0.490f, 0.0f, 0.0f);
	glEnd();

}

// ----------------------------------- Scene Kitchen ------------------------------

void sceneKitchen(void)
{
	//local function declarations
	void Cooking_Girl(void);
	void Gas(void);
	void SteamLine1(void);
	void SteamLine2(void);
	void SteamLine3(void);
	void SteamLine4(void);
	void SteamLine5(void);
	void Fridge(void);

	//code
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -1.1f);
	glBegin(GL_QUADS);                    // floor quad

	glColor3f(0.7f, 0.3f, 0.1f);
	glVertex3f(1.0f, -0.5f, 0.0f);

	glVertex3f(1.0f, -0.3f, 0.0f);

	glVertex3f(-1.0f, -0.3f, 0.0f);

	glVertex3f(-1.0f, -0.5f, 0.0f);

	glEnd();

	glTranslatef(0.0f, 0.0f, -0.4f);
	glBegin(GL_QUADS);                    // table lower quad

	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.36f, -0.21f, 0.0f);

	glVertex3f(-0.99f, -0.21f, 0.0f);

	glVertex3f(-0.99f, -0.86f, 0.0f);

	glVertex3f(-0.36f, -0.86f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                 // table shadow part

	glColor3f(0.39f, 0.39f, 0.39f);
	glVertex3f(-0.36f, -0.18f, 0.0f);

	glVertex3f(-0.99f, -0.18f, 0.0f);

	glVertex3f(-0.99f, -0.21f, 0.0f);

	glVertex3f(-0.36f, -0.21f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // table upper longer quad

	glColor3f(0.66f, 0.66f, 0.66f);
	glVertex3f(-0.26f, -0.134f, 0.0f);

	glVertex3f(-1.0f, -0.134f, 0.0f);

	glVertex3f(-1.0f, -0.18f, 0.0f);

	glVertex3f(-0.26f, -0.18f, 0.0f);

	glEnd();

	glTranslatef(0.0f, 0.0f, -1.0f);

	glBegin(GL_QUADS);                    // left window quad outer

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.62f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.048f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.048f, 0.04f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.62f, 0.04f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // left window quad left door

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.315f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.075f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.075f, 0.075f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.315f, 0.075f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // right window quad outer

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.238f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.666f, 0.91f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.666f, 0.04f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.238f, 0.04f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                    // right window quad left door

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.933f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.693f, 0.885f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.693f, 0.075f, 0.0f);

	glColor3f(0.8f, 1.0f, 1.0f);
	glVertex3f(0.933f, 0.075f, 0.0f);

	glEnd();

	Cooking_Girl();

	glTranslatef(-2.6f, 0.48f, -4.5f);
	Gas();

	glTranslatef(-0.36f, Translate_Line1_Y, 0.0f);
	SteamLine1();

	glTranslatef(0.05f, 0.02f, 0.0f);
	SteamLine2();

	glTranslatef(0.05f, 0.02f, 0.0f);
	SteamLine3();

	glTranslatef(0.01f, 0.02f, 0.0f);
	SteamLine4();

	glEnable(GL_POINT_SMOOTH);

	glLoadIdentity();
	glTranslatef(-0.17f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.6f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(-0.06f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(0.4f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(0.08f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.4f);
	glVertex3f(1.0f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(-0.17f, 0.15f, -3.0f);
	glPointSize(15.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.34f, 0.46f, 0.0f);
	glEnd();

	glLoadIdentity();
	glTranslatef(2.6f, -0.3f, -4.1f);
	Fridge();

	//for clock
	glLoadIdentity();
	glTranslatef(1.0f, -1.2f, -4.0f);
	glScalef(1.5f, 1.5f, 0.0f);
	//glRotatef(80.0f, 0.0f, 1.0f, 0.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	SGK_drawCircle(0.1f, -1.9f, 1.5f, 0.0f, 360.0f);
	glColor3f(0.0f, 0.0f, 1.0f);

	glLineWidth(3.0f);
	glEnable(GL_LINE_SMOOTH);
	SGK_drawHollowCircle(0.102f, -1.9f, 1.5f, 0.0f, 360.0f);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);

	//clock needle
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	// 12 clock
	glVertex3f(-1.9f, 1.58f, 0.0f);
	glVertex3f(-1.9f, 1.60f, 0.0f);
	// 6 clock
	glVertex3f(-1.9f, 1.42f, 0.0f);
	glVertex3f(-1.9f, 1.40f, 0.0f);

	// 3 clock
	glVertex3f(-1.8f, 1.5f, 0.0f);
	glVertex3f(-1.82f, 1.5f, 0.0f);

	// 9 clock
	glVertex3f(-1.98f, 1.5f, 0.0f);
	glVertex3f(-2.0f, 1.5f, 0.0f);

	glEnd();
	//clock needle
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	// hour hand
	glVertex3f(-1.9f, 1.5f, 0.0f);
	glVertex3f(-1.95f, 1.5f, 0.0f);

	// second hand
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.9f, 1.5f, 0.0f);
	glVertex3f(-1.82f, 1.53f, 0.0f);

	// minute hand
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.9f, 1.5f, 0.0f);
	glVertex3f(-1.83f, 1.48f, 0.0f);
	glEnd();

	glLineWidth(1.0f);

}


void Cooking_Girl(void)
{
	glTranslatef(-0.2f, -0.2f, 0.45f);

	glBegin(GL_QUADS);                      //core body

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(0.09f, 0.48f, 0.0f);


	glVertex3f(-0.11f, 0.48f, 0.0f);


	glVertex3f(-0.11f, 0.27f, 0.0f);

	glVertex3f(0.05f, 0.27f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);              //left arm shoulder part

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(-0.11f, 0.48f, 0.0f);


	glVertex3f(-0.165f, 0.35f, 0.0f);


	glVertex3f(-0.165f, 0.295f, 0.0f);

	glVertex3f(-0.11f, 0.40f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //left arm wrist part

	glColor3f(1.0f, 0.8f, 0.5f);


	glVertex3f(-0.165f, 0.35f, 0.0f);


	glVertex3f(-0.34f, 0.33f, 0.0f);


	glVertex3f(-0.34f, 0.31f, 0.0f);

	glVertex3f(-0.165f, 0.295f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right arm shoulder part

	glColor3f(1.0f, 0.9f, 0.0f);

	glVertex3f(0.09f, 0.48f, 0.0f);


	glVertex3f(0.07f, 0.405f, 0.0f);


	glVertex3f(0.155f, 0.31f, 0.0f);

	glVertex3f(0.21f, 0.32f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right arm wrist part

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.21f, 0.32f, 0.0f);
	glVertex3f(0.155f, 0.31f, 0.0f);
	glVertex3f(0.06f, 0.19f, 0.0f);
	glVertex3f(0.09f, 0.19f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //left leg

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(-0.04f, -0.2f, 0.0f);

	glVertex3f(-0.095f, -0.2f, 0.0f);

	glVertex3f(-0.072f, -0.588f, 0.0f);

	glVertex3f(-0.046f, -0.60f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right leg

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.03f, -0.2f, 0.0f);

	glVertex3f(-0.025f, -0.2f, 0.0f);

	glVertex3f(-0.018f, -0.60f, 0.0f);

	glVertex3f(0.008f, -0.588f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //right foot 

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.008f, -0.588f, 0.0f);

	glVertex3f(-0.018f, -0.60f, 0.0f);

	glVertex3f(0.1f, -0.6352f, 0.0f);

	glVertex3f(0.12f, -0.625f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //left foot 

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(-0.072f, -0.588f, 0.0f);

	glVertex3f(-0.18f, -0.632f, 0.0f);

	glVertex3f(-0.16f, -0.638f, 0.0f);

	glVertex3f(-0.046f, -0.60f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);                      //Skirt part

	glColor3f(0.1f, 0.4f, 0.8f);

	glVertex3f(0.05f, 0.27f, 0.0f);


	glVertex3f(0.066f, 0.236f, 0.0f);


	glVertex3f(0.083f, 0.171f, 0.0f);

	glVertex3f(0.098f, -0.206f, 0.0f);

	glVertex3f(-0.115f, -0.206f, 0.0f);

	glVertex3f(-0.11f, 0.27f, 0.0f);

	glEnd();


	glBegin(GL_LINES);             // right palm   

	float radious_right_palm = 0.028f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_right_palm * cos((angle * M_PI) / 180.0f) + (0.08f);
		float y = radious_right_palm * sin((angle * M_PI) / 180.0f) + (0.20f);



		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(0.08f, 0.20f, 0.0f);


		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glBegin(GL_LINES);             // left palm   

	float radious_left_palm = 0.028f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		float x = radious_left_palm * cos((angle * M_PI) / 180.0f) + (-0.34f);
		float y = radious_left_palm * sin((angle * M_PI) / 180.0f) + (0.325f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(-0.34f, 0.325f, 0.0f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);
	}

	glEnd();

	glBegin(GL_POLYGON);                      //Pan handle

	glColor3f(0.4f, 0.0f, 0.0f);

	glVertex3f(-0.272f, 0.314f, 0.0f);


	glVertex3f(-0.27f, 0.302f, 0.0f);


	glVertex3f(-0.28f, 0.294f, 0.0f);

	glVertex3f(-0.34f, 0.322f, 0.0f);

	glVertex3f(-0.482f, 0.322f, 0.0f);

	glVertex3f(-0.482f, 0.346f, 0.0f);

	glVertex3f(-0.34f, 0.346f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);                      //Pan container

	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-0.484f, 0.35f, 0.0f);


	glVertex3f(-0.484f, 0.3f, 0.0f);


	glVertex3f(-0.5f, 0.275f, 0.0f);

	glVertex3f(-0.525f, 0.26f, 0.0f);

	glVertex3f(-0.73f, 0.26f, 0.0f);

	glVertex3f(-0.76f, 0.275f, 0.0f);

	glVertex3f(-0.775f, 0.3f, 0.0f);

	glVertex3f(-0.775f, 0.35f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);                      //Aprin

	glColor3f(0.5f, 0.9f, 1.0f);

	glVertex3f(-0.02f, 0.265f, 0.0f);


	glVertex3f(0.055f, 0.265f, 0.0f);


	glVertex3f(0.055f, 0.25f, 0.0f);

	glVertex3f(0.045f, 0.25f, 0.0f);

	glVertex3f(0.045f, -0.06f, 0.0f);

	glVertex3f(0.03f, -0.09f, 0.0f);

	glVertex3f(-0.02f, -0.12f, 0.0f);

	glVertex3f(-0.07f, -0.12f, 0.0f);

	glVertex3f(-0.09f, -0.11f, 0.0f);

	glVertex3f(-0.11f, -0.084f, 0.0f);

	glVertex3f(-0.11f, 0.265f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                      //neck

	glColor3f(1.0f, 0.8f, 0.5f);

	glVertex3f(0.008f, 0.556f, 0.0f);


	glVertex3f(0.008f, 0.478f, 0.0f);


	glVertex3f(-0.044f, 0.478f, 0.0f);

	glVertex3f(-0.044f, 0.556f, 0.0f);

	glEnd();

	glBegin(GL_LINES);             // hair circle   

	float radious_hair = 0.078f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		float x = radious_hair * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_hair * sin((angle * M_PI) / 180.0f) + (0.62f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.03f, 0.62f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glBegin(GL_LINES);             // face   

	float radious_Face = 0.078f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		float x = radious_Face * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_Face * sin((angle * M_PI) / 180.0f) + (0.60f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(-0.03f, 0.60f, 0.0f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glBegin(GL_LINES);             // left eye   

	float radious_Left_Eye = 0.009f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{
		float x = radious_Left_Eye * cos((angle * M_PI) / 180.0f) + (-0.07f);
		float y = radious_Left_Eye * sin((angle * M_PI) / 180.0f) + (0.605f);

		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(-0.07f, 0.605f, 0.0f);

		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glBegin(GL_LINES);             // right eye   

	float radious_Right_Eye = 0.009f;

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Right_Eye * cos((angle * M_PI) / 180.0f) + (-0.03f);
		float y = radious_Right_Eye * sin((angle * M_PI) / 180.0f) + (0.605f);

		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(-0.03f, 0.605f, 0.0f);

		glColor3f(0.6f, 0.2f, 0.1f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glBegin(GL_LINES);               // wide smile

	float Radious_Smile = 0.02f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_Smile * cos((angle * M_PI) / 180.0f) + (-0.05f);
		float y = Radious_Smile * sin(angle * M_PI / 180.0f) + (0.575f);

		if (angle >= 180 && angle <= 360)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.05f, 0.575f, 0.0f);


			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(x, y, 0.0f);
		}

	}

	glEnd();

	glBegin(GL_LINES);               // ear

	float Radious_ear = 0.04f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_ear * cos((angle * M_PI) / 180.0f) + (0.02f);
		float y = Radious_ear * sin(angle * M_PI / 180.0f) + (0.6f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(0.02f, 0.6f, 0.0f);

		glColor3f(1.0f, 0.8f, 0.5f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

	glEnable(GL_POINT_SMOOTH);              //kanatle

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.04f, 0.57f, 0.0f);
	glEnd();

	glTranslatef(0.015f, 0.1, -0.25f);

	glBegin(GL_QUADS);     //hair left side

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.075f, 0.613f, 0.0f);

	glVertex3f(-0.09f, 0.647f, 0.0f);

	glVertex3f(-0.14f, 0.616f, 0.0f);

	glVertex3f(-0.155f, 0.582f, 0.0f);

	glEnd();

	glTranslatef(0.005f, -0.1, -0.0f);

	glBegin(GL_POLYGON);                      //hair right side

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.09f, 0.736f, 0.0f);


	glVertex3f(-0.082f, 0.714f, 0.0f);


	glVertex3f(-0.07f, 0.696f, 0.0f);

	glVertex3f(-0.05f, 0.68f, 0.0f);

	glVertex3f(-0.03f, 0.67f, 0.0f);
	glVertex3f(0.006f, 0.664f, 0.0f);

	glVertex3f(0.0f, 0.704f, 0.0f);

	glVertex3f(-0.05f, 0.742f, 0.0f);

	glEnd();

	glTranslatef(0.001f, -0.055, -0.0f);
	glBegin(GL_LINES);               // hair juuda

	float Radious_hair = 0.045f;
	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = Radious_hair * cos((angle * M_PI) / 180.0f) + (0.04f);
		float y = Radious_hair * sin(angle * M_PI / 180.0f) + (0.75f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.04f, 0.75f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);

	}

	glEnd();

}

void Gas(void)
{
	glBegin(GL_POLYGON);                //central polygon

	glColor3f(33.0f / 255.0f, 44.0f / 255.0f, 60.0f / 255.0f);

	glVertex3f(0.96f, -0.78f, 0.0f);

	glVertex3f(0.99f, -0.74f, 0.0f);

	glColor3f(212.0f / 255.0f, 212.0f / 255.0f, 212.0f / 255.0f);

	glVertex3f(0.9f, -0.3f, 0.0f);

	glVertex3f(0.85f, -0.26f, 0.0f);

	glVertex3f(-0.85f, -0.26f, 0.0f);

	glVertex3f(-0.9f, -0.3f, 0.0f);

	glColor3f(33.0f / 255.0f, 44.0f / 255.0f, 60.0f / 255.0f);

	glVertex3f(-0.99f, -0.74f, 0.0f);

	glVertex3f(-0.96f, -0.78f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                //left leg 

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.63f, -0.78f, 0.0f);


	glVertex3f(-0.87f, -0.78f, 0.0f);


	glVertex3f(-0.87f, -0.84f, 0.0f);

	glVertex3f(-0.63f, -0.84f, 0.0f);



	glEnd();

	glBegin(GL_QUADS);                //right leg 

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.63f, -0.78f, 0.0f);


	glVertex3f(0.87f, -0.78f, 0.0f);


	glVertex3f(0.87f, -0.84f, 0.0f);

	glVertex3f(0.63f, -0.84f, 0.0f);



	glEnd();

	glBegin(GL_QUADS);                //central quad

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.54f, -0.21f, 0.0f);


	glVertex3f(-0.54f, -0.21f, 0.0f);


	glVertex3f(-0.54f, -0.26f, 0.0f);

	glVertex3f(0.54f, -0.26f, 0.0f);



	glEnd();


	glBegin(GL_QUADS);                //burner central quad

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.04f, -0.07f, 0.0f);

	glVertex3f(-0.04f, -0.07f, 0.0f);

	glVertex3f(-0.04f, -0.26f, 0.0f);

	glVertex3f(0.04f, -0.26f, 0.0f);


	glEnd();

	glBegin(GL_POLYGON);                //burner left polygon

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-0.6f, -0.07f, 0.0f);

	glVertex3f(-0.7f, -0.18f, 0.0f);

	glVertex3f(-0.7f, -0.26f, 0.0f);

	glVertex3f(-0.63f, -0.26f, 0.0f);

	glVertex3f(-0.63f, -0.2f, 0.0f);

	glVertex3f(-0.58f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.15f, 0.0f);

	glVertex3f(-0.2f, -0.07f, 0.0f);

	glEnd();



	glBegin(GL_POLYGON);                //burner right polygon

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.6f, -0.07f, 0.0f);

	glVertex3f(0.7f, -0.18f, 0.0f);

	glVertex3f(0.7f, -0.26f, 0.0f);

	glVertex3f(0.63f, -0.26f, 0.0f);


	glVertex3f(0.63f, -0.2f, 0.0f);


	glVertex3f(0.58f, -0.15f, 0.0f);

	glVertex3f(0.2f, -0.15f, 0.0f);

	glVertex3f(0.2f, -0.07f, 0.0f);

	glEnd();

	GLfloat x1 = -0.45f;
	GLfloat y1 = -0.2f;
	GLfloat x2 = -0.475f;
	GLfloat y2 = -0.15f;
	GLfloat x3 = -0.5f;
	GLfloat y3 = -0.2f;
	while (x1 <= 0.5f)
	{
		glBegin(GL_TRIANGLES);
		glColor3f((251.0f / 256.0f), (183.0f / 256.0f), (65.0f / 256.0f));
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x3, y3, 0.0f);
		glEnd();

		x1 = x1 + 0.05;
		x2 = x2 + 0.05f;
		x3 = x3 + 0.05;

	}


}

void SteamLine1(void)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.00f, 0.0f, 0.0f);
	glVertex3f(0.00f, 0.3f, 0.0f);

	glEnd();
}

void SteamLine2(void)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.13f, 0.2f, 0.0f);
	glVertex3f(0.13f, 0.5f, 0.0f);

	glEnd();
}

void SteamLine3(void)
{
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.0f, 0.0f);
	glVertex3f(0.3f, 0.3f, 0.0f);

	glEnd();
}

void SteamLine4(void)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.0f, 0.0f);
	glVertex3f(0.6f, 0.3f, 0.0f);

	glEnd();
}


void Fridge(void)
{
	glBegin(GL_QUADS);              //big quad fridge

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);


	glVertex3f(-0.5f, 1.0f, 0.0f);


	glVertex3f(-0.5f, -0.9f, 0.0f);

	glVertex3f(0.5f, -0.9f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               //upper handle

	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.35f, 0.95f, 0.0f);


	glVertex3f(-0.4f, 0.95f, 0.0f);


	glVertex3f(-0.4f, 0.35f, 0.0f);

	glVertex3f(-0.35f, 0.35f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                  //lower handle

	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.35f, 0.1f, 0.0f);


	glVertex3f(-0.4f, 0.1f, 0.0f);


	glVertex3f(-0.4f, -0.7f, 0.0f);

	glVertex3f(-0.35f, -0.7f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                     //left stand quad

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.90f, 0.0f);


	glVertex3f(-0.4f, -0.90f, 0.0f);


	glVertex3f(-0.4f, -0.98f, 0.0f);

	glVertex3f(-0.3f, -0.98f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                     //left stand quad

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.90f, 0.0f);


	glVertex3f(0.3f, -0.90f, 0.0f);


	glVertex3f(0.3f, -0.98f, 0.0f);

	glVertex3f(0.4f, -0.98f, 0.0f);

	glEnd();




	glLineWidth(7.0f);
	glBegin(GL_LINES);
	glColor3f(0.6f, 0.2f, 0.1f);

	glVertex3f(-0.5f, 0.256f, 0.0f);
	glVertex3f(0.5f, 0.256f, 0.0f);

	glEnd();
	glLineWidth(1.0f);

}

// --------------------------------- Scene Study ---------------------------------

void sceneStudy(void)
{
	displayScene();

}
