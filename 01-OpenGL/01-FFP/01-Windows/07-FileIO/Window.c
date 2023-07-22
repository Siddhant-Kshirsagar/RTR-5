//windows header file
#include<windows.h>
#include<stdio.h>	// For File IO
#include<stdlib.h>	// For exit

//Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declaration
FILE *gpFILE = NULL; // gp = global pointer FILE = Datatype 

//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//local variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SGKWindow");

	//code
	gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Open"), TEXT("Error"), MB_OK|MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully\n");	// print string in file 

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
	switch (iMsg)
	{
	case WM_DESTROY:
		if (gpFILE)
		{
			fprintf(gpFILE, "Program Ended Successfully\n");
			fclose(gpFILE);
			gpFILE = NULL;
		}
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
