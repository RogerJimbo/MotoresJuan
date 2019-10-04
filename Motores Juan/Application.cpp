#include "Application.h"
#include <Windows.h>

#include "Module.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

Application::Application()
{
	//Modules
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	modulegui = new ModuleGui(this);
	modscene = new ModuleScene(this);

	// Gui Elements
	configuration = new GUI_Config(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(modulegui);
	AddModule(modscene);
	AddModule(renderer3D); // Renderer last!

	//Main Gui Elements
	AddGuiElement(configuration);

}

Application::~Application()
{ list_modules.clear(); }

bool Application::Init()
{
	bool ret = true;

	JSON_Object* config;

	config_file_name = "JSON Files/config2.json";
	config_name = "JSON Files/config.json";

	if (json_object_get_boolean(json_value_get_object(json_parse_file(config_name.c_str())), "SavedInfo"))
		config = json_value_get_object(json_parse_file(config_name.c_str()));
	else
		config = json_value_get_object(json_parse_file(config_file_name.c_str()));

	// Init
	LOG("Application Init");
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++)	//Modules
		ret = (*iter)->Init(*json_object_get_object(config, (*iter)->config_name.c_str()));
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret; iter++)	//Gui Elements
		ret = (*iter)->Init(*json_object_get_object(config, (*iter)->config_name.c_str()));

	// Start
	LOG("Application Start"); 	
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++)	//Modules
		ret = (*iter)->Start();
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret; iter++)	//Gui Elements
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
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	LOG("Application Update");
	// Pre Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->PreUpdate(dt);
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret == UPDATE_CONTINUE; iter++)	//Gui Elements
		ret = (*iter)->PreUpdate(dt);
	// Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->Update(dt);
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret == UPDATE_CONTINUE; iter++)	//Gui Elements
		ret = (*iter)->Update(dt);
	// Post Update
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret == UPDATE_CONTINUE; iter++)	//Modules
		ret = (*iter)->PostUpdate(dt);
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret == UPDATE_CONTINUE; iter++)	//Gui Elements
		ret = (*iter)->PostUpdate(dt);

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	LOG("Application CleanUp");
	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end() && ret; iter++) { ret = (*iter)->CleanUp(); }
	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end() && ret; iter++) { ret = (*iter)->CleanUp(); }

	return ret;
}

void Application::AddModule(Module* mod) { list_modules.push_back(mod); }

void Application::AddGuiElement(GUI_Element* mod) { list_guielems.push_back(mod); }

void Application::RequestBrowser(char* url) { ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL); }

void Application::Save_Config() { saveconfig = true; }

void Application::SaveConfigToFile()
{
	JSON_Value* config = json_value_init_object();

	json_object_set_boolean(json_object(config), "SavedInfo", true);

	for (list<Module*>::iterator iter = list_modules.begin(); iter != list_modules.end(); iter++) 
	{
		JSON_Value* module_config = json_value_init_object();
		(*iter)->Save_Config(*json_object(module_config));
		json_object_set_value(json_object(config), (*iter)->config_name.c_str(), module_config);
	}

	for (list<GUI_Element*>::iterator iter = list_guielems.begin(); iter != list_guielems.end(); iter++)
	{
		JSON_Value* module_config = json_value_init_object();
		(*iter)->Save_Config(*json_object(module_config));
		json_object_set_value(json_object(config), (*iter)->config_name.c_str(), module_config);
	}

	if (config == NULL) { LOG("Error opening config file."); }
	else { LOG("Sucess opening config file."); }

	json_serialize_to_file(config, config_name.c_str());
	json_value_free(config);
}