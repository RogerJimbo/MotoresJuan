#include "Quadtree.h"
#include "GameObject.h"


Quadtree::Quadtree()
{
}

Quadtree::Quadtree(AABB limits)
{
	tree_BB = new AABB(limits);
}

Quadtree::~Quadtree()
{
}

void Quadtree::Insert(GameObject* gameobject)
{
	if (gameobject != nullptr && tree_BB->Intersects(gameobject->BoundingBox))
	{
		if (children)
		{
			for (int i = 0; i < 4; i++)
			{
				children[i]->Insert(gameobject);
			}
		}
	}
}

void Quadtree::Split()
{

}

void Quadtree::Intersect(vector<GameObject*> gameobjects)
{

}