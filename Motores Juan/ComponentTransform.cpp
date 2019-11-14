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