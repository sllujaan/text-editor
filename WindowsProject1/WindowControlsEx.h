#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include"AppLog.h"


#define TASK_SUCCESS 0
#define TASK_FAILURE 1


struct WINDOW_CONFIG {
	LPCWSTR title;
	HICON hIcon;
	size_t width;
	size_t height;
	WNDPROC wndproc;
};



class WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"WINDOW";

	
	errno_t centerWindow(HWND hwnd);

	static LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:
	HWND _hwndParent;
	HWND _hwndSelf;
	HINSTANCE _hInst;
	int _nCmd;

	BOOL canCreateWindow();
	void showWindowCreationError();
	errno_t registerWindow(WNDCLASSEX& wcex);
	errno_t createWindow(WINDOW_CONFIG& windConfig);
	errno_t applyConsistentStyle(HWND hwnd);
	HFONT getFont(size_t size, LPCWSTR fontFamily = L"Microsoft New Tai Lue", LPCWSTR fontStyle = L"Regular");
	HWND getListViewDetailsControl();
	errno_t InitListViewColumns(HWND hWndListView);
	errno_t insertListViewDetailsItems(HWND hWndListView);

public:
	WindowControlsEx() = delete;
	WindowControlsEx(HWND hwnd, int nCmdShow);
	errno_t createNewWindow(WINDOW_CONFIG& windConfig);
	errno_t createNewWindowEx(WNDCLASSEX& wcex);
	errno_t registerWindow();


};