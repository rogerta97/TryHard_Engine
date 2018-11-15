#pragma once

#include "Resource.h"
#include "GameObject.h"

class Prefab : public Resource
{
public:
	Prefab();
	~Prefab();

	void SaveAsBinary();
	void LoadFromBinary();

	void LoadPrefabData(JSON_Object* gameobject_obj);

	GameObject* GetRootGameObject(); 
	void SetRootGameObject(GameObject* root_go);

	void CreateContainingMeshResources();

private:

	GameObject * root; 
};

