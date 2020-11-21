#pragma once
#include<Windows.h>
#include"WindowControlsEx.h"



class ListView : public WindowControlsEx {
private:
	LPCWSTR CLASS_NAME = L"LIST VIEW";
	HWND _hwndStatic;
	BOOL mouseOnStatic = FALSE;

	//window procedures----------
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProcListView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	//private methods-------------
	HWND getListViewWindow();
	WNDCLASSEX* getWindowClass();
	errno_t handleStaticWindows();
	errno_t _createWindow();
	HWND getStaticWindow(LPCWSTR text, size_t posX, size_t posY);
	void handleMouseCursor();
	void handleResizer();
	void resizeStatic();
	void handleChangeJournals();


public:
	ListView(): WindowControlsEx(NULL, 0) {}
	ListView(HWND hwnd, int nCmdShow);
	errno_t initWindow();
	
	


};