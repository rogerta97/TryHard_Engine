#pragma once

#include <list>
#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "JSON\parson.h"

class ComponentTransform;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	void Start(); 
	void Update(); 

	Component* AddComponent(CompType new_cmp);
	void AddComponentFromJSON(JSON_Object* cmp_obj, const char* cmp_type);
	bool AddChild(GameObject* child); 

	void DeleteGameObject(); 
	void DeleteRecursive(); 
	void DeleteAllComponents(); 
	bool DeleteComponent(CompType cmp); 

	void SetSelectedRecursive(bool selected); 
	bool PrintHierarchyRecursive(int mask, int& node_clicked, int& id); 

	//Save & Load
	void Save(JSON_Object* scene_obj, int index);
	bool Load(JSON_Object* scene_obj, int index);

	//Utility
	bool HasComponents() ; 
	bool HasChilds() const; 
	GameObject* GetChild(int index) const; 
	GameObject* GetChild(const char* name) const; 
	void DeleteChildFromList(GameObject* child_to_delete);
	void GetEnclosedAABB(float3& min, float3& max); 

	void SetCenterCamDataRecursive(float3& position_amm, float& distance_amm); 

	//Getters & Setters
	Component* GetComponent(CompType cmp_type) const;
	GameObject* GetParent() const;
	GameObject* GetRootParent(); 

	void SetParent(GameObject* new_parent); 
	void SetActive(bool activated);

	void SetStatic(bool set_static);
	bool GetIsStatic();

	std::list<GameObject*>* GetChildList();

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
	UID unique_id;
	ComponentTransform* transform;

private:
	std::list<GameObject*> child_list; 
	bool active; 
	bool is_static;
};

