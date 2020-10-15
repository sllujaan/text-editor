#pragma once
#include<Windows.h>
#include"WindowControlsEx.h"



class ListView : public WindowControlsEx {
private:
	
public:
	ListView(): WindowControlsEx(NULL, 0) {}
	ListView(HWND hwnd, int nCmdShow);
};