#include "GUI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "ParShapes/par_shapes.h"

#include "GameObject.h"
#include "ImGui/imgui_internal.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

par_shapes_mesh* cube = par_shapes_create_cube();

GUI_Hierarchy::GUI_Hierarchy(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) 
{
	elem_name = "Hierarchy"; 
	prim_posx = 0.0f; prim_posy = 0.0f; prim_posz = 0.0f;
	prim_sizeX = 100; prim_sizeY = 100;
}

GUI_Hierarchy::~GUI_Hierarchy() {}

update_status GUI_Hierarchy::Update(float dt)
{
	prim_posx = App->inspector->meshPos[0];
	prim_posy = App->inspector->meshPos[1];
	prim_posz = App->inspector->meshPos[2];
	par_shapes_translate(cube, prim_posx, prim_posy, prim_posz);
	
	return UPDATE_CONTINUE;
}


void GUI_Hierarchy::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{	
		for (auto item = App->modscene->gameobjects.begin(); item != App->modscene->gameobjects.end(); ++item)
		{
			//ImGui::Text((*item)->name.c_str());

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			
			bool opened = ImGui::TreeNodeEx((*item)->name.c_str(), node_flags);
			
			if(opened)
			{	
				ImGui::TreePop();
			}
			
			
			if (ImGui::IsItemClicked(0))
			{
				if (selectedGO != (*item))
				{
					if (selectedGO != nullptr)
					{
						selectedGO->selected = false;
						for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->selected = false; }
					}
					selectedGO = (*item);
					selectedGO->selected = true;
					for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->selected = true; }
				}
			}
		}
		ImGui::Separator();
		static int slices = 30; static int stacks = 30;

		if (ImGui::CollapsingHeader("Create Primitives"))
		{
			ImGui::Separator();

			if (ImGui::Button("Create Cube"))
				App->modscene->CreatePrimitives(cube, "Cube");
			
			if (ImGui::Button("Create Plane"))
				App->modscene->CreatePrimitives(par_shapes_create_plane(prim_sizeX, prim_sizeY), "Plane");

			if (ImGui::Button("Create Sphere"))
				App->modscene->CreatePrimitives(par_shapes_create_parametric_sphere(prim_sizeX, prim_sizeY), "Sphere");

			if (ImGui::Button("Create Cone"))
				App->modscene->CreatePrimitives(par_shapes_create_cone(prim_sizeX, prim_sizeY), "Cone");
		}
	}
	ImGui::End();
}





