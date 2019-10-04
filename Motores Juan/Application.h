#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "ModuleScene.h"

#include "GUI_Config.h"


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
	ModuleScene* modscene;

	GUI_Config* configuration;
	GUI_Hierarchy* hierarchy;
	GUI_Scene* scene;

private:
	Timer	ms_timer;
	float	dt;

	list<Module*> list_modules;
	list<GUI_Element*>list_guielems;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(char* url);

	void Save_Config();

	string config_name;
	string config_file_name;

private:
	void AddModule(Module* mod);
	void AddGuiElement(GUI_Element* elem);
	void PrepareUpdate();
	void FinishUpdate();
	void SaveConfigToFile();
	
	bool saveconfig = false;

};