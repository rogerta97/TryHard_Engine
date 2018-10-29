#pragma once

#include "Globals.h"

enum CompType {CMP_TRANSFORM = 0, CMP_MESH, CMP_MATERIAL, CMP_CAMERA};

class GameObject; 

class Component {
public:
	Component(GameObject* parent);
	Component(); 
	~Component();

	virtual bool Start(); 
	virtual bool Update(); 
	virtual bool CleanUp(); 

	//Setters & Getters
	CompType GetType() const;
	void SetType(CompType new_type);

	GameObject* GetGameObject() const;
	void SetGameObject(GameObject* new_type);

	bool active;
	UID unique_id; 

protected:

	GameObject * gameobject; 
	CompType component_type; 
	
};
