#pragma once


#include"mySample.h"
#include <ShObjIdl.h>
#include<stdio.h>
#include"settingsWindow.h"
//#include <string>
#include"File.h"
#include"debug.h"
#include<Shlwapi.h>


#define PACKVERSION(major,minor) MAKELONG(minor,major)


#define BUTTON_ID_OK 0
#define ID_FILE_MENU 1
#define ID_EXIT_MENU 0x1df
#define ID_TOOLS_MENU 0x2f0
#define ID_ABOUT_MENU 0x100
#define ID_OPEN_MENU 0x011
#define ID_NEW_MENU 0x111
#define ID_SAVEAS_MENU 0x992
#define ID_SAVE_MENU 0x998
#define ID_SETTINGS_MENU 0x000
#define ID_SETTINGS_COMBOBOX_MENU 0x123
#define ID_SELECT_ALL_MENU 0x354
#define ID_CHANGE_FONT_MENU 0xfde

#define CUSTOM_SELCHANGE 0xabc





HMENU hMenu;
static HWND hwndEdit;
static HWND hwndRichEdit;
BOOL titleUpdatedOnTextModified = FALSE;
BOOL titleUntitled = FALSE;
BOOL NewEditRich = TRUE;
BOOL EditRichFocus = FALSE;
wstring OPENED_FILE_PATH = L"";
wstring OPENED_FILE_NAME = L"";










void handleReadFile_LPCWSTR(HWND hWnd, LPCWSTR path);


void handleEnableManues(HWND hWnd) {
    EnableMenuItem(GetMenu(hWnd), ID_SAVE_MENU, MF_ENABLED | MF_BYCOMMAND);
    //EnableMenuItem(GetMenu(hWnd), ID_SAVEAS_MENU, MF_ENABLED | MF_BYCOMMAND);
}
void handleDisableManues(HWND hWnd) {
    EnableMenuItem(GetMenu(hWnd), ID_SAVE_MENU, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
    //EnableMenuItem(GetMenu(hWnd), ID_SAVEAS_MENU, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
}

void handleManuDisableEnable(HWND hWnd, BOOL Disable = FALSE) {
    if (Disable) handleEnableManues(hWnd);
    else handleDisableManues(hWnd);
}


void handleTitleOnTextModified(HWND hWnd) {

    if (titleUpdatedOnTextModified == FALSE) {

        //get main window title--
        const int size = GetWindowTextLength(hWnd);
        //wchar_t* data = new wchar_t[size + 1];
        LPCWSTR data = new WCHAR[size + 1];

        GetWindowText(hWnd, (LPWSTR)data, size + 1);

        wstring wstr = data;
        wstring subStr = L"*" + wstr;
        LPCWSTR subTitle = subStr.c_str();

        SetWindowText(hWnd, (LPCWSTR)subTitle);

        titleUpdatedOnTextModified = TRUE;

    }

}



void handleTitleOnTextSaved(HWND hWnd) {
    if (titleUpdatedOnTextModified == TRUE) {

        //get main window title--
        const int size = GetWindowTextLength(hWnd);
        //wchar_t* data = new wchar_t[size + 1];
        LPCWSTR data = new WCHAR[size + 1];

        GetWindowText(hWnd, (LPWSTR)data, size + 1);

        wstring wstr = data;
        wstring subStr = wstr.substr(1, size);
        LPCWSTR subTitle = subStr.c_str();

        SetWindowText(hWnd, (LPCWSTR)subTitle);

        titleUpdatedOnTextModified = FALSE;

    }
}





void generateNewTextWindow(HWND hWnd) {
    SetWindowText(hWnd, L"Untitled - Text-Editor");
    SetWindowText(hwndEdit, L"");
    titleUpdatedOnTextModified = FALSE;
    titleUntitled = TRUE;
}


void handleSaveTextPath(HWND hWnd, LPCWSTR path) {

    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);
    
    File file;
    int FILE_ID = file.writeFile_LPCWSTR(text, (LPWSTR)path, TRUE);

    generateNewTextWindow(hWnd);
    handleDisableManues(hwndMain);

    //storing file name globally---
    OPENED_FILE_NAME = L"";
    OPENED_FILE_PATH = L"";
    titleUntitled = FALSE;
}


void handleSaveTextPathKeepOpen(HWND hWnd, LPCWSTR path) {

    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);

    File file;
    int FILE_ID = file.writeFile_LPCWSTR(text, (LPWSTR)path, TRUE);

    //generateNewTextWindow(hWnd);
    handleTitleOnTextSaved(hwndMain);
    handleDisableManues(hwndMain);

    //storing file name globally---
    //OPENED_FILE_NAME = L"";
    //OPENED_FILE_PATH = L"";
    titleUntitled = FALSE;
}





