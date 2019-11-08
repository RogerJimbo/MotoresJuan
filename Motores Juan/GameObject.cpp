#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

GameObject::GameObject() {}

GameObject::GameObject(GameObject* parent, string name)
{
	this->parent = parent;
	this->name = name;
}

GameObject::~GameObject() {}

void GameObject::Update()
{
	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter) { (*iter)->ComponentUpdate(); }
}

GameObject * GameObject::AddChildren(std::string name)
{
	GameObject* ret = new GameObject(this, name);
	children.push_back(ret);
	return ret;
}

void GameObject::AddParent(GameObject* newparent, GameObject* child)
{
	if (newparent != parent) child->parent = newparent;
}

Component* GameObject::AddComponent(Component_Type comp_type)
{
	Component* ret;

	switch (comp_type)
	{
	case NO_TYPE:
		break;

	case MESH:
		ret = new ComponentMesh(comp_type, this);
		components.push_back(ret);
		break;

	case MATERIAL:
		ret = new ComponentMaterial(this);
		components.push_back(ret);
		break;

	case TRANSFORM:
		ret = new ComponentTransform(this);
		components.push_back(ret);
		break;

	case CAMERA:
		ret = new ComponentCamera(this);
		components.push_back(ret);
		break;
	}

	return ret;
}

Component* GameObject::GetComponent(Component_Type comp_type)
{
	for (auto item = components.begin(); item != components.end(); item++)
		if ((*item)->GetCompType() == comp_type) return (*item);	
}

void GameObject::RecursiveHierarchy()
{
	for (auto item = children.begin(); item != children.end(); ++item)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (selectedGO == (*item))
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool opened = ImGui::TreeNodeEx((*item)->name.c_str(), node_flags);

		if (ImGui::IsItemClicked(0))
		{
			if (selectedGO != (*item))
			{
				if (selectedGO != nullptr)
				{
					selectedGO->is_selected = false;
					for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->is_selected = false; }
				}
				selectedGO = (*item);
				selectedGO->is_selected = true;
				for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->is_selected = true; }
			}
		}

		if (opened)
		{
			(*item)->RecursiveHierarchy();
			ImGui::TreePop();
		}
	}
}