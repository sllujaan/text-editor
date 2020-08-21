
#include "AppSettings.h"


AppSettings::AppSettings(HWND hWnd, HINSTANCE hInstance, int nCmdShow) : WindowControls(this->hWndSettings, hInstance)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;
}

void AppSettings::initWindow()
{
    this->createWindow();

    this->createGroupBox();
    //this->createListView();

    this->createGroupBoxSample();
    this->initSampleText();
    //this->insertListViewItems(5);
    this->createTooltilp();

    this->handleFocuses();
    
    this->createListBox();
    this->initListViewBox();

    this->createEditControlFontStyles();
    this->createListBox_FontStyles();

    this->createEditControlTest();

    this->_createListBox_fontStyles();


    //when every thing is ready start window
    ShowWindow(this->hWndSettings, this->nCmdShowGlobal);
}

void AppSettings::setSettings(size_t fonstSizeIndex, size_t fontFamilyIndex, size_t fontSyleIndex)
{

    //1. validate parameters indexs with listbox total index items.
    if (!this->isValidIndex(this->_fSizeTotalItems, fonstSizeIndex)) return this->showConfigKeysCorrupted();
    if (!this->isValidIndex(this->_fFamilyTotalItems, fontFamilyIndex)) return this->showConfigKeysCorrupted();
    if (!this->isValidIndex(this->_fStyleTotalItems, fontSyleIndex)) return this->showConfigKeysCorrupted();

    //2. if step 1 is true then assgin them as properties.
    this->_fSizeIndex = fonstSizeIndex;
    this->_fFamilyIndex = fontFamilyIndex;
    this->_fStyleIndex = fontSyleIndex;
}

AppSettings::~AppSettings()
{

}

void AppSettings::registerWindow()
{
    // Register the window class.
    //const wchar_t CLASS_NAME[] = L"Settings Window";

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProcSettings;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->hInst;
    wcex.lpszClassName = this->CLASS_NAME;
    //wcex.hIcon = LoadIcon(this->hInst, IDI_SHIELD);

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



LRESULT AppSettings::runProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hdc;
    //NMLVDISPINFO* plvdi;



    
    //OutputDebugStringW((LPCWSTR)L"setings procedure---------------------------------------\r\n");

    


    switch (message)
    {
    case WM_NOTIFY:
        this->HandleWM_NOTIFY(lParam);
        break;

    case WM_CREATE:
        this->centerWindow(hwnd);

        //disable parent window
        EnableWindow(this->hWndParent, FALSE);
        
        break;

    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"*********COMMAND*************__\r\n");
        switch (LOWORD(wParam))
        {
        case IDM_CODE_SAMPLES:
            OutputDebugStringW((LPCWSTR)L"IDM_CODE_SAMPLES called____++++__\r\n");
            break;
        default:
            break;
        }

        switch (HIWORD(wParam))
        {
        case LBN_SETFOCUS:
            OutputDebugStringW((LPCWSTR)L"_))))))))))))))))))LBN_SETFOCUS_\r\n");
            break;
        case LBN_SELCHANGE:
            this->handleListBoxSelectionChange((HWND)lParam);
            OutputDebugStringW((LPCWSTR)L"LBN_SELCHANGE\r\n");
            
            break;

        default:
            break;
        }

        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    case WM_CLOSE:
        //Enable parent window
        EnableWindow(this->hWndParent, TRUE);
        SetFocus(this->hWndParent);
        DestroyWindow(hwnd);
        
        return 1;
        break;

    case WM_DESTROY:
        DestroyWindow(hwnd);
        //PostQuitMessage(0);

        break;
    default:
        //OutputDebugStringW((LPCWSTR)L"=================default===============\r\n");
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }


    return 0;
}

