#pragma once
#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"

#include "Parson\parson.h"

#include <list>
#include <vector>

using namespace std;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGui* modulegui;

private:
	Timer	ms_timer;
	float	dt;

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(char* url);

	void Save_Config() { saveconfig = true; }

	string config_name;

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void SaveConfigToFile();
	
	bool saveconfig = false;

};