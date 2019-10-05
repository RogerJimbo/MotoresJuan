#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GUI_Console: public GUI_Element
{
public:
	GUI_Console(Application* app, bool start_enabled = true);
	~GUI_Console();

	bool Init(const JSON_Object& config);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw();
private:
};
