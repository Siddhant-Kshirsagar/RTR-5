//windows header file
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<dxgi.h>

// d3d related header file
#include<d3d11.h>
#include<d3dcompiler.h>

#pragma warning(disable:4838)
#include"XNAMath_204/xnamath.h"
#include"D3D.h"
#include"Sphere.h"

// d3d related library
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"Sphere.lib")

//Macros 
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; // initialization of struct => this work on all type (if we want to initialize all value to 0)
BOOL gbFullscreen = FALSE;
FILE *gpFILE = NULL;

HWND ghwnd = NULL; // g = global handle of window
BOOL gbActive = FALSE;

CHAR gszLogFileName[] = "Log.txt";

// d3d related global variable
IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;
ID3D11DepthStencilView *gpID3D11DepthStencilView = NULL;

ID3D11VertexShader *gpID3D11VertexShader = NULL;
ID3D11PixelShader *gpID3D11PixelShader = NULL;
ID3D11InputLayout *gpID3D11InputLayout = NULL;
ID3D11Buffer *gpID3D11Buffer_PositionBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_NormalBuffer = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11RasterizerState *gpID3D11RasterizerState = NULL;
ID3D11Buffer *gpID3D11Buffer_IndexBuffer = NULL;


struct CBUFFER
{
	//XMMATRIX WorldViewProjectionMatrix; // similar to OpenGL model is equal to 'world' word
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;

	XMVECTOR LightAmbient;
	XMVECTOR LightDiffuse;
	XMVECTOR LightSpecular;
	XMVECTOR LightPosition;

	XMVECTOR MaterialAmbient;
	XMVECTOR MaterialDiffuse;
	XMVECTOR MaterialSpecular;
	float MaterialShineness;

	unsigned int KeyPressed;
};

XMMATRIX perspectiveProjectionMatrix;

// for light
float lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
float lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
float lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
float lightPosition[] = { 0.0f,0.0f,-5.0f,1.0f };

float materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
float materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
float materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
float materialShineness = 128.0f;

BOOL bLightingEnable = FALSE;

// for sphere
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumElements;
unsigned int gNumVertices;

unsigned int currentWinWidth = 0;
unsigned int currentWinHeight = 0;

char rotationAxis = 'd';

float lightAngleZero = 0.0f;
float lightAngleOne = 0.0f;
float lightAngleTwo = 0.0f;

float clearColor[4];

