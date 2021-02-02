#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include<WinUser.h>
#include<fileapi.h>
#include<Shlwapi.h>
#include<thread>
//#include<winioctl.h>
#include"WindowControlsEx.h"
#include"resource.h"
#include"FileEx.h"

#include"File/File.h"
#include"Log.h"

#include<vector>


using namespace std;



class TreeView : public WindowControlsEx {


private:
	LPCWSTR CLASS_NAME = L"TREEVIEW";
	HWND _hwndTV;
	INT imgIndex_folderOpen = 0;
	INT imgIndex_folderClosed = 0;
	thread _thread1;
	HANDLE _hChangeHandle[2];

	MY_FILES::FILE_TREE _fileTree;

	//tree item selected record.
	MY_FILES::LP_FILE_TREE_STORE _treeItemSelectedRecord;
	


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
	errno_t handleRightClick(LPARAM lParam);
	errno_t initRightClickMenu(MY_FILES::LP_FILE_TREE_STORE treeItemRecord);
	errno_t createContextMenuPopUp(MY_FILES::FILE_TYPE);
	errno_t handleTVItemSelectChange();
	errno_t watchDir();
	void handleChangeJournals();
	void FindExtension();
	MY_FILES::LP_FILE_TREE_STORE findTreeViewItemRecord(HTREEITEM _hTreeItem);

public:
	TreeView() : WindowControlsEx(NULL, 0) {}
	TreeView(HWND hwnd, int nCmdShow);
	~TreeView();
	errno_t initWindow();

};