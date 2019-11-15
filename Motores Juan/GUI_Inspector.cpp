#include "GUI_Inspector.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "GameObject.h"
#include "ComponentMesh.h"

GUI_Inspector::GUI_Inspector(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Inspector"; }

GUI_Inspector::~GUI_Inspector() {}

void GUI_Inspector::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{
		static bool foo = false;
		const char* name = "";

		for (auto item = App->modscene->root->children.begin(); item != App->modscene->root->children.end(); ++item)
			if ((*item)->is_selected) { name = (*item)->name.c_str(); }

		if (App->modscene->object_selected)
		{
			App->modscene->object_selected->ShowInspectorInfo();
		}

		ImGui::Text(name);
		ImGui::Separator();
		ImGui::Checkbox("Active", &foo);

	}
	ImGui::End();
}