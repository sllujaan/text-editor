#include "File.h"


//BOOL MY_FILES::operator == (FILE_TREE_STRUCT& str, std::nullptr_t nullStruct) {
//	if (str.name == nullptr) return TRUE;
//	if (str.type == nullptr) return TRUE;
//	if (str.level < 1) return TRUE;
//	if (str.path == nullptr) return TRUE;
//
//	return FALSE;
//
//}
//
//BOOL MY_FILES::operator != (FILE_TREE_STRUCT& str, std::nullptr_t nullStruct)
//{
//	if (str.name == nullptr) return FALSE;
//	if (str.type == nullptr) return FALSE;
//	if (str.level < 1) return FALSE;
//	if (str.path == nullptr) return FALSE;
//
//	return TRUE;
//}

errno_t MY_FILES::FILE_TREE::initTreeCach()
{
	//1. iterate through all items of the tree.
	//2. find tree item level.
	//3. insert into treeCached

	this->tree = this->_fileTree->tree;
	this->treeCached = this->_fileTree->treeCached;


	//1. iterate through all items of the tree
	for (int i = 0; i < this->tree.size(); i++) {
		//2. find tree item level.
		//this->tree[i].level;

		//3. insert into treeCached
		this->addTreeCach(this->tree[i].level, this->tree[i]);
	}
	return TASK_SUCCESS;
}

FTSPTR MY_FILES::FILE_TREE::getTreeIitemsByLevel(size_t level)
{
	if (this->treeCached.size() == 0) {
		std::cout << "cached tree is empty in: " << __FUNCTION__;
		std::cout<< " at line: "<<__LINE__<< std::endl;
		return nullptr;
	}

	for (int i = 0; i < this->treeCached.size(); i++) {
		if (this->treeCached[i].CACHED_LEVEL == level) {
			return &this->treeCached[i].tree;
		}
	}


	std::cout << "level (" << level << ") does not exist in cached tree in (";
	std::cout << __FUNCTION__ << ") ";
	std::cout << "at line: " << __LINE__ << std::endl;
	return nullptr;
}




MY_FILES::FILE_TREE_STRUCT* MY_FILES::FILE_TREE::operator[](size_t index)
{
	if(index >= this->tree.size()) return nullptr;
	return &this->tree[index];
}

errno_t MY_FILES::FILE_TREE::setTreeViewHadlesWin32(TREEVIEW_WIN32 treeViewWin32)
{
	this->_treeViewWin32._hwndTV = treeViewWin32._hwndTV;
	this->_treeViewWin32.imgIndex_folderClosed = treeViewWin32.imgIndex_folderClosed;
	this->_treeViewWin32.imgIndex_folderOpen = treeViewWin32.imgIndex_folderOpen;

	return TASK_SUCCESS;
}


errno_t MY_FILES::FILE_TREE::readDirToTree(const wchar_t* path, unsigned int level)
{

	if (this->_treeViewWin32 == nullptr) {
		LOG(L"cannot read directory because _treeViewWin32 was not set");
		return TASK_FAILURE;
	}

	//instanciate the class object itself
	if (this->_fileTree == nullptr) {
		this->_fileTree = new FILE_TREE();
	}

	/*std::string newPath = path;
	newPath += "\\*";*/

	std::wstring oldPath(path);
	std::wstring newPath = oldPath + L"\\*";


	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	WCHAR szDir[MAX_PATH];
	
	wcscpy_s<MAX_PATH>(szDir, newPath.c_str());
	//strcpy_s<MAX_PATH>(szDir, newPath.c_str());
	HANDLE hFind = INVALID_HANDLE_VALUE;



	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cout << "FindFirstFile failed" << std::endl;
		return 1;
	}


	// List all the files in the directory with some info about them.


	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);

			if (NOT_DIR(ffd.cFileName)) {
				LOG(L"Not Directory");
			}
			else {

				MY_FILES::FILE_TREE_STRUCT treeItem = this->createTreeStruct(newPath, path, ffd, level, "dir");

				this->_fileTree->addTreeItem(treeItem);
				//-------------------------------------
				newPath = oldPath + L"\\";
				newPath.append(ffd.cFileName);
				LOG(L"-------dir--------");
				LOG(newPath.c_str());
				//readDir(newPath.c_str(), level + 1);
				this->readDirToTree(newPath.c_str(), level + 1);
			}

		}
		else
		{

			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			LOG(L"______file_______\r\n");
			LOG_INT(level);
			LOG(path);
			LOG(ffd.cFileName);
			MY_FILES::FILE_TREE_STRUCT treeItem = this->createTreeStruct(newPath, path, ffd, level, "file");

			this->_fileTree->addTreeItem(treeItem);
			//-------------------------------------

			
			LOG(L"<<_____________>>");
		}
	} while (FindNextFile(hFind, &ffd) != 0);



	return TASK_SUCCESS;
}




