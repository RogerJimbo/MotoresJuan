#include "GUI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

GUI_Hierarchy::GUI_Hierarchy(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Hierarchy"; }

GUI_Hierarchy::~GUI_Hierarchy() {}

void GUI_Hierarchy::Draw(bool* open)
{
	ImGui::SetNextWindowSize(ImVec2(270, 700));
	ImGui::SetNextWindowPos(ImVec2(0, 18));

	if (ImGui::Begin(elem_name.c_str(), open))
	{


	}
	ImGui::End();

}