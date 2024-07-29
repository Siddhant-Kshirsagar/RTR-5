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

#define FBO_WIDTH 512
#define FBO_HEIGHT 512

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // initialization of struct => this work on all type (if we want to initialize_Cube all value to 0)

BOOL gbFullscreen = FALSE;
HWND ghwnd = NULL; // g = global handle of window
BOOL gbActive = FALSE; 

FILE *gpFILE = NULL;

// OpenGL related variable 

// for cube
GLuint shaderProgramObject_Cube = 0;

GLuint vao_cube = 0;
GLuint vbo_positionCube = 0;
GLuint vbo_colorCube = 0;
GLuint vbo_texCoordCube = 0;

GLuint mvpMatrixUniform_Cube = 0;
// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix_Cube;

GLuint textureSamplerUniform_Cube = 0;

GLfloat cAngle = 0.0f;

// sphere related global variable 
GLuint shaderProgramObject_PV_Sphere = 0;
GLuint shaderProgramObject_PF_Sphere = 0;

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


GLuint modelMatrixUniform_Sphere = 0;
GLuint viewMatrixUniform_Sphere = 0;
GLuint projectionMatrixUniform_Sphere = 0;

GLuint lightAmbientUniform_Sphere[3];
GLuint lightDiffuseUniform_Sphere[3];
GLuint lightSpecularUniform_Sphere[3];
GLuint lightPositionUniform_Sphere[3];

GLuint materialDiffuseUniform_Sphere = 0;
GLuint materialAmbientUniform_Sphere = 0;
GLuint materialSpecularUniform_Sphere = 0;
GLuint materialShininessUniform_Sphere = 0;

GLuint keyPressedUniform_Sphere = 0;

BOOL bLightingEnable = FALSE;

GLfloat lightAngleZero_Sphere = 0.0f;
GLfloat lightAngleOne_Sphere = 0.0f;
GLfloat lightAngleTwo_Sphere = 0.0f;

struct LIGHT
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct LIGHT light[3];

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f,1.0f };
GLfloat materialShininess = 128.0f;

// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix_Sphere;

GLchar chooseShader = 'v'; // char 'v' for per vertex light / char 'f' per fragment light

// FBO(Frame Buffer Object) related variable
GLint winWidth = 0;
GLint winHeight = 0;

GLuint FBO = 0; // Frame Buffer Object
GLuint RBO = 0; // Render Buffer Object
GLuint texture_FBO = 0;
BOOL bFBOResult = FALSE;

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
	int initialize_Cube(void);
	void uninitialize_Cube(void);
	void display_Cube(void);
	void update_Cube(void);

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
	iResult = initialize_Cube();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize_Cube() failed."), TEXT("Error"), MB_OK|MB_ICONERROR);
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
				display_Cube();

				//Update
				update_Cube();
			}

		}
	}

	// Uninitialization
	uninitialize_Cube();

	return((int)msg.wParam);
}

//Callback function defination
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function declarations
	void ToggleFullScreen(void);
	void resize_Cube(int, int);

	switch (iMsg)
	{
	case WM_SETFOCUS: // when window is in focus (window is activate)
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS: // when window is not in focus(window is deactivate)
		gbActive = FALSE;
		break;
	case WM_SIZE:
		resize_Cube(LOWORD(lParam),HIWORD(lParam));
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
				chooseShader = 'v';
				bLightingEnable = FALSE;
			}
			break;

		case 'F':
		case 'f':
			chooseShader = 'f';
			break;
		case 'V':
		case 'v':
			chooseShader = 'v';
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

