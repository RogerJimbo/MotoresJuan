#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include <string>
#include <map>

struct Mesh;

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;


class GameObject
{
public:
	GameObject();
	~GameObject();
};
#endif 