//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//Function declaration
	HRESULT initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//local variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SGKWindow");

	HRESULT hr = S_OK;

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
	gpFILE = fopen(gszLogFileName, "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log file cannot be open"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(gpFILE, "Program Started Successfully\n\n");
		fprintf(gpFILE, "====================================================\n\n");
		fclose(gpFILE);
	}
	

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
	hr = initialize();
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "initialize failed\n\n");
		fclose(gpFILE);
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "initialize successfully\n\n");
		fclose(gpFILE);
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
	HRESULT resize(int, int);

	// local variable 
	HRESULT hr = S_OK;

	switch (iMsg)
	{
	case WM_SETFOCUS: // when window is in focus (window is activate)
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS: // when window is not in focus(window is deactivate)
		gbActive = FALSE;
		break;
	case WM_SIZE:
		if (gpID3D11DeviceContext)
		{
			hr = resize(LOWORD(lParam), HIWORD(lParam));
			if (FAILED(hr))
			{
				gpFILE = fopen(gszLogFileName, "a+");
				fprintf(gpFILE, "resize() failed\n\n");
				fclose(gpFILE);
				return(hr);
			}
			else
			{
				gpFILE = fopen(gszLogFileName, "a+");
				fprintf(gpFILE, "resize() successfully\n\n");
				fclose(gpFILE);
			}
		}
		break;

	//case WM_ERASEBKGND:
	//	return(0);

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

HRESULT initialize(void)
{
	//function declarations
	HRESULT resize(int width, int height);
	void printD3DInfo(void);

	// variable declaration
	HRESULT hr = S_OK;

	//code
	// 1: initialize swapchain descriptor structure
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	ZeroMemory((void *)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = ghwnd;
	dxgiSwapChainDesc.Windowed = TRUE;

	// create d3d swap , device, context, rtv(render target view)
	D3D_DRIVER_TYPE d3d_driver_type;
	D3D_DRIVER_TYPE d3d_driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP, // WARP = WINDOWS ADVANCED RASTERIZER PLATFORM (HARDWARE LEVEL PEKSHA KAMI QUALITY CHA DRIVER DETO JAR KA HARDWARE DRIVER NASEL)
		D3D_DRIVER_TYPE_SOFTWARE,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_NULL
	};
	D3D_FEATURE_LEVEL d3dFeatureLevelRequired = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeatureLevelAcquired = D3D_FEATURE_LEVEL_10_0;

	//UINT 
	UINT numDriverTypes;
	UINT numFeatureLevel = 1;

	numDriverTypes = sizeof(d3d_driver_types) / sizeof(d3d_driver_types[0]);

	for (UINT i = 0; i < numDriverTypes; i++)
	{
		d3d_driver_type = d3d_driver_types[i];
		D3D11CreateDeviceAndSwapChain(
			NULL,
			d3d_driver_type,
			NULL,
			0,
			&d3dFeatureLevelRequired,
			numFeatureLevel,
			D3D11_SDK_VERSION,
			&dxgiSwapChainDesc,
			&gpIDXGISwapChain,
			&gpID3D11Device,
			&d3dFeatureLevelAcquired,
			&gpID3D11DeviceContext);

			if (SUCCEEDED(hr))
			{
				break;
			}
	}


	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "D3d11CreateDeviceAndSwapChain() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		if (d3d_driver_type == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(gpFILE, "Hardware driver found\n\n");
		}
		else if (d3d_driver_type == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(gpFILE, "WARP driver found\n\n");
		}
		else if (d3d_driver_type == D3D_DRIVER_TYPE_SOFTWARE)
		{
			fprintf(gpFILE, "Software driver found\n\n");
		}
		else if (d3d_driver_type == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(gpFILE, "Reference driver found\n\n");
		}
		else if (d3d_driver_type == D3D_DRIVER_TYPE_NULL)
		{
			fprintf(gpFILE, "NULL driver found\n\n");
		}
		else if (d3d_driver_type == D3D_DRIVER_TYPE_UNKNOWN)
		{
			fprintf(gpFILE, "Unknown driver found\n\n");
		}
		else
		{
			fprintf(gpFILE, "Undefined driver found\n");
		}
		fprintf(gpFILE, "\n\n====================================================\n\n");
		fprintf(gpFILE, "Which feature level we found\n\n");

		if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(gpFILE, "11.0 feature level found\n\n");
		}
		else if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(gpFILE, "10.0 feature level found\n\n");
		}
		else if (d3dFeatureLevelAcquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(gpFILE, "10.1 feature level found\n\n");
		}
		else
		{
			fprintf(gpFILE, "Unknown feature level found\n\n");
		}

		fprintf(gpFILE, "D3d11CreateDeviceAndSwapChain() succeeded\n\n");
		fprintf(gpFILE, "\n\n====================================================\n\n");
		fclose(gpFILE);
	}

	printD3DInfo();

	// Vertex Shader
	const char *vertexShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 lightAmbient;" \
		"float4 lightDiffuse;" \
		"float4 lightSpecular;" \
		"float4 lightPosition;" \
		"float4 materialAmbient;" \
		"float4 materialDiffuse;" \
		"float4 materialSpecular;" \
		"float materialShineness;" \
		"uint KeyPressed;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 transformedNormal:NORMAL0;" \
		"float3 lightDirection:NORMAL1;" \
		"float3 viewerVector:NORMAL2;" \
		"};" \
		"vertex_output main(float4 pos:POSITION, float3 norm:NORMAL)" \
		"{" \
		"vertex_output output;" \
		"if(KeyPressed == 1)" \
		"{" \
		"float4 eyeCoordinates = mul(viewMatrix,mul(worldMatrix,pos));" \
		"output.transformedNormal = mul((float3x3)worldMatrix,norm);" \
		"output.lightDirection = (lightPosition - eyeCoordinates);" \
		"output.viewerVector= -eyeCoordinates.xyz;" \
		"}" \
		"else" \
		"{" \
		"output.transformedNormal = (float3)(0.0,0.0,0.0);" \
		"output.lightDirection = (float3)(0.0,0.0,0.0);" \
		"output.viewerVector= (float3)(0.0,0.0,0.0);" \
		"}" \
		"float4 position = mul(projectionMatrix,mul(viewMatrix,mul(worldMatrix,pos)));" \
		"output.position = position;" \
		"return(output);" \
		"}";

	ID3DBlob *pID3DBlob_VertexShaderSourceCode = NULL;
	ID3DBlob *pID3DBlob_Error = NULL;

	// compile above shader
	hr = D3DCompile(vertexShaderSourceCode,
		lstrlenA(vertexShaderSourceCode) + 1,
		"VS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		&pID3DBlob_VertexShaderSourceCode,
		&pID3DBlob_Error);
		
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "vertex shader compilation error : %s\n\n",(char*)pID3DBlob_Error->GetBufferPointer());
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "vertex shader compile successfully\n\n");
		fclose(gpFILE);
	}


	// create vertex shader from above code
	hr = gpID3D11Device->CreateVertexShader(
		pID3DBlob_VertexShaderSourceCode->GetBufferPointer(),
		pID3DBlob_VertexShaderSourceCode->GetBufferSize(),
		NULL,
		&gpID3D11VertexShader);

	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateVertexShader() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateVertexShader() succeded\n\n");
		fclose(gpFILE);
	}

	// set above created vertex shader into pipeline
	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	// Pixel Shader
	const char *pixelShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldMatrix;" \
		"float4x4 viewMatrix;" \
		"float4x4 projectionMatrix;" \
		"float4 lightAmbient;" \
		"float4 lightDiffuse;" \
		"float4 lightSpecular;" \
		"float4 lightPosition;" \
		"float4 materialAmbient;" \
		"float4 materialDiffuse;" \
		"float4 materialSpecular;" \
		"float materialShineness;" \
		"uint KeyPressed;" \
		"}" \
		"struct vertex_output" \
		"{" \
		"float4 position:SV_POSITION;" \
		"float3 transformedNormal:NORMAL0;" \
		"float3 lightDirection:NORMAL1;" \
		"float3 viewerVector:NORMAL2;" \
		"};" \
		"float4 main(vertex_output input): SV_TARGET" \
		"{" \
		"float3 normalizedTransfomedNormals = normalize(input.transformedNormal);" \
		"float3 normalizedLightDirection = normalize(input.lightDirection);" \
		"float3 normalizedViewerVector = normalize(input.viewerVector);" \
		"float3 phongADSLight;" \
		"if(KeyPressed == 1)" \
		"{" \
		"float3 reflectionVector = reflect(-normalizedLightDirection,normalizedTransfomedNormals);" \
		"float3 ambientLight = lightAmbient * materialAmbient;" \
		"float3 diffuseLight = lightDiffuse * materialDiffuse * max(dot(normalizedLightDirection,normalizedTransfomedNormals),0.0);" \
		"float3 specularLight = lightSpecular * materialSpecular * pow(max(dot(reflectionVector,normalizedViewerVector),0.0),materialShineness);" \
		"phongADSLight = ambientLight + diffuseLight + specularLight;" \
		"}" \
		"else" \
		"{" \
		"phongADSLight = float3(1.0,1.0,1.0);" \
		"}" \
		"float4 color = float4(phongADSLight,1.0);"\
		"return color;" \
		"}";

	ID3DBlob *pID3DBlob_PixelShaderSourceCode = NULL;
	pID3DBlob_Error = NULL;

	// compile above shader
	hr = D3DCompile(pixelShaderSourceCode,
		lstrlenA(pixelShaderSourceCode) + 1,
		"PS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&pID3DBlob_PixelShaderSourceCode,
		&pID3DBlob_Error);

	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "pixel shader compilation error : %s\n\n", (char *)pID3DBlob_Error->GetBufferPointer());
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "pixel shader compile successfully\n\n");
		fclose(gpFILE);
	}


	// create vertex shader from above code
	hr = gpID3D11Device->CreatePixelShader(
		pID3DBlob_PixelShaderSourceCode->GetBufferPointer(),
		pID3DBlob_PixelShaderSourceCode->GetBufferSize(),
		NULL,
		&gpID3D11PixelShader);

	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreatePixelShader() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreatePixelShader() succeded\n\n");
		fclose(gpFILE);
	}

	// set above created pixel shader into pipeline
	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	// release error blob
	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;
	}

	// release pixel shader blob
	if (pID3DBlob_PixelShaderSourceCode)
	{
		pID3DBlob_PixelShaderSourceCode->Release();
		pID3DBlob_PixelShaderSourceCode = NULL;
	}

	// initialize input element structure
	// similiar to glBindAttribLocation()
	D3D11_INPUT_ELEMENT_DESC d3dInputElementDesc[2];
	ZeroMemory((void *)d3dInputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC)* _ARRAYSIZE(d3dInputElementDesc));

	// position
	d3dInputElementDesc[0].SemanticName = "POSITION";
	d3dInputElementDesc[0].SemanticIndex = 0;
	d3dInputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc[0].InputSlot = 0;
	d3dInputElementDesc[0].AlignedByteOffset = 0;
	d3dInputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc[0].InstanceDataStepRate = 0;

	// normal
	d3dInputElementDesc[1].SemanticName = "NORMAL";
	d3dInputElementDesc[1].SemanticIndex = 0;
	d3dInputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc[1].InputSlot = 1;
	d3dInputElementDesc[1].AlignedByteOffset = 0;
	d3dInputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc[1].InstanceDataStepRate = 0;

	// using above structure create input layout
	hr = gpID3D11Device->CreateInputLayout(d3dInputElementDesc, _ARRAYSIZE(d3dInputElementDesc), pID3DBlob_VertexShaderSourceCode->GetBufferPointer(), pID3DBlob_VertexShaderSourceCode->GetBufferSize(), &gpID3D11InputLayout);

	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateInputLayout() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateInputLayout() succeeded\n\n");
		fclose(gpFILE);
	}

	// set above input layout in pipe
	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	// now we can release vertex shader source code blob
	if (pID3DBlob_VertexShaderSourceCode)
	{
		pID3DBlob_VertexShaderSourceCode->Release();
		pID3DBlob_VertexShaderSourceCode = NULL;
	}

	// declare geometry
	// DirectX follows left hand rule , that's why give coordinates in clock-wise direction
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	gpFILE = fopen(gszLogFileName, "a+");
	fprintf(gpFILE, "getSphereVertexData() gNumVertices = %d \t  gNumElements = %d\n\n", gNumVertices,gNumElements);
	fclose(gpFILE);

	// position
	// create vertex buffer for position
	D3D11_BUFFER_DESC d3d11BufferDesc;
	ZeroMemory((void *)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to openGL GL_DYNAMIC_DRAW
	d3d11BufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void *)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_vertices;

	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() failed for position vertex buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() succeeded for position vertex buffer\n\n");
		fclose(gpFILE);
	}

	// Normals VB
	// Follow Same Above Method For Normals And Textures
	ZeroMemory(&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory((void *)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_normals;
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_NormalBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() failed for normals vertex buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() succeeded for normals vertex buffer\n\n");
		fclose(gpFILE);
	}

	//  Index 
	// create index buffer
	ZeroMemory(&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11BufferDesc.ByteWidth = gNumElements * sizeof(unsigned short);
	d3d11BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory((void *)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_elements;
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_IndexBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() failed for index vertex buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() succeeded for index vertex buffer\n\n");
		fclose(gpFILE);
	}

	// create constant buffer to send transformation like uniform data
	ZeroMemory((void *)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDesc.Usage = D3D11_USAGE_DEFAULT; // similar to openGL GL_DYNAMIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(CBUFFER);
	d3d11BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// create constant buffer using above structure
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() failed for constant buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() succeeded for constant buffer\n\n");
		fclose(gpFILE);
	}
	
	// set above buffer into pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);
	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	// create and set rasterizer state to off backface culling
	D3D11_RASTERIZER_DESC d3d11RasterizerDesc;

	ZeroMemory((void *)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3d11RasterizerDesc.MultisampleEnable = FALSE;
	d3d11RasterizerDesc.DepthBias = 0;
	d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
	d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3d11RasterizerDesc.DepthClipEnable = TRUE;
	d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
	d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
	d3d11RasterizerDesc.ScissorEnable = FALSE;

	hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpID3D11RasterizerState);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateRasterizerState() failed for Rasterizer State buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateRasterizerState() succeeded for Rasterizer State constant buffer\n\n");
		fclose(gpFILE);
	}

	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);


	// define clear color
	clearColor[0] = 0.75f;
	clearColor[1] = 0.75f;
	clearColor[2] = 0.75f;
	clearColor[3] = 1.0f;

	// orthographic projection
	perspectiveProjectionMatrix = XMMatrixIdentity();


	// warmup resize
	hr = resize(WIN_WIDTH, WIN_HEIGHT);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "resize() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "resize() successfully\n\n");
		fclose(gpFILE);
	}
	return(hr);
}
void printD3DInfo(void)
{
	// variable declaration
	IDXGIFactory *pIDXGIFactory = NULL;
	IDXGIAdapter *pIDXGIAdaptor = NULL;
	DXGI_ADAPTER_DESC dxgiAdaptorDesc;
	HRESULT hr = S_OK;
	char str[255];

	// code
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateDXGIFactory() failed\n\n");
		fclose(gpFILE);
		exit(0);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateDXGIFactory() successfully\n\n");
		fclose(gpFILE);
	}

	if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdaptor) != DXGI_ERROR_NOT_FOUND)
	{
		ZeroMemory((void *)&dxgiAdaptorDesc, sizeof(DXGI_ADAPTER_DESC));
		pIDXGIAdaptor->GetDesc(&dxgiAdaptorDesc);

		WideCharToMultiByte(CP_ACP, 0, dxgiAdaptorDesc.Description, 255, str, 255, NULL, NULL);
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "==================================================================\n\n");
		fprintf(gpFILE, "Graphic Card Name = %s\n\n", str);
		fprintf(gpFILE, "Memory in bytes = %I64d\n\n", dxgiAdaptorDesc.DedicatedVideoMemory);
		fprintf(gpFILE, "Memory in GB = %d\n\n", (int)ceil(dxgiAdaptorDesc.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0));
		fprintf(gpFILE, "==================================================================\n\n");
		fclose(gpFILE);

	}

	if (pIDXGIAdaptor)
	{
		pIDXGIAdaptor->Release();
		pIDXGIAdaptor = NULL;
	}

	if (pIDXGIFactory)
	{
		pIDXGIFactory->Release();
		pIDXGIFactory = NULL;
	}
}



