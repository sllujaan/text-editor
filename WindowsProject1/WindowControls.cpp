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

    HWND hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        (int)posX, (int)posY, (int)width, (int)height, this->_hwndSelf, NULL, this->_hInst, NULL);

    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    return hwndEdit;
}

void WindowControls::setWindowControlsHandle(HWND hwndSelf, HINSTANCE hInstSelf)
{
    if(hwndSelf) this->_hwndSelf = hwndSelf;
    if (hInstSelf) this->_hInst = hInstSelf;
}

HFONT WindowControls::getFont(size_t size, LPCWSTR fontFamily, LPCWSTR fontStyle)
{
    
    int fontWeigh = FW_NORMAL;
    BOOL italic = FALSE;

    if (fontStyle == L"Bold") {
        fontWeigh = FW_BOLD;
    }
    else if (fontStyle == L"Italic") {
        italic = TRUE;
    }
    else if (fontStyle == L"Bold Italic") {
        fontWeigh = FW_BOLD;
        italic = TRUE;
    }
    else if (fontStyle == L"Light") {
        fontWeigh = FW_LIGHT;
    }



    HFONT hFont = CreateFont(int(size), 0, 0, 0, fontWeigh, italic, FALSE, FALSE, ANSI_CHARSET,
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
HWND WindowControls::getButton(HWND hWndParent, LPCWSTR buttonText, short UID_BUTTON, size_t posX, size_t posY, LONG push)
{
    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        buttonText,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | push,  // Styles 
        (int)posX,         // x position 
        (int)posY,         // y position 
        80,        // Button width
        24,        // Button height
        hWndParent,     // Parent window
        (HMENU)UID_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    this->applyConsistentStyle(hwndButton);
    
    return hwndButton;
}
void WindowControls::applyConsistentStyle(HWND hwnd)
{
    SendMessage(hwnd, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
}
HWND WindowControls::getStatic(HWND hWndParent, LPCWSTR buttonText, size_t posX, size_t posY)
{
    HWND hwndStatic = CreateWindow(
        L"STATIC",  // Predefined class; Unicode assumed 
        buttonText,      // Button text 
        WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,  // Styles 
        (int)posX,         // x position 
        (int)posY,         // y position 
        80,        // Button width
        18,        // Button height
        hWndParent,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    this->applyConsistentStyle(hwndStatic);

    return hwndStatic;
}
HWND WindowControls::getTabControl(size_t posX, size_t posY, size_t width, size_t height)
{
    if (!this->canCreateWindow()) { this->showWindowCreationError(); return NULL; }
    
    INITCOMMONCONTROLSEX icex;

    // Initialize common controls.
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    //InitCommonControlsEx(&icex);

    HWND hwndTab = CreateWindow(WC_TABCONTROL, L"",
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
        (int)posX, (int)posY, (int)width, (int)height,
        this->_hwndSelf, NULL, this->_hInst, NULL);

    SendMessage(hwndTab, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);

    return hwndTab;
}

HWND WindowControls::getRadioButton(HWND hWndParent, LPCWSTR buttonText, short UID_BUTTON, size_t posX, size_t posY)
{
    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        buttonText,      // Button text 
        WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
        (int)posX,         // x position 
        (int)posY,         // y position 
        80,        // Button width
        17,        // Button height
        hWndParent,     // Parent window
        (HMENU)UID_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    this->applyConsistentStyle(hwndButton);

    return hwndButton;
}

HWND WindowControls::getGroupBox(HWND hwndParent, LPCWSTR name, int posX, int posY, int width, int height)
{
    HWND hwndGroupBox = CreateWindowEx(
        0,
        L"BUTTON",      // Button text 
        name,
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_CENTER,  // Styles
        posX,         // x position 
        posY,         // y position 
        width,        // Button width
        height,        // Button height
        hwndParent,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);

    return hwndGroupBox;
}

HWND WindowControls::DoCreateDisplayWindow(HWND hwndTab)
{
    HWND hwndStatic = CreateWindow(WC_STATIC, L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        100, 100, 100, 100,        // Position and dimensions; example only.
        hwndTab, NULL, this->_hInst,    // g_hInst is the global instance handle
        NULL);
    return hwndStatic;

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
