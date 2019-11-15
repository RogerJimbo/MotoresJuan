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

}

GameObject::GameObject(GameObject* parent, string name)
{
	this->parent = parent;
	this->name = name;
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
			
				BoundingBox.SetNegativeInfinity();
				BoundingBox.Enclose((float3*)mesh->vertices, mesh->num_vertices);	

				//obb = mesh->GetBoundingBox();
				//obb.Transform(GetComponent<C_Transform>()->GetGlobalTransform());

				if (!App->renderer3D->boundingbox) { DrawBoundingBox(BoundingBox); }
			}
		}

		for (auto iter = children.begin(); iter != children.end(); ++iter)
			BoundingBox.Enclose((*iter)->BoundingBox);
	}

	for (auto iter = children.begin(); iter != children.end(); ++iter) { (*iter)->Draw(); }
}

void GameObject::DrawBoundingBox(const AABB& boundingbox)
{
	glLineWidth(2.f);
	glColor3f(255,255, 255);

	glBegin(GL_LINES);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(0).x, boundingbox.CornerPoint(0).y, boundingbox.CornerPoint(0).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(3).x, boundingbox.CornerPoint(3).y, boundingbox.CornerPoint(3).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(2).x, boundingbox.CornerPoint(2).y, boundingbox.CornerPoint(2).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(6).x, boundingbox.CornerPoint(6).y, boundingbox.CornerPoint(6).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(1).x, boundingbox.CornerPoint(1).y, boundingbox.CornerPoint(1).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(4).x, boundingbox.CornerPoint(4).y, boundingbox.CornerPoint(4).z);

	glVertex3f(boundingbox.CornerPoint(5).x, boundingbox.CornerPoint(5).y, boundingbox.CornerPoint(5).z);
	glVertex3f(boundingbox.CornerPoint(7).x, boundingbox.CornerPoint(7).y, boundingbox.CornerPoint(7).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}

AABB GameObject::GetBoundingBox(GameObject* mesh)
{
	return mesh->BoundingBox;
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
			ComponentMesh*auxmesh = (ComponentMesh*)this->GetComponent(MESH);
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
}

void GameObject::SelectGO()
{
	/*if (ImGui::IsItemClicked(0))
	{
		if (App->modscene->object_selected != this)
		{
			if (App->modscene->object_selected != nullptr)
			{
				App->modscene->object_selected->is_selected = false;
				for (int i = 0; i < App->modscene->object_selected->children.size(); ++i) { App->modscene->object_selected->children[i]->SelectChildren(false); }
			}
			App->modscene->object_selected = this;
			App->modscene->object_selected->is_selected = true;
			for (int i = 0; i < App->modscene->object_selected->children.size(); ++i) { App->modscene->object_selected->children[i]->SelectChildren(true); }
		}
	}*/
}

void GameObject::DeselectGO()
{

}