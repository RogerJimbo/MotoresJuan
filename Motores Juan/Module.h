#pragma once

class Application;
struct PhysBody3D;

#include "Globals.h"
#include "Parson\parson.h"
#include <list>

using namespace std;

class Module
{
private :
	bool enabled;

public:
	Application* App = nullptr;

	Module(Application* parent, bool start_enabled) : App(parent) {}

	virtual ~Module() {}

	virtual bool Init(const JSON_Object& config) { return true; }

	virtual bool Start() { return true; }

	virtual update_status PreUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual update_status Update(float dt) { return UPDATE_CONTINUE; }

	virtual update_status PostUpdate(float dt) { return UPDATE_CONTINUE; }

	virtual bool CleanUp() { return true; }

	virtual void Save_Config(JSON_Object& config) const {}
	virtual void Load_Config(const JSON_Object& config) {}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2) {}

	string module_name;
};