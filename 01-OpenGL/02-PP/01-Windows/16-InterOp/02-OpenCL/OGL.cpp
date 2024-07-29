//windows header file
#include<windows.h> // for win32 API
#include<stdio.h> // For File IO
#include<stdlib.h> // For Exit

//OpenGL Header
#include<gl/glew.h> // this must be before gl/GL.h 
#include<gl/GL.h>

// OpenCL OpenGL InterOperability relalted header file
#define CL_TARGET_OPENCL_VERSION 200
#include<CL/opencl.h>

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
#pragma comment(lib,"opencl.lib")

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
GLuint vao = 0;
GLuint vbo_position = 0;

GLuint mvpMatrixUniform = 0;
// mat4 is datatype means 4 * 4 matrix (present in vmath.h)
mat4 perspectiveProjectionMatrix;

// OpenCL related global variable 
#define MESH_ARRAY_SIZE 1024 * 1024 * 4

int mesh_width = 1024;
int mesh_height = 1024;

float position[1024][1024][4];

GLuint vbo_gpu;

BOOL bOnGPU = FALSE;

float animationTime = 0.0f;

cl_device_id oclDeviceId;
cl_context oclContext = NULL;
cl_command_queue oclCommandQueue = NULL;
cl_program oclProgram = NULL;
cl_kernel oclKernel = NULL;
cl_mem ocl_graphics_resource = NULL;
cl_int oclResult;


enum
{
	AMC_ATTRIBUTE_POSITION = 0
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

		case 'C':
		case 'c':
			bOnGPU = FALSE;
			break;

		case 'G':
		case 'g':
			bOnGPU = TRUE;
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

	// check OpenCL support and if supported set default opencl device 0
	cl_platform_id oclPlatformId;
	cl_uint dev_count;
	cl_device_id *oclDeviceIds = NULL;
	
	// get default opencl platform id

