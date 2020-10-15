#pragma once
#include<Windows.h>
#include"AppLog.h"


#define TASK_SUCCESS 0
#define TASK_FAILURE 1



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
	errno_t createNewWindow();
	
};