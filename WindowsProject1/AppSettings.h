#pragma once
#include<Windows.h>
#include<CommCtrl.h>


class AppSettings {
private:
	//private stuff
	HWND hWndParent;
	HINSTANCE hInst;
	int nCmdShowGlobal;


public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);

private:
	//private stuff
	void createComboBox();
	int CALLBACK createWindow();
	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

};