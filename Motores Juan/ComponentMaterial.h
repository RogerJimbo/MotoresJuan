#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();
};

