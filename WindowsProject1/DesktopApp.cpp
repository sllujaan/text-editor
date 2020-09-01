// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include<iostream>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include<Richedit.h>
#include<TextServ.h>
#include<shellapi.h>


#include"AppLog.h"
#include"windowManager.h"
#include"AppConfigure.h"
#include"search.h"
#include"AppSettings.h"

using namespace std;


//  Global variables
HWND    hwndMain;
WNDPROC lpfnMainWndProc; //  Original wndproc for the combo box

//global sub windows instances---
appConfig _appConfig;
AppSettings* settings;
Search* _search_app;
config::FILE _config_file;
int fontSizeIndex = 0;
int fontFamilyIndex = 0;
int fontStyleIndex = 0;

//void handleAppConfiguration();


//#ifndef LOG 
//#define LOG(x) OutputDebugStringW((LPCWSTR)x) \
//			;OutputDebugStringW((LPCWSTR)L"\r\n") //for new line
//#endif // !LOG(x)
//


#define MAINWIN_WIDTH 500
#define MAINWIN_HEIGHT 400
HINSTANCE hInst;
int nCmdShowGlobal;
HWND hwndEdit;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //declaration of the window procedures method.
// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Untitled - Text-Editor");

//font style
HFONT hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
    OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));


#include"windowStuff.h"




#define processCallCount 0



// Global variables
HMENU hMenuMain;

//font
/*
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
*/











// Forward declarations of functions included in this code module:


