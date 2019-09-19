#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "ImGui/imgui.h"

class ModuleGui : public Module
{
public:
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool imgui_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

};

#endif // !__MODULEGUI_H__