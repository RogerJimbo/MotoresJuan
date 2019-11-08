#pragma once
#include "Component.h"

class ComponentCamera : public Component
{

public:
	ComponentCamera(GameObject* gameobject);
	~ComponentCamera();

	bool Update();

	void CreateFrustrum();
	void SetAspectRatio();

public:

	

};

