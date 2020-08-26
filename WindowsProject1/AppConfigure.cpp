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



	/*errno_t err_config = f.initReadConfigKeys();
	if (!err_config) {
		LOG_WCHAR(L"config success =>>");
	}*/

	int value;
	f.getKeyValue("fontFamilyIndex", value);



	return 0;
}

config::FILE::FILE(const wchar_t* path)
{
	size_t len = wcslen(path) + 1;
	this->filePath = new wchar_t[len + 1];
	errno_t err = wcscpy_s(this->filePath, len, path);

	//read text and store it as attribute variable
	this->readText();
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
	this->CLOSE();
	if (!this->file) return FALSE;
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
	string str_reg_valid = ".*(;*"+key+"=\\d{1,2};).*";
	regex reg_valid(str_reg_valid, regex_constants::icase);
	
	if (regex_match(text, reg_valid)) {
		LOG_WCHAR(L"valid expression");
		this->findKeyValue(text, key, value);
		return 0;
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
	const size_t LOOP_MAX_COUNT = 10;
	size_t LOOP_CURRENT_COUNT = 0;

	while (_key_found_index != string::npos) {
		LOOP_CURRENT_COUNT++;

		_key_found_index = text.find(key + "=", _key_found_index);
		size_t _digit_index_start = _key_found_index + key.size() + 1;
		size_t _digit_index_close = text.find(';', _digit_index_start);
		LOG_INT(_digit_index_start);
		LOG_INT(_digit_index_close);

		string _digitStr = string(text.begin() + _digit_index_start, text.begin() + _digit_index_close);

		try {
			int _val = stoi(_digitStr);
			value = _val;
			LOG_INT(value);
		}
		catch (...) {
			LOG_WCHAR(L"Not found got to next iteration.");
			_key_found_index = text.find(key + "=", _digit_index_close + 1);
			//LOG_INT(_key_found_index);
		}
		
		if(LOOP_CURRENT_COUNT > LOOP_MAX_COUNT) break;
	}

	//_key_found_index = text.find(key + "=");
	////LOG_INT(foundIndex);
	////sLOG_INT(key.size());

	//if (_key_found_index != string::npos) {
	//	//if(foundIndex+key.size()+1)
	//	size_t _digit_index_start = _key_found_index + key.size() + 1;
	//	size_t _digit_index_close = text.find(';', _digit_index_start);
	//	string _digitStr = string(text.begin() + _digit_index_start, text.begin() + _digit_index_close);
	//	LOG_INT(stoi(_digitStr));

	//	return 0;
	//}

	return 1;
}

string config::FILE::readText()
{
	if (!this->isFile()) return nullptr;

	//close the stream if already open
	this->CLOSE();

	this->file.open(this->filePath, ios::in);

	string wholeText;
	string lineText;

	while (getline(this->file, lineText)) {
		wholeText = lineText + "\n";
	}

	this->fileText = wholeText;
	return wholeText;
}
