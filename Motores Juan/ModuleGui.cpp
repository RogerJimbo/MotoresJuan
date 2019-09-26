#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
#include "ModuleWindow.h"


#include <array>

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



	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update(float dt)
{
	//Engine Windows
	if (!active_engine_windows[ABOUT]) { CreateAboutWindow();}

	if (active_engine_windows[CONFIG]) { CreateConfigWindow(); }

	if (!active_engine_windows[HARDWARE]) { CreateHardwareWindow(); }

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
			if (ImGui::MenuItem("About", NULL, &active_engine_windows[ABOUT])) {}

			if (ImGui::MenuItem("GitHub", false, true)) { App->RequestBrowser("https://github.com/RogerJimbo/MotoresJuan"); }

			if (ImGui::MenuItem("Hardware Info", NULL, &active_engine_windows[HARDWARE])) {}

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

void ModuleGui::CreateAboutWindow()
{
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(0, 20));

	if (close_tab && ImGui::Begin("About"))
	{
		ImGui::Text("Motores Juan"), ImGui::Separator();
		ImGui::Text("This is a video game engine with academic purposes.");
		ImGui::Text("By Roger Sanchez and Ivan Drofiak.");
		ImGui::Text("Licensed under the MIT License."), ImGui::Separator();
		ImGui::End();
	}
	
}

void ModuleGui::CreateHardwareWindow()
{
	//CPU Caché
	int cpu_cache = SDL_GetCPUCacheLineSize();
	ImGui::SetNextWindowSize(ImVec2(300, 300));
	ImGui::SetNextWindowPos(ImVec2(100, 100));
	ImGui::Begin("Hardware Info");
	ImGui::Text("CPU's: %i", cpu_cache);
	//SDL Version
	SDL_version ver;
	SDL_GetVersion(&ver);
	ImGui::Text("SDL Version: %d", ver);


	ImGui::End();
}

void ModuleGui::CreateConfigWindow()
{
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(0, 20));

	ImGui::Begin("Configuration");

	if (ImGui::TreeNode("Application"))		// FPS Histograms 
	{
		
		ImGui::Text("Limit Framerate: ");
		static float TestData[6] = { 0.f,-4.f,3.f,-2.f,0.f,4.f };
		ImGui::PlotHistogram("", TestData, 6, 0, "Framerate", 0.f, 100.f, ImVec2(0,80));
		ImGui::PlotHistogram("", TestData, 6, 0, "Miliseconds", 0.f, 100.f, ImVec2(0, 80));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Window"))	//Window Configuration
	{
		static bool active, fullscreen,resizable,borderless,checkbox = false;
		ImGui::Checkbox("Active", &active);
		static int bright_slider_scroll, width_slider_scroll, height_slider_scroll = 0;
		ImGui::SliderInt("Brightness", &bright_slider_scroll, 0, 1000 );
		ImGui::SliderInt("Width", &width_slider_scroll, 640, 2048);
		ImGui::SliderInt("Height", &height_slider_scroll, 480, 1536);

		ImGui::Checkbox("FullScreen", &fullscreen); ImGui::SameLine();	ImGui::Checkbox("Resizable", &resizable);
		ImGui::Checkbox("Borderless", &borderless); ImGui::SameLine();	ImGui::Checkbox("Full Desktop", &checkbox);



		ImGui::TreePop();
	}

	ImGui::End();
}

