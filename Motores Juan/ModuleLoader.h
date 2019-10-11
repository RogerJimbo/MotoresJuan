#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include <list>

class Application;
using namespace std;

struct Mesh
{
	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
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

	string module_name;

};