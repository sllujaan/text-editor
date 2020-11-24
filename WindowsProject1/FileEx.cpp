#include "FileEx.h"

FileEx::FileEx()
{

}

errno_t FileEx::ListFiles(LPCWSTR _path)
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	return TASK_SUCCESS;
}
