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


	//read key value.
	//f.getKeyValue();

	//f.getText();
	int value;
	errno_t err_val = f.getKeyValueTemp("fontSizeIndex", &value);
	if (!err_val) {
		LOG_WCHAR(L"key value found =>>");
		LOG_INT(value);
	}

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

errno_t config::FILE::getKeyValue(string key, int* value)//wchar_t** destination, const wchar_t* key
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
	LOG_STR(text);
	

	//check if valid kay pair values--
	string str_reg_valid = ".*("+ key +"=\\d{1,2};).*";
	LOG_STR(str_reg_valid);
	regex reg_valid(str_reg_valid, regex_constants::icase);
	regex reg_digits("[a-zA-Z=]+|;");
	
	if (regex_match(text, reg_valid)) {
		LOG_WCHAR(L"valid expression");
		
		//take the valid expression from text
		

		string digits = regex_replace(text, reg_digits, "");
		//LOG_STR(digits);
		
		INT num = stoi(digits);

		//return number as parameter value
		*value = num;
		return 0;
		/*LOG_INT(num);

		LOG_STR(text);*/
	}

	


	//wstring wstr = wstring(line.begin(), line.end());

	//MessageBox(NULL,
	//	(LPCWSTR)wstr.c_str(),
	//	(LPCWSTR)L"env",
	//	NULL);


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
	this->getKeyValueTemp(this->_key_fSize, &this->fontSizeIndex);
	this->getKeyValueTemp(this->_key_fFamily, &this->fontFamilyIndex);
	this->getKeyValueTemp(this->_key_fStyle, &this->fontStyleIndex);
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
