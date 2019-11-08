#include "Globals.h"
#include "ModuleOwnFileSystem.h"
#include "Shlwapi.h"

#include "PhysFS/include/physfs.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#include <iostream>
#include <fstream>
#include <ctime>


#pragma comment( lib, "PhysFS/libx86/physfs.lib" )
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

ModuleOwnFileSystem::ModuleOwnFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleOwnFileSystem::~ModuleOwnFileSystem()
{
	PHYSFS_deinit();
}

bool ModuleOwnFileSystem::Start()
{
	//ImportFiles(ASSETS_FOLDER);

	CreateMyDirectory("Library");

	CreateEmptyFile("Library\\from.txt");
	CreateEmptyFile("Library\\to.txt");

	CopyContent("Library\\from.txt", "Library\\to.txt");

	//DeleteFile("Library\\to.txt");
	
	return true;
}

void ModuleOwnFileSystem::CreateMyDirectory(string path)
{
	if (PathFileExists(path.c_str()) == false)
	{
		if (CreateDirectory(path.c_str(), NULL) == 0) { LOG("Error Creating Directory: %d", GetLastError()); }
		else LOG("Directory Created Succesfully");
	}
	else LOG("Library Directory Already Created");

}

void ModuleOwnFileSystem::CreateEmptyFile(string path)
{
	if (PathFileExists(path.c_str()) == false)
	{
		HANDLE file = CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE) { LOG("Error Creating File: %d ", GetLastError()); }
		LOG("File '%s' Succesfully Created", path.c_str());
		CloseHandle(file);
	}
	else LOG("File Already Exists");
}

void ModuleOwnFileSystem::ImportFiles(const char* path)
{


}

void ModuleOwnFileSystem::DestroyFile(const char* name)
{
	if (DeleteFile(name) != 0) { LOG("Error Deleting File") }
	else 	LOG("File Successfully Deleted");
}

void ModuleOwnFileSystem::CopyContent(const char* source_file, const char* destination_file)
{
	FILE *source, *destination;

	char text;

	source = fopen(source_file, "r");
	destination = fopen(destination_file, "w");

	text = fgetc(source);

	while (text != EOF)
	{
		fputc(text, destination);
		text = fgetc(source);
	}

	fclose(source);
	fclose(destination);
}

void ModuleOwnFileSystem::GetFolderFiles()
{

}