LRESULT AppSettings::WndProcSettings(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    AppSettings* pThis;

    if (message == WM_NCCREATE)
    {
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<AppSettings*>(lpcs->lpCreateParams);

        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    }
    else
    {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

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

LRESULT AppSettings::SubClassEditControl(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AppSettings* pThis;

    //OutputDebugStringW((LPCWSTR)L"&&&&&&&&&&& SubClassEditControl ]]]]]]]\r\n");

    // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
    pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            OutputDebugStringW((LPCWSTR)L"Enter~~~~\r\n");
            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
    default:
        OutputDebugStringW((LPCWSTR)L"default~~~~\r\n");
        return CallWindowProc(pThis->oldProc, hwnd, message, wParam, lParam);
        break;
    }

    return 0;

}

LRESULT AppSettings::LB_FontSize_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AppSettings* pThis;

    //OutputDebugStringW((LPCWSTR)L"&&&&&&&&&&& SubClassEditControl ]]]]]]]\r\n");

    // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
    pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


    switch (message)
    {
        OutputDebugStringW((LPCWSTR)L"massage__\r\n");
    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"COMMAND__\r\n");
        switch (HIWORD(wParam))
        {
        case LBN_SETFOCUS:
            OutputDebugStringW((LPCWSTR)L"_))))))))))))))))))LBN_SETFOCUS_\r\n");
            break;
        case LBN_SELCHANGE:
            pThis->handleListBoxSelectionChange((HWND)lParam);
            OutputDebugStringW((LPCWSTR)L"LBN_SELCHANGE\r\n");

            break;

        default:
            break;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            OutputDebugStringW((LPCWSTR)L"Enter^^^^\r\n");
            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
    default:
        OutputDebugStringW((LPCWSTR)L"default^^^^^\r\n");
        return CallWindowProc(pThis->oldProc, hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

LRESULT AppSettings::EC_fontFamily_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    AppSettings* pThis;

    //OutputDebugStringW((LPCWSTR)L"&&&&&&&&&&& SubClassEditControl ]]]]]]]\r\n");

    // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
    pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


    switch (message)
    {
    //case WM_KEYDOWN:
    //    switch (wParam)
    //    {
    //    case VK_RETURN:
    //        //Do your stuff
    //        OutputDebugStringW((LPCWSTR)L"Enter////\r\n");
    //        break;  //or return 0; if you don't want to pass it further to def proc
    //        //If not your key, skip to default:
    //    }
    //    break;
    case WM_KEYUP:
        
        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            OutputDebugStringW((LPCWSTR)L"Enter////\r\n");
            pThis->handleSearchControls((HWND)lParam);
            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
        break;

    default:
        OutputDebugStringW((LPCWSTR)L"default/////\r\n");
        return CallWindowProc(pThis->oldProc_EC_fontFamily, hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

LRESULT AppSettings::_ec_test_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    AppSettings* pThis;

    // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
    pThis = reinterpret_cast<AppSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (message)
    {
        //case WM_KEYDOWN:
        //    switch (wParam)
        //    {
        //    case VK_RETURN:
        //        //Do your stuff
        //        OutputDebugStringW((LPCWSTR)L"Enter////\r\n");
        //        break;  //or return 0; if you don't want to pass it further to def proc
        //        //If not your key, skip to default:
        //    }
        //    break;
    case WM_KEYUP:

        switch (wParam)
        {
        case VK_RETURN:
            //Do your stuff
            OutputDebugStringW((LPCWSTR)L"Enter$$$\r\n");
            //pThis->handleSearchControls(hwnd);

            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
        pThis->handleSearchControls(hwnd);
        break;

    default:
        OutputDebugStringW((LPCWSTR)L"default$$$\r\n");
        return CallWindowProc(pThis->_ec_test_oldProc, hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void AppSettings::centerWindow(HWND hwnd)
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

void AppSettings::initListFontSize()
{

    LPCWSTR fontSize[] = { L"8", L"10", L"12", L"14", L"16", L"18", L"20", L"22", L"24", L"26"};

    

}

//void AppSettings::createListView()
//{
//    // CreateListView: Creates a list-view control in report view.
//    // Returns the handle to the new control
//    // TO DO:  The calling procedure should determine whether the handle is NULL, in case 
//    // of an error in creation.
//    //
//    // HINST hInst: The global handle to the applicadtion instance.
//    // HWND  hWndParent: The handle to the control's parent window. 
//    //
//
//    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
//    icex.dwICC = ICC_LISTVIEW_CLASSES;
//    InitCommonControlsEx(&icex);
//
//    RECT rcClient;     // The parent window's client area.
//
//    GetClientRect(this->hWndGroupBox, &rcClient);
//
//    // Create the list-view window in report view with label editing enabled.
//    HWND hWndListView = CreateWindow(WC_LISTVIEW,
//        L"List view",
//        WS_CHILD | WS_VISIBLE | LVS_LIST | WS_BORDER | LVS_SINGLESEL,
//        10, 20,
//        (rcClient.right - rcClient.left) - 20,
//        (rcClient.bottom - rcClient.top) - 30,
//        //100, 200,
//        this->hWndSettings,
//        (HMENU)IDM_CODE_SAMPLES,
//        this->hInst,
//        NULL);
//
//    
//
//    this->hwndListView = hWndListView;
//
//
//    /*this->oldProc = (WNDPROC)SetWindowLongPtr(this->hwndListView,
//        GWLP_WNDPROC, (LONG_PTR)this->SubClassListViewProc);*/
//
//    /*if (this->oldProc == 0) {
//        OutputDebugStringW((LPCWSTR)L"SetWindowLongPtr Failed \r\n");
//    }*/
//
//
//    
//}

void AppSettings::insertListViewItems(int cItems)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText = (wchar_t*)L"8"; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;
    lvI.iItem = 0;

    ListView_InsertItem(this->hwndListView, &lvI);


    for (int i = 9; i <= 72; i++) {
        lvI.iItem = i;
        //converting int to wstring--
        std::wstring str = std::to_wstring(i);
        lvI.pszText = (wchar_t*)str.c_str();
        ListView_InsertItem(this->hwndListView, &lvI);
    }
    
    //MessageBox(0, TEXT("inserting itmes list view "), 0, 0);



}

void AppSettings::initListView()
{
    
}

void AppSettings::createGroupBox()
{
    HWND hwndGroupBox = this->getGroupBox(L"Font", 10, 10, 100, 200);

    this->hWndGroupBox = hwndGroupBox;

    /*HWND hGr = CreateWindow(L"BUTTON", L"My GRoup Box",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 10, 200, 200, this->hWndSettings, (HMENU)-1, this->hInst, NULL);*/

        // Put the value in a safe place for future use
        SetWindowLongPtr(this->hWndGroupBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        this->oldProc = (WNDPROC)SetWindowLongPtr(this->hWndGroupBox, GWLP_WNDPROC, (LONG_PTR)this->LB_FontSize_WndProc);

}

void AppSettings::createGroupBoxSample()
{
    HWND hwndGroupBox = this->getGroupBox(L"Sample", 10, 320, 300, 100);

    this->hWndGroupBoxSample = hwndGroupBox;
}

void AppSettings::initListViewSample()
{

}

void AppSettings::initSampleText()
{

    RECT rcClient;                       // The parent window's client area.

    GetClientRect(this->hWndGroupBoxSample, &rcClient);


    HWND hwndStatic = CreateWindowEx(
        0,
        L"static",      // Button text 
        L"AaBbYyZz",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,  // Styles
        10,         // x position 
        20,         // y position
        (rcClient.right - rcClient.left) - 20,  // window width
        (rcClient.bottom - rcClient.top) - 30,  // window height
        this->hWndGroupBoxSample,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(this->hWndGroupBoxSample, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.


    HFONT font = this->getFont(this->fontSize, this->fontFamily);

    SendMessage(hwndStatic, WM_SETFONT, (WPARAM)font, TRUE);

    this->hWndGroupBoxSampleText = hwndStatic;
   
}

void AppSettings::createTooltilp()
{
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | WS_VISIBLE | TTS_NOPREFIX | TTS_ALWAYSTIP, 0, 0, 0, 0, this->hwndListView, NULL, 0, NULL);

    TTTOOLINFO ti = { 0 };
    ti.cbSize = sizeof(TTTOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = this->hWndGroupBoxSampleText;
    ti.lpszText = (wchar_t*)L"Tooltip string";
    GetClientRect(this->hWndGroupBoxSampleText, &ti.rect);

    if (!SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)&ti))
        MessageBox(0, TEXT("Failed: TTM_ADDTOOL"), 0, 0);

}

void AppSettings::HandleWM_NOTIFY(LPARAM lParam)
{
    /*LPNMITEMACTIVATE lpnmitem;
    std::wstring str;*/
    int fontSize;

    int index = ListView_GetSelectionMark(this->hwndListView);
    if (index == -1) return;

    fontSize = index + 8;

    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(fontSize), TRUE);

    return;
    //below code is just for reference pupose

    //switch (((LPNMHDR)lParam)->code)
    //{
    //case NM_CLICK:
    //    lpnmitem = (LPNMITEMACTIVATE)lParam;
    //    lpnmitem->iItem;

    //    //fontSize = this->getFontSizeFromIndex(lpnmitem->iItem+8);

    //    //SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(fontSize), TRUE);

    //    ///*if (lpnmitem->iItem == FONT_SIZE_8) {
    //    //    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(18), TRUE);
    //    //    OutputDebugStringW((LPCWSTR)L"FONT_SIZE_8\r\n");
    //    //}*/

    //    //str = L"" + std::to_wstring(lpnmitem->iItem);

    //    //OutputDebugStringW( (LPCWSTR)str.c_str() );
    //    //
    //    //OutputDebugStringW((LPCWSTR)L"------++++++++LVN_BEGINDRAG-------------------\r\n");
    //    break;
    //default:
    //    break;
    //}


}

int AppSettings::getFontSizeFromIndex(int index)
{
    if (index % 2 == 0) return ++index;
    else return index;
}

void AppSettings::handleFocuses()
{

    //SendMessage(this->hwndListView, LVM_EDITLABEL, 0, 0);
    ListView_EditLabel(this->hwndListView, 0);
    //SetFocus(this->hwndListView);
    //ListView_SetSelectionMark(this->hwndListView, (72-8));
}

void AppSettings::createListBox()
{
    RECT rcClient;     // The parent window's client area.
    GetClientRect(this->hWndGroupBox, &rcClient);

    size_t posX = 20;
    size_t posY = 30;
    size_t width = (rcClient.right - rcClient.left) - 20;
    size_t height = (rcClient.bottom - rcClient.top) - 30;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndGroupBox, (int)posX, (int)posY, (int)width, (int)height);

    this->hWndListBox_FontSize = hListBox;

    // Put the value in a safe place for future use
    //SetWindowLongPtr(this->hWndListBox_FontSize, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    //this->oldProc = (WNDPROC)SetWindowLongPtr(this->hWndListBox_FontSize, GWLP_WNDPROC, (LONG_PTR)this->LB_FontSize_WndProc);

}

void AppSettings::initListViewBox()
{
    int pos = 0;

    for (int i = 8; i <= 72; i++) {
        //converting int to wstring.
        std::wstring str = std::to_wstring(i);
        pos = (int)SendMessage(this->hWndListBox_FontSize, LB_ADDSTRING, 0,
            (LPARAM)(wchar_t*)str.c_str());
    }

    SendMessage(this->hWndListBox_FontSize, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    SendMessage(this->hWndListBox_FontSize, LB_SETCURSEL, this->_fSizeIndex, 0);
    SetFocus(this->hWndListBox_FontSize);
}

void AppSettings::setSampleTextFontSize(size_t size)
{
    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(size), TRUE);
}

void AppSettings::handleListBoxSelectionChange(HWND hWnd)
{
    if (hWnd == this->hWndListBox_FontSize) {
        LRESULT index = SendMessage(this->hWndListBox_FontSize, LB_GETCURSEL, 0, 0);
        LPCWSTR itemText = this->getSelectedFontFamily();
        //this->setSampleTextFontSize(index + 8);
        SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(index + 8, itemText), TRUE);

        //free up memory
        delete itemText;
    }
    else if (hWnd == this->hWndEditControlFontStyles){
        /*LRESULT index = SendMessage(this->hWndEditControlFontStyles, LB_GETCURSEL, 0, 0);
        if (index < 0) return;*/
        
        //get text lenght
        /*LRESULT length = SendMessage(this->hWndEditControlFontStyles, LB_GETTEXTLEN, index, 0);

        LPCWSTR itemText = new WCHAR[(int)length + 1];

        SendMessage(this->hWndEditControlFontStyles, LB_GETTEXT, index, (LPARAM)itemText);
        */
        //OutputDebugStringW((LPCWSTR)itemText);

        LPCWSTR itemText = this->getSelectedFontFamily();

        if (!itemText) return;

        
        LRESULT indexFontSize = SendMessage(this->hWndListBox_FontSize, LB_GETCURSEL, 0, 0);
        SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(indexFontSize+8, itemText), TRUE);
        

        delete itemText;

    }

    
    
    
    
}

HWND AppSettings::getGroupBox(LPCWSTR name, int posX, int posY, int width, int height)
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
        this->hWndSettings,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(this->hWndSettings, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)this->getFont(14), TRUE);

    return hwndGroupBox;
}

void AppSettings::createComboBox()
{

    // Create the Combobox
//
// Uses the CreateWindow function to create a child window of 
// the application window. The WC_COMBOBOX window style specifies  
// that it is a combobox.

    int xpos = 100;            // Horizontal position of the window.
    int ypos = 100;            // Vertical position of the window.
    int nwidth = 200;          // Width of the window
    int nheight = 200;         // Height of the window
    HWND hwndParent = this->hWndSettings; // Handle to the parent window

    HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        xpos, ypos, nwidth, nheight, hwndParent, NULL, this->hInst,
        NULL);








    //CreateWindowEx(WS_EX_STATICEDGE, L"COMBOBOX", L"MyCombo1",
    //    CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
    //    200, 10, 100, 209, this->hWndSettings, NULL, this->hInst, NULL); // 100 = ID of this control



}

void AppSettings::createEditControlFontStyles()
{
    HWND hwndEdit = this->getEditControl(200, 10, 100, 20);
    this->hWndEditControlFontStyles = hwndEdit;
    
    // Put the value in a safe place for future use
    SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    this->oldProc_EC_fontFamily = (WNDPROC)SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_WNDPROC, (LONG_PTR)this->EC_fontFamily_WndProc);
    
}

