#pragma once
#include "GUI_Element.h"
#include "SDL/include/SDL.h"

class GameObject;

class GUI_Hierarchy : public GUI_Element
{
public:
	GUI_Hierarchy(Application* app, bool start_enabled = true);
	~GUI_Hierarchy();

	void Draw(bool* open);

public:
	bool createCube = false;
	bool createSphere = false;
	bool createPlane = false;

	GameObject* selectedGO = nullptr;

private:
	int numberPrimitives = 0;
	const char* prim_name;
};

