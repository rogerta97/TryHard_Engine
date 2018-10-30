#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "UI_InspectorPanel.h"
#include "Primitive.h"
#include "OpenGL.h"

#include "GameObject.h"
#include "ComponentTransform.h"

#include <fstream>

ModuleScene::ModuleScene(bool start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{

}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
			
	start_time = performance_timer.Read();
	selected_go = nullptr;

	App->renderer3D->OnResize(1000, 1000);
	octree = new Octree();

	SetDefaultScene(); 
	SaveScene("Test"); 

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleScene::DeleteGameObjectsNow()
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

void ModuleScene::CleanScene()
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr)
			(*it)->DeleteRecursive(); 
	}

	App->scene->DeleteGameObjectsNow(); 
}

void ModuleScene::AddGameObjectToDeleteList(GameObject * to_del)
{
	go_to_delete.push_back(to_del); 
}

void ModuleScene::AddGOToStaticList(GameObject * go)
{
	static_gameobjects.push_back(go);
}

void ModuleScene::DeleteGOFromStaticList(GameObject * go)
{
	for (auto it = static_gameobjects.begin(); it != static_gameobjects.end(); it++)
	{
		if ((*it) == go)
		{
			static_gameobjects.erase(it); 
			return; 
		}
	}
}

GameObject * ModuleScene::CreateGameObject()
{
	GameObject* new_go = new GameObject(""); 


	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go; 
}

GameObject * ModuleScene::CreateGameObject(const char* name)
{
	GameObject* new_go = new GameObject(name);

	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go;
}

GameObject * ModuleScene::CreateGameObject(std::list<GameObject*> list_childs, const char* name = "")
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

int ModuleScene::GetGameObjectsAmmount()
{
	return scene_gameobjects.size();
}

void ModuleScene::DeleteGameObjectFromList(GameObject* go)
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

void ModuleScene::AddGameObjectToScene(GameObject* go)
{
	scene_gameobjects.push_back(go); 
}

bool ModuleScene::IsTextureUsed(int id, GameObject* skip)
{
	bool using_texture = false; 

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it) != skip)
			if ((*it)->IsUsingTexture(id, using_texture))
				return true; 
	}
	
	return false; 
}

void ModuleScene::SetSelectedGameObject(GameObject * selected)
{
	if (App->imgui->inspector_panel == nullptr)
		return; 

	App->imgui->inspector_panel->SetGameObject(selected);
	selected_go = selected; 
	
}

GameObject* ModuleScene::GetSelectedGameObject() const
{
	return selected_go;
}

void ModuleScene::SetDefaultScene()
{
	//Create Empty GO with a camera
	GameObject* main_cam = CreateGameObject("Main Camera");

	main_cam->AddComponent(CMP_CAMERA);
}

void ModuleScene::SaveScene(const char* scene_name)
{
	string new_scene_path = App->file_system->GetScenesPath() + std::string("\\") + std::string(scene_name) + std::string(".json");

	if (App->file_system->IsFileInDirectory(App->file_system->GetScenesPath().c_str(), scene_name))
	{
		CONSOLE_DEBUG("Scene '%s' already exist. Overwritting..."); 
	}
	else
	{
		//Create the new json file 
		std::ofstream stream;
		stream.open(new_scene_path, std::fstream::binary | std::fstream::out);

		string file = scene_name + std::string(".json"); 

		JSON_Value* scene_v = json_value_init_object();

		JSON_Object* scene_obj = json_value_get_object(scene_v);

		App->camera->GetCameraGO()->Save(scene_obj);

		json_serialize_to_file(scene_v, new_scene_path.c_str());

		stream.close();
	}
}

// Update
update_status ModuleScene::Update(float dt)
{
	pPlane p(0, 1, 0, 500);
	p.axis = true;
	p.color = {0.0f, 0.0f, 0.0f };

	glDisable(GL_TEXTURE_2D); 
	p.Render();
	glEnable(GL_TEXTURE_2D);

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr || (*it)->IsActive() == false)
		{
			(*it)->Update(); 
		}
	}

	if (octree->GetRoot() != nullptr)
		octree->Draw(); 

	if (go_to_delete.size() != 0)
		DeleteGameObjectsNow(); 
	 
	return UPDATE_CONTINUE;
}



