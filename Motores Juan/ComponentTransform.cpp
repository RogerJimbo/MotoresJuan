#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* parent) 
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };

	local_matrix.SetIdentity();
	global_matrix.SetIdentity();
}

ComponentTransform::~ComponentTransform() 
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 1.0f };

	local_matrix.SetIdentity();
	global_matrix.SetIdentity();
}

void ComponentTransform::SetLocalMatrix()
{
	this->local_matrix.Set(float4x4::FromTRS(position, rotation, scale));
}

void ComponentTransform::SetGlobalMatrix(GameObject* GO)
{
	for (auto item = GO->children.begin(); item != GO->children.end(); ++item)
	{
		ComponentTransform* transform = (ComponentTransform*)GO->GetComponent(TRANSFORM);
		if (transform != nullptr)
		{
			if (GO->parent == nullptr)
			{
				transform->global_matrix = transform->local_matrix;
			}
			else
			{
				transform->global_matrix = ((ComponentTransform*)GO->parent->GetComponent(TRANSFORM))->global_matrix*transform->local_matrix;
			}
		}
		SetGlobalMatrix(*item);
	}
}