#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"
#include "UI_TagPanel.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "DebugDraw.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ImGuizmo/ImGuizmo.h"

#include <fstream>
#include <vector> 
#include <algorithm> 
#include <iostream>


#include "mmgr\mmgr.h"


ModuleScene::ModuleScene(bool start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
	delete current_scene; 
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	current_scene = new Scene(); 
	current_scene->SetDefaultScene();
	App->scene->SetSelectedGameObject(nullptr);

	App->renderer3D->OnResize(1000, 1000);
	current_scene->octree = new Octree();
	current_scene->octree->draw = false;

	App->imgui->tag_panel->AddTag("Untagged");

	App->camera->SetGameCamera(App->scene->GetGameObject("Main Camera"));

	current_scene->SetGuizmoMode(TRANSLATE);

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading scene");

	current_scene->CleanUp(); 

	return true;
}

void ModuleScene::DrawSceneGameObjects(GameObject* camera)
{
	current_scene->DrawSceneGameObjects(camera);
}

void ModuleScene::DeleteGameObjectsNow()
{
	current_scene->DeleteGameObjectsNow();
}

void ModuleScene::CleanScene()
{
	current_scene->CleanScene(); 
}

void ModuleScene::AddGameObjectToDeleteList(GameObject * to_del)
{
	current_scene->AddGameObjectToDeleteList(to_del); 
}

void ModuleScene::AddGOToStaticList(GameObject * go)
{
	current_scene->AddGOToStaticList(go); 
}

void ModuleScene::DeleteGOFromStaticList(GameObject * go)
{
	current_scene->DeleteGOFromStaticList(go); 
}

GameObject * ModuleScene::LoadPrefab(const char * prf_name)
{
	GameObject* root_go = new GameObject(prf_name);

	string dest_str = App->file_system->GetPrefabPath() + string("\\") + prf_name + ".jprefab";

	std::ifstream stream;
	stream.open(dest_str);

	JSON_Value* scene_v = json_parse_file(dest_str.c_str());
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	int obj_ammount = json_object_dotget_number(scene_obj, "Info.obj_num");

	list<GameObject*> obj_list;

	for (int i = 0; i < obj_ammount; i++)
	{
		GameObject* new_go = new GameObject("", false);
		new_go->Load(scene_obj, i);
		AddGameObjectToScene(new_go);
		obj_list.push_back(new_go);
	}

	auto it = obj_list.begin();
	root_go = (*it);

	stream.close();

	return root_go;
}

GameObject * ModuleScene::CreateGameObject()
{
	GameObject* new_go = new GameObject("");

	//Here we will add the component transform, every GO should have it.

	current_scene->AddGameObjectToScene(new_go);


	return new_go;
}

GameObject * ModuleScene::CreateUIElement(UI_Widgget_Type widdget, GameObject * force_parent)
{
	return current_scene->CreateUIElement(widdget, force_parent);
}

GameObject * ModuleScene::GetGameObjectByID(UID uid)
{
	return current_scene->GetGameObjectByID(uid);
}

GameObject * ModuleScene::GetGameObject(const char * name)
{
	return current_scene->GetGameObject(name);
}

std::list<GameObject*> ModuleScene::GetAllGameObjectsWith(CompType type)
{

	return current_scene->GetAllGameObjectsWith(type);
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
	return current_scene->GetGameObjectsAmmount();
}

void ModuleScene::DeleteGameObjectFromList(GameObject* go)
{
	current_scene->DeleteGameObjectFromList(go); 
}

void ModuleScene::AddGameObjectToScene(GameObject* go)
{
	current_scene->AddGameObjectToScene(go); 
}

bool ModuleScene::IsTextureUsed(int id, GameObject* skip)
{
	/*bool using_texture = false;

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it) != skip)
			if ((*it)->IsUsingTexture(id, using_texture))
				return true;
	}*/

	return false;
}

void ModuleScene::TestLineAgainstGOs(LineSegment line)
{
	current_scene->TestLineAgainstGOs(line); 

}

void ModuleScene::TestLineAgainstUIGOs(LineSegment line)
{
	current_scene->TestLineAgainstUIGOs(line);
}

void ModuleScene::DrawGuizmo()
{
	current_scene->DrawGuizmo(); 
}

GameObject * ModuleScene::GetClosestGO(LineSegment line, std::list<GameObject*> go_list)
{
	return current_scene->GetClosestGO(line, go_list);
}