void hanleSaveText(HWND hWnd) {
    
    OPENFILENAME ofn;

    LPWSTR fileName[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = (LPWSTR)L"Text Documents (*.txt)\0*.txt*\0";
    ofn.nFilterIndex = 1;

    BOOL OPEN = GetSaveFileName(&ofn);
    if (!OPEN) return;

    MessageBox(
        hWnd,
        ofn.lpstrFile,
        (LPCWSTR)L"save",
        MB_OK
    );


    //saveText();
}



void hanleSaveAsTextKeepOpen(HWND hWnd) {
    OPENFILENAME ofn;

    LPWSTR fileName[100];
    LPWSTR fileTitle[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = (LPWSTR)L"Text Documents (*.txt)\0*.txt*\0";
    ofn.nFilterIndex = 1;

    ofn.lpstrDefExt = L"txt";

    //saving file name-----
    ofn.lpstrFileTitle = (LPWSTR)fileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = 100;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    BOOL OPEN = GetSaveFileName(&ofn);
    if (!OPEN) return;





    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);


    File file;
    int FILE_ID = file.writeFile_LPCWSTR(text, (LPWSTR)ofn.lpstrFile, TRUE);

    //generateNewTextWindow(hWnd);

    handleReadFile_LPCWSTR(hWnd, (LPCWSTR)fileName);
    handleDisableManues(hwndMain);

    //storing file name globally---
    OPENED_FILE_NAME = (LPCWSTR)fileTitle;
    OPENED_FILE_PATH = (LPCWSTR)fileName;
    titleUntitled = FALSE;

    /*
    //storing file name globally---
    OPENED_FILE_NAME = (LPCWSTR)fileTitle;
    OPENED_FILE_PATH = (LPCWSTR)fileName;
    titleUntitled = FALSE;
    */


}


void hanleSaveAsText(HWND hWnd) {
    OPENFILENAME ofn;

    LPWSTR fileName[100];
    LPWSTR fileTitle[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = (LPWSTR)L"Text Documents (*.txt)\0*.txt*\0";
    ofn.nFilterIndex = 1;

    ofn.lpstrDefExt = L"txt";

    //saving file name-----
    ofn.lpstrFileTitle = (LPWSTR)fileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = 100;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    BOOL OPEN = GetSaveFileName(&ofn);
    if (!OPEN) return;





    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);
    

    File file;
    int FILE_ID = file.writeFile_LPCWSTR(text, (LPWSTR)ofn.lpstrFile, TRUE);

    generateNewTextWindow(hWnd);
    handleDisableManues(hwndMain);

    //storing file name globally---
    OPENED_FILE_NAME = L"";
    OPENED_FILE_PATH = L"";
    titleUntitled = FALSE;

    /*
    //storing file name globally---
    OPENED_FILE_NAME = (LPCWSTR)fileTitle;
    OPENED_FILE_PATH = (LPCWSTR)fileName;
    titleUntitled = FALSE;
    */


}




void handleSaveFileA(HWND hWnd) {
    if (OPENED_FILE_PATH.length() > 0) {
        handleSaveTextPath(hWnd, (LPCWSTR)OPENED_FILE_PATH.c_str());
    }
    else {
        hanleSaveAsText(hWnd);
    }
}





int DisplayResourceNAMessageBox(HWND hWnd, LPCSTR fileName = NULL)
{   
    int msgboxID = 0;

    if (fileName == NULL) {
        msgboxID = MessageBox(
            hWnd,
            (LPCWSTR)L"Do you want to save changes to Untitled?",
            (LPCWSTR)L"save",
            MB_YESNOCANCEL | MB_ICONQUESTION
        );
    }
    else {
        msgboxID = MessageBox(
            hWnd,
            (LPCWSTR)fileName,
            (LPCWSTR)L"save",
            MB_YESNOCANCEL | MB_ICONQUESTION
        );
    }
    

    switch (msgboxID)
    {
    case IDYES:
        // TODO: add code
        OutputDebugStringW((LPCWSTR)L"User chose the yes button....");
        handleSaveFileA(hWnd);
        break;
    case IDNO:
        OutputDebugStringW((LPCWSTR)L"User chose the no button....");
        generateNewTextWindow(hWnd);
        handleDisableManues(hWnd);
        // TODO: add code
        break;

    }

    return msgboxID;
}





void handleNewWindowA(HWND hWnd) {
    if (titleUntitled && titleUpdatedOnTextModified) {
        DisplayResourceNAMessageBox(hWnd, NULL);
    }
    else if (!titleUntitled && titleUpdatedOnTextModified) {
        wstring wstr = L"Do you want to save changes to ";
        wstring message = wstr + OPENED_FILE_NAME + L" ?";

        DisplayResourceNAMessageBox(hWnd, (LPCSTR)message.c_str());
    }
    else {
        generateNewTextWindow(hWnd);
        handleDisableManues(hwndMain);
    }
}






void hanleNewWidow(HWND hWnd) {
    int editLength = GetWindowTextLength(hwndEdit);

    //converting integer to wide string....
    wstring wstr = to_wstring(editLength);

    //converting wstring to wchar_t*...
    const wchar_t* wchar_ptr = wstr.c_str();

     
    //get main window title--
    const int size = GetWindowTextLength(hWnd);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR data = new WCHAR[size + 1];

    GetWindowText(hWnd, (LPWSTR)data, size+1);

    if (wcscmp((LPWSTR)data, (LPWSTR)L"*Untitled - Text-Editor") == 0) {

        if (editLength > 0) {
            MessageBox(
                hWnd,
                (LPCWSTR)L"Do you want to save changes to Untitled?",
                (LPCWSTR)L"Text Editor",
                BS_PUSHBUTTON
            );
            //return;
        }
        
    }
    else if (titleUpdatedOnTextModified) {

        wstring wstr = data;
        wstring subStr = L"Do you want to save changes to: " + wstr.substr(1, size);
        LPCWSTR subTitle = subStr.c_str();

        //geting filename from the url---
        LPCSTR fileName = PathFindFileNameA((LPCSTR)subTitle);


        int msgboxID = MessageBox(
            hWnd,
            (LPCWSTR)fileName,
            (LPCWSTR)L"save",
            MB_YESNOCANCEL | MB_ICONQUESTION
        );

        switch (msgboxID)
        {
        case IDYES:
            // TODO: add code
            OutputDebugStringW((LPCWSTR)L"User choose the yes button....");
            break;
        case IDNO:
            OutputDebugStringW((LPCWSTR)L"User choose the no button....");
            generateNewTextWindow(hWnd);
            // TODO: add code
            break;

        }

        
        
    }
    else {
        SetWindowText(hwndEdit, L"");
    }
    
}


void handleEdit(HWND hWnd, LPARAM lParam) {


    hwndEdit = CreateWindowEx(
        0, L"EDIT",   // predefined class 
        NULL,         // no window title 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        0, 0, 0, 0,   // set size in WM_SIZE message 
        hWnd,         // parent window 
        NULL,   // edit control ID 
        NULL,
        NULL);        // pointer not needed


    MoveWindow(hwndEdit,
        0, 0,                  // starting x- and y-coordinates 
        LOWORD(lParam),        // width of client area 
        HIWORD(lParam),        // height of client area 
        TRUE);                 // repaint window 

// Add text to the window. 
    //SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);
}


HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
    int x, int y,          // Location.
    int width, int height, // Dimensions.
    HINSTANCE hinst)       // Application or DLL instance.
{
    LoadLibrary(TEXT("Msftedit.dll"));

    HWND hwndEdit = CreateWindowEx(0, MSFTEDIT_CLASS, TEXT(""), // TEXT("") was TEXT("Type here")
         WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        x, y, width, height,
        hwndOwner, NULL, hinst, NULL);

    return hwndEdit;
}

