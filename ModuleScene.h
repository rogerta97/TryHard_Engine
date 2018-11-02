#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "GameObject.h"
#include "Octree.h"

#define MAX_SNAKE 2

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void DrawSceneGameObjects(GameObject* camera); 
	void DeleteGameObjectsNow(); 
	void CleanScene(); 
	void AddGameObjectToDeleteList(GameObject* to_del); 
	
	void AddGOToStaticList(GameObject* go); 
	void DeleteGOFromStaticList(GameObject* go); 

	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(std::list<GameObject*> list_childs, const char* name);
	int GetGameObjectsAmmount(); 

	void DeleteGameObjectFromList(GameObject* go);
	void AddGameObjectToScene(GameObject* go); 
	bool IsTextureUsed(int id, GameObject* skip); 
	void SetDefaultScene(); 

	void TestLineAgainstGOs(LineSegment line);

	GameObject* GetClosestGO(LineSegment line, std::list<GameObject*> go_list);

	//Setters & Getters
	void SetSelectedGameObject(GameObject* selected);

	GameObject* GetSelectedGameObject() const;

	GameObject* GetGameObjectByID(UID uid);
	GameObject* GetGameObject(const char* name);

	std::list<GameObject*> GetAllGameObjectsWith(CompType type);

	//Load & Save
	void SaveScene(const char* scene_name); 
	void LoadScene(const char* scene_path);

	const char* GetSceneName() const;
	void SetSceneName(const char* new_name);

public:

	std::list<GameObject*> scene_gameobjects; 
	std::list<GameObject*> static_gameobjects;
	std::list<GameObject*> go_to_delete; 
	GameObject* selected_go; 
	std::string scene_name; 
	Octree* octree; 

};
