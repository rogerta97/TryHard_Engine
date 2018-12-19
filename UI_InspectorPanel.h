#ifndef _UI_INSPECTOR_PANEL_H_
#define _UI_INSPECTOR_PANEL_H_

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
	void PrintCheckBoxProperties();
	void PrintImageProperties();
	void PrintInputFieldProperties();
	void PrintButtonProperties();
	void PrintMeshProperties(); 
	void PrintMaterialProperties();
	void PrintCanvasScalerProperties();

	void DeleteElement(CompType type);

private: 
	GameObject * gameobject = nullptr; 
	bool show_addcmp_ui; 

	bool show_tex_explorer = false;
	bool show_mesh_explorer = false;

};

#endif

