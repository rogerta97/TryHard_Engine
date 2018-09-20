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
	return CompType();
}

void Component::SetType(CompType new_type)
{
}