	oclResult = clGetPlatformIDs(1, &oclPlatformId, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFILE, "clGetPlatformIDs() failed.\n");
		uninitialize();
		exit(0);
	}

	// get opencl device count
	oclResult = clGetDeviceIDs(oclPlatformId, CL_DEVICE_TYPE_GPU, 0, NULL, &dev_count);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFILE, "clGetDeviceIDs() to get device count failed.\n");
		uninitialize();
		exit(0);
	}	
	else if (dev_count == 0)
	{
		fprintf(gpFILE, "there are no openCL supported device.\n");
		uninitialize();
		exit(0);
	}
	else
	{
		oclDeviceIds = (cl_device_id *)malloc(sizeof(cl_device_id) * dev_count);
		if (oclDeviceIds == NULL)
		{
			fprintf(gpFILE, "malloc() failed for oclDeviceIds.\n");
			uninitialize();
			exit(0);
		}

		// fill the opencl device ids array
		oclResult = clGetDeviceIDs(oclPlatformId, CL_DEVICE_TYPE_GPU, dev_count, oclDeviceIds, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clGetDeviceIDs() failed. to get opencl device id array\n");
			uninitialize();
			exit(0);
		}

		// set the default opencl device 
		oclDeviceId = oclDeviceIds[0];

		// free oclDeviceIds
		if (oclDeviceIds)
		{
			free(oclDeviceIds);
			oclDeviceIds = NULL;
		}

		// declare opencl context properties array
		cl_context_properties oclContextProperties[] =
		{
			CL_GL_CONTEXT_KHR,(cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR,(cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM,(cl_context_properties)oclPlatformId,
			0
		};

		// create opencl context
		oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceId, NULL, NULL, &oclResult);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clCreateContext() failed.\n");
			uninitialize();
			exit(0);
		}

		// create opencl command queue
		oclCommandQueue = clCreateCommandQueueWithProperties(oclContext, oclDeviceId, 0, &oclResult);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clCreateCommandQueueWithProperties() failed.\n");
			uninitialize();
			exit(0);
		}

		// declare opencl kernel code
		const char *oclKernelSourceCode =
			"__kernel void sineWaveKernel(__global float4 * pos, int width, int height, float time)" \
			"{ "\
			"int x = get_global_id(0);"\
			"int y = get_global_id(1);"\

			"float u = (float)x / (float)width;" \
			"float v = (float)y / (float)height;" \

			"u = u * 2.0f - 1.0f;" \
			"v = v * 2.0f - 1.0f;" \

			"float frequency = 4.0f;" \

			"float w = sin(u * frequency + time) * cos(v * frequency + time) * 0.5f;" \

			"pos[y * width + x] = (float4)(u, w, v, 1.0f);" \
			"}";

		// from above kernel source code create opencl program
		oclProgram = clCreateProgramWithSource(oclContext, 1,(const char **)&oclKernelSourceCode,NULL,&oclResult);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clCreateProgramWithSource() failed.\n");
			uninitialize();
			exit(0);
		}

		// build above opencl program
		oclResult = clBuildProgram(oclProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clBuildProgram() failed.\n");

			// get build error 
			cl_int oclBuildResult;
			char szBuffer[1024];

			oclBuildResult = clGetProgramBuildInfo(oclProgram, oclDeviceId, CL_PROGRAM_BUILD_LOG, sizeof(szBuffer), szBuffer, NULL);
			if (oclBuildResult != CL_SUCCESS)
			{
				fprintf(gpFILE, "clGetProgramBuildInfo() failed.\n");
				uninitialize();
				exit(0);
			}
			else
			{
				fprintf(gpFILE, "OpenCL program build error : %s", szBuffer);
				uninitialize();
				exit(0);
			}
			uninitialize();
			exit(0);
		}

		// create opencl kernel
		oclKernel = clCreateKernel(oclProgram, "sineWaveKernel", &oclResult);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clCreateKernel() failed.\n");
			uninitialize();
			exit(0);
		}
	}

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
		uninitialize();
	}

	// Fragment shader
	// step 6 : write the fragement shader code
	const GLchar *fragmentShaderCode =
		"#version 460 core" \
		"\n" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = vec4(1.0,0.5,0.0,1.0);" \
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

	// step 16: declare position

	// position array inline initialization
	for (int i = 0; i < mesh_width; i++)
	{
		for (int j = 0; j < mesh_height; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				position[i][j][k] = 0.0f;
			}
		}
	}

	// step 17 : create VAO (vertex array object) 
	glGenVertexArrays(1, &vao);

	// step 18 : bind with VAO (vertex array object)
	glBindVertexArray(vao);

	// vbo for CPU
	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_position);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for GPU
	// step 19 : VBO(Vertex Buffer Object) for position
	glGenBuffers(1, &vbo_gpu);

	// step 20 : bind with VBO( Vertex Buffer Object) for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);

	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Register OpenGL Buffer to OpenCL Graphics Resource for InterOperability
	ocl_graphics_resource = clCreateFromGLBuffer(oclContext, CL_MEM_WRITE_ONLY, vbo_gpu, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFILE, "clCreateFromGLBuffer() failed.\n");
		uninitialize();
		exit(0);
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
	void launchCPUKernel(int, int, float);
	void uninitialize(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// step 1 : use shader program
	glUseProgram(shaderProgramObject);

	// Transformation
	mat4 modelViewMatrix = mat4::identity();
	// order of multiplication is very important
	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

	// push above mvp(model view projection) into vertex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// step 2 : bind with VAO(vertex array object)
	glBindVertexArray(vao);

	if (bOnGPU == TRUE)
	{
		// set kernel argument
		oclResult = clSetKernelArg(oclKernel,0, sizeof(cl_mem), (void *)&ocl_graphics_resource);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clSetKernelArg() failed for 0 th argument.\n");
			uninitialize();
			exit(0);
		}

		oclResult = clSetKernelArg(oclKernel, 1, sizeof(cl_int), (void *)&mesh_width);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clSetKernelArg() failed for 1 th argument.\n");
			uninitialize();
			exit(0);
		}

		oclResult = clSetKernelArg(oclKernel, 2, sizeof(cl_int),
			(void *)&mesh_height);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clSetKernelArg() failed for 2 th argument.\n");
			uninitialize();
			exit(0);
		}

		oclResult = clSetKernelArg(oclKernel, 3, sizeof(cl_float), (void *)&animationTime);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clSetKernelArg() failed for 3 th argument.\n");
			uninitialize();
			exit(0);
		}

		// map opencl kernel

		oclResult = clEnqueueAcquireGLObjects(oclCommandQueue, 1, &ocl_graphics_resource, 0, NULL, NULL);

		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clEnqueueAcquireGLObjects() failed.\n");
			uninitialize();
			exit(0);
		}

		// launch openCL kernel
		size_t globalWorkSize[2];
		globalWorkSize[0] = mesh_width;
		globalWorkSize[1] = mesh_height;

		oclResult = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
			if (oclResult != CL_SUCCESS)
			{
				fprintf(gpFILE, "clEnqueueNDRangeKernel() failed.\n");
				uninitialize();
				exit(0);
			}

		// unmapped graphic resource
		oclResult = clEnqueueReleaseGLObjects(oclCommandQueue, 1, &ocl_graphics_resource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFILE, "clEnqueueReleaseGLObjects() failed.\n");
			uninitialize();
			exit(0);
		}

		// finish
		clFinish(oclCommandQueue);

		// rebind with the OpenGL buffer object
		glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
	}
	else
	{
		// launch CPU kernel
		launchCPUKernel(mesh_width, mesh_height, animationTime);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

		glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), position, GL_DYNAMIC_DRAW);
	}

	// to tell OpenGL where we mapped opengl buffer 
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, NULL, NULL);

	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	// step 3 : draw geometry / shape / model /scene
	glDrawArrays(GL_POINTS, 0, mesh_width * mesh_height);

	// unbind vao 
	glBindVertexArray(0);

	// unbind vao 
	glBindVertexArray(0);

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void launchCPUKernel(int width, int height, float time)
{
	// code 
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				float u = (float)i / (float)width;
				float v = (float)j / (float)height;

				// normalization 
				u = u * 2.0f - 1.0f;
				v = v * 2.0f - 1.0f;

				float frequency = 4.0f;
				float w = sinf(u * frequency + time) * cosf(v * frequency + time) * 0.5f;

				// fill data
				if (k == 0) // mapped with x
				{
					position[i][j][k] = u;
				}
				if (k == 1) // mapped with y
				{
					position[i][j][k] = w;
				}
				if (k == 2) // mapped with z
				{
					position[i][j][k] = v;
				}
				if (k == 3) // mapped with w(matrix 4th value)
				{
					position[i][j][k] = 1.0f;
				}

			}
		}
	}
}


void update(void)
{
	//code
	animationTime = animationTime + 0.01f;
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

	// delete vbo gpu
	if (vbo_gpu)
	{
		if (ocl_graphics_resource)
		{
			clReleaseMemObject(ocl_graphics_resource);
			ocl_graphics_resource = NULL;
		}
		glDeleteBuffers(1, &vbo_gpu);
		vbo_gpu = 0;
	}

	// release openCL kernel
	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}

	// release opencl program
	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}

	// release opencl command queue
	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	// relase opencl context
	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}

	// delete vbo for position
	if (vbo_position)
	{
		glDeleteBuffers(1, &vbo_position);
		vbo_position = 0;
	}

	// delete vao 
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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


