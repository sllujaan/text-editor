#pragma once
#include <string>
#include<shlwapi.h>
using namespace std;


class Validator {
private:
	//private stuff

public:
	//public stuff
	Validator() {}
	int validateFile(wstring fileName, const char* fileExtension);
};
