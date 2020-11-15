#include "TreeView.h"

LRESULT TreeView::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPMINMAXINFO mmi = nullptr;

    switch (message)
    {
    /*case WM_SIZE:
        LOG_WCHAR(L"WM_SIZE");
        LOG_INT((int)LOWORD(lParam));
        LOG_INT((int)HIWORD(lParam));
        break;*/
    case WM_GETMINMAXINFO:
        mmi = (LPMINMAXINFO)lParam;
        mmi->ptMinTrackSize.x = 200;
        mmi->ptMinTrackSize.y = 200;
        mmi->ptMaxTrackSize.x = 800;
        mmi->ptMaxTrackSize.y = 800;
        break;
    default:
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT TreeView::wndProcTreeView(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TreeView* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<TreeView*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<TreeView*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

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

WNDCLASSEX* TreeView::getWindowClass()
{
    WNDCLASSEX* wcex = new WNDCLASSEX();

    wcex->cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex->lpfnWndProc = this->wndProcTreeView;
    wcex->cbClsExtra = 0;
    wcex->cbWndExtra = 0;
    wcex->hInstance = this->_hInst;
    wcex->lpszClassName = this->CLASS_NAME;
    wcex->hIcon = 0;
    wcex->hCursor = LoadCursor(NULL, IDC_SIZEWE);
    wcex->hIconSm = 0;

    //wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex->hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    //wcex.lpszMenuName = NULL;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return wcex;
}

errno_t TreeView::_createWindow()
{
    if (!this->canCreateWindow()) { this->showWindowCreationError(); return TASK_FAILURE; }

    if (this->registerWindow(*this->getWindowClass()) == TASK_FAILURE) return TASK_FAILURE;

    //if (this->createWindow(wndConfig) == TASK_FAILURE) return TASK_FAILURE;
    this->_hwndSelf = this->getTreeViewWindow();
    if (this->_hwndSelf == NULL) return TASK_FAILURE;

    return TASK_SUCCESS;
}

HWND TreeView::getTreeViewWindow()
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"Tree View",    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        600, 400, 400, 400,

        this->_hwndParent,       // Parent window    
        NULL,       // Menu
        this->_hInst,  // Instance handle
        this        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->_hwndParent,
            (LPCWSTR)L"Tree View window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return NULL;
    }

    return hwnd;
}

errno_t TreeView::initTreeViewControl()
{

    InitCommonControls();

    HWND hwndTV = CreateWindowEx(0,
        WC_TREEVIEW,
        TEXT("Tree View"),
        WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
        0,
        0,
        200,
        300,
        this->_hwndSelf,
        NULL,
        this->_hInst,
        NULL);

    if (hwndTV == NULL) {
        MessageBox(
            this->_hwndSelf,
            (LPCWSTR)L"Tree View control creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );
        return TASK_FAILURE;
    }

    this->_hwndTV = hwndTV;

    return TASK_SUCCESS;
}

BOOL TreeView::InitTreeViewImageLists(HWND hwndTV)
{
    HIMAGELIST himl;  // handle to image list 
    HBITMAP hbmp;     // handle to bitmap 
    INT g_nOpen;

    // Create the image list. 
    if ((himl = ImageList_Create(20,
        20,
        FALSE,
        1, 0)) == NULL)
    {
        return FALSE;
    }
        
    LPCWSTR resource = MAKEINTRESOURCE(IDI_ICON6);

    // Add the open file, closed file, and document bitmaps.
    hbmp = LoadBitmap(NULL, resource);
    g_nOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    // Fail if not all of the images were added. 
    if (ImageList_GetImageCount(himl) < 1) return FALSE;

    // Associate the image list with the tree-view control. 
    TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);

    return TRUE;
}

HTREEITEM TreeView::AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel, HTREEITEM _hPrev)
{

    if (_hPrev == NULL) _hPrev = (HTREEITEM)TVI_FIRST;

    TVITEM tvi;
    TVINSERTSTRUCT tvins;
    static HTREEITEM hPrev = _hPrev;
    static HTREEITEM hPrevRootItem = _hPrev;
    static HTREEITEM hPrevLev2Item = _hPrev;
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

    if (hPrev == NULL) return NULL;

    // Save the handle to the item. 
    if (nLevel == 1)
        hPrevRootItem = hPrev;
    else if (nLevel == 2)
        hPrevLev2Item = hPrev;

    // The new item is a child item. Give the parent item a 
    // closed folder bitmap to indicate it now has child items. 
    if (nLevel > 1)
    {
        hti = TreeView_GetParent(hwndTV, hPrev);
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hItem = hti;
        //tvi.iImage = g_nClosed;
        //tvi.iSelectedImage = g_nClosed;
        TreeView_SetItem(hwndTV, &tvi);
    }


    return hPrev;
}

TreeView::TreeView(HWND hwnd, int nCmdShow) : WindowControlsEx(hwnd, nCmdShow)
{

}

errno_t TreeView::initWindow()
{
    this->_createWindow();
    this->initTreeViewControl();


    HTREEITEM rootItem1 = this->AddItemToTree(this->_hwndTV, (LPTSTR)L"item1", 1, NULL);
    HTREEITEM rootItem2 = this->AddItemToTree(this->_hwndTV, (LPTSTR)L"item2", 1, NULL);


    this->AddItemToTree(this->_hwndTV, (LPTSTR)L"child 1", 2, rootItem1);
    //this->AddItemToTree(this->_hwndTV, (LPTSTR)L"child 2", 2);


    /*if (!this->InitTreeViewImageLists(this->_hwndTV)) {
        MessageBox(
            this->_hwndSelf,
            (LPCWSTR)L"Tree View InitTreeViewImageLists failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );
        return TASK_FAILURE;
    }*/

    return TASK_SUCCESS;
}
