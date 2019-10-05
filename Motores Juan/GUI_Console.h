#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Console: public GUI_Element
{
public:
	GUI_Console(Application* app, bool start_enabled = true);
	~GUI_Console();


private:
};