void AppSettings::createListBox_FontStyles()
{
    int posX = 200;
    int posY = 50;
    int width = 100;
    int height = 200;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndSettings, posX, posY, width, height);

    this->hWndEditControlFontStyles = hListBox;


    //data for font families-----------
    /*LPCWSTR fontFamilies[] = {
        L"Arial", L"Arial Black", L"Calibri", L"Cambria", L"Cambria Math", L"MS New Tai Lue",
        L"MS PhagsPa", L"Times New Roman", L"DaunPenh", L"Georgia Pro", L"Shonar Bangla", L"Vrinda",
        L"Simplified Arabic", L"Sakkal Majalla", L"Andalus", L"Yu Gothic", L"Tahoma", L"SimSun"
    };*/

    //inserting items-----------------
    int pos = 0;

    for (int i = 0; i < ARRAYSIZE(this->fontFamilies) ; i++) {

        pos = (int)SendMessage(this->hWndEditControlFontStyles, LB_ADDSTRING, 0,
            (LPARAM)fontFamilies[i]);
        
        //set font for indidual item----

    }

    //find string from list box--
    LRESULT searchIndex = SendMessage(this->hWndEditControlFontStyles, LB_FINDSTRING, -1, (LPARAM)this->fontFamily);
    if(searchIndex == LB_ERR) searchIndex = 0;

    SendMessage(this->hWndEditControlFontStyles, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    SendMessage(this->hWndEditControlFontStyles, LB_SETCURSEL, this->_fFamilyIndex, 0);
    //SetFocus(this->hWndEditControlFontStyles);
}

