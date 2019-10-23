#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include <list>

class Application;
using namespace std;

struct Mesh
{
	// index in VRAM
	uint id_indices = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	//Normals
	uint* normals = nullptr;	
	uint num_normals = 0;
	uint id_normals = 0;

	// unique vertex in VRAM
	uint id_vertices = 0; 
	uint num_vertices = 0;
	float* vertices = nullptr;

	// Textures
	uint id_texcoords = 0;
	uint texture = 0;
	uint texture_width = 0;
	uint texture_height = 0;
	float* texture_coords = nullptr;
};

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();

	bool Init(const JSON_Object& config);
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
	bool Import(const string& pFile);

	uint Texturing(const char* file_name);

	string module_name;
};