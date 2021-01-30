#pragma once



#define CAT_NAME(X, Y) L#X" "#Y "\r\n"

#define TASK_SUCCESS 0
#define TASK_FAILURE 1



#ifdef CONSOLE_APP 
//we are running console application
#ifndef LOG_INT
#define LOG_INT(x)
#endif // LOG_INT


#ifdef CONSOLE_DEBUG
#define LOG(X) std::cout << X << std::endl
#define LOG_ANY(X) X
#else
#define LOG(X)
#define LOG_ANY(X)
#endif // CONSOLE_DEBUG

#else
//we are running win32 application
#ifdef WIN32_DEBUG

#ifndef LOG_INT
#define LOG_INT(x) {\
	wstring ws = to_wstring(x); \
	OutputDebugStringW((LPCWSTR)ws.c_str());
#endif // !LOG_INT


#define LOG(X) OutputDebugStringW(X)
#define LOG_ANY(X) X

#else
#define LOG_INT(X)
#define LOG(X)
#define LOG_ANY(X)
#endif

#endif // UNICODE





