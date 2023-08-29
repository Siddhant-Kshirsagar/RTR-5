//windows header file
#include<windows.h>

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//local variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SGKWindow");

	//WNDCLASSEX Initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register WNDCLASSEX
	RegisterClassEx(&wndclass);

	//Create Window
	hwnd = CreateWindow(szAppName,
		TEXT("Siddhant Ganesh Kshirsagar"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//show window
	ShowWindow(hwnd, iCmdShow);

	//Paint/Redraw Window
	UpdateWindow(hwnd);

	//Message Loop
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

//Callback function defination
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[256];
	int iNum1, iNum2, iResult;
	iNum1 = 10;
	iNum2 = 5;
	iResult = 15;
	wsprintf(str, "Addition Of %d + %d = %d", iNum1, iNum2, iResult);
	switch (iMsg)
	{
	case WM_CREATE:
		MessageBox(hwnd, str, TEXT("Message"), MB_OK);
		break;
	case WM_SIZE:
		MessageBox(hwnd, TEXT("WM_SIZE Message Arrived"), TEXT("Message"), MB_OK);
		break;
	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			MessageBox(hwnd, TEXT("WM_KEYDOWN -> VK_ESCAPE Message Arrived"), TEXT("Message"), MB_OK);
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
			MessageBox(hwnd, TEXT("WM_CHAR -> 'F'/'f' Key Pressed"), TEXT("Message"), MB_OK);
			break;
		default:
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("Left Mouse Button is Clicked"), TEXT("Message"), MB_OK);
		break;
	case WM_RBUTTONDOWN:
		MessageBox(hwnd, TEXT("Right Mouse Button is Clicked"), TEXT("Message"), MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
