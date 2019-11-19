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

GameObject::GameObject() 
{
	BoundingBox.SetNegativeInfinity();
}

GameObject::GameObject(GameObject* parent, string name)
{
	this->parent = parent;
	this->name = name;

	BoundingBox.SetNegativeInfinity();
}

GameObject::~GameObject() {}

void GameObject::Update()
{
	for (list<Component*>::iterator iter = components.begin(); iter != components.end(); ++iter) { (*iter)->Update(); }
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

			if (App->modscene->object_selected)
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
			

				//obb = mesh->GetBoundingBox();
				//obb.Transform(GetComponent<C_Transform>()->GetGlobalTransform());
				/*ComponentTransform* transform = (ComponentTransform*)this->GetComponent(TRANSFORM);
				obb = this->GetBoundingBox();
				obb.Transform(transform->GetGlobalMatrix());

				aabb.SetNegativeInfinity();
				aabb.Enclose(obb);*/
				
				App->renderer3D->DrawBoundingBox(App->modscene->object_selected->BoundingBox); 
				
			}
		}

		for (auto iter = children.begin(); iter != children.end(); ++iter)
			BoundingBox.Enclose((*iter)->BoundingBox);
	}

	for (auto iter = children.begin(); iter != children.end(); ++iter) { (*iter)->Draw(); }
}

AABB GameObject::GetBoundingBox()
{
	return this->BoundingBox;
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

void GameObject::CalculateAABB()
{
	if (ComponentMesh* mesh = (ComponentMesh*)this->GetComponent(MESH))
	{
		BoundingBox.Enclose((float3*)mesh->vertices, mesh->num_vertices);
	}
	for (auto item = this->children.begin(); item != this->children.end(); item++)
	{
		BoundingBox.Enclose((*item)->BoundingBox);
		(*item)->CalculateAABB();
	}
}

void GameObject::RecursiveHierarchy()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (App->modscene->object_selected == this) { node_flags |= ImGuiTreeNodeFlags_Selected; }

	bool opened = ImGui::TreeNodeEx(this->name.c_str(), node_flags);

	if (ImGui::IsItemClicked())
	{
		App->modscene->object_selected = this;
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

void GameObject::ShowInspectorInfo()
{
	ImGui::Text(this->name.c_str());
	ImGui::Separator();
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Transform"))
	{
		float pos[3] = { this->pos.x, this->pos.y, this->pos.z };
		float rot[3] = { this->rot.x, this->rot.y, this->rot.z };
		float scale[3] = { this->scale.x, this->scale.y, this->scale.z };
	
		ImGui::DragFloat3("Position", pos, 0.25f);
		ImGui::SliderFloat3("Rotation", rot, 0.0f, 360.0f);
		ImGui::DragFloat3("Scale", scale, 0.25f, 1.0f, 1000.0f);
	
		ImGuiIO& io = ImGui::GetIO();
		io.WantCaptureKeyboard;
	}
	
	if (ImGui::CollapsingHeader("Mesh"))
	{
		meshes = new ComponentMesh();
		for (auto iter = this->children.begin(); iter != this->children.end(); ++iter)
		{
			ComponentMesh* auxmesh = (ComponentMesh*)(*iter)->GetComponent(MESH);
			meshes->num_vertices += auxmesh->num_vertices;
		}
	
		ImGui::Text("Number of vertices: %d", meshes->num_vertices);
		ImGui::Text("Number of children: %d", this->children.size());
	}
	
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture Width: %.01f", App->loader->TextureSize.x);
		ImGui::Text("Texture Height: %.01f", App->loader->TextureSize.y);
		ImGui::Text("Path: MotoresJuan/Game/%s", App->loader->path.c_str());
	}

	if (ImGui::Checkbox("Active", &this->active))
	{

	}
	ImGui::SameLine();

	ImGui::Checkbox("Draw AABB", &this->box);

}