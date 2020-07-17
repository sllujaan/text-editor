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
#define IDI_SEARCH_FILE 0x3b9
#define IDI_UNDO 0xa448
#define IDI_REDO 0x90c0




#define CUSTOM_SELCHANGE 0xabc
#define IDI_CLOSE_TEXT_SAVED 0x444
#define IDI_CLOSE_TEXT_SAVE_CANCEL 0x555




HMENU hMenu;
static HWND hwndEdit;
static HWND hwndRichEdit;
BOOL titleUpdatedOnTextModified = FALSE;
BOOL titleUntitled = FALSE;
BOOL NewEditRich = TRUE;
BOOL EditRichFocus = FALSE;
BOOL TOKEN_OPEN_SAVEAS = FALSE;
wstring OPENED_FILE_PATH = L"";
wstring OPENED_FILE_NAME = L"";















void handleReadFile_LPCWSTR(HWND hWnd, LPCWSTR path);
int handleNewWindowA(HWND hWnd, INT openFileToken = NULL, INT tokenNewText = NULL);
INT hanleSaveAsText(HWND hWnd);










INT handleOnClose(HWND hWnd) {

    int msgboxID = 0;

    msgboxID = handleNewWindowA(hWnd);

    if(msgboxID == IDI_CLOSE_TEXT_SAVED) { DestroyWindow(hWnd); return msgboxID; }

    return msgboxID;
    
    /*
    if (titleUpdatedOnTextModified) {
        msgboxID = handleNewWindowA(hWnd);
        return msgboxID;
    }
    else {
        return msgboxID;
    }
    */

}


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
    TOKEN_OPEN_SAVEAS = TRUE;
    SetWindowText(hWnd, L"Untitled - Text-Editor");
    SetWindowText(hwndEdit, L"");
    

    //storing file name globally---
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
    /*
    OPENED_FILE_NAME = L"";
    OPENED_FILE_PATH = L"";
    titleUntitled = FALSE;
    */
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



INT hanleSaveAsTextKeepOpen(HWND hWnd, INT saveAs = NULL) {



    /*
    if (saveAs == ID_SAVEAS_MENU) {
        INT msgBoxID = 0;

        msgBoxID = handleNewWindowA(hWnd);
        if (msgBoxID == IDCANCEL || msgBoxID == IDI_CLOSE_TEXT_SAVE_CANCEL) {
            return;
        }
    }
    */


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
    if (!OPEN) return IDI_CLOSE_TEXT_SAVE_CANCEL;





    //get edit window length--
    const int size = GetWindowTextLength(hwndEdit);
    //wchar_t* data = new wchar_t[size + 1];
    LPCWSTR text = new WCHAR[size + 1];

    GetWindowText(hwndEdit, (LPWSTR)text, size + 1);


    File file;
    int FILE_ID = file.writeFile_LPCWSTR(text, (LPWSTR)ofn.lpstrFile, TRUE);

    //generateNewTextWindow(hWnd);

    TOKEN_OPEN_SAVEAS = TRUE;
    
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


    return IDI_CLOSE_TEXT_SAVED;

}


INT hanleSaveAsText(HWND hWnd) {
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
    if (!OPEN) return IDI_CLOSE_TEXT_SAVE_CANCEL;





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

    return IDI_CLOSE_TEXT_SAVED;

    /*
    //storing file name globally---
    OPENED_FILE_NAME = (LPCWSTR)fileTitle;
    OPENED_FILE_PATH = (LPCWSTR)fileName;
    titleUntitled = FALSE;
    */


}



INT handleSaveFileA(HWND hWnd, INT tokenNewText = NULL) {
    if (OPENED_FILE_PATH.length() > 0) {
        wstring path = OPENED_FILE_PATH;
        handleSaveTextPath(hWnd, (LPCWSTR)path.c_str());
        if(!tokenNewText) handleReadFile_LPCWSTR(hWnd, (LPCWSTR)path.c_str());
        return 0;
    }
    else {
        INT ID_SAVEAS = hanleSaveAsTextKeepOpen(hWnd);
        if(ID_SAVEAS == IDI_CLOSE_TEXT_SAVED ) generateNewTextWindow(hWnd);
        return ID_SAVEAS;
    }
}





int DisplayResourceNAMessageBox(HWND hWnd, LPCSTR fileName = NULL, INT tokenNewText = NULL)
{   
    int msgboxID = 0;
    INT IDI_SAVE = 0;

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
        IDI_SAVE = handleSaveFileA(hWnd, tokenNewText);
        return IDI_SAVE;
        break;
    case IDNO:
        OutputDebugStringW((LPCWSTR)L"User chose the no button....");
        generateNewTextWindow(hWnd);
        handleDisableManues(hWnd);
        // TODO: add code
        return IDNO;
        break;
    case IDCANCEL:
        return IDCANCEL;
        break;
    }
    

    return msgboxID;
}


