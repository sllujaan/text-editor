#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include<comdef.h>
#include<regex>
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
		wchar_t* filePath;
		

	public:
		//public stuff
		FILE(const wchar_t* path);
		BOOL isFile();
		errno_t getKeyValue();//wchar_t** destination, const wchar_t* key
		errno_t CLOSE();
		
	};
};