/*
----rich edit-----

WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL
*/



/********************************************************

    FUNCTION:   SubClassProc

    PURPOSE:    Process TAB and ESCAPE keys, and pass all
                other messages to the class window
                procedure.

*********************************************************/
LRESULT CALLBACK SubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


void handleRichEditControl(HWND hWnd) {
   // hwndEdit = CreateRichEdit(hWnd, 0, 0, 100, 100, hInst)

    hwndEdit = CreateRichEdit(hWnd, 0, 0, 200, 200, hInst);

    //formating editrich control..
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    lpfnEditWndProc = (WNDPROC)SetWindowLongPtr(hwndEdit,
        GWLP_WNDPROC, (LONG_PTR)SubClassProc);

    titleUntitled = TRUE;
    titleUpdatedOnTextModified = FALSE;

    //SendMessage(hwndEdit, EM_SETSEL, 0, -1);
    //edit rich event mask
    SendMessage(hwndEdit, EM_SETEVENTMASK, 0, ENM_CHANGE);



    //sending notify message...


    

    NMHDR nmh;
    nmh.code = CUSTOM_SELCHANGE;    // Message type defined by control.
    nmh.idFrom = GetDlgCtrlID(hwndEdit);
    nmh.hwndFrom = hwndEdit;

    SendMessage(GetParent(hwndMain),
        WM_NOTIFY,
        nmh.idFrom,
        (LPARAM)&nmh);

        
}













