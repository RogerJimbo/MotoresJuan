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

void GUI_Scene::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{
		vec2 win_size = (ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		vec2 screen_size = (SCREEN_WIDTH, SCREEN_HEIGHT);
		
		ImGui::SetCursorPos(ImVec2(win_size.x - screen_size.x, win_size.y - screen_size.y));
		ImGui::Image((ImTextureID)App->renderer3D->buffer_tex, ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	}
	ImGui::End();
}
