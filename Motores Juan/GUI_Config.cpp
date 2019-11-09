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
	ImGui::Begin(elem_name.c_str(), open);
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
			ImGui::Checkbox("Lighting", &App->renderer3D->lightning);
			ImGui::Checkbox("Lights Abled", &App->renderer3D->lights_on);
			ImGui::Checkbox("Backface Culling", &App->renderer3D->backface);
			ImGui::Checkbox("Textured", &App->renderer3D->textured);
			ImGui::Checkbox("Bounding Box", &App->renderer3D->boundingbox);
			if (ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe)) { App->renderer3D->ActivateWireframe(); }

			ImGui::Separator();
			ImGui::SliderInt("Grid Size", &App->scene->grid_size, 0, 100);
		}

		if (ImGui::CollapsingHeader("Global Camera"))
		{
			ImGuiIO& io = ImGui::GetIO();
			float aspect_ratio = io.DisplaySize.x / io.DisplaySize.y;

			static int fov_slider = 0;
			static int near_plane = 0.1f;
			static int far_plane = 1000.0f;

			if (ImGui::SliderInt("FOV", &fov_slider, 1, 100)) { /* Insert function to handle FOV*/ }
			ImGui::SliderInt("Near Plane", &near_plane, 1, 100);
			ImGui::SliderInt("Far Plane", &far_plane, 1, 1000);

			ImGui::Text("Camera Position:");
			ImGui::Text("X: "); ImGui::SameLine();
			ImGui::Text(to_string(App->camera->cameraPos.x).c_str());
			ImGui::Text("Y: "); ImGui::SameLine();
			ImGui::Text(to_string(App->camera->cameraPos.y).c_str());
			ImGui::Text("Z: "); ImGui::SameLine();
			ImGui::Text(to_string(App->camera->cameraPos.z).c_str());

			//App->camera->FieldOfView(fov_slider, aspect_ratio, 0.0f, 100.0f, );
		}

		if (ImGui::CollapsingHeader("Input Info")) { ImGui::Text("Mouse Position: X:%d , Y:%d ", App->input->GetMouseX(), App->input->GetMouseY()); }

		ImGui::End();
	}
}	

void GUI_Config::Save_Config(JSON_Object& config) const
{
	json_object_set_boolean(&config, "Axis Active", App->scene->active_axis);
	json_object_set_boolean(&config, "Grid Active", App->scene->active_grid);
	json_object_set_boolean(&config, "Lightning", App->renderer3D->lightning);
	json_object_set_boolean(&config, "Lights Abled", App->renderer3D->lights_on);
	json_object_set_boolean(&config, "Backface Culling", App->renderer3D->backface);
	json_object_set_boolean(&config, "Textured", App->renderer3D->textured);
	json_object_set_boolean(&config, "Wireframe", App->renderer3D->wireframe);

	json_object_set_number(&config, "Grid Size", App->scene->grid_size);
}

void GUI_Config::Load_Config(JSON_Object& config)	//roger todo
{
	App->scene->active_axis = json_object_get_boolean(&config, "Axis Active");
	App->scene->active_grid = json_object_get_boolean(&config, "Grid Active");
	App->renderer3D->lightning = json_object_get_boolean(&config, "Lightning");
	App->renderer3D->lights_on = json_object_get_boolean(&config, "Lights Abled");
	App->renderer3D->backface = json_object_get_boolean(&config, "Backface Culling");
	App->renderer3D->textured = json_object_get_boolean(&config, "Textured");
	App->renderer3D->wireframe = json_object_get_boolean(&config, "Wireframe");
	App->scene->grid_size = json_object_get_number(&config, "Grid Size");

}