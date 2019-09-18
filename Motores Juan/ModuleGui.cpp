#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGui::~ModuleGui()
{
}

bool ModuleGui::Init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->renderer);
	//ImGui_ImplOpenGL2_Init();

	ImGui::StyleColorsDark();



	return true;
}

// PreUpdate: clear buffer
update_status ModuleGui::PreUpdate()
{
	//ImGui_ImplOpenGL2_NewFrame();
	//ImGui_ImplSDL2_NewFrame(App->window->window);
	//ImGui::NewFrame();


	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update()
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGui::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{

	return true;
}
