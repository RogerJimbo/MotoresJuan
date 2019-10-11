#pragma once
#include "Module.h"
#include "Globals.h"
#include "Parson\parson.h"
#include <list>

class Application;
using namespace std;

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

	string module_name;

	bool Import();

};