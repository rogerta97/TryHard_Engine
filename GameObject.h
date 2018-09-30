#pragma once

#include <list>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	void Start(); 
	void Update(); 

	bool AddComponent(Component* new_cmp);
	bool AddChild(GameObject* child); 

	Component* CreateComponent(CompType cmp_type); 

	void SetSelectedRecursive(bool selected); 

	void PrintHierarchyRecursive(int mask, int& node_clicked, int& id); 

	//Utility
	bool HasComponents(); 
	bool HasChilds(); 

	//Getters & Setters
	Component* GetComponent(CompType cmp_type) const;
	GameObject* GetParent() const;
	void SetActive(bool activated);
	bool IsActive() const;

	int GetNumChilds(); 

	std::list<Component*> component_list;

	bool selected; 
	
public:
	std::string name;
	GameObject* parent;

private:
	std::list<GameObject*> child_list; 
	bool active; 
};

