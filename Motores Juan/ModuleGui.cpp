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
#include "GUI_Console.h"
#include "GUI_Scene.h"
#include "GUI_Hierarchy.h"
#include <array>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled) { module_name = "Active Gui Windows"; }

ModuleGui::~ModuleGui() {}

bool ModuleGui::Init(const JSON_Object& config)
{
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	SDL_GL_CreateContext(App->window->window);

	configuration = new GUI_Config(App);
	console = new GUI_Console(App);
	scene = new GUI_Scene(App);
	hierarchy = new GUI_Hierarchy(App);

	Gui.push_back((GUI_Config*)configuration);
	Gui.push_back((GUI_Console*)console);
	Gui.push_back((GUI_Scene*)scene);
	Gui.push_back((GUI_Hierarchy*)hierarchy);

	for (int i = 0; i != NUM_ACT_WIN; i++) { active_engine_windows[i] = false; }

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

	//for (auto item = Gui.begin(); item != Gui.end(); item++) { if ((*item)->show) (*item)->Draw(); }

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleGui::Update(float dt)
{
	//Engine Windows
	if (active_engine_windows[ABOUT]) { CreateAboutWindow();}
	if (active_engine_windows[HARDWARE]) { CreateHardwareWindow(); }
	if (!active_engine_windows[CONFIG]) { CreateInfoWindow(); }	
	if (!active_engine_windows[SCENE]) { this->scene->Draw(); }
	if (!active_engine_windows[HIERARCHY]) { this->hierarchy->Draw(); }
	if (!active_engine_windows[CONSOLE]) { this->console->Draw(); }

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{ 
			ImGui::MenuItem("New Scene", "Ctrl+N", false, true);												//Create new scene
			ImGui::MenuItem("Open", "Ctrl+O", false, true);															//Import Files

			if (ImGui::MenuItem("Save Config", "Ctrl+S", false, true)) { App->Save_Config(); }		//Save data JSON
			if (ImGui::MenuItem("Load Config", "Ctrl+L", false,true)) { App->Load_Config(); }		//Load data JSON

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

			if (ImGui::MenuItem("Hardware Info", NULL, &active_engine_windows[HARDWARE])) { }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if(ImGui::MenuItem("Configuration")) {}
			if (ImGui::MenuItem("Scene", NULL, &active_engine_windows[SCENE])) {}
			if (ImGui::MenuItem("Hierarchy", NULL, &active_engine_windows[HIERARCHY])) {}
			if (ImGui::MenuItem("Console", NULL, &active_engine_windows[CONSOLE])) {}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGui::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{	
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_GL_DeleteContext(App->window->window);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

void ModuleGui::CreateAboutWindow()
{
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(0, 20));

	if (ImGui::Begin("About", &show), 0, window_flags)
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

void ModuleGui::CreateInfoWindow()
{
	ImGui::SetNextWindowSize(ImVec2(270, 700));
	ImGui::SetNextWindowPos(ImVec2(1410, 18));
	ImGui::Begin(" ");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	const char* tab_names[2] = { "Configuration", "Inspector" };
	static bool opened_tab[2] = { true, true };

	if (ImGui::BeginTabBar(" ", tab_bar_flags))

	for (int iter = 0; iter < IM_ARRAYSIZE(opened_tab); iter++)
		if (opened_tab[iter] && ImGui::BeginTabItem(tab_names[iter], &opened_tab[iter]))
		{
			if(iter == 0) { this->configuration->Draw(); }
			else if (iter == 1) {  }
			ImGui::EndTabItem();
		}

	ImGui::EndTabBar();
	ImGui::End();
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

	//CPU Caché
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
	
	IDirect3DDevice9* glGetString();	//GPU

	ImGui::End();
}

void ModuleGui::Docking()
{
	
}

void ModuleGui::Save_Config(JSON_Object& config) const
{
	json_object_set_boolean(&config, "configuration", active_engine_windows[CONFIG]);
	json_object_set_boolean(&config, "hierarchy", active_engine_windows[HIERARCHY]);
	json_object_set_boolean(&config, "about", active_engine_windows[ABOUT]); 
	json_object_set_boolean(&config, "hardware", active_engine_windows[HARDWARE]);
	json_object_set_boolean(&config, "console", active_engine_windows[CONSOLE]);
	json_object_set_boolean(&config, "inspector", active_engine_windows[INSPECTOR]);

}

void ModuleGui::Load_Config(JSON_Object& config)
{
	active_engine_windows[CONFIG] = json_object_get_boolean(&config, "configuration");
	active_engine_windows[HIERARCHY] = json_object_get_boolean(&config, "hierarchy");
	active_engine_windows[ABOUT] = json_object_get_boolean(&config, "about");
	active_engine_windows[HARDWARE] = json_object_get_boolean(&config, "hardware");
	active_engine_windows[CONSOLE] = json_object_get_boolean(&config, "console");
	active_engine_windows[INSPECTOR] = json_object_get_boolean(&config, "inspector");	
}