void ModuleScene::SetSelectedGameObject(GameObject * selected)
{
	current_scene->SetSelectedGameObject(selected); 
}

GameObject* ModuleScene::GetSelectedGameObject() const
{
	return current_scene->GetSelectedGameObject();
}

void ModuleScene::SetDefaultScene()
{
	//Create Empty GO with a camera
	GameObject* main_cam = CreateGameObject("Main Camera");

	ComponentCamera* cam = (ComponentCamera*)main_cam->AddComponent(CMP_CAMERA);
	cam->camera->frustum.farPlaneDistance = 1000;
	cam->is_editor = false;

}

void ModuleScene::SaveScene(const char* scene_name)
{
	bool overwrite = false;
	
	string name(scene_name); 
	App->file_system->DeleteAllFileExtensions(name);
	scene_name = name.c_str(); 

	// Check if a meta with the same name exists
	Scene* scene_to_save = nullptr;
	string meta_name = scene_name + string(".json.meta");
	string meta_path = App->file_system->GetScenesPath();

	if (!App->file_system->IsFileInDirectory(meta_path, meta_name.c_str()))
	{
		overwrite = true;
		scene_to_save = (Scene*)App->resources->CreateNewResource(RES_SCENE);

		scene_to_save->name = scene_name;
		scene_to_save->path = App->file_system->GetLibraryPath() + "\\Scenes" + scene_name + ".json";

		//Create Meta
		string item_meta_path = App->file_system->GetScenesPath() + string("\\") + meta_name;
		std::ofstream stream;
		stream.open(item_meta_path, std::fstream::out);

		JSON_Value* scene_v = json_value_init_object();
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		//Save Meta Info
		json_object_dotset_number(scene_obj, "MetaInfo.UID", scene_to_save->GetUID());

		json_serialize_to_file(scene_v, item_meta_path.c_str());

		SetFileAttributes(item_meta_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

		stream.close();
	}
	else
	{
		//TODO: Inform that an scene is already saved with this name, and ask he wants to overwrite it
		//final result is to set overwrite to true/false
		overwrite = true;
	}

	if (overwrite)
	{
		//Create the path were the scene is going to be saved

		string new_scene_path = App->file_system->GetLibraryPath() + std::string("\\Scenes\\") + to_string(scene_to_save->GetUID());

		if (App->file_system->GetFileExtension(scene_name) != FX_JSON)
			new_scene_path += std::string(".json");

		if (App->file_system->IsFileInDirectory(App->file_system->GetScenesPath().c_str(), scene_name))
		{
			CONSOLE_DEBUG("Scene '%s' already exist. Overwritting...", App->file_system->GetLastPathItem(new_scene_path).c_str());
		}

		//Create the new json file 
		std::ofstream stream;
		stream.open(new_scene_path, std::fstream::out);

		JSON_Value* scene_v = json_value_init_object();
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		//Save Scene Info
		json_object_dotset_number(scene_obj, "Scene.obj_num", current_scene->scene_gameobjects.size());
		json_object_dotset_number(scene_obj, "Scene.tags_num", 0);

		if (App->camera->GetGameCameraObject() != nullptr)
			json_object_dotset_number(scene_obj, "Scene.main_camera_uid", App->camera->GetGameCamera()->GetGameObject()->unique_id);
		else
			json_object_dotset_number(scene_obj, "Scene.main_camera_uid", 0);

		int index = 0;
		for (auto it = current_scene->scene_gameobjects.begin(); it != current_scene->scene_gameobjects.end(); it++)
		{
			scene_obj = json_value_get_object(scene_v);
			(*it)->Save(scene_obj, index++);
		}

		json_serialize_to_file(scene_v, new_scene_path.c_str());

		stream.close();
	}
}

void ModuleScene::LoadScene(const char * scene_name, bool clean)
{
	string name_w_termination = scene_name + string(".json");

	if (App->file_system->GetFileExtension(name_w_termination) != FX_JSON)
		name_w_termination += std::string(".json");

	Scene* scene_to_save = nullptr;
	string meta_name = scene_name + string(".json.meta");
	string meta_path = App->file_system->GetScenesPath();

	//Check if the meta path exists, if not we don't load the scene
	if (!App->file_system->IsFileInDirectory(meta_path, meta_name.c_str()))
	{
		////If the scene trying to be load is "DefaultScene", we create the scene
		//if (scene_name == "DefaultScene")
		//{
		//	current_scene->SetDefaultScene();
		//	App->scene->SetSelectedGameObject(nullptr);
		//	return;
		//}
		//	
		CONSOLE_ERROR("'.meta' file for %s is not found. Scene can not be load", scene_name); 
		return; 
	}

	//Get the ID from the meta which is the name of the file 
	string meta_file_path = meta_path + "\\" + meta_name; 

	std::ifstream stream;
	stream.open(meta_file_path.c_str(), std::fstream::in);

	JSON_Value* root = json_parse_file(meta_file_path.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	UID obj_id = json_object_dotget_number(root_obj, "MetaInfo.UID");

	if (App->file_system->IsFileInDirectory(string(App->file_system->GetLibraryPath() + "\\Scenes").c_str(), string(to_string(obj_id) + ".json").c_str()))
	{
		if (clean)
			CleanScene();

		string path = App->file_system->GetLibraryPath() + "\\Scenes\\" + to_string(obj_id) + ".json";
		std::ifstream stream;
		stream.open(path.c_str(), std::fstream::in);

		JSON_Value* root = json_parse_file(path.c_str());
		JSON_Object* root_obj = json_value_get_object(root);

		int obj_ammount = json_object_dotget_number(root_obj, "Scene.obj_num");
		UID main_cam_uid = json_object_dotget_number(root_obj, "Scene.main_camera_uid");

		int i = 0;
		while (i < obj_ammount)
		{
			string item_to_get = "GameObject_" + to_string(i);
			GameObject* new_go = new GameObject("", false);

			if (new_go->Load(root_obj, i))
				AddGameObjectToScene(new_go);

			new_go->selected = false;
			i++;
		}

		if (main_cam_uid != 0)
			App->camera->SetGameCamera(App->scene->GetGameObjectByID(main_cam_uid));

		SetSceneName(name_w_termination.c_str());

		stream.close();
	}

	App->scene->SetSelectedGameObject(nullptr); 
}

const char * ModuleScene::GetSceneName() const
{
	return current_scene->GetSceneName();
}

void ModuleScene::SetSceneName(const char * new_name)
{
	current_scene->SetSceneName(new_name); 
}

void ModuleScene::RecieveEvent(const Event & event)
{
	switch (event.type)
	{
	case Event::PLAY:
		Play();
		break;
	case Event::PAUSE:
		Pause();
		break;
	default:
		break;
	}
}

void ModuleScene::Pause()
{
}

void ModuleScene::Play()
{
	GameState& current_state = App->current_game_state;
	switch (current_state)
	{
	case RUNNING:
		LoadScene("temp_scene");
		break;

	case STOPPED:
		SaveScene("temp_scene");
		break;
	case PAUSED:

		break;

	default:
		break;
	}
}

Scene * ModuleScene::GetCurrentScene()
{
	return current_scene;
}

void ModuleScene::SetCurrentScene(Scene* scene)
{
	current_scene = scene;
}

// Update
update_status ModuleScene::Update(float dt)
{
	current_scene->Update(dt);

	/*for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr || (*it)->IsActive() == false)
		{
			(*it)->Update();
		}
	}


	if (App->camera->frustum_culling) {

		std::list<UID> intersections_list_uid;

		if (octree->GetRoot() != nullptr && App->scene->go_to_delete.empty())
		{
			octree->GetIntersections(intersections_list_uid, *App->camera->GetGameCamera()->GetFrustum());

			intersections_list_uid.sort();
			intersections_list_uid.unique();

			CONSOLE_LOG("INTERSECTIONS: %d", intersections_list_uid.size());

			for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
			{
				ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

				if (!mesh)
					continue;

				mesh->frustum_col_type = OUTSIDE_FRUSTUM;
			}

			for (auto it = intersections_list_uid.begin(); it != intersections_list_uid.end(); it++)
			{
				ComponentMesh* mesh = (ComponentMesh*)App->scene->GetGameObjectByID((*it))->GetComponent(CMP_MESH);

				if (!mesh)
					continue;


				mesh->frustum_col_type = App->camera->GetGameCamera()->camera->IsAABBInside(mesh->bounding_box);
			}
		}
	}
	else
	{
		for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

			if (!mesh)
				continue;

			if (App->camera->frustum_culling)
				mesh->frustum_col_type = App->camera->GetGameCamera()->camera->IsAABBInside(mesh->bounding_box);
			else
				mesh->frustum_col_type = INSIDE_FRUSTUM;
		}
	}


	if (go_to_delete.size() != 0)
		DeleteGameObjectsNow();
*/
	return UPDATE_CONTINUE;
}



