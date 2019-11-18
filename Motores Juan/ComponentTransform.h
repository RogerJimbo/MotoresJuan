#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	void SetLocalMatrix();
	void SetGlobalMatrix(GameObject* GO);

	float4x4 GetLocalMatrix();
	float4x4 GetGlobalMatrix();

public:
	float3 position;
	Quat rotation;
	float3 scale;

	float4x4 local_matrix;
	float4x4 global_matrix;
};
#endif // ! __COMPONENT_TRANSFORM_H__