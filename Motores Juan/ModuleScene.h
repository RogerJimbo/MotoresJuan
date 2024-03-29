#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include <vector>
#include "ModuleLoader.h"
#include "Module.h"
#include "GameObject.h"

#include "ImGuizmos/ImGuizmo.h"
#include "MathGeoLib/Geometry/LineSegment.h"

#define GRIDSIZE 50

struct par_shapes_mesh_s;

using namespace std;

struct RayHit
{
	RayHit(float distance, GameObject* obj) : distance(distance), obj(obj) {};
	float distance = 0.0f;
	GameObject* obj = nullptr;
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init(const JSON_Object& config);
	bool Start();
	bool CleanUp();

	void Save_Config(JSON_Object& config) const;
	void Draw();

	void CreatePrimitives(par_shapes_mesh_s* data, char* type);
	void CreateCamera();
	void CreateEmpty();

	void GameObjectsToDraw();

	GameObject* MousePicking();

	void Guizmos(ImGuizmo::OPERATION operation);
	void SelectGameObject(GameObject* gameobject);

	uint texture;
	string config_name;
	
	vector<uint> indices;
	vector<vec3> vertexs;
	vector<Mesh*> mesh;

	GameObject* root;
	GameObject* object_selected = nullptr;
	ComponentCamera* camera = nullptr;

	ImGuizmo::OPERATION operation = ImGuizmo::STATIC;
	ImGuizmo::MODE mode = ImGuizmo::LOCAL;

	vector<GameObject*> drawable_GOs;
	list<GameObject*>	gameobjects;

	LineSegment ray;
	float3 mouseray;

private:
	mat4x4 ViewMatrix;
};
#endif