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

	WindowControls(HWND hwndSelf, HINSTANCE hInst);
	HWND getEditControl(size_t posX, size_t posY, size_t width, size_t height);
	void setWindowControlsHandle(HWND hwndSelf);
	HWND getListBox(HWND hWndParent, int posX, int posY, int width, int height);
	HWND getButton(HWND hWndParent, LPCWSTR buttonText, short UID_BUTTON, size_t posX, size_t posY);
	void applyConsistentStyle(HWND hwnd);
	HWND getStatic(HWND hWndParent, LPCWSTR buttonText, size_t posX, size_t posY);

	 
	 //template<size_t size>
	 //errno_t _insertItems_listBox(HWND hWnd, int select, const wchar_t* (&itemsArray)[size]);
	 
public:
	HFONT getFont(size_t size, LPCWSTR fontFamily = L"Microsoft New Tai Lue", LPCWSTR fontStyle = L"Regular");
};
