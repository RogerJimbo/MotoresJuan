#include "Component.h"



Component::Component()
{
}

Component::Component(Component_Type type, GameObject* parent)
{

}

Component::~Component()
{
}

uint Component::GetUID()
{
	return uid;
}

Component_Type Component::GetCompType()
{
	return c_type;
}

void Component::ComponentUpdate()
{
}
