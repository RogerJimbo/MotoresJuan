#include "GUI_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"

GUI_Inspector::GUI_Inspector(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Inspector"; }

GUI_Inspector::~GUI_Inspector() {}

void GUI_Inspector::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{

		

	}
	ImGui::End();
}