#include "GUI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ImGui/imgui_internal.h"

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
		
		if (ImGui::CollapsingHeader("Create Cube"))
		{
			if (ImGui::Button("Cube")) { createCube = true; ++numberPrimitives; prim_name = "Cube"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createCube = false; }

			static float X[1] = { 2.0f }, Y[1] = { 2.0f }, Z[1] = { 2.0f }, posx[1] = { 0.0f }, posy[1] = { 0.0f }, posz[1] = { 0.0f };
			ImGui::Text("Size:");
			ImGui::DragFloat("X", X, .025f, .0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Y", Y, .025f, .0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Z", Z, .025f, .0f, 1000.0f, "%.2f", 1.25f);
			ImGui::Text("Position:");
			ImGui::DragFloat("X##foo1", posx, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Y##foo1", posy, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Z##foo1", posz, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);

			if (createCube) { App->modscene->ArrayCube(X[1], Y[1], Z[1], posx[1], posy[1], posz[1]); }


		}
		if (ImGui::CollapsingHeader("Create Sphere"))
		{
			if (ImGui::Button("Sphere")) { createSphere = true; ++numberPrimitives; prim_name = "Sphere"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createSphere = false; }
		}
		if (ImGui::CollapsingHeader("Create Plane"))
		{
			if (ImGui::Button("Plane")) { createPlane = true; ++numberPrimitives; prim_name = "Plane"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createPlane = false; }
			ImGui::Text("Size:");
			static float X = 2.0f, Y = 2.0f, posx[1] = { 0.0f }, posy[1] = { 0.0f }, posz[1] = { 0.0f }; 
			ImGui::DragFloatRange2(" ", &X, &Y, .25f, 0.0f, 1000.0f, "X: %.1f ", "Y: %.1f ");
			ImGui::Text("Position:");
			ImGui::DragFloat("X##foo2", posx, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Y##foo2", posy, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Z##foo2", posz, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);

			if (createPlane) { App->modscene->ArrayPlane(X, Y, posx[1], posy[1], posz[1]); }
		}
	}
	ImGui::End();
}





