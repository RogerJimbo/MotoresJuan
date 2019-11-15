#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleOwnFileSystem.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

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
	camera = new ComponentCamera(nullptr);
	App->loader->Import("BakerHouse.fbx", nullptr);

	return true;
}

void ModuleScene::Draw()
{
	if (root) 
	{ 
		for (auto item = root->children.begin(); item != root->children.end(); ++item)
		{
			(*item)->Draw();
			if ((*item)->box)
			{
				App->renderer3D->DrawBoundingBox((*item)->GetBoundingBox());
			}
		}
	}

	root->selectedGO = MousePicking();	//TO UPDATE

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

	for (auto item = gameobjects.begin(); item != gameobjects.end(); ++item)
	{
		(*item)->Update();
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
	gameobject->name = type;

	ComponentMesh* primitive = (ComponentMesh*)gameobject->AddComponent(MESH);
	primitive->num_vertices = data->npoints * 3;
	primitive->vertices = new float[primitive->num_vertices * 3];
	for (int i = 0; i < primitive->num_vertices; ++i) primitive->vertices[i] = data->points[i];

	primitive->num_indices = data->ntriangles * 3;
	primitive->indices = new uint[primitive->num_indices];
	for (int i = 0; i < primitive->num_indices; i++) primitive->indices[i] = (uint)data->triangles[i];

	//todo roger
	App->renderer3D->DrawBoundingBox(gameobject->BoundingBox);

	LOG("%s Primitive created with %d vertices and %d indices.", type, primitive->num_vertices, primitive->num_vertices);

	glGenBuffers(1, (GLuint*) & (primitive->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * primitive->num_indices, primitive->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) & (primitive->id_texcoords));
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, primitive->id_texcoords);
	glBufferData(GL_TEXTURE_COORD_ARRAY, sizeof(uint) * primitive->num_vertices * 2, primitive->texture_coords, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);

}

void ModuleScene::SelectGameObject(GameObject* gameobject)
{
	object_selected = gameobject;
	if (gameobject != nullptr) { gameobject->is_selected = true; }
}


GameObject* ModuleScene::MousePicking()
{
	GameObject* ret = nullptr;

	//1st Calculate Mouse Position
	float mouseX = App->input->GetMouseX();
	float mouseY = App->input->GetMouseY();
	//2nd Convert to OpenGL Coordinates Normalized
	float windowX = App->window->window_width;
	float windowY = App->window->window_height;
	vec2 MouseCoords = vec2((mouseX * 2.0f / windowX - 1), -(mouseY * 2.0f / windowY - 1));
	//3rd Convert to Clip Coords 
	vec4 ClipCoords = vec4(MouseCoords.x, MouseCoords.y, -1.0f, 1.0f);
	//4th Convert to Eye Space 
	mat4x4 InvertedProjection = inverse(App->renderer3D->ProjectionMatrix);
	vec4 eyeCoords = InvertedProjection * ClipCoords;
	vec4 EyeSpace = vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	//5th Convert to World Space
	vec4 rayWorld = inverse(ViewMatrix) * eyeCoords;
	vec3 MouseRay = normalize(vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	mouseray = float3(MouseRay.x, MouseRay.y, MouseRay.z);

	float3 direction;	//Direction has to go acording to the camera

	LOG("%f %f %f", MouseRay.x, MouseRay.y, MouseRay.z);
	LOG("%f %f %f", mouseray.x, mouseray.y, mouseray.z);

	ray = camera->camera_frustum.UnProjectLineSegment(-MouseRay.x, -MouseRay.y);




	if (ray.Intersects(root->BoundingBox))
	{
		LOG("victory");
	}

	camera->DrawFrustrum();
	camera->DrawRay();


	Frustum* frustum;

	list<GameObject*> IntersectedGO;

	//for (auto iter = gameobjects.begin(); iter != gameobjects.end(); iter++)
	//{

	//	// if (ray.Intersects(App->modscene->root->BoundingBox)) {}
	//	//Make a recursive function to call all the children and check their AABB


	//}

	if (IntersectedGO.empty()) { return ret; }

	//Check closer game objects 
	//Check if it hits the mesh triangles


	return ret;
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
