//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit
#define _USE_MATH_DEFINE
#include<math.h>

//OpenGL Header
#include<gl/glew.h> // this must be before gl/GL.h 
#include<gl/GL.h>

#include"vmath.h"
using namespace vmath;

#include"OGL.h"
#include"Sphere.h"

//OpenGl Related Global variable
HDC ghdc = NULL;
HGLRC ghrc = NULL;

//Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // initialization of struct => this work on all type (if we want to initialize all value to 0)

BOOL gbFullscreen = FALSE;
HWND ghwnd = NULL; // g = global handle of window
BOOL gbActive = FALSE; 

FILE *gpFILE = NULL;

// OpenGL related variable 
GLuint shaderProgramObject_PF = 0;

// for sphere
GLuint vao_sphere = 0;
GLuint vbo_positionSphere = 0;
GLuint vbo_normalSphere = 0;
GLuint vbo_elementSphere = 0;
GLuint vbo_texureSphere = 0;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gNumVertices;
GLuint gNumElements;


GLuint modelMatrixUniform = 0;
GLuint viewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;

GLuint lightAmbientUniform[3];
GLuint lightDiffuseUniform[3];
GLuint lightSpecularUniform[3];
GLuint lightPositionUniform[3];

GLuint materialDiffuseUniform = 0;  
GLuint materialAmbientUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;

GLuint keyPressedUniform = 0;

BOOL bLightingEnable = FALSE;

GLfloat lightAngleZero = 0.0f;
GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 0.0f;

struct LIGHT
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct LIGHT light[1];

struct MATERIAL
{
	float mAmbient;
	float mDiffuse;
	float mSpecular;
	float mShininess;
};

struct MATERIAL material;

GLfloat materialAmbient[4];
GLfloat materialDiffuse[4];
GLfloat materialSpecular[4];
GLfloat materialShininess = 128.0f;

// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix;

GLuint currentWinWidth = 0;
GLuint currentWinHeight = 0;

