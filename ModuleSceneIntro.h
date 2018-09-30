#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "imgui.h"
#include "GameObject.h"

#define MAX_SNAKE 2


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(std::list<GameObject*> list_childs, const char* name);

	void AddGameObjectToScene(GameObject* go); 

	void SetSelectedGameObject(GameObject* selected); 
	GameObject* GetSelectedGameObject() const;

public:

	std::list<GameObject*> scene_gameobjects; 
	GameObject* selected_go; 

};