int initialize_Cube(void)
{
	//function declarations
	void printGLInfo(void);
	void resize_Cube(int, int);
	void uninitialize_Cube(void);
	BOOL createFBO(GLint, GLint);
	BOOL initialize_Sphere(GLint, GLint);
	
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

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInit() Failed to initialize_Cube GLEW\n");
		return(-6);
	}
	
	// print GL Info 
	printGLInfo();

	// Vertex Shader
	// step 1 : vertex shader code
	const GLchar *vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"uniform mat4 uMVPMatrix;" \
		"in vec4 aPosition;" \
		"in vec2 aTexCoord;" \
		"out vec2 oTexCoord;" \
		"void main(void)" \
		"{" \
		"gl_Position= uMVPMatrix * aPosition;" \
		"oTexCoord = aTexCoord;" \
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
			szInfoLog = (GLchar *)malloc(sizeof(GLchar)*(infoLogLength + 1));

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
		uninitialize_Cube();
	}

	// Fragment shader
	// step 6 : write the fragement shader code
	const GLchar *fragmentShaderCode =
		"#version 460 core" \
		"\n" \
		"in vec2 oTexCoord;" \
		"uniform sampler2D uTextureSampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(uTextureSampler,oTexCoord);" \
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
		uninitialize_Cube();
	}

	// shader program
	// step 11 : create shader program
	shaderProgramObject_Cube = glCreateProgram();

	// step 12 : attach shader to this program
	glAttachShader(shaderProgramObject_Cube, vertexShaderObject);
	glAttachShader(shaderProgramObject_Cube, fragmentShaderObject);

	// step 13 : bind attribute location with the shader program object
	glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_POSITION, "aPosition");

	glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

	// step 14 : link the shader program
	glLinkProgram(shaderProgramObject_Cube);

	// step 15 : linking error check
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
		
	// step 15 a : get linking status 
	glGetProgramiv(shaderProgramObject_Cube, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		// step 15 b :
		glGetProgramiv(shaderProgramObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 15 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

			if (szInfoLog != NULL)
			{
				// step 15 d:
				glGetProgramInfoLog(shaderProgramObject_Cube, infoLogLength + 1, NULL, szInfoLog);

				// step 15 e:
				fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

				// step 15 f:
				free(szInfoLog);

				szInfoLog = NULL;


			}
		}

		// step 15 e:
		uninitialize_Cube();
	}

	// get shader uniform location
	mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");

	textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

	// step 16: declare position and color array 

	const GLfloat cube_position[] =
	{
		// front
	 1.0f,  1.0f,  1.0f, // top-right of front
	-1.0f,  1.0f,  1.0f, // top-left of front
	-1.0f, -1.0f,  1.0f, // bottom-left of front
	 1.0f, -1.0f,  1.0f, // bottom-right of front

	// right
	 1.0f,  1.0f, -1.0f, // top-right of right
	 1.0f,  1.0f,  1.0f, // top-left of right
	 1.0f, -1.0f,  1.0f, // bottom-left of right
	 1.0f, -1.0f, -1.0f, // bottom-right of right

	// back
	 1.0f,  1.0f, -1.0f, // top-right of back
	-1.0f,  1.0f, -1.0f, // top-left of back
	-1.0f, -1.0f, -1.0f, // bottom-left of back
	 1.0f, -1.0f, -1.0f, // bottom-right of back

	// left
	-1.0f,  1.0f,  1.0f, // top-right of left
	-1.0f,  1.0f, -1.0f, // top-left of left
	-1.0f, -1.0f, -1.0f, // bottom-left of left
	-1.0f, -1.0f,  1.0f, // bottom-right of left

	// top
	 1.0f,  1.0f, -1.0f, // top-right of top
	-1.0f,  1.0f, -1.0f, // top-left of top
	-1.0f,  1.0f,  1.0f, // bottom-left of top
	 1.0f,  1.0f,  1.0f, // bottom-right of top

	// bottom
	 1.0f, -1.0f,  1.0f, // top-right of bottom
	-1.0f, -1.0f,  1.0f, // top-left of bottom
	-1.0f, -1.0f, -1.0f, // bottom-left of bottom
	 1.0f, -1.0f, -1.0f, // bottom-right of bottom
	};

	const GLfloat cubeTexcoords[] =
	{
		// front
		1.0f, 1.0f, // top-right of front
		0.0f, 1.0f, // top-left of front
		0.0f, 0.0f, // bottom-left of front
		1.0f, 0.0f, // bottom-right of front

		// right
		1.0f, 1.0f, // top-right of right
		0.0f, 1.0f, // top-left of right
		0.0f, 0.0f, // bottom-left of right
		1.0f, 0.0f, // bottom-right of right

		// back
		1.0f, 1.0f, // top-right of back
		0.0f, 1.0f, // top-left of back
		0.0f, 0.0f, // bottom-left of back
		1.0f, 0.0f, // bottom-right of back

		// left
		1.0f, 1.0f, // top-right of left
		0.0f, 1.0f, // top-left of left
		0.0f, 0.0f, // bottom-left of left
		1.0f, 0.0f, // bottom-right of left

		// top
		1.0f, 1.0f, // top-right of top
		0.0f, 1.0f, // top-left of top
		0.0f, 0.0f, // bottom-left of top
		1.0f, 0.0f, // bottom-right of top

		// bottom
		1.0f, 1.0f, // top-right of bottom
		0.0f, 1.0f, // top-left of bottom
		0.0f, 0.0f, // bottom-left of bottom
		1.0f, 0.0f, // bottom-right of bottom
	};

	// for cube
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_cube);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_cube);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionCube);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionCube);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO(Vertex Buffer Object) for texcoord
	glGenBuffers(1, &vbo_texCoordCube);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoordCube);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//for enable depth
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// enable texture
	glEnable(GL_TEXTURE_2D);

	// initialize_Cube orthographic projection matrix 
	perspectiveProjectionMatrix_Cube = vmath::mat4::identity();

	resize_Cube(WIN_WIDTH, WIN_HEIGHT);

	if (createFBO(FBO_WIDTH, FBO_HEIGHT) == TRUE)
	{
		bFBOResult = initialize_Sphere(FBO_WIDTH,FBO_HEIGHT);
	}

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

