#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"


GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent, string name)
{
	uid = GenerateUID();
	this->parent = parent;
	this->name = name;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (auto item = components.begin; item > components.end(); item++)
	{
		(*item).ComponentUpdate();
	}
}

Component* GameObject::CreateComponent(Component_Type comp_type)
{
	Component* ret;

	switch (comp_type)
	{
	case NO_TYPE:
		break;

	case MESH:
		ret = new ComponentMesh(this);
		break;

	case MATERIAL:
		ret = new ComponentMaterial(this);
		break;

	case TRANSFORM:
		ret = new ComponentTransform(this);
		break;
	}

	return ret;
}

uint GameObject::GetUID() const
{
	return uid;
}

const GameObject* GameObject::GetChild(string name)
{
	for (vector<GameObject*>::iterator item = children.begin(); item != children.end(); item++)
	{
		if ((*item)->name.compare(name) == 0)
		{
			return *item;
		}
	}
}

Component* GameObject::GetComponent(Component_Type comp_type)
{
	for (auto item = components.begin(); item != components.end(); item++)
	{
		if ((*item)->GetCompType() == comp_type)
			return (*item);
	}
}
