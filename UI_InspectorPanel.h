#pragma once
#include "UI_Panel.h"

#include "Component.h"

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

	void PrintProperties(CompType type); 
	void PrintTransformProperties();
	void PrintMeshProperties(); 
	void PrintMaterialProperties();
	void PrintBoundingBoxProperties();

	void DeleteElement(CompType type);

private: 
	GameObject * gameobject; 
	bool show_addcmp_ui; 
};

