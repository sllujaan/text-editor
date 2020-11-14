#pragma once
#include<Windows.h>
#include"WindowControlsEx.h"

class TreeView : public WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"TREEVIEW";

	//window procedures----------
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProcTreeView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//private methods------------
	WNDCLASSEX* getWindowClass();
	errno_t _createWindow();
	HWND getTreeViewWindow();

public:
	TreeView() : WindowControlsEx(NULL, 0) {}
	TreeView(HWND hwnd, int nCmdShow);
	errno_t initWindow();

};