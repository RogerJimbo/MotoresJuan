#include "ModuleGUI.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

#include "Parson/parson.h"

#include "GUI_Config.h"
#include <array>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled) { config_name = "Gui"; }

ModuleGui::~ModuleGui() {}

bool ModuleGui::Init(const JSON_Object& config)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	SDL_GL_CreateContext(App->window->window);

	configuration = new GUI_Config(App);

	Gui.push_back((GUI_Config*)configuration);

	return true;
}

bool ModuleGui::Start()
{
	io = &ImGui::GetIO();
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

	for (auto item = Gui.begin(); item != Gui.end(); item++) { if ((*item)->show) (*item)->Draw(); }

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update(float dt)
{
	//Engine Windows
	if (!active_engine_windows[ABOUT]) { CreateAboutWindow();}

	if (!active_engine_windows[HARDWARE]) { CreateHardwareWindow(); }

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{ 
			ImGui::MenuItem("New Scene", "Ctrl+N", false, true);												//Create new scene
			ImGui::MenuItem("Open", "Ctrl+O", false, true);															//Import Files

			if (ImGui::MenuItem("Save Config", "Ctrl+S", false, true)) { App->Save_Config(); }		//Save data

			ImGui::MenuItem("Load Default Config", false);															//Loads default config

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

	//SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{	
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(App->renderer3D->context);
	//SDL_GL_DeleteContext(App->window->window);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

void ModuleGui::CreateAboutWindow()
{
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(0, 20));

	if (ImGui::Begin("About"), 0, window_flags)
	{
		ImGui::Text("Motores Juan"), ImGui::Separator();
		ImGui::Text("This is a video game engine with academic purposes.");
		ImGui::Text("By Roger Sanchez and Ivan Drofiak.");
		ImGui::Text("Licensed under the MIT License."), ImGui::Separator();

		ImGui::Text("External Libraries We Used:");
		if (ImGui::Button("MathGeoLib")) App->RequestBrowser("http://clb.demon.fi/MathGeoLib/"); ImGui::SameLine();
		if (ImGui::Button("SDL")) App->RequestBrowser("https://wiki.libsdl.org/FrontPage"); ImGui::SameLine();
		if (ImGui::Button("Open GL")) App->RequestBrowser("https://www.opengl.org/"); ImGui::SameLine();
		if (ImGui::Button("Parson")) App->RequestBrowser("https://github.com/kgabis/parson");
			
		ImGui::End();
	}	
}

void ModuleGui::CreateHardwareWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 300));
	ImGui::SetNextWindowPos(ImVec2(100, 100));
	ImGui::Begin("Hardware Info");

	static bool active = false;
	ImGui::Checkbox("Active", &active);

	//SDL Version
	SDL_version ver;
	SDL_GetVersion(&ver);
	ImGui::Text("SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);
	ImGui::Separator();

	//CPU Cach�
	int cpu_cache = SDL_GetCPUCount();
	ImGui::Text("CPU's: %i", cpu_cache);

	//RAM 
	float ram = SDL_GetSystemRAM();
	ImGui::Text("System RAM: %f Gb", ram / 1000);

	//Caps
	SDL_bool SDL_HasRDTSC();	 if (SDL_HasRDTSC) { RDTSC = "RDTSC"; }
	SDL_bool SDL_HasMMX();		 if (SDL_HasMMX) { MMX = "MMX"; }
	SDL_bool SDL_HasAVX();		 if (SDL_HasAVX) { AVX = "AVX"; }
	SDL_bool SDL_HasSSE();	     if (SDL_HasSSE) { SSE = "SSE"; }
	SDL_bool SDL_HasSSE2();		 if (SDL_HasSSE2) { SSE2 = "SSE2"; }
	SDL_bool SDL_HasSSE3();		 if (SDL_HasSSE3) { SSE3 = "SSE3"; }
	SDL_bool SDL_HasSSE41();	 if (SDL_HasSSE41) { SSE41 = "SSE41"; }
	SDL_bool SDL_HasSSE42();	 if (SDL_HasSSE42) { SSE42 = "SSE42"; }

	ImGui::Text("Caps: %s, %s, %s, %s, %s, %s, %s, %s", RDTSC, MMX, AVX, SSE, SSE2, SSE3, SSE41, SSE42);
	
	//GPU
	IDirect3DDevice9* glGetString();

	ImGui::End();
}

void ModuleGui::Save_Config(JSON_Object& config) const
{
	json_object_set_boolean(&config, "configuration", active_engine_windows[CONFIG]);
	json_object_set_boolean(&config, "hierarchy", active_engine_windows[HIERARCHY]);
	json_object_set_boolean(&config, "about", active_engine_windows[ABOUT]);
}

void ModuleGui::Load_Config(const char* name, const char* string, bool state)
{
}


