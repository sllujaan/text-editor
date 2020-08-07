#pragma once

#include<Windows.h>
#include<Richedit.h>

#define IDI_SEARCH_BUTTON 0xcb11

class Search {
private:
	//private stuff
	HWND hWndParent;
	HWND hWndSearch;
	HWND hWndEditControl;
	HWND hwndButton;
	HWND hwndRichEditParent;
	HINSTANCE hInst;
	int nCmdShowGlobal;

	const wchar_t* CLASS_NAME = L"Search Window";

	WNDPROC lpfnMainWndProc;

public:
	//public stuff
	Search(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal, HWND richEdit);
	Search() {}
	~Search();
	void registerWindow();
	void initWindow();


private:
	void createWindow();
	void centerWindow(HWND hwnd);
	void initEditControl();
	void initSearchButton();
	HFONT getFont(size_t size);
	void handleEnableDisableSearchButton();
	void handleSearchText();

protected:

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcSearch(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


};


