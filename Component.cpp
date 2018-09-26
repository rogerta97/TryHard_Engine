#include "Component.h"
#include "GameObject.h"

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

GameObject * Component::GetGameObject() const
{
	return gameobject;
}

void Component::SetGameObject(GameObject * new_go)
{
	gameobject = new_go; 
}
