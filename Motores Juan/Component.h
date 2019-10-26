#pragma once
#include "Globals.h"

class GameObject;

class Component
{
public:
	Component(GameObject* parent);
	~Component();

	Component_Type GetCompType();
	uint GetUID();

	virtual void ComponentUpdate();

public:
	uint uid = 0;
	Component_Type c_type;
};
