#pragma once

#include <list>
#include "Component.h"

#include "ComponentMesh.h"

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	void Start(); 
	void Update(); 

	bool AddComponent(Component* new_cmp);

	Component* CreateComponent(CompType cmp_type); 

	//Getters & Setters
	Component* GetComponent(CompType cmp_type); 
	
public:
	std::string name;
	GameObject* parent;

private:
	std::list<GameObject*> child_list; 
	std::list<Component*> component_list; 
};