/*
void handlenewfiletoken(int msgboxid) {

    wstring id = to_wstring(msgboxid);

    messagebox(
        null,
        (lpcwstr)id.c_str(),
        (lpcwstr)l"msgboxid",
        mb_ok
    );

    if (msgboxid == 1) {
        token_open_saveas = true;
    }
    else {
        token_open_saveas = false;
    }
}
*/


void handleNewMenu(HWND hWnd) {
    INT ID_SAVE = handleNewWindowA(hWnd, NULL, TRUE);
    
    if (ID_SAVE == IDNO || ID_SAVE == 0 || ID_SAVE == IDI_CLOSE_TEXT_SAVED) {
        OPENED_FILE_NAME = L"";
        OPENED_FILE_PATH = L"";
        titleUntitled = TRUE;
        titleUpdatedOnTextModified = FALSE;
    }
}



int handleNewWindowA(HWND hWnd, INT openFileToken, INT tokenNewText) {
    int msgboxID = 0;

    if (titleUntitled && titleUpdatedOnTextModified) {
        msgboxID = DisplayResourceNAMessageBox(hWnd, NULL, tokenNewText);
        
        return msgboxID;
    }
    else if (!titleUntitled && titleUpdatedOnTextModified) {
        wstring wstr = L"Do you want to save changes to ";
        wstring message = wstr + OPENED_FILE_NAME + L" ?";

        msgboxID = DisplayResourceNAMessageBox(hWnd, (LPCSTR)message.c_str(), tokenNewText);
        
        return msgboxID;
    }
    else if (!openFileToken){
        generateNewTextWindow(hWnd);
        handleDisableManues(hwndMain);

        //clearing global variables.
        OPENED_FILE_NAME = L"";
        OPENED_FILE_PATH = L"";
        TOKEN_OPEN_SAVEAS = FALSE;
        return msgboxID;
    }

    return msgboxID;
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
        ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL
        ,
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


    lpfnMainWndProc = (WNDPROC)SetWindowLongPtr(hwndEdit,
        GWLP_WNDPROC, (LONG_PTR)SubClassProc);

    titleUntitled = TRUE;
    titleUpdatedOnTextModified = FALSE;


    //SendMessage(hwndEdit, EM_SETSEL, 0, -1);
    //edit rich event mask
    SendMessage(hwndEdit, EM_SETEVENTMASK, 0,
        ENM_KEYEVENTS | ENM_SCROLL | ENM_UPDATE | ENM_SELCHANGE | ENM_CHANGE | ENM_DROPFILES
    );
    //SendMessage(hwndEdit, EM_SETEVENTMASK, 0, ENM_CHANGE);

    //accepting drag files......
    DragAcceptFiles(hwndEdit, TRUE);


    //sending notify message...

    
    NMHDR nmh;
    nmh.code = CUSTOM_SELCHANGE;    // Message type defined by control.
    nmh.idFrom = GetDlgCtrlID(hwndEdit);
    nmh.hwndFrom = hwndEdit;

    SendMessage(GetParent(hwndEdit),
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
    //rich edit 4.1 Msftedit.dll doesn't support WM_GETFONT message use EM_GETCHARFORMAT instead REF::(https://docs.microsoft.com/en-us/windows/win32/controls/about-rich-edit-controls#rich-edit-version-41)
    HFONT prevFont = (HFONT)SendMessage(hwndEdit, WM_GETFONT, 0, 0);

    
    /*
    MessageBox(
        NULL,
        (LPCWSTR)lpString,
        (LPCWSTR)L"Path .......---",
        MB_OK
    );
    */

    SetWindowText(hWnd, (LPCWSTR)path);
    titleUpdatedOnTextModified = FALSE;
    

    //setting token for before setting text
    TOKEN_OPEN_SAVEAS = TRUE;
    
    SetWindowTextW(hwndEdit, (LPCWSTR)lpString);

    //setting previously saved fonts
    //SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    if(prevFont) SendMessage(hwndEdit, WM_SETFONT, (WPARAM)prevFont, TRUE);
    if(!prevFont) OutputDebugStringW((LPCWSTR)L">>>>>>>>>>>>>>>>font was NULL<<<<<<<<<<<<<<");

}






void handleOpenMenu(HWND hWnd) {

    INT msgBoxID = 0;

    msgBoxID = handleNewWindowA(hWnd, TRUE);
    if ( msgBoxID == IDCANCEL || msgBoxID == IDI_CLOSE_TEXT_SAVE_CANCEL) {
        return;
    }
    
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

    
    TOKEN_OPEN_SAVEAS = TRUE;
    
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
        (LPCWSTR)L"Developed by M.Salman Altaf\r\nVersion: v1.2.4\r\nAddress: Pakistan",
        (LPCWSTR)L"About Us",
        MB_ICONINFORMATION
    );
    
}



