#include "GUI_TimerManager.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "ImGui/imgui_impl_opengl2.h"

GUI_TimerManager::GUI_TimerManager(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Time Management"; }
GUI_TimerManager::~GUI_TimerManager() {}

void GUI_TimerManager::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{
		ImGui::Text("Real Time %.2f", App->moduletimer->getRealTime() / 1000); 
		ImGui::Text("Frame Count: %i", App->moduletimer->getFrameCount());
		ImGui::Text("Delta Time: %.2f", App->moduletimer->getGameDeltaTime());
		ImGui::Text("Delta Time: %f", App->moduletimer->GetGameDeltaTime());

		ImGui::End();
	}
}

