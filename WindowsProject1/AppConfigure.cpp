#include "AppConfigure.h"

LPCWSTR appConfig::getAppConfigPath()
{
	wchar_t* pValue;
	size_t len;
	errno_t err = _wdupenv_s(&pValue, &len, L"appdataddd");

	if (err) { 
		/*pValue = new wchar_t[10];
		
		pValue = (wchar_t*)L"abcdefg";*/

		static const wchar_t* p = L"abc";

		return p;
	}

	wstring envVar(pValue);
	envVar += L"\\TextEditor.Dev\\";

	return pValue;

}

errno_t appConfig::getAppConfigPath_secure(wchar_t** buffer, size_t* buffCount)
{

	errno_t err = _wdupenv_s(buffer, buffCount, L"appdata");
	if (!*buffCount) return 1;

	size_t appDirSize = wcslen(this->appDir) + 1;
	size_t totalSize = *buffCount + 1 + appDirSize + 1;

	wchar_t* newBuffer = new wchar_t[totalSize];
	memset(newBuffer, 0, totalSize);
	

	errno_t err_cpy = wcscpy_s(newBuffer, *buffCount, *buffer);

	errno_t err_cat = wcscat_s(newBuffer, totalSize, this->appDir);

	//free up buffer
	free(*buffer);


	//reaclocation of memory for concatenation---------
	//*buffer = (wchar_t*)realloc(*buffer, totalSize + 1 );
	//errno_t err_cat = wcscat_s(*buffer, totalSize + 1, this->appDir);

	*buffer = newBuffer;


	//check if file exists
	config::FILE f(*buffer);
	const BOOL file = f.isFile();
	if (!file) {
		MessageBox(NULL,
			(LPCWSTR)L"file does not exist.",
			(LPCWSTR)L"env",
			NULL);
	}

	f.getKeyValue();

	return 0;
}

config::FILE::FILE(const wchar_t* path)
{
	size_t len = wcslen(path) + 1;
	this->filePath = new wchar_t[len + 1];
	errno_t err = wcscpy_s(this->filePath, len, path);
}

BOOL config::FILE::isFile()
{
	//converting wchar_t* to char*
	_bstr_t b(this->filePath);
	const char* path = b;
	//now check for file.
	this->file.open(path, ios::in);
	if (!this->file) return FALSE;
	return TRUE;
}

errno_t config::FILE::getKeyValue()//wchar_t** destination, const wchar_t* key
{
	//if (!this->isFile()) return 1;

	//close the stream if already open
	this->CLOSE();

	this->file.open(this->filePath, ios::in);
	
	string line;
	getline(this->file, line);

	wstring wstr = wstring(line.begin(), line.end());

	MessageBox(NULL,
		(LPCWSTR)wstr.c_str(),
		(LPCWSTR)L"env",
		NULL);


	return 0;
}

errno_t config::FILE::CLOSE()
{
	this->file.close();
	return 0;
}
