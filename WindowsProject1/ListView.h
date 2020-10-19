#pragma once
#include<Windows.h>
#include"WindowControlsEx.h"



class ListView : public WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"LIST VIEW";

	static LRESULT CALLBACK wndProcListView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
public:
	ListView(): WindowControlsEx(NULL, 0) {}
	ListView(HWND hwnd, int nCmdShow);
	errno_t _createWindow(WINDOW_CONFIG& wndConfig);
	WNDCLASSEX* getWindowClass();


};