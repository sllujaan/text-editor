#pragma once
#include<Windows.h>

class WindowControls {
private:
	HWND _hwndSelf;
	HINSTANCE _hInst;
	BOOL canCreateWindow();
	void showWindowCreationError();
protected:
	size_t fontSize = 14;
	LPCWSTR fontFamily = L"Arial Black";
	LPCWSTR fontSyle = L"Regular";



	 HWND getEditControl(size_t posX, size_t posY, size_t width, size_t height);
	 void setWindowControlsHandle(HWND hwndSelf);
	 HFONT getFont(size_t size, LPCWSTR fontFamily = L"Microsoft New Tai Lue" );
	 HWND getListBox(HWND hWndParent, int posX, int posY, int width, int height);

	 WindowControls(HWND hwndSelf, HINSTANCE hInst);
	 //template<size_t size>
	 //errno_t _insertItems_listBox(HWND hWnd, int select, const wchar_t* (&itemsArray)[size]);
	 
public:
	
};
