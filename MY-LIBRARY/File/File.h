#pragma once
#include <windows.h>
#include<iostream>
#include<vector>
#include"Log/Log.h"

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif




namespace  MY_FILES {

	struct FILE_TREE_STRUCT {
		LPCWSTR name;
		LPCWSTR parentName;
		size_t level;
		LPCSTR type;
		LPCWSTR path;

		MYLIBRARY_API operator bool();
		MYLIBRARY_API BOOL operator == (std::nullptr_t nullStruct);
		MYLIBRARY_API BOOL operator != (std::nullptr_t nullStruct);
	};

	
	/*BOOL operator == (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);
	BOOL operator != (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);*/

	class MYLIBRARY_API  FILE_TREE {
	private:
		std::vector<FILE_TREE_STRUCT> tree;
		//structure for caching tree items into specific levels for better retrieval or data
		struct FILE_TREE_STRUCT_CACHED {
			size_t CACHED_LEVEL;
			std::vector<FILE_TREE_STRUCT*> tree;
		};
		//instance of cached tree structure
		std::vector<FILE_TREE_STRUCT_CACHED> treeCached;
		
		errno_t addTreeCach(size_t level, FILE_TREE_STRUCT& _tree);
		errno_t createPushTreeCach(size_t level, FILE_TREE_STRUCT& _tree);
		BOOL isItemExists(std::vector<FILE_TREE_STRUCT*>& tree, LPCWSTR name);

	public:
		FILE_TREE();
		errno_t addTreeItem(FILE_TREE_STRUCT treeItem);
		//init cache
		errno_t initTreeCach();
		std::vector<FILE_TREE_STRUCT*>* getTreeIitemsByLevel(size_t level);

		FILE_TREE_STRUCT* operator[] (size_t);
		
		void print();
		void printCashed();

	};
}

