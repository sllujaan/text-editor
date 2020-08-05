#pragma once


#include<fstream>
#include<string>
#include<comdef.h>
using namespace std;


#define FILE_EXIST 0x2
#define FILE_ERROR 0x0
#define FILE_SUCCESS 0x1

struct File {

	fstream file;

	bool Close() {
		file.close();
		return true;
	}

	bool isFile(string fileName) {
		//converting string to char pointer.
		const char* fileNamePtr = fileName.c_str();

		file.open(fileNamePtr, ios_base::in);
		if (!file) return false;

		return true;
	}


	int writeFile_LPCWSTR(LPCWSTR text, LPCWSTR path, BOOL replace = FALSE) {

		//LPCWSTR is cosnt wchar_t*


		//converting WCHAR* to CHAR*
		_bstr_t b(path);
		const char* fileNamePtr = b;

		if (isFile(fileNamePtr)) {
			if(!replace) return FILE_EXIST;
		}

		fstream new_file;

		new_file.open(fileNamePtr, ios_base::out);

		if (!new_file) return FILE_ERROR;

		//new_file << "this text has been generated from c++ programming language.\nAnd this is new the new line.\n\tThis is new line and tab space." << endl;
		
		size_t length = wcslen(text);

		for (size_t i = 0; i < length; i++)
		{
			new_file << (char)text[i];
		}

		new_file.close();

		return FILE_SUCCESS;
	}




	string readFile(string fileName) {

		const char* fileNamePtr = fileName.c_str();

		file.open(fileNamePtr, ios_base::in);

		string msg = "No such file exists named " + (string)fileName;
		if (!file) { cout << msg << endl;return "ERROR::NOT FOUND."; }

		string lineText;
		string wholeText;

		while (getline(file, lineText)) { wholeText += lineText + '\n'; }

		return wholeText;
	}

	string readFile_LPCWSTR(LPCWSTR fileName) {

		//LPCWSTR is cosnt wchar_t*


		//converting WCHAR* to CHAR*
		_bstr_t b(fileName);
		const char* fileNamePtr = b;

		file.open(fileNamePtr, ios_base::in);

		string msg = "No such file exists named " + (string)fileNamePtr;
		if (!file) { cout << msg << endl;return "ERROR::NOT FOUND."; }

		string lineText;
		string wholeText;

		while (getline(file, lineText)) { wholeText += lineText + "\n"; }

		//cout << wholeText << endl;

		return wholeText;

		/*

		wstring wstr = wstring(wholeText.begin(), wholeText.end());

		cout << (char)wstr[0] << endl;
		cout << (char)wstr[1] << endl;
		cout << (char)wstr[2] << endl;
		cout << (char)wstr[3] << endl;


		cout << size(wstr) << endl;

		size_t length = size(wstr);

		WCHAR* lpString = new WCHAR[4];

		lpString[0] = (WCHAR)"M";
		lpString[1] = (WCHAR)"i";
		lpString[2] = (WCHAR)"k";
		lpString[3] = (WCHAR)"e";

		//lpString = wstr.c_str();
		//LPCWSTR lpString = wstr.c_str();

		//cout << wcslen(lpString) << endl;
		//cout << (char)lpString[0] << endl;

		return lpString;
		*/

	}



	string readFile_LPCWSTR_A(LPCWSTR fileName) {

		_bstr_t b(fileName);
		const char* fileNamePtr = b;

		file.open(fileNamePtr, ios_base::in);

		string msg = "No such file exists named " + (string)fileNamePtr;
		if (!file) { cout << msg << endl;return "ERROR::NOT FOUND."; }

		string lineText;
		string wholeText;

		while (getline(file, lineText)) { wholeText += lineText + "\r\n"; }

		//cout << wholeText << endl;

		return wholeText;

	}



};
