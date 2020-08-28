#include "AppConfigure.h"

appConfig::appConfig()
{
	wchar_t* path;
	size_t size;

	errno_t err_env = this->getAppConfigPath_secure(&path, &size);

	if (err_env) return;

	this->_file = new config::FILE(path);
	
	if (!this->_file->isFile()) { free(path); return; };
	
	int width;
	int height;
	errno_t err_width = this->_file->getKeyValue("wndWidth", width);
	errno_t err_height = this->_file->getKeyValue("wndHeight", height);

	if (err_width || err_height) return;

	this->wndWidth = width;
	this->wndHeight = height;

}

appConfig::~appConfig()
{
	delete this->_file;
}

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

	*buffer = newBuffer;


	return 0;
}

void appConfig::printRect(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);

	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;

	LOG_INT(nWidth);
	LOG_INT(nHeight);
}

size_t appConfig::getWndHeight()
{
	return this->wndHeight;
}

size_t appConfig::getWndWidth()
{
	return this->wndWidth;
}

config::FILE::FILE(const wchar_t* path)
{
	size_t len = wcslen(path) + 1;
	this->filePath = new wchar_t[len + 1];
	errno_t err = wcscpy_s(this->filePath, len, path);

	LOG_WCHAR(L"reading text...");
	//read text and store it as attribute variable
	this->readText();
	LOG_WCHAR(L"text read successfully.");
}

BOOL config::FILE::isFile()
{
	if (this->filePath == NULL) return FALSE;
	
	//close the stream if already open by other function
	this->CLOSE();

	//converting wchar_t* to char*
	_bstr_t b(this->filePath);
	const char* path = b;

	//now check for file.
	this->file.open(path, ios::in);
	LOG_WCHAR(L"file Opened.");
	if (!this->file) { this->CLOSE(); return FALSE; }
	return TRUE;
}

errno_t config::FILE::getKeyValue(string key, int& value)//wchar_t** destination, const wchar_t* key
{

	if (!this->isFile()) return 1;

	//close the stream if already open
	this->CLOSE();
	
	//store text in local variable
	string text = this->fileText;

	//remove all white spaces--
	regex reg_spaces("\\s");
	text = regex_replace(text, reg_spaces, "");
	if (text == "") return 1;

	//check if valid kay pair values--
	string str_reg_valid = ".*(;*"+key+"=\\d{1,10};).*";
	regex reg_valid(str_reg_valid, regex_constants::icase);
	
	
	if (regex_match(text, reg_valid)) {
		LOG_WCHAR(L"valid expression");
		return this->findKeyValue(text, key, value);
	}

	return 1;
}

errno_t config::FILE::CLOSE()
{
	this->file.close();
	return 0;
}

wstring config::FILE::getText()
{
	//converting string to wstring
	wstring ws = wstring(this->fileText.begin(), this->fileText.end());
	return ws;
}

errno_t config::FILE::getKeyValueTemp(string key, int* value)
{
	if (key == "fontSizeIndex") { *value = 5; return 0; } //0 is no error
	if (key == "fontFamilyIndex") { *value = 7; return 0; }//0 is no error
	if (key == "fontStyleIndex") { *value = 3; return 0; }//0 is no error
	return 1; //1 is error
}

errno_t config::FILE::initReadConfigKeys()
{
	errno_t err = 0;
	if(!this->isFile()) return ERROR_CONFIG;
	err = this->getKeyValueTemp(this->_key_fSize, &this->fontSizeIndex); if (err) { this->resetConfigKeys();return ERROR_CONFIG; };
	err = this->getKeyValueTemp(this->_key_fFamily, &this->fontFamilyIndex); if (err) { this->resetConfigKeys();return ERROR_CONFIG; };
	err = this->getKeyValueTemp(this->_key_fStyle, &this->fontStyleIndex); if (err) { this->resetConfigKeys();return ERROR_CONFIG; };

	return SUCCESS_CONFIG;
}

void config::FILE::resetConfigKeys()
{
	this->_key_fSize = -1;
	this->_key_fFamily = -1;
	this->_key_fStyle = -1;
}

errno_t config::FILE::findKeyValue(string text, string key, int& value)
{

	size_t _key_found_index = 0;
	//run while loop for maximum 500 times.
	const size_t LOOP_MAX_COUNT = 100;
	size_t LOOP_CURRENT_COUNT = 0;

	while (_key_found_index != string::npos) {
		LOOP_CURRENT_COUNT++;

		//take the key index where the key is found.
		_key_found_index = text.find(key + "=", _key_found_index);
		//get the digit indexes starting and closing..
		size_t _digit_index_start = _key_found_index + key.size() + 1;
		size_t _digit_index_close = text.find(';', _digit_index_start);

		string _digitStr = string(text.begin() + _digit_index_start, text.begin() + _digit_index_close);
		try {
			int _val = stoi(_digitStr);
			value = _val;
			return 0;
		}
		catch (...) {
			//kay value was invalid. Find next.
			_key_found_index = text.find(key + "=", _digit_index_close + 1);
		}
		
		//stop the loop if it exceeds max counts--
		if(LOOP_CURRENT_COUNT > LOOP_MAX_COUNT) break;
	}

	return 1;
}

string config::FILE::readText()
{
	if (!this->isFile()) return "";

	//close the stream if already open
	this->CLOSE();

	LOG_WCHAR(L"opening file..");

	this->file.open(this->filePath, ios::in);

	string wholeText;
	string lineText;

	while (getline(this->file, lineText)) {
		
		wholeText += lineText + "\n";
	}

	this->fileText = wholeText;
	return wholeText;
}

errno_t config::FILE::writeText(string text)
{
	//if (!this->isFile()) return 1;

	//close the stream if already open
	this->CLOSE();

	this->file.open(this->filePath, ios::out);

	this->file << text;

	return 0;
}