GLchar rotationAxis = 'd';

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_NORMAL
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
	if (fopen_s(&gpFILE, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log file cannot be open"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully\n");
	fprintf(gpFILE, "\n==============================================================================\n");

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
		currentWinWidth = LOWORD(lParam);
		currentWinHeight = HIWORD(lParam);
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
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
	case WM_CHAR:
	{
		switch (LOWORD(wParam))
		{
		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
			break;
		case 'L':
		case 'l':
			if (bLightingEnable == FALSE)
			{
				bLightingEnable = TRUE;
			}
			else
			{
				bLightingEnable = FALSE;
			}
			break;
		case 'X':
		case 'x':
			rotationAxis = 'x';
			break;
		case 'Y':
		case 'y':
			rotationAxis = 'y';
			break;
		case 'Z':
		case 'z':
			rotationAxis = 'z';
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
	void printGLInfo(void);
	void resize(int, int);
	void uninitialize(void);

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
		fprintf(gpFILE, "wglMakeCurrent() Failed.\n");
		return(-5);
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInit() Failed to initialize GLEW\n");
		return(-6);
	}

	// print GL Info 
	printGLInfo();

	// Vertex Shader per fragment light
	{
		// step 1 : vertex shader code
		const GLchar *vertexShaderSourceCode =
			"#version 460 core" \
			"\n" \
			"in vec4 aPosition;" \
			"in vec3 aNormal;" \
			"uniform mat4 uModelMatrix;" \
			"uniform mat4 uViewMatrix;" \
			"uniform mat4 uProjectionMatrix;" \
			"uniform vec4 uLightPosition[1];" \
			"uniform int uKeyPressed;" \
			"out vec3 oTransformedNormals;" \
			"out vec3 oLightDirection[1];" \
			"out vec3 oViewerVector;" \
			"void main(void)" \
			"{" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
			"oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;" \
			"oLightDirection[0] = vec3(uLightPosition[0]-eyeCoordinates);" \
			"oViewerVector = -eyeCoordinates.xyz;" \
			"}"\
			"else" \
			"{" \
			"oTransformedNormals = vec3(0.0,0.0,0.0);" \
			"oLightDirection[0] = vec3(0.0,0.0,0.0);" \
			"oViewerVector = vec3(0.0,0.0,0.0);" \
			"}" \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;" \
			"}";

		// step 2 : create vertex shader object
		GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

		// step 3 : give vertex source code to openGL
		glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

		// step 4 : let OpenGL compile vertex shader
		glCompileShader(vertexShaderObject);

		// step 5 :
		GLint status = 0;
		GLint infoLogLength = 0;
		GLchar *szInfoLog = NULL;

		// step 5 a:
		glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

		// step 5 b:
		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 5 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{

					// step 5 d:
					glGetShaderInfoLog(vertexShaderObject, infoLogLength + 1, NULL, szInfoLog);

					// step 5 e:
					fprintf(gpFILE, "Vertex shader compilation error log : %s\n", szInfoLog);

					// step 5 f:
					free(szInfoLog);

					szInfoLog = NULL;

				}
			}


			// step 5 g:
			uninitialize();
		}

		// Fragment shader 
		// step 6 : write the fragement shader code
		const GLchar *fragmentShaderCode =
			"#version 460 core" \
			"\n" \
			"in vec3 oTransformedNormals;" \
			"in vec3 oLightDirection[1];" \
			"in vec3 oViewerVector;" \
			"uniform vec3 uLightAmbient[1];" \
			"uniform vec3 uLightDiffuse[1];" \
			"uniform vec3 uLightSpecular[1];" \
			"uniform vec3 uMaterialAmbient;" \
			"uniform vec3 uMaterialDiffuse;" \
			"uniform vec3 uMaterialSpecular;" \
			"uniform float uMaterialShineness;" \
			"uniform int uKeyPressed;" \
			"out vec4 FragColor;" \
			"void main(void)" \
			"{" \
			"vec3 phongADSLight;" \
			"vec3 normalizeLightDirection[1];" \
			"vec3 lightAmbient[1];" \
			"vec3 lightDirection[1];" \
			"vec3 lightDiffuse[1];" \
			"vec3 reflectionVector[1];" \
			"vec3 lightSpecular[1];" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec3 normalizeTransformedNormals = normalize(oTransformedNormals);" \
			"normalizeLightDirection[0] = normalize(oLightDirection[0]);" \
			"vec3 normalizeViewerVector = normalize(oViewerVector);" \
			"for(int i = 0; i<1; i++)" \
			"{" \
			"lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;" \
			"lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(normalizeLightDirection[i],normalizeTransformedNormals),0.0);" \
			"reflectionVector[i] = reflect(-normalizeLightDirection[i],normalizeTransformedNormals);" \
			"lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],normalizeViewerVector),0.0),uMaterialShineness);" \
			"phongADSLight =  phongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];" \
			"}"\
			"FragColor = vec4(phongADSLight,1.0);" \
			"}" \
			"else" \
			"{" \
			"FragColor = vec4(1.0,1.0,1.0,1.0);" \
			"}" \
			"}";

		// step 7 : create fragment shader object
		GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

		// step 8 : give fragment shader source code to OpenGL
		glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderCode, NULL);

		// step 9 : let OpenGL compile fragment shader
		glCompileShader(fragmentShaderObject);

		// step 10 : 
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 10 a:
		glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

		// step 10 b:
		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 10 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
				if (szInfoLog != NULL)
				{
					// step 10 d:
					glGetShaderInfoLog(fragmentShaderObject, infoLogLength + 1, NULL, szInfoLog);

					// step 10 e:
					fprintf(gpFILE, "fragmenet shader compilation log : %s\n", szInfoLog);

					// step 10 f:
					free(szInfoLog);

					szInfoLog = NULL;

				}

			}

			// step 10 g:
			uninitialize();
		}

		// shader program
		// step 11 : create shader program
		shaderProgramObject_PF = glCreateProgram();

		// step 12 : attach shader to this program
		glAttachShader(shaderProgramObject_PF, vertexShaderObject);
		glAttachShader(shaderProgramObject_PF, fragmentShaderObject);

		// step 13 : bind attribute location with the shader program object
		glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_POSITION, "aPosition");

		glBindAttribLocation(shaderProgramObject_PF, AMC_ATTRIBUTE_NORMAL, "aNormal");

		// step 14 : link the shader program
		glLinkProgram(shaderProgramObject_PF);

		// step 15 : linking error check
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 15 a : get linking status 
		glGetProgramiv(shaderProgramObject_PF, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			// step 15 b :
			glGetProgramiv(shaderProgramObject_PF, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 15 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{
					// step 15 d:
					glGetProgramInfoLog(shaderProgramObject_PF, infoLogLength + 1, NULL, szInfoLog);

					// step 15 e:
					fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

					// step 15 f:
					free(szInfoLog);

					szInfoLog = NULL;


				}
			}

			// step 15 e:
			uninitialize();
		}

		// get shader uniform location

		modelMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uModelMatrix");
		viewMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uViewMatrix");
		projectionMatrixUniform = glGetUniformLocation(shaderProgramObject_PF, "uProjectionMatrix");
		lightAmbientUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightAmbient[0]");
		lightDiffuseUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightDiffuse[0]");
		lightSpecularUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightSpecular[0]");
		lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject_PF, "uLightPosition[0]");

		materialAmbientUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialAmbient");
		materialDiffuseUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialDiffuse");
		materialSpecularUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialSpecular");
		materialShininessUniform = glGetUniformLocation(shaderProgramObject_PF, "uMaterialShineness");
		keyPressedUniform = glGetUniformLocation(shaderProgramObject_PF, "uKeyPressed");
	}

	// step 16: declare position and color array 

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// for sphere
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_sphere);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_sphere);

	// position vbo
	glGenBuffers(1, &vbo_positionSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &vbo_normalSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texture vbo
	glGenBuffers(1, &vbo_texureSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texureSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_textures), sphere_textures, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &vbo_elementSphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	glBindVertexArray(0);

	//for enable depth
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

	// initialize orthographic projection matrix 
	perspectiveProjectionMatrix = vmath::mat4::identity();

	light[0].ambient = vec3(0.0f, 0.0f, 0.0f);

	light[0].diffuse = vec3(1.0, 1.0, 1.0);

	light[0].specular = vec3(1.0f, 1.0, 1.0);

	light[0].position = vec4(0.0f, 0.0, 5.0,1.0f);

	resize(WIN_WIDTH, WIN_HEIGHT);


	return(0);
}

