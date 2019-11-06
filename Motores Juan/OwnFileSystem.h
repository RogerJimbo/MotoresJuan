#pragma once
#include <windows.h>
#include "Module.h"

#include <string>
#include <cstring>
#include <string>

class OwnFileSystem 
{
public:
	OwnFileSystem();
	~OwnFileSystem();


	void CreateMyDirectory(const char* path);


};

