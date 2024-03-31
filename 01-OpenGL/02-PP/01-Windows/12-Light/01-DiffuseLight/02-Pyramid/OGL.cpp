//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

//OpenGL Header
#include<gl/glew.h> // this must be before gl/GL.h 
#include<gl/GL.h>

#include"vmath.h"
using namespace vmath;

#include"OGL.h"

//OpenGl Related Global variable
HDC ghdc = NULL;
HGLRC ghrc = NULL;

//Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Link With OpenGL Library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")

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
GLuint shaderProgramObject = 0;

// for cube
GLuint vao_pyramid = 0;
GLuint vbo_positionPyramid = 0;
GLuint vbo_normal = 0;

GLuint mvpMatrixUniform = 0;
GLuint modelViewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;
GLuint ldUniform = 0;
GLuint kdUniform = 0; // for material 
GLuint lightPositionUniform = 0;
GLuint keyPressedUniform = 0;

BOOL bLightingEnable = FALSE;
BOOL bAnimationEnable = FALSE;

GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialDiffuse[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,2.0f,1.0f };

// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix;

GLfloat cAngle = 0.0f;

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

				if (bAnimationEnable == TRUE)
				{
					//Update
					update();
				}

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
		case 'A':
		case 'a':
			if (bAnimationEnable == FALSE)
			{
				bAnimationEnable = TRUE;
			}
			else
			{
				bAnimationEnable = FALSE;
			}
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

	// Vertex Shader
	// step 1 : vertex shader code
	const GLchar *vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 aPosition;" \
		"in vec3 aNormal;" \
		"uniform mat4 uModelViewMatrix;" \
		"uniform mat4 uProjectionMatrix;" \
		"uniform vec3 uld;" \
		"uniform vec3 ukd;" \
		"uniform vec4 uLightPosition;" \
		"uniform int uKeyPressed;" \
		"out vec3 oDiffuseLight;" \
		"void main(void)" \
		"{" \
		"if(uKeyPressed == 1)" \
		"{" \
		"vec4 eyePosition = uModelViewMatrix * aPosition;" \
		"mat3 normalMatrix = mat3(transpose(inverse(uModelViewMatrix)));" \
		"vec3 n = normalize(normalMatrix * aNormal);" \
		"vec3 s = normalize(vec3(uLightPosition-eyePosition));" \
		"oDiffuseLight = uld * ukd * dot(s,n);" \
		"}"\
		"else" \
		"{" \
		"oDiffuseLight = vec3(0.0,0.0,0.0);" \
		"}" \
		"gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;" \
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
		"in vec3 oDiffuseLight;" \
		"uniform int uKeyPressed;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"if(uKeyPressed == 1)" \
		"{" \
		"FragColor = vec4(oDiffuseLight,1.0);" \
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
	shaderProgramObject = glCreateProgram();

	// step 12 : attach shader to this program
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// step 13 : bind attribute location with the shader program object
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

	// step 14 : link the shader program
	glLinkProgram(shaderProgramObject);

	// step 15 : linking error check
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
		
	// step 15 a : get linking status 
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		// step 15 b :
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			// step 15 c:
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));

			if (szInfoLog != NULL)
			{
				// step 15 d:
				glGetProgramInfoLog(shaderProgramObject, infoLogLength + 1, NULL, szInfoLog);

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
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

	modelViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelViewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	ldUniform = glGetUniformLocation(shaderProgramObject, "uld");
	kdUniform = glGetUniformLocation(shaderProgramObject, "ukd");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
	keyPressedUniform = glGetUniformLocation(shaderProgramObject, "uKeyPressed");


	// step 16: declare position and color array 
	const GLfloat pyramid_position[] =
	{
		// front
		0.0f,  1.0f,  0.0f, // front-top
	   -1.0f, -1.0f,  1.0f, // front-left
		1.0f, -1.0f,  1.0f, // front-right

		// right
		0.0f,  1.0f,  0.0f, // right-top
		1.0f, -1.0f,  1.0f, // right-left
		1.0f, -1.0f, -1.0f, // right-right

		// back
		0.0f,  1.0f,  0.0f, // back-top
		1.0f, -1.0f, -1.0f, // back-left
	   -1.0f, -1.0f, -1.0f, // back-right

		// left
		0.0f,  1.0f,  0.0f, // left-top
	   -1.0f, -1.0f, -1.0f, // left-left
	   -1.0f, -1.0f,  1.0f, // left-right
	};

	GLfloat pyramid_Normals[] =
	{
		// front
		0.000000f, 0.447214f,  0.894427f, // front-top
		0.000000f, 0.447214f,  0.894427f, // front-left
		0.000000f, 0.447214f,  0.894427f, // front-right

		// right			    
		0.894427f, 0.447214f,  0.000000f, // right-top
		0.894427f, 0.447214f,  0.000000f, // right-left
		0.894427f, 0.447214f,  0.000000f, // right-right

		// back
		0.000000f, 0.447214f, -0.894427f, // back-top
		0.000000f, 0.447214f, -0.894427f, // back-left
		0.000000f, 0.447214f, -0.894427f, // back-right

		// left
	   -0.894427f, 0.447214f,  0.000000f, // left-top
	   -0.894427f, 0.447214f,  0.000000f, // left-left
	   -0.894427f, 0.447214f,  0.000000f, // left-right
	};



	// for cube
	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao_pyramid);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao_pyramid);

	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_positionPyramid);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_positionPyramid);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_position), pyramid_position, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for normal
	glGenBuffers(1, &vbo_normal);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);

	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_Normals), pyramid_Normals, GL_STATIC_DRAW);

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//for enable depth
	glClearDepth(1.0f); // compulsory
	glEnable(GL_DEPTH_TEST);// compulsory
	glDepthFunc(GL_LEQUAL);// compulsory

	// step 7 : - set clear color of window to blue (here OpenGL Start)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialize orthographic projection matrix 
	perspectiveProjectionMatrix = vmath::mat4::identity();


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
	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// cube
	// 
	mat4 modelViewMatrix = mat4::identity();

	// Transformation
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

	// rotation matrix
	mat4 rotationMatrix = mat4::identity();
	rotationMatrix = vmath::rotate(cAngle, 0.0f, 1.0f, 0.0f);


	// scale matrix
	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, modelViewMatrix);

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_pyramid);

	if (bLightingEnable == TRUE)
	{
		glUniform1i(keyPressedUniform, 1);
		glUniform3fv(ldUniform, 1, lightDiffuse);
		glUniform3fv(kdUniform, 1, materialDiffuse);
		glUniform4fv(lightPositionUniform, 1, lightPosition);
	}
	else
	{
		glUniform1i(keyPressedUniform, 0);
	}

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	//code
	//cube rotate
	cAngle = cAngle - 1.0f;
	if (cAngle <= 0.0f)
	{
		cAngle = cAngle + 360.0f;
	}
}

