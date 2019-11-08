#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GameObject;

class GUI_Hierarchy : public GUI_Element
{
public:
	GUI_Hierarchy(Application* app, bool start_enabled = true);
	~GUI_Hierarchy();

	update_status Update(float dt);

	void Draw(bool* open);

public:
	bool createCube = false;
	bool createSphere = false;
	bool createPlane = false;

private:

	int numberPrimitives = 0;
	const char* prim_name;

	int prim_posx, prim_posy, prim_posz;
	int prim_sizeX, prim_sizeY;

};