BOOL initialize_Sphere(GLint textureWidth,GLint textureHeight)
{
	//function declarations
	void resize_Sphere(int, int);
	void uninitialize_Sphere(void);

	// Vertex Shader per vertex light
	{
		// step 1 : vertex shader code
		const GLchar *vertexShaderSourceCode_PV =
			"#version 460 core" \
			"\n" \
			"in vec4 aPosition;" \
			"in vec3 aNormal;" \
			"uniform mat4 uModelMatrix;" \
			"uniform mat4 uViewMatrix;" \
			"uniform mat4 uProjectionMatrix;" \
			"uniform vec3 uLightAmbient[3];" \
			"uniform vec3 uLightDiffuse[3];" \
			"uniform vec3 uLightSpecular[3];" \
			"uniform vec4 uLightPosition[3];" \
			"uniform vec3 uMaterialAmbient;" \
			"uniform vec3 uMaterialDiffuse;" \
			"uniform vec3 uMaterialSpecular;" \
			"uniform float uMaterialShineness;" \
			"uniform int uKeyPressed;" \
			"out vec3 oPhongADSLight;" \
			"void main(void)" \
			"{" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
			"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);" \
			"vec3 lightAmbient[3];" \
			"vec3 lightDirection[3];" \
			"vec3 lightDiffuse[3];" \
			"vec3 reflectionVector[3];" \
			"vec3 lightSpecular[3];" \
			"for(int i = 0; i<3; i++)" \
			"{" \
			"lightAmbient[i] = uLightAmbient[i] * uMaterialAmbient;" \
			"lightDirection[i] = normalize(vec3(uLightPosition[i]-eyeCoordinates));"
			"lightDiffuse[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0);" \
			"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);" \
			"vec3 viewerVector = normalize(-eyeCoordinates.xyz);" \
			"lightSpecular[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector),0.0),uMaterialShineness);" \
			"oPhongADSLight =  oPhongADSLight + lightAmbient[i] + lightDiffuse[i] + lightSpecular[i];" \
			"}"\
			"}"\
			"else" \
			"{" \
			"oPhongADSLight = vec3(0.0,0.0,0.0);" \
			"}" \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;" \
			"}";

		// step 2 : create vertex shader object
		GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

		// step 3 : give vertex source code to openGL
		glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode_PV, NULL);

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
			uninitialize_Sphere();
		}

		// Fragment shader
		// step 6 : write the fragement shader code
		const GLchar *fragmentShaderCode_PV =
			"#version 460 core" \
			"\n" \
			"in vec3 oPhongADSLight;" \
			"uniform int uKeyPressed;" \
			"out vec4 FragColor;" \
			"void main(void)" \
			"{" \
			"if(uKeyPressed == 1)" \
			"{" \
			"FragColor = vec4(oPhongADSLight,1.0);" \
			"}" \
			"else" \
			"{" \
			"FragColor = vec4(1.0,1.0,1.0,1.0);" \
			"}" \
			"}";

		// step 7 : create fragment shader object
		GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

		// step 8 : give fragment shader source code to OpenGL
		glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderCode_PV, NULL);

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
			uninitialize_Sphere();
		}

		// shader program
		// step 11 : create shader program
		shaderProgramObject_PV_Sphere = glCreateProgram();

		// step 12 : attach shader to this program
		glAttachShader(shaderProgramObject_PV_Sphere, vertexShaderObject);
		glAttachShader(shaderProgramObject_PV_Sphere, fragmentShaderObject);

		// step 13 : bind attribute location with the shader program object
		glBindAttribLocation(shaderProgramObject_PV_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

		glBindAttribLocation(shaderProgramObject_PV_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

		// step 14 : link the shader program
		glLinkProgram(shaderProgramObject_PV_Sphere);

		// step 15 : linking error check
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 15 a : get linking status 
		glGetProgramiv(shaderProgramObject_PV_Sphere, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			// step 15 b :
			glGetProgramiv(shaderProgramObject_PV_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 15 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{
					// step 15 d:
					glGetProgramInfoLog(shaderProgramObject_PV_Sphere, infoLogLength + 1, NULL, szInfoLog);

					// step 15 e:
					fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

					// step 15 f:
					free(szInfoLog);

					szInfoLog = NULL;


				}
			}

			// step 15 e:
			uninitialize_Sphere();
		}

		// get shader uniform location

		modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uModelMatrix");
		viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uViewMatrix");
		projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uProjectionMatrix");

		lightAmbientUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[0]");
		lightDiffuseUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[0]");
		lightSpecularUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[0]");
		lightPositionUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[0]");


		lightAmbientUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[1]");
		lightDiffuseUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[1]");
		lightSpecularUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[1]");
		lightPositionUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[1]");

		lightAmbientUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightAmbient[2]");
		lightDiffuseUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightDiffuse[2]");
		lightSpecularUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightSpecular[2]");
		lightPositionUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uLightPosition[2]");

		materialAmbientUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialAmbient");
		materialDiffuseUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialDiffuse");
		materialSpecularUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialSpecular");
		materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uMaterialShineness");
		keyPressedUniform_Sphere = glGetUniformLocation(shaderProgramObject_PV_Sphere, "uKeyPressed");
	}

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
			"uniform vec4 uLightPosition[3];" \
			"uniform int uKeyPressed;" \
			"out vec3 oTransformedNormals;" \
			"out vec3 oLightDirection[3];" \
			"out vec3 oViewerVector;" \
			"void main(void)" \
			"{" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;" \
			"oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;" \
			"oLightDirection[0] = vec3(uLightPosition[0]-eyeCoordinates);" \
			"oLightDirection[1] = vec3(uLightPosition[1]-eyeCoordinates);" \
			"oLightDirection[2] = vec3(uLightPosition[2]-eyeCoordinates);" \
			"oViewerVector = -eyeCoordinates.xyz;" \
			"}"\
			"else" \
			"{" \
			"oTransformedNormals = vec3(0.0,0.0,0.0);" \
			"oLightDirection[0] = vec3(0.0,0.0,0.0);" \
			"oLightDirection[1] = vec3(0.0,0.0,0.0);" \
			"oLightDirection[2] = vec3(0.0,0.0,0.0);" \
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
			uninitialize_Sphere();
		}

		// Fragment shader 
		// step 6 : write the fragement shader code
		const GLchar *fragmentShaderCode =
			"#version 460 core" \
			"\n" \
			"in vec3 oTransformedNormals;" \
			"in vec3 oLightDirection[3];" \
			"in vec3 oViewerVector;" \
			"uniform vec3 uLightAmbient[3];" \
			"uniform vec3 uLightDiffuse[3];" \
			"uniform vec3 uLightSpecular[3];" \
			"uniform vec3 uMaterialAmbient;" \
			"uniform vec3 uMaterialDiffuse;" \
			"uniform vec3 uMaterialSpecular;" \
			"uniform float uMaterialShineness;" \
			"uniform int uKeyPressed;" \
			"out vec4 FragColor;" \
			"void main(void)" \
			"{" \
			"vec3 phongADSLight;" \
			"vec3 normalizeLightDirection[3];" \
			"vec3 lightAmbient[3];" \
			"vec3 lightDirection[3];" \
			"vec3 lightDiffuse[3];" \
			"vec3 reflectionVector[3];" \
			"vec3 lightSpecular[3];" \
			"if(uKeyPressed == 1)" \
			"{" \
			"vec3 normalizeTransformedNormals = normalize(oTransformedNormals);" \
			"normalizeLightDirection[0] = normalize(oLightDirection[0]);" \
			"normalizeLightDirection[1] = normalize(oLightDirection[1]);" \
			"normalizeLightDirection[2] = normalize(oLightDirection[2]);" \
			"vec3 normalizeViewerVector = normalize(oViewerVector);" \
			"for(int i = 0; i<3; i++)" \
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
			uninitialize_Sphere();
		}

		// shader program
		// step 11 : create shader program
		shaderProgramObject_PF_Sphere = glCreateProgram();

		// step 12 : attach shader to this program
		glAttachShader(shaderProgramObject_PF_Sphere, vertexShaderObject);
		glAttachShader(shaderProgramObject_PF_Sphere, fragmentShaderObject);

		// step 13 : bind attribute location with the shader program object
		glBindAttribLocation(shaderProgramObject_PF_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");

		glBindAttribLocation(shaderProgramObject_PF_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

		// step 14 : link the shader program
		glLinkProgram(shaderProgramObject_PF_Sphere);

		// step 15 : linking error check
		status = 0;
		infoLogLength = 0;
		szInfoLog = NULL;

		// step 15 a : get linking status 
		glGetProgramiv(shaderProgramObject_PF_Sphere, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			// step 15 b :
			glGetProgramiv(shaderProgramObject_PF_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0)
			{
				// step 15 c:
				szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

				if (szInfoLog != NULL)
				{
					// step 15 d:
					glGetProgramInfoLog(shaderProgramObject_PF_Sphere, infoLogLength + 1, NULL, szInfoLog);

					// step 15 e:
					fprintf(gpFILE, "Shader program linking error log : %s \n", szInfoLog);

					// step 15 f:
					free(szInfoLog);

					szInfoLog = NULL;


				}
			}

			// step 15 e:
			uninitialize_Sphere();
		}

		// get shader uniform location

		modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uModelMatrix");
		viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uViewMatrix");
		projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uProjectionMatrix");
		lightAmbientUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[0]");
		lightDiffuseUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[0]");
		lightSpecularUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[0]");
		lightPositionUniform_Sphere[0] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[0]");


		lightAmbientUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[1]");
		lightDiffuseUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[1]");
		lightSpecularUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[1]");
		lightPositionUniform_Sphere[1] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[1]");

		lightAmbientUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightAmbient[2]");
		lightDiffuseUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightDiffuse[2]");
		lightSpecularUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightSpecular[2]");
		lightPositionUniform_Sphere[2] = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uLightPosition[2]");


		materialAmbientUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialAmbient");
		materialDiffuseUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialDiffuse");
		materialSpecularUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialSpecular");
		materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uMaterialShineness");
		keyPressedUniform_Sphere = glGetUniformLocation(shaderProgramObject_PF_Sphere, "uKeyPressed");
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize_Sphere orthographic projection matrix 
	perspectiveProjectionMatrix_Sphere = vmath::mat4::identity();

	light[0].ambient = vec3(0.0f, 0.0f, 0.0f);
	light[1].ambient = vec3(0.0f, 0.0f, 0.0f);
	light[2].ambient = vec3(0.0f, 0.0f, 0.0f);

	light[0].diffuse = vec3(1.0, 0.0f, 0.0f);
	light[1].diffuse = vec3(0.0, 1.0f, 0.0f);
	light[2].diffuse = vec3(0.0, 0.0f, 1.0f);

	light[0].specular = vec3(1.0f, 0.0f, 0.0f);
	light[1].specular = vec3(0.0f, 1.0f, 0.0f);
	light[2].specular = vec3(0.0f, 0.0f, 1.0f);

	resize_Sphere(WIN_WIDTH, WIN_HEIGHT);

	return(TRUE);
}

BOOL createFBO(GLint textureWidth, GLint textureHeight)
{
	//variable declaration
	GLint maxRenderBufferSize = 0;

	//code
	// step 1 : check capacity of render buffer
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

	if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
	{
		fprintf(gpFILE, "Texture Size Overflow\n");
		return(FALSE);
	}

	// step 2 : create custom frame buffer
	glGenFramebuffers(1, &FBO);

	// bind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
	// step 3 : create the texture for FBO in which we are going to render sphere
	glGenTextures(1, &texture_FBO);

	glBindTexture(GL_TEXTURE_2D,texture_FBO);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	// step 4 : attach above texture to frame buffer at default color attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_FBO, 0);

	// step 5 : now create render buffer to hold depth of custom FBO
	// generate render buffer
	glGenRenderbuffers(1, &RBO);

	// bind render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	// set storage of above render buffer of texture size for depth
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

	// step 6 : attach above depth related render buffer to FBO(Frame Buffer Object) as depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// step 7 : check the frame buffer status whether successfull or not 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFILE, "Frame Buffer status is not complete\n");
		return(FALSE);
	}

	// step 8 : unbind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return(TRUE);
}

