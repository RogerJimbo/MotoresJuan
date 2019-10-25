#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include <string>
#include <vector>
#include <map>
#include "Globals.h"

struct Mesh;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, string name);
	~GameObject();

	Component* CreateComponent(Component_Type comp_type);

private:
	string name;
	uint uid = 0;

	GameObject* parent;

	list<Component*> components;
	vector<GameObject*> children;

private:
	void Update();
};
#endif 
