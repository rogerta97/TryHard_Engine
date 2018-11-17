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
	void Draw(bool is_editor); 

public: 

	//Components & Childs
	Component* AddComponent(CompType new_cmp);
	void AddComponentFromJSON(JSON_Object* cmp_obj, const char* cmp_type, UID prefab_uid = 0);
	bool AddChild(GameObject* child); 
	bool HasComponents();
	bool HasChilds() const;
	GameObject* GetChild(int index) const;
	GameObject* GetChild(const char* name) const;
	void DeleteChildFromList(GameObject* child_to_delete);
	int GetNumChilds();
	std::list<GameObject*>* GetChildList();

	void DeleteGameObject(); 
	void DeleteRecursive(); 
	void DeleteAllComponents(); 
	bool DeleteComponent(CompType cmp); 
	GameObject* Duplicate();

	void SetSelectedRecursive(bool selected); 
	bool PrintHierarchyRecursive(int mask, int& node_clicked, int& id); 

	//Save & Load
	void Save(JSON_Object* scene_obj, int index);
	void SaveRecursive(JSON_Object* scene_obj, int& index); 
	bool Load(JSON_Object* scene_obj, int index, UID prefab_uid = 0);

	void SaveAsPrefab(); 
	void LoadPrefab(const char* prefab_name); 

	void ModifyIDSet();

	//Utility
	void GetEnclosedAABB(float3& min, float3& max); 
	void SetCenterCamDataRecursive(float3& position_amm, float& distance_amm); 
	void GetGOAmount(int& counter);

	//Getters & Setters
	Component* GetComponent(CompType cmp_type) const;
	GameObject* GetParent() const;
	GameObject* GetRootParent(); 

	std::string GetTag() const;
	void SetTag(std::string new_tag); 

	void SetParent(GameObject* new_parent); 
	void SetActive(bool activated);

	void SetStatic(bool set_static);
	bool GetIsStatic();

	std::string GetName() const; 
	void SetName(const char* name);

	bool IsActive() const;
	bool IsUsingTexture(int id, bool& used);

	std::list<Component*> component_list;
	
public:

	bool selected;
	AABB* bounding_box;
	std::string name;
	GameObject* parent;
	UID unique_id;
	ComponentTransform* transform;

private:
	std::list<GameObject*> child_list; 
	bool active; 
	bool is_static;
	std::string tag; 
};