void printGLInfo(void)
{
	//variable declaration
	GLint numExtension;
	GLint i; // for loop counter

	// code
	fprintf(gpFILE, "OpenGL Vender :	%s\n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Renderer :	%s\n", glGetString(GL_RENDERER));
	fprintf(gpFILE, "OpenGL Version :	%s\n", glGetString(GL_VERSION));
	fprintf(gpFILE, "OpenGL GLSL(Graphic Library Shading Language) Version :	%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	fprintf(gpFILE, "\n==============================================================================\n");

	// Listing of Supported Extension
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtension);

	for (i = 0; i < numExtension; i++)
	{
		fprintf(gpFILE, "\t%s\n", glGetStringi(GL_EXTENSIONS, i));
	}

	fprintf(gpFILE, "\n==============================================================================\n");
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void display(void)
{
	// variable declaration

	GLfloat row1_Y, row2_Y, row3_Y, row4_Y, row5_Y, row6_Y;
	GLfloat col1_X, col2_X, col3_X, col4_X;

	GLfloat difference_X = currentWinWidth / 6; // for column
	GLfloat difference_Y = currentWinHeight / 6; // for rows

	col1_X = 0.0f;
	col2_X = col1_X + difference_X;
	col3_X = col2_X + difference_X;
	col4_X = col3_X + difference_X;

	row1_Y = currentWinHeight - difference_Y;
	row2_Y = row1_Y - difference_Y;
	row3_Y = row2_Y - difference_Y;
	row4_Y = row3_Y - difference_Y;
	row5_Y = row4_Y - difference_Y;
	row6_Y = row5_Y - difference_Y;

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// step 1 : use shader program
	glUseProgram(shaderProgramObject_PF);

	// sphere
	{
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform, 1);

			glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform[0], 1, light[0].position);

			glUniform3fv(materialAmbientUniform, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform, 1, materialSpecular);
			glUniform1f(materialShininessUniform, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);
	}

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

	glViewport(col1_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	{
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform, 1);

			glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform[0], 1, light[0].position);

			glUniform3fv(materialAmbientUniform, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform, 1, materialSpecular);
			glUniform1f(materialShininessUniform, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);
	}

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

	glViewport(col1_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	{
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform, 1);

			glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform[0], 1, light[0].position);

			glUniform3fv(materialAmbientUniform, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform, 1, materialSpecular);
			glUniform1f(materialShininessUniform, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);
	}

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

	glViewport(col1_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

	{
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform, 1);

			glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform[0], 1, light[0].position);

			glUniform3fv(materialAmbientUniform, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform, 1, materialSpecular);
			glUniform1f(materialShininessUniform, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);
	}

	{
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

		glViewport(col1_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col1_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col1_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col2_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col3_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row1_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row2_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row3_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row4_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row5_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}

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

		glViewport(col4_X, row6_Y, (GLsizei)difference_X, (GLsizei)difference_Y);

		{
			// Transformation
			mat4 translationMatrix = mat4::identity();
			translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

			mat4 modelMatrix = mat4::identity();
			modelMatrix = translationMatrix;

			mat4 viewMatrix = mat4::identity();

			// push above mvp(model view projection) into vertex shader's mvp uniform
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

			glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

			glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			// step 2 : bind with VAO(vertex array object)
			// *** bind vao ***
			glBindVertexArray(vao_sphere);

			if (bLightingEnable == TRUE)
			{
				glUniform1i(keyPressedUniform, 1);

				glUniform3fv(lightAmbientUniform[0], 1, light[0].ambient);
				glUniform3fv(lightDiffuseUniform[0], 1, light[0].diffuse);
				glUniform3fv(lightSpecularUniform[0], 1, light[0].specular);
				glUniform4fv(lightPositionUniform[0], 1, light[0].position);

				glUniform3fv(materialAmbientUniform, 1, materialAmbient);
				glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
				glUniform3fv(materialSpecularUniform, 1, materialSpecular);
				glUniform1f(materialShininessUniform, materialShininess);

			}
			else
			{
				glUniform1i(keyPressedUniform, 0);
			}

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

			// *** unbind vao ***
			glBindVertexArray(0);
		}
	}

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	//code
	// 

	if (bLightingEnable)
	{
		if (rotationAxis == 'x')
		{
			// x Axis
			light[0].position[0] = 0.0f;
			light[0].position[1] = (GLfloat)5.0f * sin(lightAngleZero);
			light[0].position[2] = (GLfloat)5.0f * cos(lightAngleZero);
			light[0].position[3] = 1.0f;


			lightAngleZero = lightAngleZero + 0.05f;
			if (lightAngleZero >= 360.0f)
			{
				lightAngleZero = lightAngleZero - 360.0f;
			}
		}
		

		if (rotationAxis == 'y')
		{
			light[0].position[0] = (GLfloat)5.0f * sin(lightAngleOne);
			light[0].position[1] = 0.0f;
			light[0].position[2] = (GLfloat)5.0f * cos(lightAngleOne);
			light[0].position[3] = 1.0f;


			lightAngleOne = lightAngleOne + 0.05f;
			if (lightAngleOne >= 360.0f)
			{
				lightAngleOne = lightAngleOne - 360.0f;
			}
		}

		if (rotationAxis == 'z')
		{
			light[0].position[0] = (GLfloat)5.0f * sin(lightAngleTwo);
			light[0].position[1] = (GLfloat)5.0f * cos(lightAngleTwo);
			light[0].position[2] = 0.0f;
			light[0].position[3] = 1.0f;

			lightAngleTwo = lightAngleTwo + 0.05f;
			if (lightAngleTwo >= 360.0f)
			{
				lightAngleTwo = lightAngleTwo - 360.0f;
			}
		}
	}


	
}

