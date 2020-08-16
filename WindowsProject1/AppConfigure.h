#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include<comdef.h>
#include<regex>
using namespace std;




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
	private:
		//private stuff
		fstream file;
		wchar_t* filePath;
		

	public:
		//public stuff
		FILE(const wchar_t* path);
		BOOL isFile();
		errno_t getKeyValue();//wchar_t** destination, const wchar_t* key
		errno_t CLOSE();
		
	};
};