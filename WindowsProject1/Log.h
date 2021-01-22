#pragma once


#ifdef CP_DEBUG
#define LOG(X) std::cout << X << std::endl
#define LOG_ANY(X) X
#elif WIN32_DEBUGa
#define LOG(X) OutputDebugStringW(X);
#define LOG_ANY(X) X
#else
#define LOG(X)
#define LOG_ANY(X)
#endif


#define TASK_SUCCESS 0
#define TASK_FAILURE 1


#define CAT_NAME(X, Y) L#X" "#Y

