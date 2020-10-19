#include "ListView.h"

LRESULT ListView::wndProcListView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, message, wParam, lParam);
}

ListView::ListView(HWND hwnd, int nCmdShow) : WindowControlsEx(hwnd, nCmdShow)
{

}

errno_t ListView::_createWindow(WINDOW_CONFIG& wndConfig)
{
	if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }
	
    if (this->registerWindow(*this->getWindowClass()) == TASK_FAILURE) return TASK_FAILURE;
}

WNDCLASSEX* ListView::getWindowClass()
{
	WNDCLASSEX* wcex = new WNDCLASSEX();

    wcex->cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex->lpfnWndProc = this->wndProcListView;
    wcex->cbClsExtra = 0;
    wcex->cbWndExtra = 0;
    wcex->hInstance = this->_hInst;
    wcex->lpszClassName = this->CLASS_NAME;
    wcex->hIcon = 0;
    wcex->hIconSm = 0;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex->hbrBackground = (HBRUSH)(COLOR_WINDOW);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	return wcex;
}
