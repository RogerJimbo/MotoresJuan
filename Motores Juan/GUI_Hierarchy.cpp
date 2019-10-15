#include "GUI_Hierarchy.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

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
		if (ImGui::CollapsingHeader("Create Cube") || createCube)
		{
			if (ImGui::Button("Cube")) { createCube = true; ++numberPrimitives; prim_name = "Cube"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createCube = false; }
		}
		if (ImGui::CollapsingHeader("Create Sphere") || createSphere)
		{
			if (ImGui::Button("Sphere")) { createSphere = true; ++numberPrimitives; prim_name = "Sphere"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createSphere = false; }
		}
		if (ImGui::CollapsingHeader("Create Plane") || createPlane)
		{
			if (ImGui::Button("Plane")) { createPlane = true; ++numberPrimitives; prim_name = "Plane"; }
			ImGui::SameLine(); if (ImGui::Button("Delete")) { createPlane = false; }

			static float X = 0, Y = 0, posx[1] = { 0.0f }, posy[1] = { 0.0f }, posz[1] = { 0.0f };
			ImGui::DragFloatRange2("Size", &X, &Y, .25f, 0.0f, 1000.0f, "X: %.1f ", "Y: %.1f ");

			ImGui::DragFloat("X", posx, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Y", posy, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);
			ImGui::DragFloat("Z", posz, .025f, -1000.0f, 1000.0f, "%.2f", 1.25f);

			if (createPlane) App->modscene->ArrayPlane(X, Y, posx[1], posy[1], posz[1]); 
		}
	}
	ImGui::End();
}

void GUI_Hierarchy::CreatePrimitives()
{
	if (createCube) { App->modscene->ArrayCube(); }
	if (createSphere) { App->modscene->ArraySphere(); }
	//
}


