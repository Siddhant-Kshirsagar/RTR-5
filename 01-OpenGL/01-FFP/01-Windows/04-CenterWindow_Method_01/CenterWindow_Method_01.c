// Windows header file
#include<windows.h>

// Global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//local variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SGKMyWindow");
	int SystemWindowWidth, SystemWindowHeight;
	int UserWindowWidth = 800, UserWindowHeight = 600;
	int xCoordinate, yCoordinate;

	// Initialize wndclass
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register wndclass
	RegisterClassEx(&wndclass);

	// Get System Window Height and Width
	SystemWindowWidth = GetSystemMetrics(SM_CXSCREEN);
	SystemWindowHeight = GetSystemMetrics(SM_CYSCREEN);

	// Get Center Of window x and y coordinate
	xCoordinate = (SystemWindowWidth / 2) - (UserWindowWidth/2);
	yCoordinate = (SystemWindowHeight / 2) - (UserWindowHeight/2);


	// create window
	hwnd = CreateWindow(szAppName,
		TEXT("Siddhant Ganesh Kshirsagar"),
		WS_OVERLAPPEDWINDOW,
		xCoordinate,
		yCoordinate,
		UserWindowWidth,
		UserWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// paint/redraw Window
	UpdateWindow(hwnd);

	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

// Callback function defination
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
