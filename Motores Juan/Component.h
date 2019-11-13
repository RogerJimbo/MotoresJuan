#pragma once
#include "Globals.h"

class GameObject;

class Component
{
public:
	Component();
	Component(Component_Type type, GameObject* parent);
	~Component();

	Component_Type GetCompType();

	virtual bool Update();

public:
	Component_Type c_type;

	GameObject* parent;
};
