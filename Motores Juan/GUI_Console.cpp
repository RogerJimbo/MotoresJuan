#include "GUI_Console.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

GUI_Console::GUI_Console(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Console"; }

GUI_Console::~GUI_Console() {}


bool GUI_Console::Init(const JSON_Object& config)
{
	return true;
}

bool GUI_Console::Start()
{
	return true;
}

update_status GUI_Console::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status GUI_Console::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status GUI_Console::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool GUI_Console::CleanUp()
{
	return true;
}

void GUI_Console::Draw()
{
	
}


