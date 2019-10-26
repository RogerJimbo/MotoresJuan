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

	void AddParent(GameObject* newparent, GameObject* child);
	Component* AddComponent(Component_Type comp_type);
	GameObject* AddChildren(std::string name);

	void GetChild(vector<GameObject*> children);
	Component* GetComponent(Component_Type comp_type);

public:
	string name;

	GameObject* parent;

	list<Component*> components;
	vector<GameObject*> children;

private:
	void Update();
};
#endif 
