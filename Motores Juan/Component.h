#ifndef __COMPONENT_H__
#define __COMPONENT_H__

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

#endif // !__COMPONENT_H__
