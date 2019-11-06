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
{}

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