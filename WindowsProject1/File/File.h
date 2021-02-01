#pragma once
#include <windows.h>
#include<iostream>
#include<CommCtrl.h>
#include<vector>

#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include"../AppLog.h"
#include"../Log.h"


#ifndef NOT_DIR
#define NOT_DIR(X) (wcscmp(X, L".") == 0) || (wcscmp(X, L"..") == 0)
#endif


#define FTSPTR std::vector<MY_FILES::FILE_TREE_STRUCT*>*

const wchar_t* getWC(const char* c);





namespace  MY_FILES {

	typedef struct FILE_TREE_STORE {
		HTREEITEM hTreeItem;
		LPWSTR name;
		LPWSTR path;

		~FILE_TREE_STORE();

	} *LP_FILE_TREE_STORE;


	struct FILE_TREE_STRUCT {
		LPCWSTR name;
		LPCWSTR parentName;
		size_t level;
		LPCSTR type;
		LPCWSTR path;

		 operator bool();
		 BOOL operator == (std::nullptr_t nullStruct);
		 BOOL operator != (std::nullptr_t nullStruct);
	};

	struct TREEVIEW_WIN32 {
		HWND _hwndTV;
		INT imgIndex_folderOpen = 0;
		INT imgIndex_folderClosed = 0;

		BOOL operator == (std::nullptr_t nullStruct);
	};

	
	/*BOOL operator == (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);
	BOOL operator != (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);*/

	class  FILE_TREE {
	private:
		std::vector<FILE_TREE_STRUCT> tree;
		//structure for caching tree items into specific levels for better retrieval or data
		struct FILE_TREE_STRUCT_CACHED {
			size_t CACHED_LEVEL;
			std::vector<FILE_TREE_STRUCT*> tree;
		};
		//instance of cached tree structure
		std::vector<FILE_TREE_STRUCT_CACHED> treeCached;

		//file tree
		FILE_TREE* _fileTree;

		//tree view handles win32
		TREEVIEW_WIN32 _treeViewWin32;

		//tree items record
		std::vector<LP_FILE_TREE_STORE> treeItemsRecord;
		
		
		errno_t addTreeCach(size_t level, FILE_TREE_STRUCT& _tree);
		errno_t createPushTreeCach(size_t level, FILE_TREE_STRUCT& _tree);
		BOOL isItemExists(std::vector<FILE_TREE_STRUCT*>& tree, LPCWSTR name);
		HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel, HTREEITEM _hPrev, INT imageIndex);
		
		LP_FILE_TREE_STORE getRecordStruct(HTREEITEM treeitemStruct, LPCWSTR _name, LPCWSTR _path);

	public:
		FILE_TREE();
		errno_t addTreeItem(FILE_TREE_STRUCT treeItem);
		//init cache
		errno_t initTreeCach();
		FTSPTR getTreeIitemsByLevel(size_t level);
		errno_t readDirToTree(const wchar_t* path, unsigned int level = 1, HTREEITEM _hTreeItem = nullptr);

		MY_FILES::FILE_TREE_STRUCT createTreeStruct(
			std::wstring& newPath,
			const wchar_t* path,
			WIN32_FIND_DATA& ffd,
			size_t level,
			LPCSTR fileType
		);

		FILE_TREE_STRUCT* operator[] (size_t);

		errno_t setTreeViewHadlesWin32(TREEVIEW_WIN32 treeViewWin32);

		static errno_t getFileNameFromPath(LPCWSTR path, LPCWSTR* destination);
		static size_t getArrayCount(WCHAR arr[]);
		
		void print();
		void printCashed();

		errno_t getTreeItemsRecord(HTREEITEM _hTreeItem);

	};
}

