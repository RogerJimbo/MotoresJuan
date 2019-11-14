#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "ImGui/imgui.h"
#include "Parson\parson.h"
#include "Globals.h"
#include <vector>

enum EngineWindows {HIERARCHY, CONSOLE,  INSPECTOR, CONFIG, ABOUT, SCENE, HARDWARE, NUM_ACT_WIN};

class GUI_Element;
class GUI_Config;
class GUI_Hierarchy;
class GUI_Scene;
class GUI_Console;
class GUI_Inspector;

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Init(const JSON_Object& config);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void CreateInfoWindow();
	void CreateAboutWindow(bool* open);
	void CreateHardwareWindow(bool* open);
	void CreateControlsWindow(bool* open);
	void Docking();

	float horizontalFactor = (float)SCREEN_WIDTH * SCREEN_SIZE / SCREEN_WIDTH;
	float verticalFactor = (float)SCREEN_HEIGHT * SCREEN_SIZE / SCREEN_HEIGHT;

	void Save_Config(JSON_Object& config) const;
	void Load_Config(JSON_Object& config);
	
	bool active_engine_windows[NUM_ACT_WIN];

public:
	bool scene_open = true;
	bool hierarchy_open = true;
	bool console_open = true;
	bool config_open = true;
	bool inspector_open = true;
	bool timer_open = true;

	bool about_open = false;
	bool hardware_open = false;
	bool controls_open = false;

private:
	bool show = true;
	bool docking_background;

	char* RDTSC;
	char* MMX;
	char* AVX;
	char* SSE;
	char* SSE2;
	char* SSE3;
	char* SSE41;
	char* SSE42;

	float fps[100];
	float ms[100];
	float bright_slider_scroll = 1;

	ImGuiIO* io;
};

#endif 