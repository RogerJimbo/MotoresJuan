#include "GUI_Hierarchy.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/MathGeoLib.h"



GUI_Hierarchy::GUI_Hierarchy(Application* app) : GUI_Element(app)
{
	name = "Hierarchy";
}


GUI_Hierarchy::~GUI_Hierarchy()
{
}

void GUI_Hierarchy::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400));

	ImGui::Begin(name.c_str(), &show);

	ImGui::End();
}