void handleMainMenu(HWND hWnd, HMENU hMenuMain) {
    hMenuMain = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hEditMenu = CreateMenu();
    HMENU hToolsMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();
    

    HMENU hSubFileMenu = CreateMenu();
    
    
    //menu items---
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hFileMenu, (LPCWSTR)L"File");
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hEditMenu, (LPCWSTR)L"Edit");
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hToolsMenu, (LPCWSTR)L"Tools");
    AppendMenu(hMenuMain, MF_POPUP, (UINT_PTR)hHelpMenu, (LPCWSTR)L"Help");


    
    //file menu--
    AppendMenu(hFileMenu, MF_STRING, ID_NEW_MENU, (LPCWSTR)L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_OPEN_MENU, (LPCWSTR)L"Open...");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVE_MENU, (LPCWSTR)L"Save");
    AppendMenu(hFileMenu, MF_STRING, ID_SAVEAS_MENU, (LPCWSTR)L"Save As...");
    //AppendMenu(hFileMenu, MF_STRING, ID_SELECT_ALL_MENU, (LPCWSTR)L"Select All...");
    //AppendMenu(hFileMenu, MF_STRING, ID_CHANGE_FONT_MENU, (LPCWSTR)L"Change Font...");
    
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_EXIT_MENU, (LPCWSTR)L"Exit");


    //Edit menu
    AppendMenu(hEditMenu, MF_STRING, IDI_UNDO, (LPCWSTR)L"Undo");
    AppendMenu(hEditMenu, MF_STRING, IDI_REDO, (LPCWSTR)L"Undo");




    //tools menu--
    AppendMenu(hToolsMenu, MF_STRING, IDI_SEARCH_FILE, (LPCWSTR)L"Search");
    AppendMenu(hToolsMenu, MF_STRING, ID_SETTINGS_MENU, (LPCWSTR)L"Settings");




    //help menu--
    AppendMenu(hHelpMenu, MF_POPUP, ID_ABOUT_MENU, (LPCWSTR)L"About");
    //AppendMenu(hSubFileMenu, MF_STRING, ID_SETTINGS_MENU, (LPCWSTR)L"Settings...");
    //AppendMenu(hSubFileMenu, MF_STRING, ID_SETTINGS_COMBOBOX_MENU, (LPCWSTR)L"Settings Combo box...");


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




void handleOnTextChange() {

    

    if (!TOKEN_OPEN_SAVEAS) {
        handleTitleOnTextModified(hwndMain);
        handleEnableManues(hwndMain);  
    }
    else {
        TOKEN_OPEN_SAVEAS = FALSE;
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



INT onExit(HWND hWnd) {
    int msgboxID_CLOSE = handleOnClose(hWnd);
    if (msgboxID_CLOSE == 0 || msgboxID_CLOSE == IDNO || msgboxID_CLOSE == IDI_CLOSE_TEXT_SAVED) { DestroyWindow(hWnd); return 1; }
    else return 0;
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

    case WM_COMMAND:
        OutputDebugStringW((LPCWSTR)L"--------------WM_COMMAND called\r\n");
        break;

    case WM_KEYDOWN:
        break;

    case WM_KEYUP:
    case WM_CHAR:
        switch (wParam)
        {
        
        case VK_TAB:
            
            
            
            break;
        case VK_ESCAPE:
            handleTitleOnTextSaved(hwndMain);
            handleDisableManues(hwndMain);
            break;
        case VK_RETURN:
            break;
            return 0;
        }

    case WM_SYSCHAR:
        OutputDebugStringW((LPCWSTR)L"--------------WM_SYSCHAR called\r\n");
        break;
        
    }

    //  Call the original window procedure for default processing. 
    return CallWindowProc(lpfnMainWndProc, hwnd, msg, wParam, lParam);
}




/*
random hexadecimal codes--



077f
3e0f
7a6b
22eb
193d
c7f2
37b0
c259
b3a1
4146
e7ae
66c8
e192
093e
260b
6cb9
1841
7a4c
6895
c52a
f428
316e
ebf2
584b
3812
2e97
5c0e
8a9d
6274
4c7f
6737
07c1
10f7
10d9
0cc1
4dd4
e245
5ead
ca60
f935
ca74
856d
1d53
e2c3
9d6a
9199
4645
ea6b
d060
48ad
17d2
9993
1961
f455
f876
36d7
31f9
966d
889b
8c31
4fc4
9ef9
8933
d96a
b718
1446
d051
c484
4985
21d5
f2ad
da2d
e50d
11ec
d7b7
e0c0
f8d4
3e39
34f3
3829
b665
ca7e
239b
e868
0599
0de7
6e95
352a
4667
5d81
324a
0a66
7c9b
408a
84b2
2ea5

*/