#include "windowManager.h"

windowManager::windowManager(HWND hWnd)
{
	this->_hwnd_self = hWnd;
}

size_t windowManager::getWndWidth()
{
    GetWindowRect(this->_hwnd_self, &this->rectWindow);
    int nWidth = this->rectWindow.right - this->rectWindow.left;
    return (size_t)nWidth;
}

size_t windowManager::getWndHeight()
{
    GetWindowRect(this->_hwnd_self, &this->rectWindow);
    int nHeight = this->rectWindow.bottom - this->rectWindow.top;
    return (size_t)nHeight;
}
