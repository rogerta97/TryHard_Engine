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

	Component* AddComponent(CompType new_cmp);
	bool AddChild(GameObject* child); 

	void DeleteGameObject(); 
	void DeleteRecursive(); 
	void DeleteAllComponents(); 
	bool DeleteComponent(CompType cmp); 

	void SetSelectedRecursive(bool selected); 

	bool PrintHierarchyRecursive(int mask, int& node_clicked, int& id); 

	//Utility
	bool HasComponents() ; 
	bool HasChilds() const; 
	GameObject* GetChild(int index) const; 
	void DeleteChildFromList(GameObject* child_to_delete);
	void GetEnclosedAABB(float3& min, float3& max); 

	void SetCenterCamDataRecursive(float3& position_amm, float& distance_amm); 

	//Getters & Setters
	Component* GetComponent(CompType cmp_type) const;
	GameObject* GetParent() const;

	void SetParent(GameObject* new_parent); 
	void SetActive(bool activated);

	void SetStatic(bool set_static);
	bool GetIsStatic();

	std::string GetName() const; 
	void SetName(const char* name);

	bool IsActive() const;

	bool IsUsingTexture(int id, bool& used);

	int GetNumChilds(); 

	std::list<Component*> component_list;

	bool selected;
	AABB* bounding_box; 
	
public:
	std::string name;
	GameObject* parent;

private:
	std::list<GameObject*> child_list; 
	bool active; 
	bool is_static;
};

