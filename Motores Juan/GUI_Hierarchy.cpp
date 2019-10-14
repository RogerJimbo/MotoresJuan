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
		if (ImGui::CollapsingHeader("Create"))
		{
			if (ImGui::Button("Cube")) { createCube = true; ++numberPrimitives; prim_name = "Cube"; }
			if (ImGui::Button("Sphere")) { createSphere = true; ++numberPrimitives; prim_name = "Sphere"; }
			if (ImGui::Button("Plane")) { createPlane = true; ++numberPrimitives; prim_name = "Plane"; }
		}
		for (int i = 0; i < numberPrimitives; ++i)
		{
			ImGui::Selectable(prim_name, ImGuiSelectableFlags_AllowDoubleClick);
			if (ImGui::IsMouseDoubleClicked(0))
			{
				if (prim_name == "Cube") { createCube = false; }
				else if (prim_name == "Sphere") { createSphere = false; }
				else if (prim_name == "Plane") { createPlane = false; }
			}
		}
	}
	ImGui::End();
}


