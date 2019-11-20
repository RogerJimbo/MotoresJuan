#pragma once
#include "Component.h"
#include "MathGeoLib/Geometry/Frustum.h"

class ComponentCamera : public Component
{

public:
	ComponentCamera(Component_Type type, GameObject* parent);
	~ComponentCamera();

	bool Update();

	void DrawFrustrum();
	void DrawRay();
	void SetAspectRatio();

public:

	Frustum camera_frustum;
	float3 frustumpos;

};