size_t AppSettings::getFontSizeSampleText()
{
    return size_t();
}

LPCWSTR AppSettings::getSelectedFontFamily()
{
    LRESULT index = SendMessage(this->hWndEditControlFontStyles, LB_GETCURSEL, 0, 0);

    //get text lenght
    LRESULT length = SendMessage(this->hWndEditControlFontStyles, LB_GETTEXTLEN, (int)index, 0);

    LPCWSTR itemText = new WCHAR[(int)length + 1];

    SendMessage(this->hWndEditControlFontStyles, LB_GETTEXT, index, (LPARAM)itemText);

    return itemText;
}

BOOL AppSettings::isValidIndex(size_t totalItems, size_t index)
{
    if (index < totalItems) return TRUE;
    return FALSE;
}

void AppSettings::showConfigKeysCorrupted()
{
    MessageBox(this->hWndParent,
        (LPCWSTR)L"Configuratin variables were corrupted! The App has adapted default Settings.",
        (LPCWSTR)L"App Configuration",
        MB_ICONEXCLAMATION);
}

void AppSettings::handleSearchControls(HWND hWnd)
{
    ////if (hWnd == this->hWndEditControlFontStyles) {
    //    //get main window text lenght--
    //    const int size = GetWindowTextLength(this->hWndEditControlFontStyles);
    //    //wchar_t* data = new wchar_t[size + 1];
    //    LPCWSTR data = new WCHAR[size + 1];

    //    GetWindowText(this->hWndEditControlFontStyles, (LPWSTR)data, size + 1);

    //    MessageBox(
    //        this->hWndSettings,
    //        (LPCWSTR)data,
    //        (LPCWSTR)L"search",
    //        MB_OK
    //    );
    ////}

    int len = GetWindowTextLength(hWnd); //this->_hwnd_editControlTest
    //LOG_INT(len);

    LPWSTR text[50];

    GetWindowText(hWnd, (LPWSTR)text, 50);

    LRESULT searchIndex = SendMessage(this->hWndEditControlFontStyles, LB_FINDSTRING, 0,  (LPARAM)text);

    if (searchIndex != LB_ERR) {
        SendMessage(this->hWndEditControlFontStyles, LB_SETCURSEL, searchIndex, 0);
        SendMessage(this->hWndEditControlFontStyles, LB_SETTOPINDEX, searchIndex, 0);
    }
    else {
        SendMessage(this->hWndEditControlFontStyles, LB_SETCURSEL, -1, 0);
    }

    //LOG_WCHAR(L"searchIndex ==>");
    //LOG_INT(searchIndex);

    ////delete data;

}

