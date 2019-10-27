#pragma once
#include "GUI_Element.h"

class GameObject;
class ComponentMesh;

class GUI_Inspector : public GUI_Element
{
public:
	GUI_Inspector(Application* app, bool start_enabled = true);
	~GUI_Inspector();


	void Draw(bool* open);

public:
	GameObject* selectedGO = nullptr;
	ComponentMesh* meshes = nullptr;

	int numChildren = 0;

};

