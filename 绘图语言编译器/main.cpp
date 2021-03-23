#pragma warning(disable:4996)
#include "语义分析器.h"
#include <tchar.h> 

#define MAX_CHARS 200

extern HDC hDC;						// 窗口句柄，全局变量
char SrcFilePath[MAX_CHARS];		// 用于存放源程序文件路径
TCHAR Name[] = _T("绘图语言编译器");

static bool CheckSrcFile(LPSTR);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = _T("绘图语言编译器");

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nFunsterStil)

{
	HWND hwnd;              
	MSG messages;     
	WNDCLASSEX wincl; 
	int i;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure; 
	wincl.style = CS_DBLCLKS;             
	wincl.cbSize = sizeof(WNDCLASSEX);


	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                
	wincl.cbClsExtra = 0;                     
	wincl.cbWndExtra = 0;  						
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;


	if (!RegisterClassEx(&wincl))
		return 0;

	hwnd = CreateWindowEx(
		0,
		szClassName,        
		_T("绘图语言编译器"),      
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,      
		CW_USEDEFAULT,      
		1000,               
		700,                
		HWND_DESKTOP,        
		NULL,               
		hThisInstance,       
		NULL                 
	);

	ShowWindow(hwnd, nFunsterStil);
	hDC = GetDC(hwnd);

	strcpy(SrcFilePath, "test1.txt");

	if (!CheckSrcFile(SrcFilePath)) return 1;
	Parser(SrcFilePath);
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)        
	{
	case WM_DESTROY:
		PostQuitMessage(0);      
		break;
	default:              
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
//检查源程序文件是否合法函数实现
bool CheckSrcFile(LPSTR lpszCmdParam)
{
	FILE * file = NULL;

	if (strlen(lpszCmdParam) == 0)
	{
		MessageBoxA(NULL, "未指定源程序文件 !", "错误", MB_OK);
		return false;
	}
	if ((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		MessageBoxA(NULL, "打开源程序文件出错 !", "错误", MB_OK);
		MessageBoxA(NULL, lpszCmdParam, "文件名", MB_OK);
		return false;
	}
	else fclose(file);
	return true;
}