void AppSettings::createEditControlTest()
{
    HWND hwndEdit = this->getEditControl(350, 100, 150, 20);
    this->_hwnd_editControlTest = hwndEdit;

    // Put the value in a safe place for future use
    SetWindowLongPtr(this->_hwnd_editControlTest, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    this->_ec_test_oldProc = (WNDPROC)SetWindowLongPtr(this->_hwnd_editControlTest, GWLP_WNDPROC, (LONG_PTR)this->_ec_test_proc);

}

void AppSettings::_createListBox_fontStyles()
{
    int posX = 400;
    int posY = 150;
    int width = 100;
    int height = 200;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndSettings, posX, posY, width, height);

    //const char* arr[] = { "abc", "def" };

    //this->_insertItems_listBox(hListBox, 0, this->fontFamilies);


}

size_t AppSettings::getFontSize()
{
    return this->fontSize;
}

LPCWSTR AppSettings::getFontFamily()
{
    return this->fontFamily;
}

LPCWSTR AppSettings::getFontStyle()
{
    return this->fontSyle;
}


void AppSettings::createWindow()
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        this->CLASS_NAME,                     // Window class
        L"Settings",    // Window text
        WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 650, 500,

        this->hWndParent,       // Parent window    
        NULL,       // Menu
        this->hInst,  // Instance handle
        this        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            this->hWndParent,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );
    }

    //ShowWindow(hwnd, this->nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);



    this->hWndSettings = hwnd;
    //give this handle to base class i.e. (WindowControls).
    this->setWindowControlsHandle(this->hWndSettings);

}

//template<size_t size>
//inline errno_t AppSettings::_insertItems_listBox(HWND hwnd, int select, const wchar_t* (&itemsArray)[size])
//{
//
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