void uninitialize(void)
{
	//function declarations
	void ToggleFullScreen(void);

	//code

	// for shader program object 
	if (shaderProgramObject)
	{
		// step 1 : use shader Program object
		glUseProgram(shaderProgramObject);

		// step 2 : get number of attached shader
		GLint numShader = 0;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShader);
			
		if (numShader > 0)
		{
			GLuint *pShader = (GLuint *)malloc(numShader * sizeof(GLuint));
			if (pShader != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShader, NULL, pShader);

				for (GLint i = 0; i < numShader; i++)
				{
					glDetachShader(shaderProgramObject, pShader[i]);

					glDeleteShader(pShader[i]);

					pShader[i] = NULL;
				}

				free(pShader);

				pShader = NULL;
			}
		}


		glUseProgram(0);

		glDeleteProgram(shaderProgramObject);

		shaderProgramObject = 0;
	}

	// cube 
	// 
	if (vbo_normal)
	{
		glDeleteBuffers(1, &vbo_normal);
		vbo_normal = 0;
	}
	// delete vbo for position
	if (vbo_positionPyramid)
	{
		glDeleteBuffers(1, &vbo_positionPyramid);
		vbo_positionPyramid = 0;
	}

	// delete vao 
	if (vao_pyramid)
	{
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
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


