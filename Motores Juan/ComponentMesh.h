#include "Component.h"

class ComponentMaterial;

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

public:
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_texCoords = 0;
	uint num_texCoords = 0;
	float* texCoords = nullptr;

};

