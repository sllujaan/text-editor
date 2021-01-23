#pragma once
#include<Windows.h>
#include<string>
using namespace std;

#ifdef WIN32_DEBUG

#ifndef LOG_WCHAR 
#define LOG_WCHAR(x) OutputDebugStringW((LPCWSTR)x); \
			OutputDebugStringW((LPCWSTR)L"\r\n") //for new line
#endif // !LOG(x)

#ifndef LOG_INT 
#define LOG_INT(x) {\
	wstring ws = to_wstring(x); \
	OutputDebugStringW((LPCWSTR)ws.c_str()); \
	OutputDebugStringW((LPCWSTR)L"\r\n");}
#endif // !LOG_INT 

#ifndef LOG_STR
#define LOG_STR(x) {\
	wstring ws = wstring(x.begin(), x.end()); \
	OutputDebugStringW((LPCWSTR)ws.c_str()); \
	OutputDebugStringW((LPCWSTR)L"\r\n");}
#endif // !LOG_STR

#ifndef LOG_CHAR
#define LOG_CHAR(x) {\
	string s(1, x);\
	LOG_STR(s);}
#endif // !LOG_CHAR

#else
#define LOG_WCHAR
#define LOG_INT
#define LOG_STR
#define LOG_CHAR
#endif // WIN32_DEBUG


void LOG_LAST_ERROR();