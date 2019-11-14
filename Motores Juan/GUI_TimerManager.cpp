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
		//ImGuiIO& io = ImGui::GetIO();
		//ImTextureID my_tex_id = io.Fonts->TexID;

		//Add a button with image

		ImTextureID texture_id; 

		GLuint imagetexture = 0;

		

		ImVec2 imagesize { 2.f,2.f };

	//	ImGui::ImageButton(texture_id, imagesize,);

		ImGui::Text("%f", App->moduletimer->getGameTime());

		ImGui::End();
	}
}

