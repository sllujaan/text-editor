#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include<comdef.h>
#include<regex>
#include<direct.h>

#include"AppLog.h"
using namespace std;

#define ERROR_CONFIG 1
#define SUCCESS_CONFIG 0

#define TASK_SUCCESS 0
#define TASK_FAILURE 1




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
		errno_t getKeyValue(string key, int& value);//wchar_t** destination, const wchar_t* key
		errno_t CLOSE();
		wstring getText();
		errno_t getKeyValueTemp(string key, int* value);
		errno_t initReadConfigKeys();
		void resetConfigKeys();
		errno_t writeText(string text);
		errno_t createNewFile();

	private:
		errno_t findKeyValue(string text, string key, int& value);


	protected:
		const wchar_t* _appDirName = L"\\TextEditor.Dev";
		const wchar_t* _appFileName = L"\\config.txt";

	};
};



class appConfig : public config::FILE {

private:
	const wchar_t* appDir = L"\\TextEditor.Dev\\config.txt";
	config::FILE* _file;

public:
	size_t wndWidth = 500;
	size_t wndHeight = 400;

public:
	appConfig();
	~appConfig();
	LPCWSTR getAppConfigPath();
	errno_t getAppConfigPath_secure(wchar_t** buffer, size_t* buffCount);
	errno_t getAppDataPath(wchar_t** buffer, size_t* buffCount);
	void printRect(HWND hwnd);
	size_t getWndHeight();
	size_t getWndWidth();
	//this method is responsible for making sure that configuration file's direcotry extists.
	//if not exits it will create that one.
	size_t resolvePath();

};
