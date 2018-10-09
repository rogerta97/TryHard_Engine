#pragma once

#include <list>
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

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

	void DeleteRecursive(); 
	void DeleteAllComponents(); 
	bool DeleteComponent(CompType cmp); 

	Component* CreateComponent(CompType cmp_type); 

	void SetSelectedRecursive(bool selected); 

	void PrintHierarchyRecursive(int mask, int& node_clicked, int& id); 

	//Utility
	bool HasComponents(); 
	bool HasChilds(); 

	void SetCenterCamDataRecursive(float3& position_amm, float& distance_amm); 

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

