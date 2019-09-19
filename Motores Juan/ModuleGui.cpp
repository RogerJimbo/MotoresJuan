#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

#include "ModuleWindow.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGui::~ModuleGui()
{
}

bool ModuleGui::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	return true;
}

// PreUpdate: clear buffer
update_status ModuleGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update(float dt)
{
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::End();

	ImGui::Text("Hola");

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGui::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{


	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}