int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
        NULL,             // hInstance must be NULL when loading from a file
        L"editor-png-open-2000.ico",   // the icon file name
        IMAGE_ICON,       // specifies that the file is an icon
        0,                // width of the image (we'll specify default later on)
        0,                // height of the image
        LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
        LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
        LR_SHARED         // let the system release the handle when it's no longer used
    );
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    //store ncmdshow in out gloabal vairalble
    nCmdShowGlobal = nCmdShow;

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        (int)_appConfig.getWndWidth(), (int)_appConfig.getWndHeight(),
        NULL,
        hMenuMain,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    //stroring the handle to global variable--
    hwndMain = hWnd;

    LOG_WCHAR(L"__settings object initiated.");
    //registering component windows--
    settings = new AppSettings(hWnd, hInst, nCmdShowGlobal);
    settings->registerWindow();

    //first read config variables from config file.
    handleAppConfiguration();
    //now applay the variables to settings.
    settings->setSettings(fontSizeIndex, fontFamilyIndex, fontStyleIndex);
    settings->handleSaveToConfigFile((int)_appConfig.getWndWidth(), (int)_appConfig.getWndHeight());

    setEditRichFonts();

    _search_app = new Search(hWnd, hInst, nCmdShowGlobal, hwndEdit);
    _search_app->registerWindow();

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);




    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    
    //handleOtherWindows(_In_ HINSTANCE hInstance, )

    return (int)msg.wParam;
    
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");
    //wchar_t msg[50];
    int ID_CLOSE = NULL;
    //int msgboxID_CLOSE = 0;
    ENDROPFILES* penDropFiles;




    _configVars* vars;
    /*LPCWSTR _f_family;
    LPCWSTR _f_style;
    HFONT _font;*/


    switch (message)
    {

    

    case WM_NOTIFY:
        OutputDebugStringW((LPCWSTR)L"WM_NOTIFY called__________\r\n");


        if ( (((CHANGENOTIFY*)lParam)->dwChangeType) == CN_TEXTCHANGED) {
            OutputDebugStringW((LPCWSTR)L"CHANGENOTIFY called__________\r\n");
        }

        

        switch (  ((LPNMHDR)lParam)->code )
        {
        
        case CUSTOM_SELCHANGE:
            OutputDebugStringW((LPCWSTR)L"CUSTOM_SELCHANGE called__________\r\n");
            break;

        case EN_DROPFILES:

            penDropFiles = (ENDROPFILES*)lParam;
            onDropFileSingle(hWnd, (HDROP)penDropFiles->hDrop);
            //onDropFiles((HDROP)penDropFiles->hDrop);

            ////int uNumFiles = DragQueryFile(hdrop, -1, NULL, 0);


            //LPCWSTR file[200];
            ////memset(file, 0, 200);

            //DragQueryFileA((HDROP)penDropFiles->hDrop, 0, (LPSTR)file,  200);

            //OutputDebugStringW((LPCWSTR)file );

            //// Free up memory.
            //DragFinish((HDROP)penDropFiles->hDrop);

            //OutputDebugStringW((LPCWSTR)L"EN_DROPFILES called{__________\r\n");
            break;

        case EN_VSCROLL:
            OutputDebugStringW((LPCWSTR)L"EN_VSCROLL called{__________\r\n");
            break;

        case EN_SELCHANGE:
            OutputDebugStringW((LPCWSTR)L"EN_SELCHANGE called{__________\r\n");
            break;

        case EN_CHANGE:
            OutputDebugStringW((LPCWSTR)L"WM_NOTIFY_EN_CHANGE called__________\r\n");
            break;

        default:
            break;
        }

        break;


    case WM_CREATE:
        LOG_WCHAR(L"window created......");
        handleCenterWindow(hWnd);
        //handleButton(hWnd);
        //handleNewWidow(hWnd);
        //handleManu(hWnd);
        handleMainMenu(hWnd, hMenuMain);
        //handleEdit(hWnd, lParam);
        handleRichEditControl(hWnd);
        break;

    case WM_APPLY_CONFIGURATION:
        //HFONT font = (HFONT)wParam;
        vars = (_configVars*)lParam;
        LOG_INT(vars->fontSizeIndex + 8);
        LOG_WCHAR(vars->fontFamilies[vars->fontFamilyIndex]);
        LOG_WCHAR(vars->fontStyles[vars->fontStyleIndex]);

        
        /*_f_family = vars->fontFamilies[vars->fontFamilyIndex];
        _f_style = vars->fontStyles[vars->fontStyleIndex];
        _font = vars->getFont(vars->fontSizeIndex + 8, _f_family, _f_style);*/


        LOG_WCHAR(L"=>APPLY CONFIGURATION CALLED<=");

        SendMessage(hwndEdit, WM_SETFONT, wParam, TRUE);
        
        break;

    case WM_COMMAND:

        //these messages are working fine with simple edit control------------
        if (HIWORD(wParam) == EN_CHANGE) {
                
            if ((HWND)lParam == hwndEdit) {
                OutputDebugStringW((LPCWSTR)L"lParam == hwndEdit called__________\r\n");
            }

            OutputDebugStringW((LPCWSTR)L"ENNEN_CHANGE called__________\r\n");
            handleOnTextChange();
        }

        if (HIWORD(wParam) == EN_VSCROLL) {
            OutputDebugStringW((LPCWSTR)L"ENNEN_VSCROLL called__________\r\n");
        }

        //-----------------------------------------------------------


        switch ( wParam )
        {


        case EN_CHANGE:
            OutputDebugStringW((LPCWSTR)L"EN_CHANGE called__________\r\n");
            break;

        case EN_DROPFILES:
            OutputDebugStringW((LPCWSTR)L"WM_DROPFILES called__________\r\n");
            break;

        case EN_UPDATE:
            OutputDebugStringW((LPCWSTR)L"EN_UPDATE called__________\r\n");
            break;

        case EN_VSCROLL:
            OutputDebugStringW((LPCWSTR)L"EN_VSCROLL called__________\r\n");
            break;

        case EN_ALIGNLTR:
            OutputDebugStringW((LPCWSTR)L"EN_ALIGNLTR called__________\r\n");
            break;

        case ID_EXIT_MENU:
            return onExit(hWnd);
            break;

        case IDI_VIEW_HELP:
            handleViewHelp(hWnd);
            break;

        case ID_ABOUT_MENU:
            handleAbout(hWnd);
            break;
        case ID_OPEN_MENU:
            handleOpenMenu(hWnd);
            break;
        case ID_NEW_MENU:
            handleNewMenu(hWnd);
            //handleNewWindowA(hWnd, NULL ,TRUE);
            break;
        case ID_SAVE_MENU:
            //hanleSaveText(hWnd);
            //handleSaveTextPathKeepOpen(hWnd, (LPCWSTR)OPENED_FILE_PATH.c_str());
            handleSaveFile(hWnd);
            break;
        case ID_SAVEAS_MENU:
            hanleSaveAsTextKeepOpen(hWnd, ID_SAVEAS_MENU);
            break;

        case IDI_SEARCH_FILE:
            handleSearchFile(hWnd);
            break;

        case ID_SETTINGS_MENU:
            //handleSettingsWindow(hWnd);
            handleSettingsDialog(hWnd);
            break;
        case ID_SETTINGS_COMBOBOX_MENU:
            //handleSettingsComboBoxWindow(hWnd);
            break;
        case ID_SELECT_ALL_MENU:
            SendMessage(hwndEdit, EM_SETSEL, 0, -1);
            break;
        case ID_CHANGE_FONT_MENU:
            SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE); 
            break;
        case ID_DIALOG_BOX:
            handleDialolgBox(hWnd);
            //ac.printRect(hWnd);
            break;
            
        default:
            OutputDebugStringW((LPCWSTR)L"wParam default called__________\r\n");
            break;
        }

        //OutputDebugStringW((LPCWSTR)L"command called.");

        break;

    case WM_SETFOCUS:
        SetFocus(hwndEdit);
        break;




    case WM_SIZE:
        MoveWindow(hwndEdit,
            0, 0,                  // starting x- and y-coordinates 
            LOWORD(lParam),        // width of client area 
            HIWORD(lParam),        // height of client area 
            TRUE);                 // repaint window 
        break;
    case WM_GETMINMAXINFO:
        MINMAXINFO MMI;
        MMI.ptMaxSize.x = 600;
        MMI.ptMaxSize.y = 600;
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_CLOSE:
        return onExit(hWnd);
        break;

    case WM_DESTROY:
        handleDistroyManu();
        PostQuitMessage(0);
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }


    return 0;
}
