#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include<comdef.h>
using namespace std;


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
		

	public:
		//public stuff
		FILE() {}
		BOOL isFile(const wchar_t* filePath);
		
	};
};