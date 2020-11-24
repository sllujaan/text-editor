#pragma once
#include<Windows.h>
#include"AppLog.h"
#include"appUtil.h"


class FileEx {
private:

protected:

public:
	FileEx();
	errno_t ListFiles(LPCWSTR _path);


};
