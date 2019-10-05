#include "GUI_Console.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"

GUI_Console::GUI_Console(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Console"; }

GUI_Console::~GUI_Console() {}

void GUI_Console::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 400), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin(elem_name.c_str(), &show);
	ImGui::End();
}