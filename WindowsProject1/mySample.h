#pragma once



void DisplayResourceNAMessageBox(HWND hWnd = NULL)
{
    MessageBox(
        hWnd,
        (LPCWSTR)L"Resource not available\nDo you want to try again?",
        (LPCWSTR)L"Account Details",
        MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
    );

    /*
    int msgboxID = MessageBox();
    switch (msgboxID)
    {
    case IDCANCEL:
        // TODO: add code
        OutputDebugStringW(L"IDCANCEL");
        break;
    case IDTRYAGAIN:
        // TODO: add code
        OutputDebugStringW(L"IDCANCEL");
        DisplayResourceNAMessageBox(hWnd);
        break;
    case IDCONTINUE:
        // TODO: add code
        OutputDebugStringW(L"IDCONTINUE");
        break;
    }
    */
}






void DisplayWindowCannotMoveMessageBox(HWND hWnd = NULL)
{
    int msgboxID = MessageBox(
        hWnd,
        (LPCWSTR)L"Window cannot be centered.",
        (LPCWSTR)L"Window Move Details",
        MB_ICONWARNING
    );
}



BOOL CenterWindow(HWND hwndWindow)
{
    RECT rectWindow;

    
        GetWindowRect(hwndWindow, &rectWindow);

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

        


        MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);

        OutputDebugStringW((LPCWSTR)L"window centered");

        return TRUE;
}


//cetering window
void handleCenterWindow(HWND hWnd) {
    int centerd = CenterWindow(hWnd);
    if (!centerd) DisplayWindowCannotMoveMessageBox(hWnd);
}


BOOL CenterChildWindowRelativeToParent(HWND hwndWindow)
{
    HWND hwndParent;
    RECT rectWindow, rectParent;

    // make the window relative to its parent
    if ((hwndParent = GetParent(hwndWindow)) != NULL)
    {
        GetWindowRect(hwndWindow, &rectWindow);
        GetWindowRect(hwndParent, &rectParent);

        int nWidth = rectWindow.right - rectWindow.left;
        int nHeight = rectWindow.bottom - rectWindow.top;

        int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
        int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;

        MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);

        return TRUE;
    }
    OutputDebugStringW(L"window had no parent.");

    return FALSE;
}