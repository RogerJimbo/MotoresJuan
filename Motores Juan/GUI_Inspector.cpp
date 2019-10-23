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
		bool foo = false;
		ImGui::Checkbox("", &foo);
	
		if (ImGui::CollapsingHeader("Transform"))
		{
			static int vecpos[3] = { 0, 0, 0 }, vecrot[3] = { 0, 0, 0 }, vecscale[3] = { 0, 0, 0 };

			ImGui::Text("X         Y         Z");
			ImGui::DragInt3("Position", vecpos, 0.25f);
			ImGui::Text("X         Y         Z");
			ImGui::DragInt3("Rotation", vecrot, 0.25f);
			ImGui::Text("X         Y         Z");
			ImGui::DragInt3("Scale", vecscale, 0.25f);
		}
	}
	ImGui::End();
}