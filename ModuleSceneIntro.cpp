#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "UI_InspectorPanel.h"
#include "Primitive.h"
#include "OpenGL.h"

#include "GameObject.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled)
{
	name = "Scene";
}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	start_time = performance_timer.Read();

	selected_go = nullptr;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::DeleteGameObjectsNow()
{
	for (auto it = go_to_delete.begin(); it != go_to_delete.end();)
	{
		(*it)->DeleteAllComponents();
		
		if ((*it)->parent != nullptr)
			(*it)->parent->DeleteChildFromList((*it));
				
		(*it)->parent = nullptr;

		//delete (*it); 
		DeleteGameObjectFromList((*it)); 
		it = go_to_delete.erase(it);			
	}
}

void ModuleSceneIntro::CleanScene()
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr)
			(*it)->DeleteRecursive(); 
	}
}

void ModuleSceneIntro::AddGameObjectToDeleteList(GameObject * to_del)
{
	go_to_delete.push_back(to_del); 
}

GameObject * ModuleSceneIntro::CreateGameObject()
{
	GameObject* new_go = new GameObject(""); 

	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go; 
}

GameObject * ModuleSceneIntro::CreateGameObject(const char* name)
{
	GameObject* new_go = new GameObject(name);

	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go;
}

GameObject * ModuleSceneIntro::CreateGameObject(std::list<GameObject*> list_childs, const char* name = "")
{
	GameObject* parent_go = new GameObject(name);

	for (auto it = list_childs.begin(); it != list_childs.end(); it++)
	{
		(*it)->parent = parent_go; 
		parent_go->AddChild((*it)); 
	}

	AddGameObjectToScene(parent_go); 
	return parent_go; 
}

int ModuleSceneIntro::GetGameObjectsAmmount()
{
	return scene_gameobjects.size();
}

void ModuleSceneIntro::DeleteGameObjectFromList(GameObject* go)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if (go == (*it))
		{
			scene_gameobjects.erase(it);
			return;
		}
	}
}

void ModuleSceneIntro::AddGameObjectToScene(GameObject* go)
{
	scene_gameobjects.push_back(go); 
}

void ModuleSceneIntro::SetSelectedGameObject(GameObject * selected)
{
	App->imgui->inspector_panel->SetGameObject(selected);
	selected_go = selected; 
	
}

GameObject* ModuleSceneIntro::GetSelectedGameObject() const
{
	return selected_go;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	pPlane p(0, 1, 0, 500);
	p.axis = true;
	//p.Scale(0.01f, 0.01f, 0.01f);
	p.color = {0.15f, 0.15f, 0.15f};  
	p.Render();

	Sphere s({0,0,0},100);

	Plane p2({ 0,0,0 }, 0);

	Circle intersects = s.Intersect(p2);

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr || (*it)->IsActive() == false)
		{
			(*it)->Update(); 
		}
	}

	if (go_to_delete.size() != 0)
		DeleteGameObjectsNow(); 
	 
	return UPDATE_CONTINUE;
}