void handleReadFile(HWND hWnd, char* path) {
    /*
    MessageBox(
        NULL,
        (LPCWSTR)path,
        (LPCWSTR)L"Path",
        MB_OK
    );



    File file;
    string content = file.readFile((string)path);
    const char* content_ptr = content.c_str();

    OutputDebugStringW((LPWSTR)content_ptr);

    */
    

    
    

    File file;
    string content = file.readFile((string)path);

    //converting string to wide string.
    wstring wstr = wstring(content.begin(), content.end());
    
    //converting wstring to wchar_t*
    const wchar_t* wstr_ptr = wstr.c_str();

    SetWindowText(hwndEdit, wstr_ptr);


    MessageBox(
        NULL,
        (LPCWSTR)path,
        (LPCWSTR)L"Path",
        MB_OK
    );

    SetWindowText(hWnd, (LPCWSTR)path);

    displayDialog("first", "second str");

}



void handleReadFile_LPCWSTR(HWND hWnd, LPCWSTR path) {

    File file;

    string data = file.readFile_LPCWSTR(path);

    //converting string to wstring
    wstring wstr = wstring(data.begin(), data.end());
    //converting wstring to wchar_t*
    LPCWSTR lpString = wstr.c_str();


    //saving edit control fonts
    HFONT prevFont = (HFONT)SendMessage(hwndEdit, WM_GETFONT, 0, 0);


    MessageBox(
        NULL,
        (LPCWSTR)lpString,
        (LPCWSTR)L"Path .......---",
        MB_OK
    );

    SetWindowText(hWnd, (LPCWSTR)path);
    titleUpdatedOnTextModified = FALSE;
    
    
    SetWindowTextW(hwndEdit, (LPCWSTR)lpString);

    //setting previously saved fonts
    //SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    if(prevFont) SendMessage(hwndEdit, WM_SETFONT, (WPARAM)prevFont, TRUE);
    if(!prevFont) OutputDebugStringW((LPCWSTR)L">>>>>>>>>>>>>>>>font was NULL<<<<<<<<<<<<<<");
}






void handleOpenMenu(HWND hWnd) {
    
    OPENFILENAME ofn;

    char* fileName[100];
    LPWSTR fileTitle[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = (LPWSTR)fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = (LPWSTR)L"Text Documents (*.txt)\0*.txt*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"txt";
    ofn.Flags = OFN_FILEMUSTEXIST;

    //saving file name-----
    ofn.lpstrFileTitle = (LPWSTR)fileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = 100;

    
    BOOL OPEN = GetOpenFileName(&ofn);
    if (!OPEN) return;

    

    
    handleReadFile_LPCWSTR(hWnd, (LPCWSTR)fileName);
    //handleReadFile(hWnd, (char*)ofn.lpstrFile);
    

    //storing file name globally---
    OPENED_FILE_NAME = (LPCWSTR)fileTitle;
    OPENED_FILE_PATH = (LPCWSTR)fileName;
    titleUntitled = FALSE;
}




void handleAbout(HWND hWnd) {
    OutputDebugStringW((LPCWSTR)L"handleAbout called.");
    
    int msgboxID = MessageBox(
        hWnd,
        (LPCWSTR)L"Developed by M.Salman Altaf.",
        (LPCWSTR)L"About Us",
        MB_ICONINFORMATION
    );
    
}



void handleMainMenu(HWND hWnd, HMENU hMenuMain) {
    hMenuMain = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();
    
    
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hFileMenu, (LPCWSTR)L"File");
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hHelpMenu, (LPCWSTR)L"Help");
    AppendMenu(hHelpMenu, MF_POPUP, ID_ABOUT_MENU, (LPCWSTR)L"About");
    AppendMenu(hSubFileMenu, MF_STRING, ID_SETTINGS_MENU, (LPCWSTR)L"Settings...");
    //AppendMenu(hSubFileMenu, MF_STRING, ID_SETTINGS_COMBOBOX_MENU, (LPCWSTR)L"Settings Combo box...");

    AppendMenu(hFileMenu, MF_STRING, ID_NEW_MENU, (LPCWSTR)L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_OPEN_MENU, (LPCWSTR)L"Open...");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVE_MENU, (LPCWSTR)L"Save");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVEAS_MENU, (LPCWSTR)L"Save As...");
    //AppendMenu(hFileMenu, MF_STRING, ID_SELECT_ALL_MENU, (LPCWSTR)L"Select All...");
    //AppendMenu(hFileMenu, MF_STRING, ID_CHANGE_FONT_MENU, (LPCWSTR)L"Change Font...");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubFileMenu, (LPCWSTR)L"Tools");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_EXIT_MENU, (LPCWSTR)L"Exit");

    SetMenu(hWnd, hMenuMain);


    handleDisableManues(hWnd);
}



