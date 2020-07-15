#pragma once
#include<Windows.h>
#include<CommCtrl.h>

class AppSettings {
private:
	//private stuff
	HWND hWndSettings;
	HWND hWndParent;
	HINSTANCE hInst;
	int nCmdShowGlobal;


public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	~AppSettings();

private:
	//private stuff
	void createComboBox();
	int CALLBACK createWindow();
	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void centerWindow(HWND hwnd);


};