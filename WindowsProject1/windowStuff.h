#pragma once


#include"mySample.h"
#include <ShObjIdl.h>
#include<stdio.h>
#include"settingsWindow.h"
//#include <string>
#include"File.h"
#include"debug.h"


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
#define ID_SETTINGS_DIALOG_MENU 0x123


HMENU hMenu;
static HWND hwndEdit;



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

void hanleSaveAsText(HWND hWnd) {
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


    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);
    

    File file;
    int res = file.writeFile_LPCWSTR(text, (LPWSTR)ofn.lpstrFile);
    
    if (!res) {
        MessageBox(
            hWnd,
            (LPCWSTR)L"Error while writing to file.",
            (LPCWSTR)L"save",
            MB_ICONERROR
        );
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

    if (wcscmp((LPWSTR)data, (LPWSTR)L"Untitled - Text-Editor") == 0) {

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


void handleRichEditControl(HWND hWnd) {
   // hwndEdit = CreateRichEdit(hWnd, 0, 0, 100, 100, hInst)
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


    

    MessageBox(
        NULL,
        (LPCWSTR)lpString,
        (LPCWSTR)L"Path .......---",
        MB_OK
    );

    SetWindowText(hWnd, (LPCWSTR)path);
    
    SetWindowTextW(hwndEdit, (LPCWSTR)lpString);
    //SetWindowText(hwndEdit, (LPCWSTR)lpString);
    
}






void handleOpenMenu(HWND hWnd) {
    
    OPENFILENAME ofn;

    char* fileName[100];

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

    
    BOOL OPEN = GetOpenFileName(&ofn);
    if (!OPEN) return;
    
    handleReadFile_LPCWSTR(hWnd, (LPCWSTR)fileName);
    //handleReadFile(hWnd, (char*)ofn.lpstrFile);


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
    AppendMenu(hSubFileMenu, MF_STRING, ID_SETTINGS_DIALOG_MENU, (LPCWSTR)L"Settings Dialog box...");

    AppendMenu(hFileMenu, MF_STRING, ID_NEW_MENU, (LPCWSTR)L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_OPEN_MENU, (LPCWSTR)L"Open...");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVE_MENU, (LPCWSTR)L"Save");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVEAS_MENU, (LPCWSTR)L"Save As...");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubFileMenu, (LPCWSTR)L"Tools");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_EXIT_MENU, (LPCWSTR)L"Exit");

    SetMenu(hWnd, hMenuMain);
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