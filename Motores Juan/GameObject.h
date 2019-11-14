#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include <list>
#include <string>
#include <vector>
#include <map>
#include "glmath.h"

#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/Geometry/AABB.h"

struct Mesh;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, string name);
	~GameObject();

	void AddParent(GameObject* newparent, GameObject* child);
	Component* AddComponent(Component_Type comp_type);
	GameObject* AddChildren(std::string name);

	void Update();

	void Draw();
	void DrawBoundingBox(const AABB& BB);
	void SelectChildren(bool selected);

	Component* GetComponent(Component_Type comp_type);
	AABB GetBoundingBox(GameObject* mesh);

	void RecursiveHierarchy();

	void SelectGO();
	void DeselectGO();

public:
	string name;

	GameObject* parent;
	GameObject* selectedGO = nullptr;

	ComponentCamera* camera = nullptr;

	vec3 pos = { 0.0f, 0.0f, 0.0f };
	vec3 scale = { 0.0f, 0.0f, 0.0f };
	Quat rot = { 0.0f, 0.0f, 0.0f, 0.0f };

	AABB BoundingBox;
	AABB obb;
	AABB aabb;

	list<Component*> components;
	vector<GameObject*> children;

	bool is_selected = false;
};

#endif 