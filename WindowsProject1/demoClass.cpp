#include "demoClass.h"

Learnings::Demo::Demo(HWND hWnd, HINSTANCE hInst, int nCmdShow) : WindowControls(hWnd, hInst)
{
    this->_hwndParent = hWnd;
    this->_hInstSelf = hInst;
}

Learnings::Demo::~Demo()
{
    
}

void Learnings::Demo::registerWindow()
{
    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->demo_wndProc;
    //wcex.cbClsExtra = 0;
    //wcex.cbWndExtra = 0;
    wcex.hInstance = this->_hInstSelf;
    wcex.lpszClassName = this->CLASS_NAME;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //RegisterClassEx(&wcex);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            TEXT("Call to RegisterClassEx failed!"),
            TEXT("Windows Desktop Guided Tour"),
            NULL);
    }
}

void Learnings::Demo::initWindow()
{
    this->createWindow();
    this->createTreeView();
}

LRESULT Learnings::Demo::demo_wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Demo* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<Demo*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<Demo*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    }

    if (pThis) {
        // Now that we have recovered our "this" pointer, let the
        // member function finish the job.
        return pThis->runProc(hwnd, message, wParam, lParam);
    }

    // We don't know what our "this" pointer is, so just do the default
    // thing. Hopefully, we didn't need to customize the behavior yet.

    LOG_WCHAR(L"no procedure fro demo.");
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Learnings::Demo::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        this->centerWindow(hwnd);
        LOG_WCHAR(L"demo window centered.");
        break;

    case WM_DESTROY:
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }

    //  Call the original window procedure for default processing. 
    //return CallWindowProc(this->lpfnMainWndProc, hwnd, message, wParam, lParam);
    return 0;
    //return DefWindowProc(hwnd, message, wParam, lParam);
}

void Learnings::Demo::createWindow()
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        200, 200, 500, 600,

        this->_hwndParent,       // Parent window    
        NULL,       // Menu
        this->_hInstSelf,  // Instance handle
        this        // Additional application data
    );


    if (hwnd == NULL)
    {
        MessageBox(
            this->_hwndParent,
            (LPCWSTR)L"Demo window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return;
    }

    this->_hwndSelf = hwnd;


}

void Learnings::Demo::centerWindow(HWND hwnd)
{
    RECT rectWindow;


    GetWindowRect(hwnd, &rectWindow);

    int nWidth = rectWindow.right - rectWindow.left;
    int nHeight = rectWindow.bottom - rectWindow.top;


    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    //screen center
    int nScreenWidthCenter = nScreenWidth / 2;
    int nScreenHeightCenter = nScreenHeight / 2;

    //center handwindow
    int nX = nScreenWidthCenter - (nWidth / 2);
    int nY = nScreenHeightCenter - (nHeight / 2);


    MoveWindow(hwnd, nX, nY, nWidth, nHeight, FALSE);

    OutputDebugStringW((LPCWSTR)L"window centered");

    return;
}

void Learnings::Demo::createTreeView()
{
    this->getTreeView(this->_hwndSelf, 10, 20, 20, 200, 300);
}
