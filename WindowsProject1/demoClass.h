#pragma once
#include<Windows.h>
#include<CommCtrl.h>

#include"resource.h"
#include"WindowControls.h"


#define CX_BITMAP 16
#define CY_BITMAP 16
#define NUM_BITMAPS 3





namespace Learnings {
	class Demo : public WindowControls {
	//variables------------------------------------------------------------------
		//public varaibles stuff--------
	public:
		//private varaibles stuff--------
	private:
		HINSTANCE _hInstSelf;
		HWND _hwndParent;
		HWND _hwndSelf;
		HWND _hwndTV;
		int _nCmdShow;
		const wchar_t* CLASS_NAME = L"Demo Window";


		int g_nOpen;
		int g_nClosed;
		int g_nDocument;

	//Methods-----------------------------------------------------------------------------

		//private methods stuff--------
	public:
		Demo(HWND hWnd, HINSTANCE hInst, int nCmdShow);
		Demo() : WindowControls(NULL, NULL) {}
		~Demo();
		void registerWindow();
		void initWindow();
		//private methods stuff--------
	private:
		static LRESULT CALLBACK demo_wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		void createWindow();
		void centerWindow(HWND hwnd);
		void createTreeView();
		void handleTreeViewInsertItems();
		HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel);
		BOOL InitTreeViewImageLists(HWND hwndTV);
	};
}
