#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

public:
	float3 position;
	float3 rotation;
	float3 scale;

	float4x4 local_matrix;
	float4x4 global_matrix;
};
#endif // ! __COMPONENT_TRANSFORM_H__