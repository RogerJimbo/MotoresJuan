#include "GUI_Config.h"

#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"


GUI_Config::GUI_Config(Application* app, bool start_enabled) : GUI_Element(app,start_enabled) { config_name = "Configuration"; }

GUI_Config::~GUI_Config() {}

void GUI_Config::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin(config_name.c_str(), &show);

	if (ImGui::CollapsingHeader("Application"))		// FPS Histograms 
	{
		//Calculate FPS
		string title_fps;
		string title_ms;

		ImGui::Text("Limit Framerate: ");
		for (int i = 99; i >= 0; --i)
		{
			if (i != 0) { fps[i] = fps[i - 1]; }
			else { fps[i] = ImGui::GetIO().Framerate; }
		}

		for (int i = 99; i >= 0; --i)
		{
			if (i != 0) { ms[i] = ms[i - 1]; }
			else { ms[i] = SDL_GetTicks(); }
		}
		//App Name and Organitzation
		static char app_name[30] = "Motores Juan";
		static char org[30] = "UPC CITM";

		ImGui::InputText("App Name", app_name, sizeof(app_name));
		ImGui::InputText("Organitzation", org, sizeof(org));

		//Max FPS
		static float sf1 = 60.0f;
		ImGui::SliderFloat("Max FPS", &sf1, 0.0f, 100.0f, "%.f FPS");

		//Memory

		//Histograms
		title_fps = "Framerate " + std::to_string(fps[0]);
		title_ms = "Milliseconds " + std::to_string(ms[0]);
		ImGui::PlotHistogram("", fps, 100, 0, title_fps.c_str(), 0.f, 50.f, ImVec2(0, 80));
		ImGui::PlotHistogram("", ms, 100, 0, title_ms.c_str(), 0.f, 50.f, ImVec2(0, 80));
	}
	if (ImGui::CollapsingHeader("Window"))	//Window Configuration
	{
		static bool active = false;
		static int width_slider_scroll, height_slider_scroll = 0;

		ImGui::Checkbox("Active", &active);
		if (ImGui::SliderFloat("Brightness", &bright_slider_scroll, 0.0f, 1.0f)) { SDL_SetWindowBrightness(App->window->window, bright_slider_scroll); }

		if (ImGui::SliderInt("Width", &width_slider_scroll, 640, 2048) && resizable)
			SDL_SetWindowSize(App->window->window, width_slider_scroll, height_slider_scroll);

		if (ImGui::SliderInt("Height", &height_slider_scroll, 480, 1536) && resizable)
			SDL_SetWindowSize(App->window->window, width_slider_scroll, height_slider_scroll);

		ImGui::Text("Refresh Rate: %.3f ms/frame", ImGui::GetIO().Framerate);

		if (ImGui::Checkbox("FullScreen", &fullscreen))
		{
			if (fullscreen) { App->window->SetFullscreen(true); }
			else { App->window->SetFullscreen(false); }
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &resizable))
		{
			if (resizable) { resizable; SDL_SetWindowResizable(App->window->window, (SDL_TRUE)); }
			else {
				!resizable; SDL_SetWindowResizable(App->window->window, (SDL_FALSE));
			}
		}

		if (ImGui::Checkbox("Borderless", &borderless))
		{
			if (borderless) { SDL_SetWindowBordered(App->window->window, SDL_FALSE); !borderless; }
			else { SDL_SetWindowBordered(App->window->window, SDL_TRUE); }
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Full Desktop", &fulldesktop))
		{
			if (fulldesktop) SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			else	SDL_SetWindowFullscreen(App->window->window, 0);
		}
	}

	if (ImGui::CollapsingHeader("Input Info"))
	{
		ImGui::Text("Mouse Position: X:%d , Y:%d ", App->input->GetMouseX(), App->input->GetMouseY());
	}
	ImGui::End();
}