void resize_Cube(int width, int height)
{
	//code
	if (height <= 0)
	{
		height = 1;
	}

	winWidth = width;
	winHeight = height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjectionMatrix_Cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void resize_Sphere(int width, int height)
{
	//code
	if (height <= 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}


void display_Cube(void)
{
	// function declaration
	void display_Sphere(GLint, GLint);
	void update_Sphere(void);
	void resize_Cube(int, int);

	// render FBO(Frame Buffer Object) scene
	if (bFBOResult == TRUE)
	{
		display_Sphere(FBO_WIDTH, FBO_HEIGHT);

		update_Sphere();
	}
	// call resize_Cube() again to compenset the change by display_Sphere
	resize_Cube(winWidth, winHeight);

	// reset color to white to compenset the change by display_Sphere
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// step 1 : use shader program
	glUseProgram(shaderProgramObject_Cube);

	// cube
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	
	// scale matrix
	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	// rotate matrix
	mat4 rotationMatrix1 = mat4::identity();
	rotationMatrix1 = vmath::rotate(cAngle, 1.0f, 0.0f, 0.0f);

	mat4 rotationMatrix2 = mat4::identity();
	rotationMatrix2 = vmath::rotate(cAngle, 0.0f, 1.0f, 0.0f);

	mat4 rotationMatrix3 = mat4::identity();
	rotationMatrix3 = vmath::rotate(cAngle, 0.0f, 0.0f, 1.0f);

	mat4 rotationMatrix = mat4::identity();
	rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix_Cube * modelViewMatrix;

	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, modelViewProjectionMatrix);

	// texture related code
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture_FBO);

	glUniform1i(textureSamplerUniform_Cube, 0);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_cube);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	// unbind vao 
	glBindVertexArray(0);

	//glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void display_Sphere(GLint textureWidth, GLint textureHeight)
{
	//function declaration
	void resize_Sphere(int, int);

	//code
	// Bind with FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// call resize_Sphere() to change size of sphere side code
	resize_Sphere(textureWidth, textureHeight);

	// set  glClearColor to black to compenset the change done by display Cube
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (chooseShader == 'v')
	{
		// step 1 : use shader program
		glUseProgram(shaderProgramObject_PV_Sphere);

		// sphere
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform_Sphere, 1);

			glUniform3fv(lightAmbientUniform_Sphere[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform_Sphere[0], 1, light[0].position);

			glUniform3fv(lightAmbientUniform_Sphere[1], 1, light[1].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light[1].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[1], 1, light[1].specular);
			glUniform4fv(lightPositionUniform_Sphere[1], 1, light[1].position);

			glUniform3fv(lightAmbientUniform_Sphere[2], 1, light[2].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light[2].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[2], 1, light[2].specular);
			glUniform4fv(lightPositionUniform_Sphere[2], 1, light[2].position);

			glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);
			glUniform1f(materialShininessUniform_Sphere, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform_Sphere, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);

		glUseProgram(0);
	}
	if (chooseShader == 'f')
	{
		// step 1 : use shader program
		glUseProgram(shaderProgramObject_PF_Sphere);

		// sphere
		// Transformation
		mat4 translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);

		mat4 modelMatrix = mat4::identity();
		modelMatrix = translationMatrix;

		mat4 viewMatrix = mat4::identity();

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, GL_FALSE, modelMatrix);

		glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);

		// step 2 : bind with VAO(vertex array object)
		// *** bind vao ***
		glBindVertexArray(vao_sphere);

		if (bLightingEnable == TRUE)
		{
			glUniform1i(keyPressedUniform_Sphere, 1);

			glUniform3fv(lightAmbientUniform_Sphere[0], 1, light[0].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[0], 1, light[0].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[0], 1, light[0].specular);
			glUniform4fv(lightPositionUniform_Sphere[0], 1, light[0].position);

			glUniform3fv(lightAmbientUniform_Sphere[1], 1, light[1].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[1], 1, light[1].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[1], 1, light[1].specular);
			glUniform4fv(lightPositionUniform_Sphere[1], 1, light[1].position);

			glUniform3fv(lightAmbientUniform_Sphere[2], 1, light[2].ambient);
			glUniform3fv(lightDiffuseUniform_Sphere[2], 1, light[2].diffuse);
			glUniform3fv(lightSpecularUniform_Sphere[2], 1, light[2].specular);
			glUniform4fv(lightPositionUniform_Sphere[2], 1, light[2].position);

			glUniform3fv(materialAmbientUniform_Sphere, 1, materialAmbient);
			glUniform3fv(materialDiffuseUniform_Sphere, 1, materialDiffuse);
			glUniform3fv(materialSpecularUniform_Sphere, 1, materialSpecular);
			glUniform1f(materialShininessUniform_Sphere, materialShininess);

		}
		else
		{
			glUniform1i(keyPressedUniform_Sphere, 0);
		}

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elementSphere);

		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);

		glUseProgram(0);
	}

	// unBind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void update_Cube(void)
{
	//code

	//cube rotate
	cAngle = cAngle - 1.0f;
	if (cAngle <= 0.0f)
	{
		cAngle = cAngle + 360.0f;
	}
}

