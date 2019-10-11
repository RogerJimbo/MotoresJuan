#include "GUI_Console.h"
#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"

#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui\imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"

GUI_Console::GUI_Console(Application* app, bool start_enabled) : GUI_Element(app, start_enabled) { elem_name = "Console"; }

GUI_Console::~GUI_Console() {}

void GUI_Console::Draw(bool* open)
{
	if (ImGui::Begin(elem_name.c_str(), open))
	{
		if (ImGui::Button("Clear Console")) buffer.clear(); console_elems.clear();
		ImGui::BeginChild("Scrolling Bar");

		ImGui::TextUnformatted(buffer.begin());

		if (scrolling)	ImGui::SetScrollHere(1.0f);
		!scrolling;

		ImGui::EndChild();
	}
	ImGui::End();
}

void GUI_Console::CreateLog(const char* log)
{      
	int buffsize, newsize = buffer.size();
	buffer.appendf("%s \n", log);

	while (buffsize < newsize)
	{
		if (buffer[buffsize] == '\n') console_elems.push_back(buffsize);	
		buffsize++;
	}
	scrolling;
}



