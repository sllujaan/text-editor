#pragma once
#include<Windows.h>

class WindowControls {
private:
	HWND _hwndSelf;
	HINSTANCE _hInst;
	BOOL canCreateWindow();
	void showWindowCreationError();
protected:
	 HWND getEditControl(size_t posX, size_t posY, size_t width, size_t height);
	 void setWindowControlsHandle(HWND hwndSelf);
	 HFONT getFont(size_t size, LPCWSTR fontFamily = L"Microsoft New Tai Lue" );
public:
	WindowControls(HWND hwndSelf, HINSTANCE hInst);
};