void update_Sphere(void)
{
	//code
	// 

	if (bLightingEnable)
	{
		// animating light zero
		light[0].position[0] = 0.0f;
		light[0].position[1] = (GLfloat)5.0f * sin(lightAngleZero_Sphere);
		light[0].position[2] = (GLfloat)5.0f * cos(lightAngleZero_Sphere);
		light[0].position[3] = 1.0f;


		lightAngleZero_Sphere = lightAngleZero_Sphere + 0.05f;
		if (lightAngleZero_Sphere >= 360.0f)
		{
			lightAngleZero_Sphere = lightAngleZero_Sphere - 360.0f;
		}

		// animating light one
		light[1].position[0] = (GLfloat)5.0f * sin(lightAngleOne_Sphere);
		light[1].position[1] = 0.0f;
		light[1].position[2] = (GLfloat)5.0f * cos(lightAngleOne_Sphere);
		light[1].position[3] = 1.0f;


		lightAngleOne_Sphere = lightAngleOne_Sphere + 0.05f;
		if (lightAngleOne_Sphere >= 360.0f)
		{
			lightAngleOne_Sphere = lightAngleOne_Sphere - 360.0f;
		}

		// animating light two
		light[2].position[0] = (GLfloat)5.0f * sin(lightAngleTwo_Sphere);
		light[2].position[1] = (GLfloat)5.0f * cos(lightAngleTwo_Sphere);
		light[2].position[2] = 0.0f;
		light[2].position[3] = 1.0f;

		lightAngleTwo_Sphere = lightAngleTwo_Sphere + 0.05f;
		if (lightAngleTwo_Sphere >= 360.0f)
		{
			lightAngleTwo_Sphere = lightAngleTwo_Sphere - 360.0f;
		}
	}

}


