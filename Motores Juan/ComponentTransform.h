#ifndef  __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(Component_Type type, GameObject* parent);
	~ComponentTransform();

	void SetLocalMatrix();
	void SetGlobalMatrix(GameObject* GO);

	float4x4 GetLocalMatrix() { return local_matrix; }
	float4x4 GetGlobalMatrix() { return global_matrix; }

	float3 getPosition() { return position; }
	float3 getRotation() { return rotation.ToEulerXYX(); }
	float3 getScale() { return scale; }

	void setPosition(float3 &position);
	void setRotation(float3& rotation);
	void setScale(float3& scale);

public:
	float3 position;
	Quat rotation;
	float3 scale;

	float4x4 local_matrix;
	float4x4 global_matrix;
};
#endif // ! __COMPONENT_TRANSFORM_H__