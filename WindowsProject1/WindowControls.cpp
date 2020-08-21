#include "WindowControls.h"

BOOL WindowControls::canCreateWindow()
{
    if (this->_hwndSelf == NULL || this->_hInst == NULL) return false;
    return true;
}

void WindowControls::showWindowCreationError()
{
    MessageBox(NULL,
        (LPCWSTR)L"window creation failed! either hwnd or hInst is NULL.\r\nConsider setWindowControlsHandle(HWND hwndSelf)",
        (LPCWSTR)L"WindowControls",
    MB_ICONEXCLAMATION);
}

HWND WindowControls::getEditControl(size_t posX, size_t posY, size_t width, size_t height)
{

    if (!this->canCreateWindow()) { this->showWindowCreationError(); return NULL; }

    HWND hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"text",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        (int)posX, (int)posY, (int)width, (int)height, this->_hwndSelf, NULL, this->_hInst, NULL);

    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    return hwndEdit;
}

void WindowControls::setWindowControlsHandle(HWND hwndSelf)
{
    this->_hwndSelf = hwndSelf;
}

HFONT WindowControls::getFont(size_t size, LPCWSTR fontFamily)
{
    HFONT hFont = CreateFont(int(size), 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontFamily);

    return hFont;
}

HWND WindowControls::getListBox(HWND hWndParent, int posX, int posY, int width, int height)
{
    // Adding a ListBox.
    HWND hListBox = CreateWindowEx(WS_EX_CLIENTEDGE
        , L"LISTBOX", NULL
        , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_DISABLENOSCROLL | LBS_NOTIFY,
        posX, posY, width, height,
        hWndParent, NULL, this->_hInst, NULL);

    return hListBox;
}
//
//template<size_t size>
//inline errno_t WindowControls::_insertItems_listBox(HWND hwnd, int select, const wchar_t* (&itemsArray)[size])
//{
//    int pos = 0;
//
//    for (int i = 0; i < ARRAYSIZE(itemsArray); i++) {
//
//        pos = (int)SendMessage(hwnd, LB_ADDSTRING, 0,
//            (LPARAM)itemsArray[i]);
//
//        //set font for indidual item----
//
//    }
//
//    //find string from list box--
//    LRESULT searchIndex = SendMessage(hwnd, LB_FINDSTRING, -1, (LPARAM)this->fontFamily);
//    if (searchIndex == LB_ERR) searchIndex = 0;
//
//    SendMessage(hwnd, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
//    SendMessage(hwnd, LB_SETCURSEL, select, 0);
//
//    return 0;
//}

WindowControls::WindowControls(HWND hwndSelf, HINSTANCE hInst)
{
    this->_hwndSelf = hwndSelf;
    this->_hInst = hInst;
}
