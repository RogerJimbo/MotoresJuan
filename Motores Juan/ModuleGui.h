#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "ImGui/imgui.h"

enum EngineWindows {HIERARCHY, CONSOLE,  INSPECTOR, CONFIG, ABOUT, HARDWARE, NUM_ACT_WIN};

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void CreateAboutWindow();
	void CreateConfigWindow();
	void CreateHardwareWindow();

	float horizontalFactor = (float)SCREEN_WIDTH * SCREEN_SIZE / SCREEN_WIDTH;
	float verticalFactor = (float)SCREEN_HEIGHT * SCREEN_SIZE / SCREEN_HEIGHT;
	
private:

	bool active_engine_windows[NUM_ACT_WIN];
	bool close_tab = true;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool full_desktop = false;

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

	SDL_bool resize_true = SDL_TRUE;
	SDL_bool resize_false = SDL_FALSE;
	
};

#endif // !__MODULEGUI_H__