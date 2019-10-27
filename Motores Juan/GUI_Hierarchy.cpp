#include "GUI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ImGui/imgui_internal.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "ParShapes/par_shapes.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

GUI_Hierarchy::GUI_Hierarchy(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Hierarchy"; }

GUI_Hierarchy::~GUI_Hierarchy() {}

void GUI_Hierarchy::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{	
		for (auto item = App->modscene->gameobjects.begin(); item != App->modscene->gameobjects.end(); ++item)
		{
			ImGui::Text((*item)->name.c_str());

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
			ImGui::Text("Size:");
			static int X = 100, Y = 100;
			ImGui::DragIntRange2(" ", &X, &Y, .25f, 0, 1000, "X: %.1i ", "Y: %.1i ");

			ImGui::Text("Position:");
			static float posx = 0.0f , posy = 0.0f , posz = 0.0f;
			ImGui::DragFloat("X##foo1", &posx, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Y##foo1", &posy, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Z##foo1", &posz, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);

			
			ImGui::Separator();

			par_shapes_mesh* cube = par_shapes_create_cube();
			if (ImGui::Button("Create Cube"))
				App->modscene->CreatePrimitives(cube, "Cube");

			par_shapes_translate(cube, posx, posy, posz);


			if (ImGui::Button("Create Plane"))
				App->modscene->CreatePrimitives(par_shapes_create_plane(X, Y), "Plane");

			if (ImGui::Button("Create Sphere"))
				App->modscene->CreatePrimitives(par_shapes_create_parametric_sphere(50, 50), "Sphere");

			if (ImGui::Button("Create Cone"))
				App->modscene->CreatePrimitives(par_shapes_create_cone(50, 50), "Cone");
		}
	}
	ImGui::End();
}





