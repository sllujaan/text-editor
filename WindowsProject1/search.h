#pragma once

#include<Windows.h>



class Search {
private:
	//private stuff
	HWND hWndParent;
	HWND hWndSearch;
	HINSTANCE hInst;
	int nCmdShowGlobal;
	static const int IDI_EDIT_CONTROL = 0xcb11;

public:
	//public stuff
	Search(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	~Search();


private:
	int CALLBACK createWindow();
	
	static LRESULT CALLBACK WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void centerWindow(HWND hwnd);
	void initEditControl();
	void initSearchButton();
	HFONT getFont(size_t size);
};


