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
	errno_t err = _wdupenv_s(buffer, buffCount, L"APPDATA");
	if (!*buffCount) return 1;

	size_t appDirSize = wcslen(this->appDir);
	size_t totalSize = *buffCount + appDirSize + 1;

	wchar_t* newStr = new wchar_t[totalSize];

	errno_t err_cat = wcscat_s(*buffer, totalSize, this->appDir);

	//wstring str = *buffer;
	//str += this->appDir;

	return 0;
}