void uninitialize(void)
{
	//function declarations
	void ToggleFullScreen(void);

	//code

	// for shader program object 
	if (shaderProgramObject_PF)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject_PF);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject_PF, GL_ATTACHED_SHADERS, &numShader);
			
		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_PF, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject_PF, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = NULL;
				}

				free(pShader);

				pShader = NULL;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject_PF);

		shaderProgramObject_PF = 0;
	}

	// sphere 
	// 
 	// delete vbo for texture
	if (vbo_texureSphere)
	{
		glDeleteBuffers(1, &vbo_texureSphere);
		vbo_texureSphere = 0;
	}
	// delete vbo for element
	if (vbo_elementSphere)
	{
		glDeleteBuffers(1, &vbo_elementSphere);
		vbo_elementSphere = 0;
	}

	// delete vbo for normal
	if (vbo_normalSphere)
	{
		glDeleteBuffers(1, &vbo_normalSphere);
		vbo_normalSphere = 0;
	}
	// delete vbo for position
	if (vbo_positionSphere)
	{
		glDeleteBuffers(1, &vbo_positionSphere);
		vbo_positionSphere = 0;
	}

	// delete vao 
	if (vao_sphere)
	{
		glDeleteVertexArrays(1, &vao_sphere);
		vao_sphere = 0;
	}
	
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
		fprintf(gpFILE, "\n==============================================================================\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}


