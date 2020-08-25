#pragma once

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

