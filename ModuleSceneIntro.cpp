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
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	start_time = performance_timer.Read();

 

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
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

void ModuleSceneIntro::AddGameObjectToScene(GameObject* go)
{
	scene_gameobjects.push_back(go); 
}

void ModuleSceneIntro::SetSelectedGameObject(GameObject * selected)
{
	selected_go = selected;
	App->imgui->inspector_panel->SetGameObject(selected); 

	//Child "selected" boolean should also change to true, but they should not be set as current gameobject. 
	selected->SelectGameObjectRecursive(); 	
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
	p.color = {0.15f, 0.15f, 0.15f};  //Em mola que flipas el rosa de moment deixal porfa //ANS: la xupas
	p.Render();

	Sphere s({0,0,0},100);

	Plane p2({ 0,0,0 }, 0);

	Circle intersects = s.Intersect(p2);

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr)
		{
			(*it)->Update(); 
		}
	}
	 
	return UPDATE_CONTINUE;
}



