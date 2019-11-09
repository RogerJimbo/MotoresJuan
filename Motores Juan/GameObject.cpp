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

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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

void GameObject::Draw()
{
	for (auto iter = components.begin(); iter != components.end(); ++iter)
	{
		if ((*iter)->c_type == MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*iter);

			glColor3f(1.0, 1.0, 1.0);

			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindTexture(GL_TEXTURE_2D, mesh->texture);
			glTexCoordPointer(2, GL_FLOAT, 0, &mesh->texture_coords[0]);

			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			if (is_selected)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glColor3f(1.0, 1.0, 1.0);

				glEnableClientState(GL_VERTEX_ARRAY);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
				glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0]);

				glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}

	for (auto iter = children.begin(); iter != children.end(); ++iter)
	{
		(*iter)->Draw();
	}
	
}

void GameObject::SelectChildren(bool selected)
{
	for (auto item = children.begin(); item != children.end(); ++item)
	{
		(*item)->is_selected = selected;
		(*item)->SelectChildren(selected);
	}
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
	
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (selectedGO == this)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool opened = ImGui::TreeNodeEx(this->name.c_str(), node_flags);

	if (ImGui::IsItemClicked(0))
	{
		if (selectedGO != this)
		{
			if (selectedGO != nullptr)
			{
				selectedGO->is_selected = false;
				for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->SelectChildren(false); }
			}
			selectedGO = this;
			selectedGO->is_selected = true;
			for (int i = 0; i < selectedGO->children.size(); ++i) { selectedGO->children[i]->SelectChildren(true); }
		}
	}

	if (opened)
	{
		for (auto item = children.begin(); item != children.end(); ++item)
		{
			(*item)->RecursiveHierarchy();
		}
		ImGui::TreePop();
	}
	

	
}