#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include<comdef.h>
#include<regex>
using namespace std;

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







class appConfig {

private:
	const wchar_t* appDir = L"\\TextEditor.Dev\\config.txt";

public:
	appConfig() {}
	LPCWSTR getAppConfigPath();
	errno_t getAppConfigPath_secure(wchar_t** buffer, size_t* buffCount);

	
};


namespace config {
	class FILE {
	//attributes-------------------
	private:
		//private stuff
		fstream file;
		wchar_t* filePath;
		string fileText;
		string readText();

		//config keys
		string _key_fSize = "fontSizeIndex";
		string _key_fFamily = "fontFamilyIndex";
		string _key_fStyle = "fontStyleIndex";

	public:
		int fontSizeIndex = -1;
		int fontFamilyIndex = -1;
		int fontStyleIndex = -1;

	//methods-----------------

	public:
		//public stuff
		FILE(const wchar_t* path);
		FILE() {}
		BOOL isFile();
		errno_t getKeyValue(string key, int* value);//wchar_t** destination, const wchar_t* key
		errno_t CLOSE();
		wstring getText();
		errno_t getKeyValueTemp(string key, int* value);
		errno_t initReadConfigKeys();
		void resetConfigKeys();

	};
};