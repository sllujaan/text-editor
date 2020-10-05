#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include"WindowControls.h"

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
		BOOL InitTreeViewImageLists(HWND hwndTV);
	};
}
