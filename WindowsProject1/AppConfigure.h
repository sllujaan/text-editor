#pragma once
#include<Windows.h>
#include<string>
using namespace std;


class appConfig {

private:
	const wchar_t* appDir = L"\\TextEditor.Dev\\";

public:
	appConfig() {}
	LPCWSTR getAppConfigPath();
	errno_t getAppConfigPath_secure(wchar_t** buffer, size_t* buffCount);

	
};