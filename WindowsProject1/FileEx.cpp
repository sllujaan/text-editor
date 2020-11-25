#include "FileEx.h"

FileEx::FileEx()
{

}

errno_t FileEx::ListFiles(TCHAR _path)
{
	WIN32_FIND_DATA FindFileData = {};
	PTCHAR _dir = new TCHAR(MAX_PATH);
	_dir = (PTCHAR)L"C:\\Users\\SALMAN-ALTAF\\Desktop\\tesDir\\New folder";


	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Find the first file in the directory.

	hFind = FindFirstFile(_dir, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) { return TASK_FAILURE; }

	LOG_WCHAR(L"FindFirstFile succeeded.");
	LOG_WCHAR(FindFileData.cFileName);

	BOOL nextFile = FindNextFile(hFind, &FindFileData);

	if (nextFile == 0) return TASK_FAILURE;

	LOG_WCHAR(L"FindNextFile succeeded.");
	LOG_WCHAR(FindFileData.cFileName);


	return TASK_SUCCESS;
}
