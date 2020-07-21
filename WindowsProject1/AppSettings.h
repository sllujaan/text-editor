#pragma once
#include<Windows.h>
#include<CommCtrl.h>



class AppSettings {

private:

protected:
	//private stuff
	HWND hWndSettings;
	HWND hWndParent;
	HINSTANCE hInst;
	int nCmdShowGlobal;



public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	AppSettings() {};
	~AppSettings();

private:
	//private stuff
	void createComboBox();
	void createWindow();	
	static void centerWindow(HWND hwnd);
	void initListFontSize();


protected:


	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return ((AppSettings*)(::GetWindowLongPtr(hwnd, GWLP_USERDATA)))->runProc(hwnd, message, wParam, lParam);
	};

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};
