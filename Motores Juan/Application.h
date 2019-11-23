#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "Parson\parson.h"
#include <list>
#include <vector>

class ModuleWindow;
class ModuleInput;
class ModuleScene;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleGui;
class ModuleLoader;
class ModuleTimer;

class GUI_Element;
class GUI_Config;
class GUI_Console;
class GUI_Hierarchy;
class GUI_Scene;
class GUI_Inspector;

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
	ModuleLoader* loader;
	ModuleTimer* moduletimer;

	GUI_Config* configuration = nullptr;
	GUI_Hierarchy* hierarchy = nullptr;
	GUI_Scene* scene = nullptr;
	GUI_Console* console = nullptr;
	GUI_Inspector* inspector = nullptr;

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
	void PostUpdate();
	bool CleanUp();

	void RequestBrowser(char* url);

	void Save_Config();
	void Load_Config();

	string config_name;
	float milliseconds;

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

extern Application* App;