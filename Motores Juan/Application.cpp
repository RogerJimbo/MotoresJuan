#include "Application.h"
#include <Windows.h>

Application::Application()
{
	// Main Modules
	AddModule(window = new ModuleWindow(this));
	AddModule(camera = new ModuleCamera3D(this));
	AddModule(input = new ModuleInput(this));
	AddModule(modulegui = new ModuleGui(this));
	AddModule(modscene = new ModuleScene(this));
	AddModule(loader = new ModuleLoader(this));
	AddModule(renderer3D = new ModuleRenderer3D(this)); // Renderer last!

	//Main Gui Elements
	AddGuiElement(configuration = new GUI_Config(this));
	AddGuiElement(console = new GUI_Console(this));
	AddGuiElement(hierarchy = new GUI_Hierarchy(this));
	AddGuiElement(scene = new GUI_Scene(this));
}

Application::~Application()
{ list_modules.clear(); }

bool Application::Init()
{
	bool ret = true;

	JSON_Object* config = json_value_get_object(json_parse_file(config_name.c_str()));
	config_name = "JSON Files/config.json";

	// Init
	LOG("Application Init");
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++)	//Modules
		ret = (*iter)->Init(*json_object_get_object(config, (*iter)->module_name.c_str()));

	// Start
	LOG("Application Start"); 	
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++)	//Modules
		ret = (*iter)->Start();

	ms_timer.Start();
	return ret;
}

void Application::PrepareUpdate()
{
	dt = (float)ms_timer.ReadTime() / 1000.0f;
	ms_timer.Start();
}

void Application::FinishUpdate()
{
	if (saveconfig) { SaveConfigToFile();  !saveconfig; }
	if (loadconfig) { LoadConfigFromFile(); !loadconfig; }
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	// Pre Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->PreUpdate(dt);
	// Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->Update(dt);
	// Post Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->PostUpdate(dt);
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	LOG("Application CleanUp");
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++) { ret = (*iter)->CleanUp(); }
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret; iter++) { ret = (*iter)->CleanUp(); RELEASE(*iter); }

	return ret;
}

void Application::AddModule(Module* mod) { list_modules.push_back(mod); }

void Application::AddGuiElement(GUI_Element* mod) { list_guielems.push_back(mod); }

void Application::RequestBrowser(char* url) { ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL); }

void Application::Save_Config() { saveconfig = true; }

void Application::Load_Config() { loadconfig = true; }

void Application::SaveConfigToFile()
{
	JSON_Value* config = json_value_init_object();

	json_object_set_boolean(json_object(config), "SavedInfo", true);

	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end(); iter++) 
	{
		JSON_Value* module_config = json_value_init_object();
		(*iter)->Save_Config(*json_object(module_config));
		json_object_set_value(json_object(config), (*iter)->module_name.c_str(), module_config);
	}

	if (config == NULL) { LOG("Error opening config file."); }
	else { LOG("Sucess opening config file."); }

	json_serialize_to_file(config, config_name.c_str());
	json_value_free(config);
}

void Application::LoadConfigFromFile()
{
	JSON_Object* config = json_value_get_object(json_parse_file(config_name.c_str()));

	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end(); iter++)
		(*iter)->Load_Config(*json_object_get_object(config, config_name.c_str()));
}