errno_t MY_FILES::FILE_TREE::addTreeCach(size_t level, FILE_TREE_STRUCT& _tree)
{
	//1. find the level in cached tree.
	
	//2. find if the name of item already exists at that level in cached tree. if not
	//   insert that item's address in cache tree at that level.

	//3. if the level is not found in cached tree
	//   create that new level in cached tree and
	//   insert that item's address in cache tree at that level.
	BOOL levelFound = FALSE;

	//if treeCached is empty
	if (this->treeCached.size() == 0) {
		this->createPushTreeCach(level, _tree);
		return TASK_SUCCESS;
	}
	
	for (int i = 0; i < this->treeCached.size(); i++) {
		//1. find that level in cached tree and
		//   insert that item's address in cache tree at that level.
		if (this->treeCached[i].CACHED_LEVEL == level) {
			//2. find if the name of item already exists at that level in cached tree. if not
			//   insert that item's address in cache tree at that level.

			if (!this->isItemExists(treeCached[i].tree, _tree.name)) {
				this->treeCached[i].tree.push_back(&_tree);
			}
			else {
				std::cout << "item already exists in cache at level: " << level << std::endl;
			}

			//this->treeCached[0].tree.push_back(&_tree);
			levelFound = TRUE;
			return TASK_SUCCESS;
		}

	}


	//3. if the level is not found in cached tree
	//   create that new level in cached tree and
	//   insert that item's address in cache tree at that level.
	if (levelFound == FALSE) {
		this->createPushTreeCach(level, _tree);
		return TASK_SUCCESS;
	}


	return TASK_SUCCESS;
}

errno_t MY_FILES::FILE_TREE::createPushTreeCach(size_t level, FILE_TREE_STRUCT& _tree)
{
	//create temporary cached tree structure
	FILE_TREE_STRUCT_CACHED treeStructTemp;
	treeStructTemp.CACHED_LEVEL = level;
	treeStructTemp.tree.push_back(&_tree);

	//push that cahed tree structure in treeCached.
	this->treeCached.push_back(treeStructTemp);

	return TASK_SUCCESS;
}

BOOL MY_FILES::FILE_TREE::isItemExists(std::vector<FILE_TREE_STRUCT*>& tree, LPCWSTR name)
{

	for (int i = 0; i < tree.size(); i++) {
		if (wcscmp(tree[i]->name, name) == 0) return TRUE;
	}

	return FALSE;
}

MY_FILES::FILE_TREE::FILE_TREE()
{
	std::cout << "FILE_TREE constructor called" << std::endl;
	
}

errno_t MY_FILES::FILE_TREE::addTreeItem(FILE_TREE_STRUCT treeItem)
{
	if(treeItem == nullptr) {
		std::cout << "treeItem was invalid or NULL" << std::endl;
		return TASK_FAILURE;
	}

	this->tree.push_back(treeItem);

	return TASK_SUCCESS;
}

