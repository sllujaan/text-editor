#pragma once
#include<Windows.h>

class windowManager {
private:
	HWND _hwnd_self;
	RECT rectWindow;

public:
	windowManager(HWND hWnd);
	size_t getWndWidth();
	size_t getWndHeight();

};