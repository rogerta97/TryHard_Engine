#pragma once

enum CompType {CMP_TRANSFORM = 0, CMP_RENDERER, CMP_MATERIAL};

class GameObject; 

class Component {
public:
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

protected:

	GameObject * gameobject; 
	CompType component_type; 
	
};
