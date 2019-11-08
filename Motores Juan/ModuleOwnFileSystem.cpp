#include "Globals.h"
#include "ModuleOwnFileSystem.h"

#include "PhysFS/include/physfs.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#include <iostream>
#include <fstream>
#include <ctime>

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

using namespace std;

ModuleOwnFileSystem::ModuleOwnFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CreateEmptyFile("Library\\from.txt");
	CreateEmptyFile("Library\\to.txt");

	DeleteFile("Library\\hello.txt");
}

ModuleOwnFileSystem::~ModuleOwnFileSystem()
{
	PHYSFS_deinit();
}

bool ModuleOwnFileSystem::Start()
{
	ImportFiles(ASSETS_FOLDER);


	CopyContent("Library\\from.txt", "Library\\to.txt");
	
	return true;
}

void ModuleOwnFileSystem::CreateMyDirectory(string path)
{
	if (CreateDirectory(path.c_str(), NULL) == 0) { LOG("Error Creating Directory: %d", GetLastError()); }
	else LOG("Directory Created Succesfully");
}

void ModuleOwnFileSystem::CreateEmptyFile(const char* path)
{
	HANDLE file = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, NULL,
											CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE) { LOG("Error Creating File: %d ", GetLastError()); }

	CloseHandle(file);
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