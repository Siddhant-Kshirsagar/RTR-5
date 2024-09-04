//windows header file
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>

// d3d related header file
#include<d3d11.h>
#include<d3dcompiler.h>

#pragma warning(disable:4838)
#include"XNAMath_204/xnamath.h"
#include"D3D.h"

// d3d related library
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

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
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer = NULL;
ID3D11RasterizerState *gpID3D11RasterizerState = NULL;


struct CBUFFER
{
	XMMATRIX WorldViewProjectionMatrix; // similar to OpenGL model is equal to 'world' word
	XMMATRIX perspectiveProjectionMatrix; 
};

XMMATRIX perspectiveProjectionMatrix;

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

	// Vertex Shader
	const char *vertexShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldViewProjectionMatrix;" \
		"}" \
		"float4 main(float4 pos:POSITION) : SV_POSITION" \
		"{" \
		"float4 position = mul(worldViewProjectionMatrix,pos);" \
		"return(position);" \
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
		"float4 main(void): SV_TARGET" \
		"{"
		"float4 color = float4(1.0f,1.0f,1.0f,1.0f);"\
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
	D3D11_INPUT_ELEMENT_DESC d3dInputElementDesc;
	ZeroMemory((void *)&d3dInputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

	d3dInputElementDesc.SemanticName = "POSITION";
	d3dInputElementDesc.SemanticIndex = 0;
	d3dInputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc.InputSlot = 0;
	d3dInputElementDesc.AlignedByteOffset = 0;
	d3dInputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc.InstanceDataStepRate = 0;

	// using above structure create input layout
	hr = gpID3D11Device->CreateInputLayout(&d3dInputElementDesc, 1, pID3DBlob_VertexShaderSourceCode->GetBufferPointer(), pID3DBlob_VertexShaderSourceCode->GetBufferSize(), &gpID3D11InputLayout);

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
	const float rectangle_position[] =
	{
		-1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
	};

	// create vertex buffer for position
	D3D11_BUFFER_DESC d3d11BufferDesc;
	ZeroMemory((void *)&d3d11BufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3d11BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // similar to openGL GL_DYNAMIC_DRAW
	d3d11BufferDesc.ByteWidth = sizeof(rectangle_position);
	d3d11BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3d11BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// another method pass as second parameter to CreateBuffer
	//D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	//ZeroMemory((void *)&d3d11SubresourceData, sizeof(d3d11SubresourceData));
	//d3d11SubresourceData.pSysMem = triangl_Position;

	// create vertex buffer using above structure
	hr = gpID3D11Device->CreateBuffer(&d3d11BufferDesc, NULL, &gpID3D11Buffer_PositionBuffer);
	if (FAILED(hr))
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() failed for vertex buffer\n\n");
		fclose(gpFILE);
		return(hr);
	}
	else
	{
		gpFILE = fopen(gszLogFileName, "a+");
		fprintf(gpFILE, "CreateBuffer() succeeded for vertex buffer\n\n");
		fclose(gpFILE);
	}

	// we will set above buffer into pipeline not here but in display because of our buffer is dynamic
	// 
	// copy manually data from CPU buffer to GPU buffer
	D3D11_MAPPED_SUBRESOURCE d3dMappedSubresource;
	ZeroMemory((void *)&d3dMappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource
	gpID3D11DeviceContext->Map(gpID3D11Buffer_PositionBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&d3dMappedSubresource);

	// now copy actual data
	memcpy(d3dMappedSubresource.pData, rectangle_position, sizeof(rectangle_position));

	// unmap position buffer
	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_PositionBuffer, 0);

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
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 1.0f;
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

HRESULT resize(int width, int height)
{
	// variable declaration
	HRESULT hr = S_OK;

	//code
	if (height <= 0)
	{
		height = 1;
	}

	// release render target view
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();
		gpID3D11RenderTargetView = NULL;
	}

	// resize swap chain buffer according to the changed 
	gpIDXGISwapChain->ResizeBuffers(1,width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// create new render target view
	// a) get the buffer for rtv from swapchain into the texture
	ID3D11Texture2D *piD3D11Texture2D = NULL;
	gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&piD3D11Texture2D);

	// b) create RTV using above buffer
	hr = gpID3D11Device->CreateRenderTargetView(piD3D11Texture2D,NULL,&gpID3D11RenderTargetView);
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
	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);
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

	// initialize perspective projection

	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	return(hr);
}

void display(void)
{
	//code
	// similar to clear color in openGL
	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, clearColor);
	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// transformation
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 3.0f);

	XMMATRIX viewMatrix = XMMatrixIdentity();
	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	CBUFFER constantBuffer;
	ZeroMemory((void *)&constantBuffer, sizeof(CBUFFER));
	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	// smiliar to glUniformMatrix4fv() in OpenGL
	gpID3D11DeviceContext->UpdateSubresource(
		gpID3D11Buffer_ConstantBuffer, 0, NULL,&constantBuffer, 0, 0);

	
	// set position buffer into pipeline here
	UINT stride = sizeof(float) * 3; // similar to glVertexAttribPointer() 3rd parameter
	UINT offSet = NULL;// similar to glVertexAttribPointer() 6th parameter
	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offSet);

	// set primitive geometry
	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// draw the geometry
	gpID3D11DeviceContext->Draw(4, 0);

	// do double buffer by presenting swapchain
	gpIDXGISwapChain->Present(0,0);
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
	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}
	if (gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
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


