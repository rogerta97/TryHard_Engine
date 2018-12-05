#pragma once
#include "UI_Panel.h"

#include "Component.h"

class GameObject; 
class ComponentCamera; 

class UI_InspectorPanel :
	public UI_Panel
{
public:
	UI_InspectorPanel();
	~UI_InspectorPanel();

	bool Start();
	bool Update();

	void SetGameObject(GameObject* new_go); 
	GameObject* GetGameObject() const; 

	void PrintCameraProperties(ComponentCamera* cam);

private: 

	void PrintProperties(CompType type); 
	void PrintTransformProperties();
	void PrintRectTransformProperties();
	void PrintCanvasProperties();
	void PrintTextProperties();
	void PrintImageProperties();
	void PrintButtonProperties();
	void PrintMeshProperties(); 
	void PrintMaterialProperties();

	void DeleteElement(CompType type);

private: 
	GameObject * gameobject; 
	bool show_addcmp_ui; 

	bool show_tex_explorer = false;
	bool show_mesh_explorer = false;

};

