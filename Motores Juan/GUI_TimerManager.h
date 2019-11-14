#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_TimerManager : public GUI_Element
{
public:

	GUI_TimerManager(Application* app, bool start_enabled = true);
	~GUI_TimerManager();

	void Draw(bool* open);


};

