#pragma once
#include<Windows.h>
#include"AppLog.h"


#define TASK_SUCCESS 0
#define TASK_FAILURE 1


struct WINDOW_CONFIG {
	LPCSTR title;
	HICON hIcon;
	size_t width;
	size_t height;
};



class WindowControlsEx {
private:

	BOOL canCreateWindow();
	void showWindowCreationError();


protected:
	HWND _hwndParent;
	HWND _hwndSelf;
	HINSTANCE _hInst;
	int _nCmd;



public:
	WindowControlsEx() = delete;
	WindowControlsEx(HWND hwnd, int nCmdShow);
	errno_t createNewWindow(WINDOW_CONFIG *windConfig);
	errno_t createNewWindowEx(WNDCLASSEX* wcex);
};