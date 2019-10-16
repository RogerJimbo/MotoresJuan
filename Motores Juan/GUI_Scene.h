#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Scene : public GUI_Element
{
public:
	GUI_Scene(Application* app, bool start_enabled = true);
	~GUI_Scene();

	void Draw(bool* open);
	void DrawGrid(int GridSize);
	void DrawAxis(bool active);

	bool active_grid = true;
	bool active_axis = true;

	float grid_size;
};

