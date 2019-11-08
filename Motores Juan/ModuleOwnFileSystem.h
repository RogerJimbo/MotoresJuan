#pragma once
#include <windows.h>
#include "Module.h"

#include <string>
#include <cstring>
#include <string>

class ModuleOwnFileSystem : public Module
{
public:
	ModuleOwnFileSystem(Application* app, bool start_enabled = true);
	~ModuleOwnFileSystem();

	bool Start();

	void CreateMyDirectory(string path);
	void CreateEmptyFile(const char* path);
	void DestroyFile(const char* name);
	void CopyContent(const char* source_file, const char* destination_file);

	void GetFolderFiles();
	
	void ImportFiles(const char* path);


};