void handleManu(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, ID_EXIT_MENU, (LPCWSTR)L"Exit");

    AppendMenu(hMenu, MF_STRING, (UINT_PTR)hFileMenu, (LPCWSTR)L"File");

    SetMenu(hWnd, hMenu);
}

void showManuDistructionFailed() {
    MessageBox(
        NULL,
        (LPCWSTR)L"Manu Distruction Failed.",
        (LPCWSTR)L"distruction info.",
        MB_ICONWARNING
    );
}

void handleDistroyManu() {
    if (hMenu) {
        int manuDestroyed = DestroyMenu(hMenu);
        if (!manuDestroyed) showManuDistructionFailed();
    }
    
}










void handleNewWidow(HWND hWnd) {

}






void handleButton(HWND hWnd) {
    //button
    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"OK",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
        10,         // x position 
        10,         // y position 
        100,        // Button width
        30,        // Button height
        hWnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.


    if (!hwndButton)
    {
        MessageBox(NULL,
            _T("Call to CreateButton failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);
    }
}





/*

int getWindowWidth(HWND hWnd) {
    RECT rectWindow;
    GetWindowRect(hWnd, &rectWindow);
    int nWidth = rectWindow.right - rectWindow.left;
    return nWidth;
}

int getWindowHeight(HWND hWnd) {
    RECT rectWindow;
    GetWindowRect(hWnd, &rectWindow);
    int nHeight = rectWindow.bottom - rectWindow.top;
    return nHeight;
}






int getClientWidth(HWND hWnd) {
    RECT windowRect, clientRect;
    GetWindowRect(hWnd, &windowRect);
    GetClientRect(hWnd, &clientRect);

    int nWidth =  (clientRect.bottom - clientRect.top);
    return nWidth;

    //(windowRect.bottom - windowRect.top)
}

int getClientHeight(HWND hWnd) {
    RECT windowRect, clientRect;
    GetWindowRect(hWnd, &windowRect);
    GetClientRect(hWnd, &clientRect);

    int nHeight = (clientRect.right - clientRect.left);
    return nHeight;

    //(windowRect.right - windowRect.left)
}





MAINWIN_WIDTH - 17, MAINWIN_HEIGHT - 59

*/



/*
    FILE* file;
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    LPCWSTR data = new WCHAR[size + 1];
    fread((void*)data, size, 1, file);
    //data[size + 1] = '\0';
 
 
*/





/********************************************************

    FUNCTION:   SubClassProc

    PURPOSE:    Process TAB and ESCAPE keys, and pass all
                other messages to the class window
                procedure.

*********************************************************/
LRESULT CALLBACK SubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        //SendMessage(hwndEdit, EM_SETEVENTMASK, 0, ENM_CHANGE);
        OutputDebugStringW((LPCWSTR)L"---------------WM_KEYDOWN called>>>>>>>>>>>>\r\n");
        //SendMessage(hwndEdit, ENM_CHANGE, 0, 0);

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
            
            handleTitleOnTextModified(hwndMain);
            OutputDebugStringW((LPCWSTR)L"VK_TAB\r\n");
            handleEnableManues(hwndMain);
            
            break;
        case VK_ESCAPE:
            OutputDebugStringW((LPCWSTR)L"VK_ESCAPE");
            handleTitleOnTextSaved(hwndMain);
            handleDisableManues(hwndMain);
            break;
        case VK_RETURN:
            OutputDebugStringW((LPCWSTR)L"VK_RETURN");
            break;
            return 0;
        }
    }

    //  Call the original window procedure for default processing. 
    return CallWindowProc(lpfnEditWndProc, hwnd, msg, wParam, lParam);
}
