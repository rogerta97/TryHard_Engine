#include "GameObject.h"

#include "ComponentMesh.h"


GameObject::GameObject()
{
	name = ""; 
	parent = nullptr; 
}

GameObject::GameObject(const char * name)
{
	this->name = name; 
}


GameObject::~GameObject()
{

}

void GameObject::Start()
{
}

void GameObject::Update()
{
}

Component * GameObject::GetComponent(CompType cmp_type)
{
	return nullptr;
}

bool GameObject::AddComponent(Component * new_cmp)
{
	return false;
}

Component * GameObject::CreateComponent(CompType cmp_type)
{
	Component* new_cmp = nullptr; 

	switch (cmp_type)
	{
		case CMP_TRANSFORM:
			break; 

		case CMP_RENDERER:
			new_cmp = new ComponentMesh(); 
			break;
	}

	return new_cmp; 
}