HRESULT resize(int width, int height)
{
	// variable declaration
	HRESULT hr = S_OK;

	//code
	if (height <= 0)
	{
		height = 1;
	}

	// release depth stencil view
	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}

	// release render target view
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	gpFILE = fopen(gszLogFileName, "a+");
	fprintf(gpFILE, "Widht = %d height = %d \n\n", width, height);
	fclose(gpFILE);

	// resize swap chain buffer according to the changed 
	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// create new render target view
	// a) get the buffer for rtv from swapchain into the texture
	ID3D11Texture2D *piD3D11Texture2D = NULL;
	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&piD3D11Texture2D);

	// b) create RTV using above buffer
	hr = gpID3D11Device->CreateRenderTargetView(piD3D11Texture2D, NULL, &gpID3D11RenderTargetView);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateRenderTargetView() failed\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateRenderTargetView() successfully\n\n");
		fclose(gpFILE);
	}


	// Create an empty texture according to the new changed size we will call it as depth buffer
	D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
	ZeroMemory((void *)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));

	gpFILE = fopen(gszLogFileName, "a+");
	fprintf(gpFILE, "Widht = %d height = %d \n\n", width, height);
	fclose(gpFILE);

	d3d11Texture2DDesc.Width = (UINT)width;
	d3d11Texture2DDesc.Height = (UINT)height;
	d3d11Texture2DDesc.MipLevels = 1;
	d3d11Texture2DDesc.ArraySize = 1;
	d3d11Texture2DDesc.SampleDesc.Count = 1;
	d3d11Texture2DDesc.SampleDesc.Quality = 0;
	d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3d11Texture2DDesc.CPUAccessFlags = 0;
	d3d11Texture2DDesc.MiscFlags = 0;

	ID3D11Texture2D *pID3D11Texture2D_DepthBuffer = NULL;

	hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_DepthBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateTexture2D() failed for depth buffer creation\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateTexture2D() successfully for depth buffer creation \n\n");
		fclose(gpFILE);
	}

	// create depth stencil view according to above depth buffer texture
	D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
	ZeroMemory((void *)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateDepthStencilView() failed for depth buffer creation\n\n");
		fclose(gpFILE);
		pID3D11Texture2D_DepthBuffer->Release();
		pID3D11Texture2D_DepthBuffer = NULL;
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateDepthStencilView() successfully for depth buffer creation \n\n");
		fclose(gpFILE);
	}

	// c) set this to pipeline
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);
	piD3D11Texture2D->Release();
	piD3D11Texture2D = NULL;

	// initialize and set the view // similar to openGL viewport
	D3D11_VIEWPORT d3dViewport;

	ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

	d3dViewport.TopLeftX = 0.0f;
	d3dViewport.TopLeftY = 0.0f;
	d3dViewport.Width = (float)width;
	d3dViewport.Height = (float)height;
	d3dViewport.MinDepth = 0.0f;
	d3dViewport.MaxDepth = 1.0f;

	// set above viewport in pipeline
	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

	currentWinWidth = width;
	currentWinHeight = height;

	// initialize perspective projection

	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	return(hr);
}

