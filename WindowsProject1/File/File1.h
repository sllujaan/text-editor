#pragma once
#include <windows.h>
#include<iostream>
#include<vector>


#include"../Log.h"





namespace MY_FILES {

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

	
	/*BOOL operator == (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);
	BOOL operator != (FILE_TREE_STRUCT& str1, std::nullptr_t nullStruct);*/

	class FILE_TREE {
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

