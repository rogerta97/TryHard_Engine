#pragma once
#include "UI_Panel.h"

class GameObject; 

class UI_InspectorPanel :
	public UI_Panel
{
public:
	UI_InspectorPanel();
	~UI_InspectorPanel();

	bool Start();
	bool Update();

	void SetGameObject(GameObject* new_go); 
	GameObject* GetGameObject(); 

private: 
	GameObject * gameobject; 
};

