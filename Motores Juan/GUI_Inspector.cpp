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
		static bool transform = true;
		const char* name = "";

		for (auto item = App->modscene->gameobjects.begin(); item != App->modscene->gameobjects.end(); ++item)
			if ((*item)->selected) { name = (*item)->name.c_str(); } 
		
		ImGui::Text(name);  
		ImGui::Checkbox("Active", &foo); 

		for (auto item = App->modscene->gameobjects.begin(); item != App->modscene->gameobjects.end(); ++item)
		{
			if ((*item)->selected && (*item)->children.size() > 0)
			{
				selectedGO = (*item);

				if ((ImGui::CollapsingHeader("Transform")))
				{
					ImGui::Checkbox("Active", &transform);
					static int vecpos[3] = { 0, 0, 0 }, vecrot[3] = { 0, 0, 0 }, vecscale[3] = { 1, 1, 1 };

					ImGui::DragInt3("Position", vecpos, 0.25f);
					ImGui::SliderInt3("Rotation", vecrot, 0.0f, 360.0f);
					ImGui::DragInt3("Scale", vecscale, 0.25f, 1.0f, 1000.0f);

					ImGuiIO& io = ImGui::GetIO();
					io.WantCaptureKeyboard;
				}

				if ((ImGui::CollapsingHeader("Mesh")))
				{
					meshes = new ComponentMesh();
					for (auto item = selectedGO->children.begin(); item != selectedGO->children.end(); ++item)
					{
						ComponentMesh*auxmesh = (ComponentMesh*)(*item)->GetComponent(MESH);
						meshes->num_vertices += auxmesh->num_vertices;
					}

					ImGui::Text("Number of vertices: %d", meshes->num_vertices);
					ImGui::Text("Number of children: %d", selectedGO->children.size());
				}

				if ((ImGui::CollapsingHeader("Material")))
				{

				}
			}
		}
	}
	ImGui::End();
}