void uninitialize_Cube(void)
{
	//function declarations
	void ToggleFullScreen(void);
	void uninitialize_Sphere(void);

	//code

	uninitialize_Sphere();

	// for shader program object 
	if (shaderProgramObject_Cube)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject_Cube);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject_Cube, GL_ATTACHED_SHADERS, &numShader);

		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_Cube, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject_Cube, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = NULL;
				}

				free(pShader);

				pShader = NULL;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject_Cube);

		shaderProgramObject_Cube = 0;
	}

	// cube 
	// delete vbo for position
	if (vbo_positionCube)
	{
		glDeleteBuffers(1, &vbo_positionCube);
		vbo_positionCube = 0;
	}

	if (vbo_texCoordCube)
	{
		glDeleteBuffers(1, &vbo_texCoordCube);
		vbo_texCoordCube = 0;
	}

	// delete vao 
	if (vao_cube)
	{
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}

	// If Application is exitting in fullscreen mode
	if (gbFullscreen == TRUE)
	{
		ToggleFullScreen();
		gbFullscreen = FALSE;
	}

	if (texture_FBO)
	{
		glDeleteTextures(1, &texture_FBO);
		texture_FBO = 0;
	}

	if (RBO)
	{
		glDeleteRenderbuffers(1, &RBO);
		RBO = 0;
	}

	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
		FBO = 0;
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

void uninitialize_Sphere(void)
{
	//code
	// for shader program object 
	if (shaderProgramObject_PF_Sphere)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject_PF_Sphere);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject_PF_Sphere, GL_ATTACHED_SHADERS, &numShader);

		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_PF_Sphere, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject_PF_Sphere, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = NULL;
				}

				free(pShader);

				pShader = NULL;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject_PF_Sphere);

		shaderProgramObject_PF_Sphere = 0;
	}

	if (shaderProgramObject_PV_Sphere)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject_PV_Sphere);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject_PV_Sphere, GL_ATTACHED_SHADERS, &numShader);

		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_PV_Sphere, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject_PV_Sphere, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = NULL;
				}

				free(pShader);

				pShader = NULL;
			}
		}

		glUseProgram(0);

		glDeleteProgram(shaderProgramObject_PV_Sphere);

		shaderProgramObject_PV_Sphere = 0;
	}

	// sphere 
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

}




