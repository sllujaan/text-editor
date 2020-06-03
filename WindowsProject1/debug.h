#pragma once


void displayDialog(
	const char* charc_ptr = NULL,
	string str = "",
	int num = 0,
	double ratio = 0
)
{

	wchar_t* newWChar_ptr = new wchar_t();

	//concatenate char ptr and str
	if ((charc_ptr != NULL) && (str != "")) {
		
		size_t len_charc_ptr = strlen(charc_ptr);


		wchar_t* param_ptr = new wchar_t();

		wcscpy_s(param_ptr, 5, (wchar_t*)charc_ptr);


		//wcscpy(newChar_ptr, L"abc");
		//wcscpy_s(newWChar_ptr, 5, (LPCWSTR)charc_ptr);


		//strcpy_s(newChar_ptr, 4, "abc");


		MessageBox(
			NULL,
			(LPCWSTR)param_ptr,
			(LPCWSTR)L"Debug.",
			MB_ICONERROR
		);

		

		//strcpy(charc_ptr, str)
	}
}


/*
//do concatenate--
		size_t len_charc_ptr = strlen(charc_ptr);
		//newChar_ptr = new char[len_charc_ptr + 1];

		//strcpy_s<len_charc_ptr>(newChar_ptr, charc_ptr);

		strcpy_s(newChar_ptr, len_charc_ptr+1, charc_ptr);

		//strcpy(newChar_ptr, charc_ptr);

		size_t sizeStr = str.size();
		//newStr = new char[sizeStr + 1];
		//strcpy(newStr, (char*)str.c_str());

		strcat_s(newChar_ptr, sizeStr + 1, str.c_str());

		newChar_ptr[len_charc_ptr + sizeStr + 1] = '\0';

		//char* computedChar_ptr = strcat(newChar_ptr, str.c_str());

*/