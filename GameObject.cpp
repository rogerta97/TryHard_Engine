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
	parent = nullptr; 
}


GameObject::~GameObject()
{

}

void GameObject::Start()
{
}

void GameObject::Update()
{
	if (component_list.empty())
		return; 

	for (auto it = component_list.begin(); it != component_list.end(); it++)
	{
		(*it)->Update(); 
	}
}

Component * GameObject::GetComponent(CompType cmp_type)
{
	return nullptr;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

bool GameObject::AddComponent(Component * new_cmp)
{
	component_list.push_back(new_cmp);
	return true; 
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
