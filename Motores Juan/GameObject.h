#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include <list>
#include <string>
#include <vector>
#include <map>

struct Mesh;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

using namespace std;

class GameObject
{
public:
	GameObject(GameObject* parent, string name);
	~GameObject();

	void Update();

	Component* CreateComponent(Component_Type comp_type);
	uint GetUID() const;
	const GameObject* GetChild(string name);
	Component* GetComponent(Component_Type comp_type);

private:
	string name;
	uint uid = 0;

	GameObject* parent;

	list<Component*> components;
	vector<GameObject*> children;
};

#endif 