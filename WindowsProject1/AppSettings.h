#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include <string>

#define IDM_CODE_SAMPLES 0x3e0f



class AppSettings {

private:

protected:
	//private stuff
	HWND hWndSettings;
	HWND hWndParent;
	HWND hwndListView;
	HWND hWndGroupBox;
	HWND hWndGroupBoxSample;
	HWND hWndGroupBoxSampleText;
	HINSTANCE hInst;
	int nCmdShowGlobal;
	HWND hWndListBox_FontSize;

	//const wchar_t* arr = L"ab";

	const wchar_t* CLASS_NAME = L"Settings Window";

	WNDPROC oldProc;

public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	AppSettings() {};
	~AppSettings();
	void registerWindow();
	void initWindow();

private:
	//private stuff
	void createComboBox();
	void createWindow();	
	static void centerWindow(HWND hwnd);
	void initListFontSize();
	void createListView();
	void insertListViewItems(int cItems);
	void initListView();
	void createGroupBox();
	void createGroupBoxSample();
	void initListViewSample();
	void initSampleText();
	void createTooltilp();
	void HandleWM_NOTIFY(LPARAM lParam);
	int getFontSizeFromIndex(int index);
	void handleFocuses();
	void createListBox();
	void initListViewBox();

	HWND getGroupBox(LPCWSTR name, int posX, int posY, int width, int height);

	HFONT getFont(size_t size);


protected:

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK SubClassListViewProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	
};
