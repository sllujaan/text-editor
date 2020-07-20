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
	static void centerWindow(HWND hwnd);
	void initListFontSize();


protected:
	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	AppSettings* settings;
};