void display(void)
{
	// variable declaration
	D3D11_VIEWPORT d3dViewport;
	float row1_Y, row2_Y, row3_Y, row4_Y, row5_Y, row6_Y;
	float col1_X, col2_X, col3_X, col4_X;

	float difference_X = currentWinWidth / 6; // for column
	float difference_Y = currentWinHeight / 6; // for rows

	col1_X = 0.0f;
	col2_X = col1_X + difference_X;
	col3_X = col2_X + difference_X;
	col4_X = col3_X + difference_X;

	row1_Y = 0.0f;
	//row1_Y = currentWinHeight - difference_Y;
	row2_Y = row1_Y + difference_Y;
	row3_Y = row2_Y + difference_Y;
	row4_Y = row3_Y + difference_Y;
	row5_Y = row4_Y + difference_Y;
	row6_Y = row5_Y + difference_Y;

	//code
	// similar to clear color in openGL
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);
	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 1st column 1st row
	{
		// first column of Precious Stone
		// emerad
		// ambient material
		materialAmbient[0] = 0.0215f; // r
		materialAmbient[1] = 0.1745f; // g
		materialAmbient[2] = 0.0215f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.07568f; // r
		materialDiffuse[1] = 0.61424f; // g
		materialDiffuse[2] = 0.07568f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.0633f; // r
		materialSpecular[1] = 0.727811f; // g
		materialSpecular[2] = 0.633f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.6f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row1_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 1st column 2nd row
	{
		// 2nd sphere on 1st column, jade
		// ambient material
		materialAmbient[0] = 0.135f; // r
		materialAmbient[1] = 0.2225f; // g
		materialAmbient[2] = 0.1575f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.54f; // r
		materialDiffuse[1] = 0.89f; // g
		materialDiffuse[2] = 0.63f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.316228f; // r
		materialSpecular[1] = 0.316228f; // g
		materialSpecular[2] = 0.316228f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.1f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row2_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 1st column 3rd row
	{
		// 3rd sphere on 1st column, obsidian
		// ambient material
		materialAmbient[0] = 0.05375f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.06625f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.18275f; // r
		materialDiffuse[1] = 0.17f; // g
		materialDiffuse[2] = 0.22525f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.332741f; // r
		materialSpecular[1] = 0.328634f; // g
		materialSpecular[2] = 0.346435f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.3f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row3_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 1st column 4th row
	{
		// 4th sphere on 1st column, pearl
		// ambient material
		materialAmbient[0] = 0.25f; // r
		materialAmbient[1] = 0.20725f; // g
		materialAmbient[2] = 0.20725f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 1.0f; // r
		materialDiffuse[1] = 0.829f; // g
		materialDiffuse[2] = 0.829f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.296648f; // r
		materialSpecular[1] = 0.296648f; // g
		materialSpecular[2] = 0.296648f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.088f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row4_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 1st column 5th row
	{
		// 5th sphere on 1st column, ruby
		// ambient material
		materialAmbient[0] = 0.1745f; // r
		materialAmbient[1] = 0.01175f; // g
		materialAmbient[2] = 0.01175f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.61424f; // r
		materialDiffuse[1] = 0.04136f; // g
		materialDiffuse[2] = 0.04136f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.727811f; // r
		materialSpecular[1] = 0.626959f; // g
		materialSpecular[2] = 0.626959f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.6f * 128;


		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row5_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;


		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 1st column 6th row
	{
		// 6th shpere on 1st column, turquoise
		// ambient material
		materialAmbient[0] = 0.1f; // r
		materialAmbient[1] = 0.18725f; // g
		materialAmbient[2] = 0.1745f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.396f; // r
		materialDiffuse[1] = 0.74151f; // g
		materialDiffuse[2] = 0.69102f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.297254f; // r
		materialSpecular[1] = 0.30829f; // g
		materialSpecular[2] = 0.306678f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.1f * 128;


		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col1_X;
		d3dViewport.TopLeftY = row6_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// 2nd column 1st row
	{
		// 2nd column metal
		// 1st sphere on 2nd column, brass
		// ambient material
		materialAmbient[0] = 0.329412f; // r
		materialAmbient[1] = 0.223529f; // g
		materialAmbient[2] = 0.027451f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.780392f; // r
		materialDiffuse[1] = 0.568627f; // g
		materialDiffuse[2] = 0.113725f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.992157f; // r
		materialSpecular[1] = 0.941176f; // g
		materialSpecular[2] = 0.807843f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.21794872f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row1_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 2nd column 2nd row
	{
		// 2nd sphere
		// ambient material
		materialAmbient[0] = 0.2125f; // r
		materialAmbient[1] = 0.1275f; // g
		materialAmbient[2] = 0.054f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.714f; // r
		materialDiffuse[1] = 0.4284f; // g
		materialDiffuse[2] = 0.18144f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.393548f; // r
		materialSpecular[1] = 0.271906f; // g
		materialSpecular[2] = 0.166721f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.2 * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row2_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 2nd column 3rd row
	{
		// 3rd sphere on 2nd column, chrome
		// ambient material
		materialAmbient[0] = 0.25f; // r
		materialAmbient[1] = 0.25f; // g
		materialAmbient[2] = 0.25f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4f; // r
		materialDiffuse[1] = 0.4f; // g
		materialDiffuse[2] = 0.4f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.774597f; // r
		materialSpecular[1] = 0.774597f; // g
		materialSpecular[2] = 0.774597f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.6f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row3_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 2nd column 4th row
	{
		// 4th sphere on 2nd column, copper
		// ambient material
		materialAmbient[0] = 0.19125f; // r
		materialAmbient[1] = 0.0735f; // g
		materialAmbient[2] = 0.0225f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.7038f; // r
		materialDiffuse[1] = 0.27048f; // g
		materialDiffuse[2] = 0.0828f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.256777f; // r
		materialSpecular[1] = 0.137622f; // g
		materialSpecular[2] = 0.086014f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.1f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row4_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 2nd column 5th row
	{
		// 5th sphere on 2nd column, gold
		// ambient material
		materialAmbient[0] = 0.24725f; // r
		materialAmbient[1] = 0.1995f; // g
		materialAmbient[2] = 0.0745f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.75164f; // r
		materialDiffuse[1] = 0.60648f; // g
		materialDiffuse[2] = 0.22648f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.628281f; // r
		materialSpecular[1] = 0.555802f; // g
		materialSpecular[2] = 0.366065f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.4f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row5_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 2nd column 6th row
	{
		// 6th sphere on 2nd column, silver
			// ambient material
		materialAmbient[0] = 0.19225f; // r
		materialAmbient[1] = 0.19225f; // g
		materialAmbient[2] = 0.19225f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.50754f; // r
		materialDiffuse[1] = 0.50754f; // g
		materialDiffuse[2] = 0.50754f; // b
		materialDiffuse[3] = 1.0f; // a


		// specular material
		materialSpecular[0] = 0.508273f; // r
		materialSpecular[1] = 0.508273f; // g
		materialSpecular[2] = 0.508273f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.4f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col2_X;
		d3dViewport.TopLeftY = row6_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// 3rd column 1st row
	{
		// 1st sphere on 3rd column, black
			// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.01f; // r
		materialDiffuse[1] = 0.01f; // g
		materialDiffuse[2] = 0.01f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.50f; // r
		materialSpecular[1] = 0.50f; // g
		materialSpecular[2] = 0.50f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row1_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 3rd column 2nd row
	{
		// 2nd sphere on 3rd column, cyan 
			// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.1f; // g
		materialAmbient[2] = 0.06f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.0f; // r
		materialDiffuse[1] = 0.50980392f; // g
		materialDiffuse[2] = 0.50980392f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.50196078f; // r
		materialSpecular[1] = 0.50196078f; // g
		materialSpecular[2] = 0.50196078f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row2_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 3rd column 3rd row
	{
		// 3rd sphere on 3rd column, green
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.1f; // r
		materialDiffuse[1] = 0.35f; // g
		materialDiffuse[2] = 0.1f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.45f; // r
		materialSpecular[1] = 0.55f; // g
		materialSpecular[2] = 0.45f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row3_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 3rd column 4th row
	{
		// 4th sphere on 3rd column, red
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5f; // r
		materialDiffuse[1] = 0.0f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7f; // r
		materialSpecular[1] = 0.6f; // g
		materialSpecular[2] = 0.6f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row4_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 3rd column 5th row
	{
		// 5th sphere on 3rd column, white
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.55f; // r
		materialDiffuse[1] = 0.55f; // g
		materialDiffuse[2] = 0.55f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.70f; // r
		materialSpecular[1] = 0.70f; // g
		materialSpecular[2] = 0.70f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row5_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 3rd column 6th row
	{
		// 6th sphere on 3rd column, yellow
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5f; // r
		materialDiffuse[1] = 0.5f; // g
		materialDiffuse[2] = 0.0f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.60f; // r
		materialSpecular[1] = 0.60f; // g
		materialSpecular[2] = 0.50f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.25f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col3_X;
		d3dViewport.TopLeftY = row6_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// 4th column 1st row
	{
		// 1st sphere on 4th column, black
		// ambient material
		materialAmbient[0] = 0.02f; // r
		materialAmbient[1] = 0.02f; // g
		materialAmbient[2] = 0.02f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.1f; // r
		materialDiffuse[1] = 0.1f; // g
		materialDiffuse[2] = 0.1f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.4f; // r
		materialSpecular[1] = 0.4f; // g
		materialSpecular[2] = 0.4f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row1_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 4th column 2nd row
	{
		// 2nd sphere on 4th column, cyan
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.05f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4f; // r
		materialDiffuse[1] = 0.5f; // g
		materialDiffuse[2] = 0.5f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.04f; // r
		materialSpecular[1] = 0.7f; // g
		materialSpecular[2] = 0.7f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row2_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 4th column 3rd row
	{
		// 3rd sphere on 4th column, green
		// ambient material
		materialAmbient[0] = 0.0f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.4f; // r
		materialDiffuse[1] = 0.5f; // g
		materialDiffuse[2] = 0.4f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.04f; // r
		materialSpecular[1] = 0.7f; // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row3_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 4th column 4th row
	{
		// 4th sphere on 4th column, red
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.0f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5f; // r
		materialDiffuse[1] = 0.4f; // g
		materialDiffuse[2] = 0.4f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7f; // r
		materialSpecular[1] = 0.04f; // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row4_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 4th column 5th row
	{
		// 5th sphere on 4th column, white
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.05f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5f; // r
		materialDiffuse[1] = 0.5f; // g
		materialDiffuse[2] = 0.5f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7f; // r
		materialSpecular[1] = 0.7f; // g
		materialSpecular[2] = 0.7f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row5_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}
	// 4th column 6th row
	{
		// 6th sphere on 4th column, yellow
		// ambient material
		materialAmbient[0] = 0.05f; // r
		materialAmbient[1] = 0.05f; // g
		materialAmbient[2] = 0.0f; // b
		materialAmbient[3] = 1.0f; // a

		// diffuse material
		materialDiffuse[0] = 0.5f; // r
		materialDiffuse[1] = 0.5f; // g
		materialDiffuse[2] = 0.4f; // b
		materialDiffuse[3] = 1.0f; // a

		// specular material
		materialSpecular[0] = 0.7f; // r
		materialSpecular[1] = 0.7f; // g
		materialSpecular[2] = 0.04f; // b
		materialSpecular[3] = 1.0f; // a

		// shininess
		materialShineness = 0.078125f * 128;

		// initialize and set the view // similar to openGL viewport
		ZeroMemory((void *)&d3dViewport, sizeof(D3D11_VIEWPORT));

		d3dViewport.TopLeftX = col4_X;
		d3dViewport.TopLeftY = row6_Y;
		d3dViewport.Width = (float)difference_X;
		d3dViewport.Height = (float)difference_Y;
		d3dViewport.MinDepth = 0.0f;
		d3dViewport.MaxDepth = 1.0f;

		// set above viewport in pipeline
		gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

		// transformation
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMMATRIX translationMatrix = XMMatrixIdentity();
		translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 2.0f);

		worldMatrix = translationMatrix;

		XMMATRIX viewMatrix = XMMatrixIdentity();

		CBUFFER constantBuffer;
		ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
		constantBuffer.WorldMatrix = worldMatrix;
		constantBuffer.ViewMatrix = viewMatrix;
		constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

		if (bLightingEnable == TRUE)
		{
			constantBuffer.KeyPressed = 1;

			constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0);
			constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0);
			constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0);
			constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

			constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0);
			constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0);
			constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0);
			constantBuffer.MaterialShineness = materialShineness;
		}
		else
		{
			constantBuffer.KeyPressed = 0;
		}

		// smiliar to glUniformMatrix4fv() in OpenGL
		gpID3D11DeviceContext->UpdateSubresource(
			gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		//// set position buffer into pipeline here
		UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

		//// set position buffer into pipeline here
		stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
		offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
		gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offSet);

		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'

		 //set primitive geometry
		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);
	}

	// do double buffer by presenting swapchain
	gpIDXGISwapChain->Present(0,0);
}

void update(void)
{
	//code
	// rotate

	if (bLightingEnable)
	{
		if (rotationAxis == 'x')
		{
			// x Axis
			lightPosition[0] = 0.0f;
			lightPosition[1] = (float)5.0f * sin(lightAngleZero);
			lightPosition[2] = (float)5.0f * cos(lightAngleZero);
			lightPosition[3] = 1.0f;


			lightAngleZero = lightAngleZero + 0.0005f;
			if (lightAngleZero >= 360.0f)
			{
				lightAngleZero = lightAngleZero - 360.0f;
			}
		}


		if (rotationAxis == 'y')
		{
			lightPosition[0] = (float)5.0f * sin(lightAngleOne);
			lightPosition[1] = 0.0f;
			lightPosition[2] = (float)5.0f * cos(lightAngleOne);
			lightPosition[3] = 1.0f;


			lightAngleOne = lightAngleOne + 0.0005f;
			if (lightAngleOne >= 360.0f)
			{
				lightAngleOne = lightAngleOne - 360.0f;
			}
		}

		if (rotationAxis == 'z')
		{
			lightPosition[0] = (float)5.0f * sin(lightAngleTwo);
			lightPosition[1] = (float)5.0f * cos(lightAngleTwo);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;

			lightAngleTwo = lightAngleTwo + 0.0005f;
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
	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if (gpID3D11Buffer_IndexBuffer)
	{
		gpID3D11Buffer_IndexBuffer->Release();
		gpID3D11Buffer_IndexBuffer = NULL;
	}
	if (gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
	}
	if (gpID3D11Buffer_NormalBuffer)
	{
		gpID3D11Buffer_NormalBuffer->Release();
		gpID3D11Buffer_NormalBuffer = NULL;
	}
	if (gpID3D11Buffer_PositionBuffer)
	{
		gpID3D11Buffer_PositionBuffer->Release();
		gpID3D11Buffer_PositionBuffer = NULL;
	}
	if (gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}
	if (gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	if (gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}
	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}
	if (gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();
		gpID3D11DeviceContext = NULL;
	}
	if (gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();
		gpIDXGISwapChain = NULL;
	}
	if (gpID3D11Device)
	{
		gpID3D11Device->Release();
		gpID3D11Device = NULL;
	}

	// If Application is exitting in fullscreen mode
	if (gbFullscreen == TRUE)
	{
		ToggleFullScreen();
		gbFullscreen = FALSE;
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
		fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "Program Ended Successfully\n\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
}


