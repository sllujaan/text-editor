#pragma once
#include<Windows.h>
#include<CommCtrl.h>

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
	void createListView();
	void insertListViewItems(int cItems);
	void initListView();
	void createGroupBox();
	void createGroupBoxSample();
	void initListViewSample();
	void initSampleText();

	HWND getGroupBox(LPCWSTR name, int posX, int posY, int width, int height);

	HFONT getFont(size_t size);


protected:

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	
};
