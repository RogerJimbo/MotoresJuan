#include "Globals.h"
#include "OwnFileSystem.h"

#include "PhysFS/include/physfs.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

using namespace std;

OwnFileSystem::OwnFileSystem() 
{

}

OwnFileSystem::~OwnFileSystem() 
{
	PHYSFS_deinit();
}

void OwnFileSystem::CreateMyDirectory(const char* path)
{
	string fullpath(path);

	CreateDirectory(fullpath.c_str(), NULL);

		//LPCSTR full = "G\\Users\\Roger\\Documents\\GitHub\\MotoresJuan\\Motores Juan\\Game\\Library\\test.txt";

		LPCSTR filepath = "Library\\test.juan";

		//LPCSTR will have to create three paths: Meshes, Material, ¿Animations?

		//Check if file exists and if it doesn't create new else open existent CREATE_ALWAYS  CREATE_NEW  OPEN_EXISTING


		HANDLE file = CreateFile(filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
																CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		

		


	//ERROR_ALREADY_EXISTS == GetLastError()
}

void OwnFileSystem::CopyContent(const char* source_file, const char* destination_file)
{
	PHYSFS_File* source = PHYSFS_openRead(source_file);
	PHYSFS_File* destination = PHYSFS_openWrite(destination_file);

	char* buffer = new char[PHYSFS_fileLength(source)];

	int read_lenght = PHYSFS_read(source, buffer, 1, PHYSFS_fileLength(source));

	if (PHYSFS_exists(source_file) && PHYSFS_exists(destination_file))
	{
		PHYSFS_read(source, buffer, 1, PHYSFS_fileLength(source));

		for (int i = 0; i < read_lenght; ++i)
		{
			PHYSFS_write(destination, buffer, 1, PHYSFS_fileLength(destination));
		}
	}

	LOG("Copied content from %s to %s", source, destination);
	   
	PHYSFS_close(source);
	PHYSFS_close(destination);


}