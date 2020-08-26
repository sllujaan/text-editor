#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include <string>
#include<thread>

#include"AppLog.h"
#include"WindowControls.h"
using namespace std;

#define IDM_CODE_SAMPLES 0x3e0f
#define WM_APPLY_CONFIGURATION 0x22eb

//hexadecimals
//193d
//c7f2

#define ERROR_CONFIG		1
#define SUCCESS_CONFIG		0
#define UID_BUTTON_OK		0x7a6b
#define UID_BUTTON_CANCEL	0x37b0






class AppSettings: public WindowControls {
	//attributes--------
public:

	size_t _fSizeIndex = 0;
	size_t _fFamilyIndex = 0;
	size_t _fStyleIndex = 0;

	//data for font families-----------
	LPCWSTR fontFamilies[18] = {
		L"Arial", L"Arial Black", L"Calibri", L"Cambria", L"Cambria Math", L"MS New Tai Lue",
		L"MS PhagsPa", L"Times New Roman", L"DaunPenh", L"Georgia Pro", L"Shonar Bangla", L"Vrinda",
		L"Simplified Arabic", L"Sakkal Majalla", L"Andalus", L"Yu Gothic", L"Tahoma", L"SimSun"
	};

	//data for font Styles-----------
	LPCWSTR fontStyles[5] = {
		L"Light", L"Regular", L"Bold", L"Italic", L"Bold Italic",
	};

protected:
	//private stuff
	HWND hWndSettings;
	HWND hWndParent;
	HWND hwndListView;
	HWND hWndGroupBox;
	HWND hWndGroupBoxSample;
	HWND hWndGroupBoxSampleText;
	HWND hWndEditControlFontStyles;
	HINSTANCE hInst;
	int nCmdShowGlobal;
	HWND hWndListBox_FontSize;

	//handle to fontFamily
	HWND _hwnd_listBox_fontFamily;

	//handles to fontSize.
	HWND _hwnd_editControl_FontSize;
	HWND _hwnd_listBox_fontSize;
	WNDPROC _ec_FontSize_oldProc;
	WNDPROC _LB_FontSize_oldProc;

	//handles to fontStyles-
	HWND _hwnd_listBox_fontStyes;
	WNDPROC _ec_FontStyle_oldProc;

	//test edit control
	HWND _hwnd_editControlTest;
	WNDPROC _ec_test_oldProc;


	



	size_t _fSizeTotalItems = 65;
	size_t _fFamilyTotalItems = ARRAYSIZE(this->fontFamilies);
	size_t _fStyleTotalItems = ARRAYSIZE(this->fontStyles);

	//const wchar_t* arr = L"ab";

	const wchar_t* CLASS_NAME = L"Settings Window";

	WNDPROC oldProc;
	WNDPROC oldProc_EC_fontFamily;


	//methods----------------

public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	AppSettings() :WindowControls(NULL, NULL) {};
	~AppSettings();
	void registerWindow();
	void initWindow();
	void setSettings(size_t fonstSizeIndex, size_t fontFamilyIndex, size_t fontSyleIndex);
	size_t getFontSize();
	LPCWSTR getFontFamily();
	LPCWSTR getFontStyle();

private:
	//private stuff
	void createWindow();	
	static void centerWindow(HWND hwnd);
	void initListFontSize();
	//void createListView();
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
	void initListViewBox(HWND hListBox);
	void setSampleTextFontSize(size_t size);
	void handleListBoxSelectionChange(HWND hWnd);
	//fontStyles methods--
	void createComboBox();
	void createEditControlFontStyles();
	void createListBox_FontStyles();
	size_t getFontSizeSampleText();
	void handleCopyTextToEditControl(HWND hwndListBox, HWND hwndEdit);
	void configureEditCtrlsText();
	void updateSampleText();
	void handleSaveConfigs();

	BOOL isValidIndex(size_t totalItems, size_t index);
	void showConfigKeysCorrupted();

	void handleSearchControls(HWND hwndEdit, HWND hwndListBox);
	void createEditControlTest();
	void _createListBox_fontStyles();
	template<size_t size>
	errno_t _insertItems_listBox(HWND hWnd, int select, const wchar_t* (&itemsArray) [size]);
	

	HWND getGroupBox(LPCWSTR name, int posX, int posY, int width, int height);
	void createOKButton();
	void createCancelButton();
	void createStaticsControls();
	void createEditControlFontSize();



protected:

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK SubClassEditControl(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK LB_FontSize_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK EC_fontFamily_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //EC is edit control

	static LRESULT CALLBACK _ec_test_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //EC is edit control

	static LRESULT CALLBACK _ec_FontSize_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //EC is edit control

	static LRESULT CALLBACK _ec_FontStyle_proc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //LB is listBox


};

struct _configVars : AppSettings
{
	int fontSizeIndex;
	int fontSizeFamilyIndex;
	int fontSizeStyleIndex;
};