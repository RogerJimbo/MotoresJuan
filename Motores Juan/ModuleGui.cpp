#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "MathGeoLib/Math/float3.h"

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
	
	ImGui::SetNextWindowPos({ 0,20 });
	ImGui::SetNextWindowSize({(float)App->window->window_width, (float)App->window->window_height});

	//No Windows Menu Flags (8/15)
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update(float dt)
{
	//hardcoded window sizes and positions

	// 1. Hierarchy tab
	ImGui::SetNextWindowSize(ImVec2(250, 550));
	ImGui::Begin("Scene Objects Hierarchy", 0);
	ImGui::End();

	// 2. Default Console
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(0, 570));																	
	ImGui::Begin("Console", 0, window_flags);
	ImGui::End();

	// 3. Object Inspector 
	ImGui::SetNextWindowSize(ImVec2(250, 550));
	ImGui::SetNextWindowPos(ImVec2(800, 20));																
	ImGui::Begin("Object Inspector", 0, window_flags);
	ImGui::End();

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{ 
			ImGui::MenuItem("New Scene", "Ctrl+N", false, true);		//Create new scene
			ImGui::MenuItem("Open", "Ctrl+O", false, true);					//Import Files
			ImGui::MenuItem("Save", "Ctrl+S", false, true);					//Save data
			ImGui::MenuItem("Load Default Config", false);					//Loads default config
			if (ImGui::MenuItem("Close", "ESC"))
			{
				ImGui::EndMenu();
				ImGui::EndMainMenuBar();
				ImGui::EndFrame();
				return UPDATE_STOP;
			}
			ImGui::EndMenu(); 
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", false, true))
			{
				//Display README
			}
			if (ImGui::MenuItem("GitHub", false, true))
			{
				// Open GitHub page
			}
			if (ImGui::MenuItem("Info", false, true))
			{
				//general engine info
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
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
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}
