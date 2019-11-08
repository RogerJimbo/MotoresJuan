#include "ModuleGUI.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Glew/include/glew.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

#include "Devil/includes/il.h"
#include "Parson/parson.h"
#include "Assimp/include/version.h"

#include "ImGuizmos/ImGuizmo.h"

#include "GUI_Config.h"
#include "GUI_Console.h"
#include "GUI_Scene.h"
#include "GUI_Hierarchy.h"
#include <array>

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled) { module_name = "Gui"; }

ModuleGui::~ModuleGui() {}

bool ModuleGui::Init(const JSON_Object& config)
{
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable, ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	io->Fonts->AddFontFromFileTTF("comicsans.ttf", 17);
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	SDL_GL_CreateContext(App->window->window);

	//ImGuizmo::BeginFrame();

	for (int i = 0; i != NUM_ACT_WIN; i++) { active_engine_windows[i] = false; }

	return true;
}

bool ModuleGui::Start()
{
	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	ImGui::SetNextWindowPos({ 0,20 });
	ImGui::SetNextWindowSize({(float)App->window->window_width, (float)App->window->window_height});

	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{
	Docking();
	//ImGui::ShowDemoWindow();

	//Engine Windows
	if (scene_open)				App->scene->Draw(&scene_open);
	if (hierarchy_open)			App->hierarchy->Draw(&hierarchy_open);
	if (console_open)				App->console->Draw(&console_open);
	if (config_open)				App->configuration->Draw(&config_open);
	if (inspector_open)			App->inspector->Draw(&inspector_open);
	if (about_open)				this->CreateAboutWindow(&about_open); 
	if (hardware_open)			this->CreateHardwareWindow(&hardware_open);
	if (controls_open)			this->CreateControlsWindow(&controls_open);

	// Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{ 
			ImGui::MenuItem("New Scene", false, true);	//Todo Create new scene

			if (ImGui::BeginMenu("Load Model")) 
			{
				if (ImGui::MenuItem("BakerHouse.fbx")) App->loader->Import("BakerHouse.fbx", nullptr);
				if (ImGui::MenuItem("House.fbx")) App->loader->Import("House.fbx", nullptr);
				if (ImGui::MenuItem("Medieval House.fbx")) App->loader->Import("medieval_house.fbx", nullptr);
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save Config", false, true)) { App->Save_Config(); }		//Save data JSON
			if (ImGui::MenuItem("Load Config", false,true)) { App->Load_Config(); }		//Load data JSON

			if (ImGui::MenuItem("Close", "ALT+F4"))
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
			if (ImGui::MenuItem("About", NULL, &about_open)) {}
			if (ImGui::MenuItem("Hardware Info", NULL, &hardware_open)) {}
			if (ImGui::MenuItem("GitHub", false, true)) { App->RequestBrowser("https://github.com/RogerJimbo/MotoresJuan"); }
			if (ImGui::MenuItem("Controls", NULL, &controls_open)) { }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Hierarchy", NULL, &hierarchy_open)) {}
			if (ImGui::MenuItem("Console", NULL, &console_open)) {}
			if(ImGui::MenuItem("Scene", NULL, &scene_open)) {}
			if (ImGui::MenuItem("Configuration", NULL, &config_open)) {}
			if(ImGui::MenuItem("Inspector", NULL, &inspector_open)) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt) { return UPDATE_CONTINUE; }

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

void ModuleGui::CreateControlsWindow(bool* open)
{
	ImGui::OpenPopup("Controls");
	if (ImGui::BeginPopupModal("Controls"))
	{
		ImGui::Text("Use 'Right Click +WASD' to move arround the scene."), ImGui::Separator();
		ImGui::Text("Use 'Q' to move upwards and 'E' to move downwards."), ImGui::Separator();
		ImGui::Text("Use 'Alt' & Left Click to orbit arround the object."), ImGui::Separator();
		ImGui::Text("Use 'Mouse Wheel' to zoom In and Out."); ImGui::Separator();
		ImGui::Text("Use Mouse Middle Button to move the camera."); ImGui::Separator();
		ImGui::Text("Use 'F' to focus the camera on the object."); ImGui::Separator();
		ImGui::Text("Use 'Shift' to duplicate movement speed."); ImGui::Separator();

		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); controls_open = false; }
		ImGui::EndPopup();
	}
}

