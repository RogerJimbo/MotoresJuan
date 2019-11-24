#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include "MathGeoLib/MathGeoLib.h"

using namespace std;

class GameObject;

class Quadtree
{
public:

	Quadtree(AABB limits);
	Quadtree();
	~Quadtree();

	void Insert(GameObject* gameobject);
	void Split();
	void Intersect(vector<GameObject*> gameobjects);

public:
	AABB* tree_BB;

	vector<GameObject*> objects;

	Quadtree* children[4];

	int max_GO = 0;
	int splits = 0;
	int max_splits = 0;
};

#endif // !__QUADTREE_H__

