#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include<WinUser.h>
#include"WindowControlsEx.h"
#include"resource.h"

class TreeView : public WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"TREEVIEW";
	HWND _hwndTV;

	//window procedures----------
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProcTreeView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//private methods------------
	WNDCLASSEX* getWindowClass();
	errno_t _createWindow();
	HWND getTreeViewWindow();
	errno_t initTreeViewControl();
	BOOL InitTreeViewImageLists(HWND hwndTV);
	HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel, HTREEITEM _hPrev);
	errno_t handleTVItemSelectChange();

public:
	TreeView() : WindowControlsEx(NULL, 0) {}
	TreeView(HWND hwnd, int nCmdShow);
	errno_t initWindow();

};