void ModuleGui::CreateAboutWindow(bool* open)
{
	ImGui::OpenPopup("About");
	if (ImGui::BeginPopupModal("About"))
	{
		ImGui::Text("Motores Juan"), ImGui::Separator();
		ImGui::Text("This is a video game engine with academic purposes.");
		ImGui::Text("By Roger Sanchez and Ivan Drofiak.");
		ImGui::Text("Licensed under the MIT License."), ImGui::Separator();

		ImGui::Text("External Libraries We Used:");
		if (ImGui::Button("MathGeoLib")) App->RequestBrowser("http://clb.demon.fi/MathGeoLib/"); ImGui::SameLine();
		if (ImGui::Button("SDL")) App->RequestBrowser("https://wiki.libsdl.org/FrontPage"); ImGui::SameLine();
		if (ImGui::Button("Open GL")) App->RequestBrowser("https://www.opengl.org/"); ImGui::SameLine();
		if (ImGui::Button("Parson")) App->RequestBrowser("https://github.com/kgabis/parson"); ImGui::SameLine();
		if (ImGui::Button("DevIL")) App->RequestBrowser("http://openil.sourceforge.net/"); ImGui::SameLine();
		if (ImGui::Button("Glew")) App->RequestBrowser("http://glew.sourceforge.net/"); ImGui::SameLine();
		if (ImGui::Button("Assimp")) App->RequestBrowser("http://www.assimp.org/"); 	ImGui::Separator();
		if (ImGui::Button("ParShapes")) App->RequestBrowser("https://github.com/prideout/par"); 	ImGui::Separator();
		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); about_open = false; }
		ImGui::EndPopup();
	}		
}

void ModuleGui::CreateInfoWindow()
{
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	const char* tab_names[2] = { "Configuration", "Inspector" };
	static bool opened_tab[2] = { true, true };

	if (ImGui::BeginTabBar(" ", tab_bar_flags))

	for (int iter = 0; iter < IM_ARRAYSIZE(opened_tab); iter++)
		if (opened_tab[iter] && ImGui::BeginTabItem(tab_names[iter], &opened_tab[iter]))
		{
			if(iter == 0) {  }
			else if (iter == 1) {  }
			ImGui::EndTabItem();
		}

	ImGui::EndTabBar();
}

void ModuleGui::CreateHardwareWindow(bool* open)
{
	ImGui::OpenPopup("Hardware Info");
	if (ImGui::BeginPopupModal("Hardware Info"))
	{
		static bool active = false;
		ImGui::Checkbox("Active", &active);

		//Versions
		float mathgeolibversion = 2.0f;
		SDL_version ver; 
		SDL_GetVersion(&ver);


		ImGui::Text("SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);
		ImGui::Text("Assimp Version: %i", aiGetVersionMajor());
		ImGui::Text("Open GL Version: %s", glGetString(GL_VERSION));
		ImGui::Text("DevIL Version: %i", IL_VERSION);
		ImGui::Text("MathGeoLib Version: %.02f", mathgeolibversion);
		ImGui::Text("Glew Version: %s", glewGetString(GLEW_VERSION));

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

		ImGui::Separator();
		ImGui::Text("GPU Vendor: "); ImGui::SameLine(); ImGui::Text((char*)glGetString(GL_VENDOR));
		ImGui::Text("GPU Model: "); ImGui::SameLine(); ImGui::Text((char*)glGetString(GL_RENDERER));

		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); hardware_open = false; }

		ImGui::EndPopup();
	}
}

void ModuleGui::Docking()
{
	static bool open = true;
	bool opt_fullscreen = open;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
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

