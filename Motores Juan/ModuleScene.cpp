#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleOwnFileSystem.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "ImGuizmos/ImGuizmo.h"
#include "ParShapes/par_shapes.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled) { config_name = "Scene"; }
ModuleScene::~ModuleScene() {}

bool ModuleScene::Init(const JSON_Object& config) { return true; }

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "Root");
	App->loader->Import("BakerHouse.fbx", nullptr);

	return true;
}

void ModuleScene::Draw()
{
	vector<GameObject*> GOs = App->modscene->root->children;
	for (auto item = GOs.begin(); item != GOs.end(); item++)
	{
		for (auto child = (*item)->children.begin(); child != (*item)->children.end(); ++child)
		{
			for (auto iter = (*child)->components.begin(); iter != (*child)->components.end(); ++iter)
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

				if ((*item)->is_selected)
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
	}

	if (App->modscene->object_selected != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_IDLE)
		{
			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) operation = ImGuizmo::STATIC;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) operation = ImGuizmo::TRANSLATE;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) operation = ImGuizmo::ROTATE;
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) operation = ImGuizmo::SCALE;
		}
	}
}

void ModuleScene::CreateCamera()
{
	GameObject* gameobject = App->modscene->root->AddChildren("Camera");
	App->modscene->gameobjects.push_back(gameobject);
	gameobject->name = "Camera";

	ComponentCamera* camera = (ComponentCamera*)gameobject->AddComponent(CAMERA);
	ComponentTransform* transformations = (ComponentTransform*)gameobject->AddComponent(TRANSFORM);

	LOG("Game Object Camera Created.");
}

void ModuleScene::CreateEmpty()
{
	GameObject* gameobject = App->modscene->root->AddChildren("Game Object");
	App->modscene->gameobjects.push_back(gameobject);
	gameobject->name = "Game Object";

	ComponentTransform* empty = (ComponentTransform*)gameobject->AddComponent(TRANSFORM);
	LOG("New Empty Game Object.")
}

void ModuleScene::CreatePrimitives(par_shapes_mesh_s* data, char* type)
{
	GameObject* gameobject = App->modscene->root->AddChildren(type);
	App->modscene->gameobjects.push_back(gameobject);

	//GameObject* newGO = new GameObject();
	gameobject->name = type;
	//gameobject->children.push_back(newGO);

	ComponentMesh* primitive = (ComponentMesh*)gameobject->AddComponent(MESH);
	primitive->num_vertices = data->npoints * 3;
	primitive->vertices = new float[primitive->num_vertices * 3];
	for (int i = 0; i < primitive->num_vertices; ++i) primitive->vertices[i] = data->points[i];

	primitive->num_indices = data->ntriangles * 3;
	primitive->indices = new uint[primitive->num_indices];
	for (int i = 0; i < primitive->num_indices; i++) primitive->indices[i] = (uint)data->triangles[i];

	LOG("%s Primitive created with %d vertices and %d indices.", type, primitive->num_vertices, primitive->num_vertices);

	glGenBuffers(1, (GLuint*) & (primitive->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * primitive->num_indices, primitive->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) & (primitive->id_texcoords));
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, primitive->id_texcoords);
	glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(uint) * primitive->num_vertices * 2, primitive->texture_coords, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);

	//App->modscene->gameobjects.push_back(newGO);
}

void ModuleScene::SelectGameObject(GameObject* gameobject)
{
	object_selected = gameobject;
	if (gameobject != nullptr) { gameobject->is_selected = true; }
}

void ModuleScene::Guizmos(ImGuizmo::OPERATION operation)
{
	ComponentTransform* transform = (ComponentTransform*)object_selected->GetComponent(TRANSFORM);

	if (transform)
	{
		ImGuizmo::Enable(object_selected);
		if (operation == ImGuizmo::STATIC) { ImGuizmo::Enable(false); }

		float mouse_position_X = App->input->GetMouseX();
		float mouse_position_Y = App->input->GetMouseY();

		float window_size_X = ImGui::GetWindowSize().x;
		float window_size_Y = ImGui::GetWindowSize().y;

		ImGuizmo::SetRect(mouse_position_X, mouse_position_Y, window_size_X, window_size_Y);

		if (App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) { operation = ImGuizmo::TRANSLATE; }
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) { operation = ImGuizmo::ROTATE; }
			if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) { operation = ImGuizmo::SCALE; }
		}

		ImGuizmo::SetOrthographic(false);
	}
}

bool ModuleScene::CleanUp() { return true; }
void ModuleScene::Save_Config(JSON_Object& config) const { }
