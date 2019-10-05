#pragma once

class Application;

#include "Globals.h"
#include "ImGui/imgui.h"
#include "Parson\parson.h"
#include <list>

using namespace std;

class GUI_Element
{
public:
	Application* App = nullptr;

	GUI_Element(Application* app, bool active = true):App(app), show(active) {}
	~GUI_Element(){}

	virtual bool Init(const JSON_Object& config) { return true; }

	virtual bool Start() { return true; }

	virtual update_status PreUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual update_status Update(float dt) { return UPDATE_CONTINUE; }

	virtual update_status PostUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual void Save_Config(JSON_Object& config) const {}
	virtual void Load_Config(const JSON_Object& config) {}

	virtual void Draw() {}

	virtual bool CleanUp() { return true; }

	string elem_name;
	bool show = true;
};