#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Scene : public GUI_Element
{
public:
	GUI_Scene(Application* app);
	~GUI_Scene();

	void Draw();
};

