#pragma once

#include <list>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Start(); 
	void Update(); 

	bool AddComponent(Component* new_cmp);

	//Getters & Setters
	Component* GetComponent(CompType cmp_type); 
	
public:
	GameObject* parent;

private:
	std::list<GameObject*> child_list; 
	std::list<Component*> component_list; 
};

