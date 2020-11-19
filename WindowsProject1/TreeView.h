#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include<WinUser.h>
#include<fileapi.h>
#include<thread>
#include"WindowControlsEx.h"
#include"resource.h"

using namespace std;

class TreeView : public WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"TREEVIEW";
	HWND _hwndTV;
	INT imgIndex_folderOpen = 0;
	INT imgIndex_folderClosed = 0;


	//window procedures----------
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProcTreeView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//private methods------------
	WNDCLASSEX* getWindowClass();
	errno_t _createWindow();
	HWND getTreeViewWindow();
	errno_t initTreeViewControl();
	BOOL InitTreeViewImageLists(HWND hwndTV);
	HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel, HTREEITEM _hPrev, INT imageIndex);
	errno_t handleTVItemSelectChange();
	errno_t watchDir();

public:
	TreeView() : WindowControlsEx(NULL, 0) {}
	TreeView(HWND hwnd, int nCmdShow);
	errno_t initWindow();

};