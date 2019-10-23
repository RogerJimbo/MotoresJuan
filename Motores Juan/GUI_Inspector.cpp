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
		static bool foo = false;
		static bool transformation = true;

		static char bufpass[64] = "Game Object Name";
		ImGui::Checkbox("Active", &foo); ImGui::SameLine();
		ImGui::InputText(" ", bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);

		
		if ((ImGui::CollapsingHeader("Transform")) )
		{
			ImGui::Checkbox("Active2", &transformation); 
			static int vecpos[3] = { 0, 0, 0 }, vecrot[3] = { 0, 0, 0 }, vecscale[3] = { 1, 1, 1 };

			ImGui::Text("X         Y         Z");
			ImGui::DragInt3("Position", vecpos, 0.25f);
			ImGui::SliderInt3("Rotation", vecrot, 0.0f, 360.0f);
			ImGui::DragInt3("Scale", vecscale, 0.25f, 1.0f, 1000.0f);




			//ImGui::ShowStyleEditor();

		} 
	}
	ImGui::End();
}