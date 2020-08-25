
#include "AppSettings.h"


AppSettings::AppSettings(HWND hWnd, HINSTANCE hInstance, int nCmdShow) : WindowControls(this->hWndSettings, hInstance)
{
    this->hWndParent = hWnd;
    this->hInst = hInstance;
    this->nCmdShowGlobal = nCmdShow;
}

void AppSettings::initWindow()
{
    //creating settings Window.
    this->createWindow();

    //fontsize--fontFamily--fontStyles Static Controls.
    this->createStaticsControls();
    
    //fontsize--fontFamily--fontStyles Static Controls.
    this->createEditControlFontSize();      //fontSize editControl.
    this->createEditControlFontStyles();    //fontFamily editControl.
    this->createEditControlTest();          //fontStyles editControl.

    //fontsize--fontFamily--fontStyles Static Controls.
    this->createListBox();                  //fontSize listBox.
    this->createListBox_FontStyles();       //fontFamily listBox.
    this->_createListBox_fontStyles();      //fontStyles listBox.

    //Sample groupBox and StaticControl.
    this->createGroupBoxSample();           //sample GroupBox.
    this->initSampleText();                 //sample StaticControl.

    //OK and Cancel Buttons.
    this->createOKButton();                 //Button OK
    this->createCancelButton();             //Button Cancel

    //configure all edit controls texts.
    this->configureEditCtrlsText();

    //Now set Focuses and show window beacuse every thing is ready.
    this->handleFocuses();                  //handling Focuses.
    ShowWindow(this->hWndSettings, this->nCmdShowGlobal);  //now every thing is ready show the window.

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
    wcex.hIcon = 0;
    wcex.hIconSm = 0;

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
        
        //removing icon.
        ::SendMessage(hwnd, WM_SETICON, 0, NULL);
        //::SendMessage(hwnd, WM_SETICON, 1, NULL);
        break;

    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"*********COMMAND*************__\r\n");
        switch (LOWORD(wParam))
        {
        case IDM_CODE_SAMPLES:
            OutputDebugStringW((LPCWSTR)L"IDM_CODE_SAMPLES called____++++__\r\n");
            break;
        case UID_BUTTON_OK:
            this->handleSaveConfigs();
            break;
        case UID_BUTTON_CANCEL:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
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
            //pThis->handleListBoxSelectionChange((HWND)lParam);
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
            //pThis->handleSearchControls((HWND)lParam);
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
        pThis->handleSearchControls(pThis->hWndEditControlFontStyles, pThis->_hwnd_listBox_fontFamily);
        break;

    default:
        OutputDebugStringW((LPCWSTR)L"default$$$\r\n");
        return CallWindowProc(pThis->_ec_test_oldProc, hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

LRESULT AppSettings::_ec_FontSize_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
            OutputDebugStringW((LPCWSTR)L"Enter fontSize\r\n");
            //pThis->handleSearchControls(hwnd);

            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
        pThis->handleSearchControls(pThis->_hwnd_editControl_FontSize, pThis->_hwnd_listBox_fontSize);
        break;

    default:
        OutputDebugStringW((LPCWSTR)L"default fontSize\r\n");
        return CallWindowProc(pThis->_ec_FontSize_oldProc, hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

LRESULT AppSettings::_ec_FontStyle_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
            OutputDebugStringW((LPCWSTR)L"Enter fontSize\r\n");
            //pThis->handleSearchControls(hwnd);

            break;  //or return 0; if you don't want to pass it further to def proc
            //If not your key, skip to default:
        }
        pThis->handleSearchControls(pThis->_hwnd_editControlTest, pThis->_hwnd_listBox_fontStyes);
        break;

    default:
        OutputDebugStringW((LPCWSTR)L"default fontSize\r\n");
        return CallWindowProc(pThis->_ec_FontStyle_oldProc, hwnd, message, wParam, lParam);
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
        //SetWindowLongPtr(this->hWndGroupBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        //this->oldProc = (WNDPROC)SetWindowLongPtr(this->hWndGroupBox, GWLP_WNDPROC, (LONG_PTR)this->LB_FontSize_WndProc);

}

void AppSettings::createGroupBoxSample()
{
    HWND hwndGroupBox = this->getGroupBox(L"Sample", 15, 260, 330, 100);

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

    //SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(fontSize), TRUE);

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
    //RECT rcClient;     // The parent window's client area.
    //GetClientRect(this->hWndGroupBox, &rcClient);

    size_t posX = 15;
    size_t posY = 60;
    size_t width = 100;//(rcClient.right - rcClient.left) - 20;
    size_t height = 200;//(rcClient.bottom - rcClient.top) - 30;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndSettings, (int)posX, (int)posY, (int)width, (int)height);
    this->_hwnd_listBox_fontSize = hListBox;

    //inserting items to listBox.
    this->initListViewBox(hListBox);


    // Put the value in a safe place for future use
    //SetWindowLongPtr(this->_hwnd_listBox_fontSize, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    //this->_LB_FontSize_oldProc = (WNDPROC)SetWindowLongPtr(this->_hwnd_listBox_fontSize, GWLP_WNDPROC, (LONG_PTR)this->_LB_FontSize_proc);
    
}

void AppSettings::initListViewBox(HWND hListBox)
{
    int pos = 0;

    for (int i = 8; i <= 72; i++) {
        //converting int to wstring.
        std::wstring str = std::to_wstring(i);
        pos = (int)SendMessage(hListBox, LB_ADDSTRING, 0,
            (LPARAM)(wchar_t*)str.c_str());
    }

    SendMessage(hListBox, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    SendMessage(hListBox, LB_SETCURSEL, this->_fSizeIndex, 0);
    //SetFocus(this->hWndListBox_FontSize);
}

void AppSettings::setSampleTextFontSize(size_t size)
{
    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(size), TRUE);
}

void AppSettings::handleListBoxSelectionChange(HWND hWnd)
{

    if (hWnd == this->_hwnd_listBox_fontSize) {
        this->handleCopyTextToEditControl(hWnd, this->_hwnd_editControl_FontSize);
    }
    else if (hWnd == this->_hwnd_listBox_fontFamily) {
        this->handleCopyTextToEditControl(hWnd, this->hWndEditControlFontStyles);
    }
    else if (hWnd == this->_hwnd_listBox_fontStyes) {
        this->handleCopyTextToEditControl(hWnd, this->_hwnd_editControlTest);
    }

    this->updateSampleText();



    //if (hWnd == this->hWndListBox_FontSize) {
    //    LRESULT index = SendMessage(this->hWndListBox_FontSize, LB_GETCURSEL, 0, 0);
    //    LPCWSTR itemText = this->getSelectedFontFamily();
    //    //this->setSampleTextFontSize(index + 8);
    //    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(index + 8, itemText), TRUE);

    //    //free up memory
    //    delete itemText;
    //}
    //else if (hWnd == this->hWndEditControlFontStyles){
    //    /*LRESULT index = SendMessage(this->hWndEditControlFontStyles, LB_GETCURSEL, 0, 0);
    //    if (index < 0) return;*/
    //    
    //    //get text lenght
    //    /*LRESULT length = SendMessage(this->hWndEditControlFontStyles, LB_GETTEXTLEN, index, 0);

    //    LPCWSTR itemText = new WCHAR[(int)length + 1];

    //    SendMessage(this->hWndEditControlFontStyles, LB_GETTEXT, index, (LPARAM)itemText);
    //    */
    //    //OutputDebugStringW((LPCWSTR)itemText);

    //    LPCWSTR itemText = this->getSelectedFontFamily();

    //    if (!itemText) return;

    //    
    //    LRESULT indexFontSize = SendMessage(this->hWndListBox_FontSize, LB_GETCURSEL, 0, 0);
    //    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)this->getFont(indexFontSize+8, itemText), TRUE);
    //    

    //    delete itemText;

    //}

    //
    //
    
    
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

    SendMessage(hwndGroupBox, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);

    return hwndGroupBox;
}

void AppSettings::createOKButton()
{
    HWND hwndButton = this->getButton(this->hWndSettings, L"OK", UID_BUTTON_OK, 175, 370);

}

void AppSettings::createCancelButton()
{
    HWND hwndButton = this->getButton(this->hWndSettings, L"Cancel", UID_BUTTON_CANCEL, 265, 370);
}

void AppSettings::createStaticsControls()
{
    HWND hwndFontSize = this->getStatic(this->hWndSettings, L"Font Size:", 15, 20);
    HWND hwndFontStyle = this->getStatic(this->hWndSettings, L"Font Family:", 130, 20);
    HWND hwndFontFamily = this->getStatic(this->hWndSettings, L"Font Style:", 245, 20);
}

void AppSettings::createEditControlFontSize()
{
    HWND hwndEdit = this->getEditControl(15, 40, 100, 20);
    this->_hwnd_editControl_FontSize = hwndEdit;



    // Put the value in a safe place for future use
    SetWindowLongPtr(this->_hwnd_editControl_FontSize, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    this->_ec_FontSize_oldProc = (WNDPROC)SetWindowLongPtr(this->_hwnd_editControl_FontSize, GWLP_WNDPROC, (LONG_PTR)this->_ec_FontSize_proc);

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
    HWND hwndEdit = this->getEditControl(130, 40, 100, 20);
    this->hWndEditControlFontStyles = hwndEdit;
    
    // Put the value in a safe place for future use
    //SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    //this->oldProc_EC_fontFamily = (WNDPROC)SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_WNDPROC, (LONG_PTR)this->EC_fontFamily_WndProc);

    // Put the value in a safe place for future use
    SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    this->_ec_test_oldProc = (WNDPROC)SetWindowLongPtr(this->hWndEditControlFontStyles, GWLP_WNDPROC, (LONG_PTR)this->_ec_test_proc);
    
}

void AppSettings::createListBox_FontStyles()
{
    int posX = 130;
    int posY = 60;
    int width = 100;
    int height = 200;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndSettings, posX, posY, width, height);

    this->_hwnd_listBox_fontFamily = hListBox;


    //data for font families-----------
    /*LPCWSTR fontFamilies[] = {
        L"Arial", L"Arial Black", L"Calibri", L"Cambria", L"Cambria Math", L"MS New Tai Lue",
        L"MS PhagsPa", L"Times New Roman", L"DaunPenh", L"Georgia Pro", L"Shonar Bangla", L"Vrinda",
        L"Simplified Arabic", L"Sakkal Majalla", L"Andalus", L"Yu Gothic", L"Tahoma", L"SimSun"
    };*/

    //inserting items-----------------
    int pos = 0;

    for (int i = 0; i < ARRAYSIZE(this->fontFamilies) ; i++) {

        pos = (int)SendMessage(this->_hwnd_listBox_fontFamily, LB_ADDSTRING, 0,
            (LPARAM)fontFamilies[i]);
        
        //set font for indidual item----

    }

    //find string from list box--
    LRESULT searchIndex = SendMessage(this->_hwnd_listBox_fontFamily, LB_FINDSTRING, -1, (LPARAM)this->fontFamily);
    if(searchIndex == LB_ERR) searchIndex = 0;

    SendMessage(this->_hwnd_listBox_fontFamily, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    SendMessage(this->_hwnd_listBox_fontFamily, LB_SETCURSEL, this->_fFamilyIndex, 0);
    //SetFocus(this->hWndEditControlFontStyles);
}

size_t AppSettings::getFontSizeSampleText()
{
    return size_t();
}

void AppSettings::handleCopyTextToEditControl(HWND hwndListBox, HWND hwndEdit)
{
    LRESULT index = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);

    //get text lenght
    LRESULT length = SendMessage(hwndListBox, LB_GETTEXTLEN, (int)index, 0);

    LPCWSTR itemText = new WCHAR[(int)length + 1];

    SendMessage(hwndListBox, LB_GETTEXT, index, (LPARAM)itemText);
    SetWindowText(hwndEdit, itemText);
    //make the selection working if edit control doesn't have focus.
    //SendMessage(hwndEdit, EM_SETSEL, 0, -1);

    LOG_WCHAR(itemText);

    delete itemText;
}

void AppSettings::configureEditCtrlsText()
{
    this->handleCopyTextToEditControl(this->_hwnd_listBox_fontSize, this->_hwnd_editControl_FontSize);
    this->handleCopyTextToEditControl(this->_hwnd_listBox_fontFamily, this->hWndEditControlFontStyles);
    this->handleCopyTextToEditControl(this->_hwnd_listBox_fontStyes, this->_hwnd_editControlTest);
}

void AppSettings::updateSampleText()
{
    LRESULT indexFontSize = SendMessage(this->_hwnd_listBox_fontSize, LB_GETCURSEL, 0, 0);
    LRESULT indexFontFamily = SendMessage(this->_hwnd_listBox_fontFamily, LB_GETCURSEL, 0, 0);
    LRESULT indexFontStyles = SendMessage(this->_hwnd_listBox_fontStyes, LB_GETCURSEL, 0, 0);
    /*if (indexFontSize != LB_ERR) {
        LOG_INT(indexFontSize + 8);
    }*/

    /*if (indexFontFamily != LB_ERR) {
        LOG_INT(indexFontFamily);
        LOG_WCHAR((LPCWSTR)this->fontFamilies[indexFontFamily]);
    }*/

    /*if (indexFontStyles != LB_ERR) {
        LOG_INT(indexFontStyles);
        LOG_WCHAR((LPCWSTR)this->fontStyles[indexFontStyles]);
    }*/

    if ((indexFontSize == LB_ERR) || (indexFontFamily == LB_ERR) || (indexFontStyles == LB_ERR)) {
        SetWindowText(this->hWndGroupBoxSampleText, L"");
        return;
    }

    size_t fontSize = indexFontSize + 8;
    LPCWSTR fontFamily = (LPCWSTR)this->fontFamilies[indexFontFamily];
    LPCWSTR fontStyle = (LPCWSTR)this->fontStyles[indexFontStyles];


    HFONT font = this->getFont(fontSize, fontFamily, fontStyle);
    SendMessage(this->hWndGroupBoxSampleText, WM_SETFONT, (WPARAM)font, TRUE);
    


}

void AppSettings::handleSaveConfigs()
{
    LRESULT indexFontSize = SendMessage(this->_hwnd_listBox_fontSize, LB_GETCURSEL, 0, 0);
    LRESULT indexFontFamily = SendMessage(this->_hwnd_listBox_fontFamily, LB_GETCURSEL, 0, 0);
    LRESULT indexFontStyles = SendMessage(this->_hwnd_listBox_fontStyes, LB_GETCURSEL, 0, 0);


    if ((indexFontSize == LB_ERR) || (indexFontFamily == LB_ERR) || (indexFontStyles == LB_ERR)) {
        MessageBox(
            this->hWndSettings,
            (LPCWSTR)L"Some of the Fonts are missing! Please select all the fonts and then try to save.",
            (LPCWSTR)L"Configuration!",
            MB_ICONWARNING
        );
        return;
    }

    _configVars* vars = new _configVars();
    vars->fontSizeIndex = (int)indexFontSize;
    vars->fontSizeFamilyIndex = (int)indexFontFamily;
    vars->fontSizeStyleIndex = (int)indexFontStyles;

    HFONT _font_sampleText =  (HFONT)SendMessage(this->hWndGroupBoxSampleText, WM_GETFONT, 0, 0);


    SendMessage(this->hWndParent , WM_APPLY_CONFIGURATION, (WPARAM)_font_sampleText, (LPARAM)vars);
    SendMessage(this->hWndSettings, WM_CLOSE, 0, 0);

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

void AppSettings::handleSearchControls(HWND hwndEdit, HWND hwndListBox)
{
    int len = GetWindowTextLength(hwndEdit);

    LPWSTR text[50];

    GetWindowText(hwndEdit, (LPWSTR)text, 50);

    LRESULT searchIndex = searchIndex = SendMessage(hwndListBox, LB_FINDSTRING, 0, (LPARAM)text);
    
    if (searchIndex != LB_ERR) {
        SendMessage(hwndListBox, LB_SETCURSEL, searchIndex, 0);
        SendMessage(hwndListBox, LB_SETTOPINDEX, searchIndex, 0);
    }
    else {
        SendMessage(hwndListBox, LB_SETCURSEL, -1, 0);
    }

}

void AppSettings::createEditControlTest()
{
    HWND hwndEdit = this->getEditControl(245, 40, 100, 20);
    this->_hwnd_editControlTest = hwndEdit;

    // Put the value in a safe place for future use
    SetWindowLongPtr(this->_hwnd_editControlTest, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    this->_ec_FontStyle_oldProc = (WNDPROC)SetWindowLongPtr(this->_hwnd_editControlTest, GWLP_WNDPROC, (LONG_PTR)this->_ec_FontStyle_proc);

}

void AppSettings::_createListBox_fontStyles()
{
    int posX = 245;
    int posY = 60;
    int width = 100;
    int height = 200;

    // Adding a ListBox.
    HWND hListBox = this->getListBox(this->hWndSettings, posX, posY, width, height);
    this->_hwnd_listBox_fontStyes = hListBox;

    this->_insertItems_listBox(hListBox, 0, this->fontStyles);

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
        0,          // Optional window styles. Called Exended Window Styles.
        this->CLASS_NAME,                       // Window class
        L"Settings",    // Window text
        WS_SYSMENU,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 377, 442,

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

template<size_t size>
inline errno_t AppSettings::_insertItems_listBox(HWND hwnd, int select, const wchar_t* (&itemsArray)[size])
{

    int pos = 0;
    
    for (int i = 0; i < ARRAYSIZE(itemsArray); i++) {
    
        pos = (int)SendMessage(hwnd, LB_ADDSTRING, 0,
            (LPARAM)itemsArray[i]);
    
        //set font for indidual item----
    
    }
    
    //find string from list box--
    LRESULT searchIndex = SendMessage(hwnd, LB_FINDSTRING, -1, (LPARAM)this->fontFamily);
    if (searchIndex == LB_ERR) searchIndex = 0;
    
    SendMessage(hwnd, WM_SETFONT, (WPARAM)this->getFont(16), TRUE);
    SendMessage(hwnd, LB_SETCURSEL, select, 0);
    
    return 0;
}
