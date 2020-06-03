#pragma once


LRESULT CALLBACK WndProcSettings(HWND, UINT, WPARAM, LPARAM); //declaration of the window procedures method.







void handleSettingsDialogBoxWindow(HWND hWnd) {
    MessageBox(
        hWnd,
        (LPCWSTR)L"handleSettingsDialogBoxWindow.",
        (LPCWSTR)L"Settings...",
        MB_ICONERROR
    );

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




    return 1;
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