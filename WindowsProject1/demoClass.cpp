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

    //this_thread::sleep_for(std::chrono::milliseconds(3000));
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
        WS_EX_DLGMODALFRAME | WS_EX_ACCEPTFILES,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_SYSMENU | WS_VISIBLE,            // Window style

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
    HWND hwndTV = this->getTreeView(this->_hwndSelf, 10, 20, 20, 200, 300);

    this->_hwndTV = hwndTV;

    this->handleTreeViewInsertItems();

}

void Learnings::Demo::handleTreeViewInsertItems()
{
    InitCommonControls();

    if (!this->InitTreeViewImageLists(this->_hwndTV)) this->showErrorMessage(this->_hwndSelf, L"Failed to create images list");

    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level1", 1);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level2", 2);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level3", 3);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level4", 4);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level4", 5);

    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level1", 1);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level2", 2);
    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"level3", 3);

}

HTREEITEM Learnings::Demo::AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;
    static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
    static HTREEITEM hPrevRootItem = NULL;
    static HTREEITEM hPrevLev2Item = NULL;
    HTREEITEM hti;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE
        | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    // Set the text of the item. 
    tvi.pszText = lpszItem;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

    // Save the heading level in the item's application-defined 
    // data area. 
    tvi.lParam = (LPARAM)nLevel;
    tvins.item = tvi;
    tvins.hInsertAfter = hPrev;

    // Set the parent item based on the specified level. 
    if (nLevel == 1)
        tvins.hParent = TVI_ROOT;
    else if (nLevel == 2)
        tvins.hParent = hPrevRootItem;
    else
        tvins.hParent = hPrevLev2Item;

    // Add the item to the tree-view control. 
    hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM,
        0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    if (hPrev == NULL)
        return NULL;
    
    // Save the handle to the item. 
    if (nLevel == 1)
        hPrevRootItem = hPrev;
    else if (nLevel == 2)
        hPrevLev2Item = hPrev;

    if (nLevel > 1)
    {
        hti = TreeView_GetParent(hwndTV, hPrev);
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hItem = hti;
        TreeView_SetItem(hwndTV, &tvi);
    }


    return hPrev;
}

BOOL Learnings::Demo::InitTreeViewImageLists(HWND hwndTV)
{

    // InitTreeViewImageLists - creates an image list, adds three bitmaps 
    // to it, and associates the image list with a tree-view control. 
    // Returns TRUE if successful, or FALSE otherwise. 
    // hwndTV - handle to the tree-view control. 
    //
    // Global variables and constants: 
    // g_hInst - the global instance handle.
    // g_nOpen, g_nClosed, and g_nDocument - global indexes of the images. 
    // CX_BITMAP and CY_BITMAP - width and height of an icon. 
    // NUM_BITMAPS - number of bitmaps to add to the image list. 
    // IDB_OPEN_FILE, IDB_CLOSED_FILE, IDB_DOCUMENT -
    //     resource identifiers of the bitmaps.

    HIMAGELIST himl;  // handle to image list 
    HBITMAP hbmp;     // handle to bitmap 

    // Create the image list. 
    if ((himl = ImageList_Create(CX_BITMAP,
        CY_BITMAP,
        FALSE,
        NUM_BITMAPS, 0)) == NULL)
    {
        return FALSE;
    }

    LOG_WCHAR(L"Image list created.");


    
    // Add the open file, closed file, and document bitmaps. 
    hbmp = LoadBitmap(this->_hInstSelf, MAKEINTRESOURCE(IDI_SMALL));
    this->g_nOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);



    /*hbmp = LoadBitmap(this->_hInstSelf, MAKEINTRESOURCE(IDI_SMALL));
    this->g_nClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp = LoadBitmap(this->_hInstSelf, MAKEINTRESOURCE(IDI_SMALL));
    this->g_nDocument = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);*/


    // Fail if not all of the images were added. 
    if (ImageList_GetImageCount(himl) < 1) return FALSE;

    
    // Associate the image list with the tree-view control. 
    TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);

    return TRUE;
}
