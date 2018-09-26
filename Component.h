#pragma once

enum CompType {CMP_TRANSFORM = 0, CMP_RENDERER};

class GameObject; 

class Component {
public:
	Component();
	~Component();

	virtual bool Start(); 
	virtual bool Update(); 

	//Setters & Getters
	CompType GetType() const;
	void SetType(CompType new_type);

	GameObject* GetGameObject() const;
	void SetGameObject(GameObject* new_type);

protected:

	GameObject * gameobject; 
	CompType component_type;  
};
