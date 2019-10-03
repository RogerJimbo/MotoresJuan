#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Hierarchy : public GUI_Element
{
public:
	GUI_Hierarchy(Application* app);
	~GUI_Hierarchy();

	void Draw();
};