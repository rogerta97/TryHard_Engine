#pragma once
#include "Resource.h"
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include <list>
#include <map>

class GameObject; 
class Octree;

#define MAX_SNAKE 2

enum OPERATION
{
	TRANSLATE,
	ROTATE,
	SCALE,
	BOUNDS,
};

class Scene : public Resource
{
public:
	Scene();
	~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void DrawSceneGameObjects(GameObject* camera);
	void DeleteGameObjectsNow();
	void CleanScene();
	void AddGameObjectToDeleteList(GameObject* to_del);
	void AddGameObjectToTransparencyList(GameObject* to_add);
	std::map<float, GameObject*> GetSortedGOList(GameObject* new_go);

	void AddGOToStaticList(GameObject* go);
	void DeleteGOFromStaticList(GameObject* go);

	GameObject* LoadPrefab(const char* prf_name);
	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(std::list<GameObject*> list_childs, const char* name);
	int GetGameObjectsAmmount();

	void DeleteGameObjectFromList(GameObject* go);
	void AddGameObjectToScene(GameObject* go);
	bool IsTextureUsed(int id, GameObject* skip);
	void SetDefaultScene();

	void TestLineAgainstGOs(LineSegment line);
	void DrawGuizmo();
	GameObject* GetClosestGO(LineSegment line, std::list<GameObject*> go_list);

	//Setters & Getters
	void SetSelectedGameObject(GameObject* selected);
	GameObject* GetSelectedGameObject() const;
	GameObject* GetGameObjectByID(UID uid);
	GameObject* GetGameObject(const char* name);
	std::list<GameObject*> GetAllGameObjectsWith(CompType type);
	const char* GetSceneName() const;
	void SetSceneName(const char* new_name);

	//Load & Save
	void SaveScene(const char* scene_name);
	void LoadScene(const char* scene_path, bool clean = true);

	void SetGuizmoMode(OPERATION new_mode); 
	OPERATION GetGuizmoMode(); 

	Octree* octree;

public:

	std::string scene_name;

	GameObject* selected_go; 

	std::list<GameObject*> scene_gameobjects;
	std::list<GameObject*> static_gameobjects;
	std::list<GameObject*> go_to_delete;
	std::list<GameObject*> transparent_gameobjects;

	OPERATION guizmo_mode;
};

