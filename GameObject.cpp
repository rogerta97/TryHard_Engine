#include "GameObject.h"



GameObject::GameObject()
{

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
