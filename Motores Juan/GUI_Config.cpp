#include "GUI_Config.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"

GUI_Config::GUI_Config(Application* app, bool start_enabled) : GUI_Element(app,start_enabled) { elem_name = "Configuration"; }

GUI_Config::~GUI_Config() {}

void GUI_Config::Draw(bool* open)
{
	ImGui::Begin("Configuration", open);
	{
		if (ImGui::CollapsingHeader("Application"))		//Application Configuration
		{
			static float sf1 = 60.0f;
			string title_fps, title_ms;
			//Calculate FPS
			ImGui::Text("Limit Framerate: ");
			for (int i = 99; i >= 0; --i)
			{
				if (i != 0) { fps[i] = fps[i - 1]; }
				else { fps[i] = ImGui::GetIO().Framerate; fps[i] -= fps[i] - sf1; }
			}
			//Calculate ms
			for (int i = 99; i >= 0; --i)
			{
				if (i != 0) { ms[i] = ms[i - 1]; }
				else { ms[i] = App->milliseconds;}
			}
			//App Name and Organitzation
			static char app_name[30] = "Motores Juan";
			static char org[30] = "UPC CITM";

			ImGui::InputText("App Name", app_name, sizeof(app_name));
			ImGui::InputText("Organitzation", org, sizeof(org));

			//Max FPS
			ImGui::SliderFloat("Max FPS", &sf1, 0.0f, 100.0f, "%.f FPS");

			// Histograms
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
			ImGui::Text("Refresh Rate: %.3f ms/frame", ImGui::GetIO().Framerate);

			if (ImGui::SliderFloat("Brightness", &bright_slider_scroll, 0.0f, 1.0f)) { App->window->SetLightSlider(bright_slider_scroll); }
			if (ImGui::SliderInt("Width", &width_slider_scroll, 640, 2048) && resizable) { App->window->SetWindowSize(width_slider_scroll, height_slider_scroll); }
			if (ImGui::SliderInt("Height", &height_slider_scroll, 480, 1536) && resizable) { App->window->SetWindowSize(width_slider_scroll, height_slider_scroll); }
			if (ImGui::Checkbox("FullScreen", &fullscreen)) { fullscreen ? App->window->SetFullscreen(true) : App->window->SetFullscreen(false); } 	ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable)) { resizable ? App->window->SetResizable(true) : App->window->SetResizable(false); }
			if (ImGui::Checkbox("Borderless", &borderless)) { borderless ? App->window->SetBorderless(true) : App->window->SetBorderless(false); } ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fulldesktop)) { fulldesktop ? App->window->SetFullDesktop(true) : App->window->SetFullDesktop(false); }
		}

		if (ImGui::CollapsingHeader("Renderer")) 
		{
			ImGui::Checkbox("Axis Active", &App->scene->active_axis);
			ImGui::Checkbox("Grid Active", &App->scene->active_grid);
			//ImGui::SliderInt("Grid Size", App->scene->grid_size, 0, 500);
			ImGui::Checkbox("Lighting", &App->renderer3D->lightning);
			ImGui::Checkbox("Lights Abled", &App->renderer3D->lights_on);
			ImGui::Checkbox("Backface Culling", &App->renderer3D->backface);

			if (ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe)) { App->renderer3D->ActivateWireframe(); }

			ImGui::Checkbox("Texture", &none);
			ImGui::Checkbox("Bounding Box", &none);
			ImGui::Checkbox("Triangle Normals", &none);
			ImGui::Checkbox("Face Normals", &none);
		}

		if (ImGui::CollapsingHeader("Input Info")) { ImGui::Text("Mouse Position: X:%d , Y:%d ", App->input->GetMouseX(), App->input->GetMouseY()); }

		ImGui::End();
	}
}	