errno_t MY_FILES::FILE_TREE::getFileNameFromPath(LPCWSTR path, LPCWSTR* destination)
{


	const size_t nDrive = 4;
	WCHAR drive[nDrive];
	WCHAR dir[_MAX_FNAME];
	WCHAR file[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	if (path == nullptr)
		return TASK_FAILURE;

	_wsplitpath_s(path, drive, dir, file, ext);

	if (file[0] == L'\0' && drive[0] == L'\0')
		return TASK_FAILURE;


	if (file[0] != L'\0') {
		size_t size = FILE_TREE::getArrayCount(file);
		WCHAR* newFile = new WCHAR[size] + 1;
		wcscpy_s(newFile, size + 1, file);
		*destination = newFile;
		return TASK_SUCCESS;
	}

	size_t size = FILE_TREE::getArrayCount(drive);
	WCHAR* newDrive = new WCHAR[size] + 1;
	wcscpy_s(newDrive, size + 1, drive);
	*destination = newDrive;
	return TASK_SUCCESS;
}

MY_FILES::FILE_TREE_STRUCT MY_FILES::FILE_TREE::createTreeStruct(std::wstring& newPath, const wchar_t* path, WIN32_FIND_DATA& ffd, size_t level, LPCSTR fileType)
{
	//create tree items object-----------------
	MY_FILES::FILE_TREE_STRUCT treeItem = { 0 };
	treeItem.name = ffd.cFileName;
	treeItem.level = level;
	treeItem.parentName = nullptr;
	treeItem.type = fileType;
	treeItem.path = path;

	//get parent directory name from path----
	if (level > 1) {
		LPCWSTR ParentDirName = nullptr;
		errno_t err = getFileNameFromPath(path, &ParentDirName);
		if (!err)
			treeItem.parentName = ParentDirName;
	}
	//---------------------------------------

	return treeItem;
}

size_t MY_FILES::FILE_TREE::getArrayCount(WCHAR arr[])
{

	int i = 0;
	int count = 0;
	while (arr[i] != L'\0') {
		count++;
		i++;
	}

	return count;
}

void MY_FILES::FILE_TREE::print()
{
	if (this->tree.size() > 0) std::cout << "\nprinting tree.........." << std::endl;
	for (int i = 0; i < this->tree.size(); i++) {
		std::wcout << this->tree[i].name << "\n";
		std::wcout<< this->tree[i].type << "\n";
		std::cout << this->tree[i].level << "\n";
		std::wcout << this->tree[i].path << "\n";
		if (this->tree[i].level > 1) std::wcout << this->tree[i].parentName << std::endl;
		std::cout <<"------------"<< std::endl;
		LOG_ANY(std::cout << "structure address:\t" << &this->tree[i] << std::endl);
	}
}

void MY_FILES::FILE_TREE::printCashed()
{
	for (int i = 0; i < this->treeCached.size(); i++) {
		std::cout << "printing cashed level: \t";
		std::wcout << this->treeCached[i].CACHED_LEVEL << std::endl;

		for (size_t j = 0; j < this->treeCached[i].tree.size(); j++)
		{
			//std::wcout << this->treeCached[i].tree[j]->name << std::endl;
			std::cout << "structure address:\t" << this->treeCached[i].tree[j] << std::endl;
		}

		/*std::wcout << this->treeCached[i].tree[0]->level << "\t";
		std::cout << "structure address:\t" << this->treeCached[i].tree[0] << std::endl;*/
	}
}

MY_FILES::FILE_TREE_STRUCT::operator bool()
{
	return false;
}

BOOL MY_FILES::FILE_TREE_STRUCT::operator==(std::nullptr_t nullStruct)
{
	if (this->name == nullptr) return true;
	if (this->type == nullptr) return true;
	if (this->level < 1) return true;
	if (this->path == nullptr) return true;

	return false;
}


const wchar_t* getWC(const char* c) {
	size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(&cSize, wc, cSize, c, cSize);
	return wc;
}

BOOL MY_FILES::TREEVIEW_WIN32::operator==(std::nullptr_t nullStruct)
{
	if (this->_hwndTV == nullptr)
		return TRUE;
	if (this->imgIndex_folderClosed < 0)
		return TRUE;
	if (this->imgIndex_folderOpen < 0)
		return TRUE;
	
	return FALSE;
}
