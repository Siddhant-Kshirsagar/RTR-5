//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

//OpenGL Header
#include<gl/glew.h> // this must be before gl/GL.h 
#include<gl/GL.h>

#include<vector>

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

// for line
GLuint vao_line = 0;
GLuint vbo_positionLine = 0;

// for triangle
GLuint vao_triangle = 0;
GLuint vbo_positionTriangle = 0;

// for square
GLuint vao_square = 0;
GLuint vbo_positionSquare = 0;

// for circle
GLuint vao_circle = 0;
GLuint vbo_positionCircle = 0;

GLuint mvpMatrixUniform = 0;
GLuint colorUniform = 0;
// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix;

// code
int iKeyPressed = 0;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
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

		case 'T':
		case 't':
			iKeyPressed = 1;
			break;

		case 'S':
		case 's':
			iKeyPressed = 2;
			break;

		case 'C':
		case 'c':
			iKeyPressed = 3;
			break;

		case 'G':
		case 'g':
			iKeyPressed = 4;
			break;

		default:
			iKeyPressed = 0;

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
		fprintf(gpFILE,"wglMakeCurrent() Failed.\n");
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

	{
		// Vertex Shader
		// step 1 : vertex shader code
		const GLchar *vertexShaderSourceCode =
			"#version 460 core" \
			"\n" \
			"uniform mat4 uMVPMatrix;" \
			"in vec4 aPosition;" \
			"void main(void)" \
			"{" \
			"gl_Position= uMVPMatrix * aPosition;" \
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
			"uniform vec4 uColor;" \
			"out vec4 FragColor;" \
			"void main(void)" \
			"{" \
			"FragColor = uColor;" \
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

		glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");

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

		colorUniform = glGetUniformLocation(shaderProgramObject, "uColor");

		// step 16: declare position and color array 

		// position array inline initialization
		const GLfloat triangle_position[] =
		{
			0.0f,1.0f,0.0f, // glVertex3f() 1 st call for triangle 
			-1.0f,-1.0f,0.0f, // glVertex3f() 2nd call for triangle
			1.0f,-1.0f,0.0f // glVertex3f() 3rd  call for triangle
		};

		const GLfloat square_position[] =
		{
			1.0f,1.0f,0.0f, // glVertex3f() 1st call for square
			-1.0f,1.0f,0.0f,// glVertex3f() 2nd call for square
			-1.0f,-1.0f,0.0f,// glVertex3f() 3rd call for square
			1.0f,-1.0f,0.0f// glVertex3f() 4th call for square
		};

		float *circle_position = NULL;

		circle_position = (float *)malloc((sizeof(float) * 1080));
		if (circle_position == NULL)
		{
			fprintf(gpFILE, "malloc() failed for cirlce\n");
			return(-6);
		}
		int i = 0;

		for (int iAngle = 0; iAngle < 360; iAngle = iAngle + 1)
		{
			float x = 0.0f + 1.0f * cos((iAngle * M_PI) / 180.0f);
			float y = 0.0f + 1.0f * sin((iAngle * M_PI) / 180.0f);
			float z = 0.0f;

			circle_position[i] = x;
			i++;
			circle_position[i] = y;
			i++;
			circle_position[i] = z;
			i++;
		}

		//// for line
		const float line_position[] = {
			// horizontal line
			-2.0f,0.0f,0.0f,
			2.0f,0.0f,0.0f,

			// vertical line
			0.0f,2.0f,0.0f,
			0.0f,-2.0f,0.0f
		};

		// for Triangle
		// step 17 : create VAO (vertex array object) 
		glGenVertexArrays(1, &vao_triangle);

		// step 18 : bind with VAO (vertex array object)
		glBindVertexArray(vao_triangle);

		// step 19 : VBO(Vertex Buffer Object) for position
		glGenBuffers(1, &vbo_positionTriangle);

		// step 20 : bind with VBO( Vertex Buffer Object) for position
		glBindBuffer(GL_ARRAY_BUFFER, vbo_positionTriangle);

		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_position), triangle_position, GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// for square
		// step 17 : create VAO (vertex array object) 
		glGenVertexArrays(1, &vao_square);

		// step 18 : bind with VAO (vertex array object)
		glBindVertexArray(vao_square);

		// step 19 : VBO(Vertex Buffer Object) for position
		glGenBuffers(1, &vbo_positionSquare);

		// step 20 : bind with VBO( Vertex Buffer Object) for position
		glBindBuffer(GL_ARRAY_BUFFER, vbo_positionSquare);

		glBufferData(GL_ARRAY_BUFFER, sizeof(square_position), square_position, GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// for circle
		// step 17 : create VAO (vertex array object) 
		glGenVertexArrays(1, &vao_circle);

		// step 18 : bind with VAO (vertex array object)
		glBindVertexArray(vao_circle);

		// step 19 : VBO(Vertex Buffer Object) for position
		glGenBuffers(1, &vbo_positionCircle);

		// step 20 : bind with VBO( Vertex Buffer Object) for position
		glBindBuffer(GL_ARRAY_BUFFER, vbo_positionCircle);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1080, circle_position, GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// for line
		// step 17 : create VAO (vertex array object) 
		glGenVertexArrays(1, &vao_line);

		// step 18 : bind with VAO (vertex array object)
		glBindVertexArray(vao_line);

		// step 19 : VBO(Vertex Buffer Object) for position
		glGenBuffers(1, &vbo_positionLine);

		// step 20 : bind with VBO( Vertex Buffer Object) for position
		glBindBuffer(GL_ARRAY_BUFFER, vbo_positionLine);

		glBufferData(GL_ARRAY_BUFFER, sizeof(line_position), line_position, GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

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
	//function declaration
	void drawTriangle(void);
	void drawCircle(void);
	void drawSquare(void);
	void drawLine(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if (iKeyPressed == 1)
	{
		drawLine();
		drawTriangle();
	}
	else if (iKeyPressed == 2)
	{
		drawLine();
		drawSquare();
	}
	else if (iKeyPressed == 3)
	{
		drawLine();
		drawCircle();
	}
	else if (iKeyPressed == 4)
	{
		drawLine();
	}

	SwapBuffers(ghdc);
}

void drawLine(void)
{
	// code

	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	// variable for transformation
	mat4 modelViewMatrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	// variable for lineCount
	int lineCount = 0;

	// positive x axis (Vertical line)
	for (float xAxis = 0.05f; xAxis <= 2.0f; xAxis = xAxis + 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(xAxis, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// negative x axis (Vertical line)
	for (float xAxis = -0.05f; xAxis >= -2.0f; xAxis = xAxis - 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(xAxis, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// positive y axis (Horizontal line)
	for (float yAxis = 0.05f; yAxis <= 2.0f; yAxis = yAxis + 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, yAxis, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// positive y axis (Horizontal line)
	for (float yAxis = -0.05f; yAxis >= -2.0f; yAxis = yAxis - 0.05f)
	{
		lineCount = lineCount + 1;
		if (lineCount % 5 == 0)
		{
			glLineWidth(2.0f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, yAxis, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 0.0f, 1.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}
	lineCount = 0;

	// center horizontal line
	{
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 1.0f, 0.0f, 0.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 0, 2);

		// unbind vao 
		glBindVertexArray(0);
	}

	// center vertical line
	{
		translationMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
		modelViewMatrix = mat4::identity();
		modelViewMatrix = translationMatrix;

		// order of multiplication is very important
		mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniform4f(colorUniform, 0.0f, 1.0f, 0.0f, 1.0f);

		// push above mvp(model view projection) into vertex shader's mvp uniform
		glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		// step 2 : bind with VAO(vertex array object)
		glBindVertexArray(vao_line);

		// step 3 : draw geometry / shape / model /scene
		glDrawArrays(GL_LINES, 2, 2);

		// unbind vao 
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

void drawTriangle(void)
{
	// code
	
	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_triangle);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 3);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
}

void drawCircle(void)
{
	//code
	
	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_circle);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 360);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
}

void drawSquare(void)
{
	//code

// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Triangle
	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	modelViewMatrix = vmath::translate(0.0f, 0.0f, -4.9f);

	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao_square);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);
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

	// line 

	// delete vbo for position
	if (vbo_positionLine)
	{
		glDeleteBuffers(1, &vbo_positionLine);
		vbo_positionLine = 0;
	}

	// delete vao 
	if (vao_line)
	{
		glDeleteVertexArrays(1, &vao_line);
		vao_line = 0;
	}

	// circle 

	// delete vbo for position
	if (vbo_positionCircle)
	{
		glDeleteBuffers(1, &vbo_positionCircle);
		vbo_positionCircle = 0;
	}

	// delete vao 
	if (vao_circle)
	{
		glDeleteVertexArrays(1, &vao_circle);
		vao_circle = 0;
	}

	// square 

	// delete vbo for position
	if (vbo_positionSquare)
	{
		glDeleteBuffers(1, &vbo_positionSquare);
		vbo_positionSquare = 0;
	}

	// delete vao 
	if (vao_square)
	{
		glDeleteVertexArrays(1, &vao_square);
		vao_square = 0;
	}



	// triangle 

	// delete vbo for position
	if (vbo_positionTriangle)
	{
		glDeleteBuffers(1, &vbo_positionTriangle);
		vbo_positionTriangle = 0;
	}

	// delete vao 
	if (vao_triangle)
	{
		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
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


