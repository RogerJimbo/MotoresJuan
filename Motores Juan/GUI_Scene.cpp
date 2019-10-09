#include "GUI_Scene.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

GUI_Scene::GUI_Scene(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Scene"; }

GUI_Scene::~GUI_Scene() {}

void GUI_Scene::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(400, 100));
	ImGui::Begin(elem_name.c_str(), &show);
	ImGui::End();
}
