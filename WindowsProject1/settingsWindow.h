#pragma once


LRESULT CALLBACK WndProcSettings(HWND, UINT, WPARAM, LPARAM); //declaration of the window procedures method.



INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);




LRESULT CALLBACK SubClassProcComboBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void handleSettingsComboBoxWindow(HWND hWnd) {
    //  Create a combo box child window.
    DWORD dwBaseUnits;
    static HWND hwndCombo1;
    POINT pt;
    static HWND   hwndEdit1;

    dwBaseUnits = GetDialogBaseUnits();

    hwndCombo1 = CreateWindow(L"COMBOBOX", L"",
        CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
        (6 * LOWORD(dwBaseUnits)) / 4,
        (2 * HIWORD(dwBaseUnits)) / 8,
        (100 * LOWORD(dwBaseUnits)) / 4,
        (50 * HIWORD(dwBaseUnits)) / 8,
        hWnd, NULL, NULL, NULL);

    //  Get the edit window handle to each combo box. 
    pt.x = 1;
    pt.y = 1;

    hwndEdit1 = ChildWindowFromPoint(hwndCombo1, pt);

    lpfnMainWndProc = (WNDPROC)SetWindowLongPtr(hwndEdit1,
        GWLP_WNDPROC, (LONG_PTR)SubClassProcComboBox);

}


LRESULT CALLBACK SubClassProcComboBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:

        OutputDebugStringW((LPCWSTR)L"---------------WM_KEYDOWN called>>>>>>>>>>>>\r\n");
        break;
    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"---------------WM_COMMAND called>>>>>>>>>>>>\r\n");

        switch (wParam)
        {
        case EN_CHANGE:
            OutputDebugStringW((LPCWSTR)L">>>>>>>>>>>>EN_CHANGE called<<<<<<<<<<<<");
            break;
        }
        break;
    case WM_KEYUP:
    case WM_CHAR:
        switch (wParam)
        {
        case VK_TAB:

            break;
        case VK_ESCAPE:
            OutputDebugStringW((LPCWSTR)L"VK_ESCAPE");

            break;
        case VK_RETURN:
            OutputDebugStringW((LPCWSTR)L"VK_RETURN");
            break;
            return 0;
        }
    }

    //  Call the original window procedure for default processing. 
    return CallWindowProc(lpfnMainWndProc, hwnd, msg, wParam, lParam);
}
















int CALLBACK handleSettingsWindow(HWND hWnd) {

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcSettings;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIcon = LoadIcon(hInst, IDI_SHIELD);

    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Settings",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 300,

        hWnd,       // Parent window    
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        MessageBox(
            hWnd,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );

        return 0;
    }

    ShowWindow(hwnd, nCmdShowGlobal);


    //handleSettingsComboBoxWindow(hwnd);

    return 1;
}


BOOL CALLBACK GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    OutputDebugStringW((LPCWSTR)L"WM_INITDIALOG called__________\r\n");

    switch (message)
    {
    case WM_INITDIALOG:
        OutputDebugStringW((LPCWSTR)L"WM_INITDIALOG called successfully__________\r\n");
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            DestroyWindow(hwndDlg);
            break;
        }
    }
    return FALSE;
}


void handleSearchFile(HWND hWnd) {
    /*MessageBox(
        hWnd,
        (LPCWSTR)L"Search feature will be available in upcoming versions!",
        (LPCWSTR)L"Search",
        MB_OK | MB_ICONASTERISK
    );*/


    Search* search = new Search(hWnd, hInst, nCmdShowGlobal);


}



void handleSettingsDialog(HWND hWnd) {
    /*MessageBox(
        hWnd,
        (LPCWSTR)L"Settings will be available in upcoming versions!",
        (LPCWSTR)L"Settings",
        MB_OK | MB_ICONASTERISK
    );*/

    //handleSettingsWindow(hWnd);

    AppSettings* settings = new AppSettings(hWnd, hInst, nCmdShowGlobal);

    //EnableWindow(hWnd, FALSE);

}






//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProcSettings(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;


    switch (message)
    {
    case WM_CREATE:
        handleCenterWindow(hWnd);
        break;

    case WM_COMMAND:
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    
    case WM_DESTROY:
        DestroyWindow(hWnd);
        //PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }


    return 0;
}
















































/*
void handleSettingsWindow(HWND hWnd) {

    //HINSTANCE hInstance;
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    HWND hSettingsWindow = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        hWnd,       // Parent window
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
    );

    //SetParent(hSettingsWindow, hWnd);

    if (hSettingsWindow == NULL)
    {
        MessageBox(
            hWnd,
            (LPCWSTR)L"Settings window creation failed.",
            (LPCWSTR)L"ERROR",
            MB_ICONERROR
        );
    }
}

*/

/*(LPCWSTR)L"Settings window creation failed."*/


INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}



