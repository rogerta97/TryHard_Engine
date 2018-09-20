#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

bool Component::Start()
{
	return false;
}

bool Component::Update()
{
	return false;
}

CompType Component::GetType() const
{
	return component_type;
}

void Component::SetType(CompType new_type)
{
	component_type = new_type; 
}
