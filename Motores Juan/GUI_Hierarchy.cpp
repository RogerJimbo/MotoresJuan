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
		App->modscene->root->RecursiveHierarchy();

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

		if (ImGui::CollapsingHeader("Create"))
		{
			if (ImGui::Button("Camera")) App->modscene->CreateCamera();

			if (ImGui::Button("Empty Game Object")) 	App->modscene->CreateEmpty();
		}
	}
	ImGui::End();
}