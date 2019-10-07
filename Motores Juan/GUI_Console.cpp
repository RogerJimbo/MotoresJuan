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

void GUI_Console::Draw()
{
	
}

