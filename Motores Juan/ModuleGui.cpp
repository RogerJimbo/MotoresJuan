#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"

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
	float test[4];
	//Default Window
	ImGui::Begin("Motores Juan Window");
	ImGui::Text("Pepe las pelotas gonzalez");
	ImGui::ColorEdit4("Coloreeee", test);
	ImGui::End();

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{ 
			ImGui::MenuItem("New", "Ctrl+N", false, true);
			ImGui::MenuItem("Open", "Ctrl+o", false, true);
			if (ImGui::MenuItem("Close"))
			{
				ImGui::EndMenu();
				return UPDATE_STOP;
			}
			ImGui::EndMenu(); 
		}
		if (ImGui::BeginMenu("Menu 2")) { ImGui::EndMenu(); }
		if (ImGui::BeginMenu("Menu 3")) { ImGui::EndMenu(); }
		if (ImGui::BeginMenu("Menu 4")) { ImGui::EndMenu(); }
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
