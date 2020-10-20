#include "ListView.h"

LRESULT ListView::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LOG_WCHAR(L"list view runProc");
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT ListView::wndProcListView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ListView* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<ListView*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<ListView*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    }

    if (pThis) {
        // Now that we have recovered our "this" pointer, let the
        // member function finish the job.
        return pThis->runProc(hwnd, message, wParam, lParam);
    }

    // We don't know what our "this" pointer is, so just do the default
    // thing. Hopefully, we didn't need to customize the behavior yet.

    return DefWindowProc(hwnd, message, wParam, lParam);
}

ListView::ListView(HWND hwnd, int nCmdShow) : WindowControlsEx(hwnd, nCmdShow)
{

}

errno_t ListView::_createWindow()
{
	if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }
	
    if (this->registerWindow(*this->getWindowClass()) == TASK_FAILURE) return TASK_FAILURE;

    //if (this->createWindow(wndConfig) == TASK_FAILURE) return TASK_FAILURE;
    this->_hwndSelf = this->getListViewWindow();
    if (this->_hwndSelf == NULL) return TASK_FAILURE;

    

    return TASK_SUCCESS;


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

HWND ListView::getListViewWindow()
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"List View",    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        100, 100, 400, 400,

        this->_hwndParent,       // Parent window    
        NULL,       // Menu
        this->_hInst,  // Instance handle
        this        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->_hwndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return NULL;
    }

    return hwnd;
}
