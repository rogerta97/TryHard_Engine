#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* parent)
{
}

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

bool Component::CleanUp()
{
	return false;
}

void Component::Draw(bool is_editor)
{
}

void Component::OnEvent(const Event & new_event)
{
}

void Component::FitToRect()
{
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

void Component::Save(JSON_Object * scene_obj, const char* name)
{

}

void Component::Load(JSON_Object * scene_obj)
{
}
