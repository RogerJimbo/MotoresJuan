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

#include "GUI_Element.h"
#include "GUI_Config.h"
#include "GUI_Console.h"

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
	GUI_Console* console;
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
	void Load_Config();

	string config_name;

private:
	void AddModule(Module* mod);
	void AddGuiElement(GUI_Element* elem);
	void PrepareUpdate();
	void FinishUpdate();
	void SaveConfigToFile();
	void LoadConfigFromFile();
	
	bool saveconfig = false;
	bool loadconfig = false;
};