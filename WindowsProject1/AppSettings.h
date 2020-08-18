#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include <string>
using namespace std;

#define IDM_CODE_SAMPLES 0x3e0f


#define ERROR_CONFIG 1
#define SUCCESS_CONFIG 0




#ifndef LOG_WCHAR 
#define LOG_WCHAR(x) OutputDebugStringW((LPCWSTR)x); \
			OutputDebugStringW((LPCWSTR)L"\r\n") //for new line
#endif // !LOG(x)

#ifndef LOG_INT 
#define LOG_INT(x) \
do { \
	wstring ws = to_wstring(x); \
	OutputDebugStringW((LPCWSTR)ws.c_str()); \
	OutputDebugStringW((LPCWSTR)L"\r\n"); /*for new line*/ \
} \
while (0);
#endif // !LOG_INT 

#ifndef LOG_STR
#define LOG_STR(x) \
do { \
	wstring ws = wstring(x.begin(), x.end()); \
	OutputDebugStringW((LPCWSTR)ws.c_str()); \
	OutputDebugStringW((LPCWSTR)L"\r\n"); /*for new line*/ \
} \
while (0);
#endif // !LOG_STR






//inline void logNumValue(double number) {
//	wstring ws = to_wstring(number);
//	OutputDebugStringW((LPCWSTR)ws.c_str());
//	OutputDebugStringW((LPCWSTR)L"\r\n"); //for new line
//}

#ifndef LOG 
	#define LOG(x) OutputDebugStringW((LPCWSTR)x); \
			OutputDebugStringW((LPCWSTR)L"\r\n") //for new line
#endif // !LOG(x)

#ifndef LOG_INT 
#define LOG_INT(x) \
	wstring ws = to_wstring(x); \
	OutputDebugStringW((LPCWSTR)ws.c_str()); \
	OutputDebugStringW((LPCWSTR)L"\r\n"); //for new line
#endif // !LOG_INT 




class AppSettings {
	//attributes--------
private:

	//data for font families-----------
	LPCWSTR fontFamilies[18] = {
		L"Arial", L"Arial Black", L"Calibri", L"Cambria", L"Cambria Math", L"MS New Tai Lue",
		L"MS PhagsPa", L"Times New Roman", L"DaunPenh", L"Georgia Pro", L"Shonar Bangla", L"Vrinda",
		L"Simplified Arabic", L"Sakkal Majalla", L"Andalus", L"Yu Gothic", L"Tahoma", L"SimSun"
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

	size_t fontSize = 14;
	LPCWSTR fontFamily = L"Arial Black";
	LPCWSTR fontSyle = L"Regular";

	size_t _fSizeIndex = 0;
	size_t _fFamilyIndex = 0;
	size_t _fStyleIndex = 0;

	size_t _fSizeTotalItems = 64;
	size_t _fFamilyTotalItems = ARRAYSIZE(this->fontFamilies);
	size_t _fStyleTotalItems = 5;

	//const wchar_t* arr = L"ab";

	const wchar_t* CLASS_NAME = L"Settings Window";

	WNDPROC oldProc;
	WNDPROC oldProc_EC_fontFamily;


	//methods----------------

public:
	//public stuff
	AppSettings(HWND hWnd, HINSTANCE hInst, int nCmdShowGlobal);
	AppSettings() {};
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
	void setSampleTextFontSize(size_t size);
	void handleListBoxSelectionChange(HWND hWnd);
	HWND getListBox(HWND hWndParent, int posX, int posY, int width, int height);
	//fontStyles methods--
	void createComboBox();
	void createEditControlFontStyles();
	void createListBox_FontStyles();
	size_t getFontSizeSampleText();
	LPCWSTR getSelectedFontFamily();
	BOOL isValidIndex(size_t totalItems, size_t index);
	void showConfigKeysCorrupted();

	void handleSearchControls(HWND hWnd);
	

	HWND getGroupBox(LPCWSTR name, int posX, int posY, int width, int height);

	HFONT getFont(size_t size, LPCWSTR fontFamily = L"Microsoft New Tai Lue");


protected:

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK SubClassEditControl(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK LB_FontSize_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK EC_fontFamily_WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //EC is edit control
};
