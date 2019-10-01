#include "Application.h"
#include <Windows.h>

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	modulegui = new ModuleGui(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(modulegui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	JSON_Object* config;
	JSON_Value* config_value;

	p2List_item<Module*>* item = list_modules.getFirst();

	config_value = json_parse_file(config_name.c_str());
	config = json_value_get_object(config_value);

	 
	while(item != NULL && ret == true)
	{	
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
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
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

void Application::RequestBrowser(char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SaveConfigToFile()
{
	JSON_Value *schema = json_parse_string("{\"name\":\"\"}");
	JSON_Value *user_data = json_parse_file("config.json");
	JSON_Value* config = json_value_init_object();

	const char* name = "roger";			
	const char* string = "my string";	

	if (user_data == NULL || json_validate(schema, user_data) != JSONSuccess)
	{

		user_data = json_value_init_object();
		json_object_set_string(json_object(user_data), name, string);			
		json_serialize_to_file(user_data, "config.json");

		modulegui->Save_Config(json_object(config));
	}
	name = json_object_get_string(json_object(user_data), "name");

	json_value_free(schema);
	json_value_free(user_data);
	json_value_free(config);
}