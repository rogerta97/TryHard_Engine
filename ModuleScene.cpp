#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"
#include "UI_TagPanel.h"
#include "ComponentCheckbox.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "UI_CheckBox.h"
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
	App->imgui->tag_panel->AddTag("UI");
	App->imgui->tag_panel->AddTag("Canvas");
	App->imgui->tag_panel->AddTag("Main Camera");

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

std::list<GameObject*> ModuleScene::GetAllObjectsWithTag(const std::string & tag_to_search)
{
	return current_scene->GetAllObjectsWithTag(tag_to_search);
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
	current_scene->TestLineAgainstUIGOsForGame(line);
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

	string item_meta_path = App->file_system->GetScenesPath() + string("\\") + meta_name;

	if (!App->file_system->IsFileInDirectory(meta_path, meta_name.c_str()))
	{
		overwrite = true;
		scene_to_save = (Scene*)App->resources->CreateNewResource(RES_SCENE);

		scene_to_save->name = scene_name;
		scene_to_save->path = App->file_system->GetLibraryPath() + "\\Scenes" + scene_name + ".json";

		//Create Meta
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
		//Get the ID from the meta 
		
		std::ofstream stream;
		stream.open(item_meta_path, std::fstream::out);

		JSON_Value* scene_v_meta = json_parse_file(item_meta_path.c_str());
		JSON_Object* scene_obj_meta = json_value_get_object(scene_v_meta);

		UID scene_id = json_object_dotget_number(scene_obj_meta, "MetaInfo.UID");

		stream.close();

		string new_scene_path = App->file_system->GetLibraryPath() + std::string("\\Scenes\\") + to_string(scene_id);

		if (App->file_system->GetFileExtension(scene_name) != FX_JSON)
			new_scene_path += std::string(".json");

		if (App->file_system->IsFileInDirectory(App->file_system->GetScenesPath().c_str(), scene_name))
		{
			CONSOLE_DEBUG("Scene '%s' already exist. Overwritting...", App->file_system->GetLastPathItem(new_scene_path).c_str());
		}

		//Create the new json file 
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

void ModuleScene::CleanAndLoadScene(const char * scene_path)
{	
	CleanScene();
	LoadScene(scene_path);
}

void ModuleScene::LoadScene(const char* scene_name)
{
	if (App->user_interface->IsInterpolating()) // If the UI is interpolating, we should save the name of the scene to load and load it once it's finished
	{
		load_when_interpolation_ends = true; 
		scene_to_load = scene_name;
		return;
	}

	string name_w_termination = scene_name + string(".json");

	if (App->file_system->GetFileExtension(name_w_termination) != FX_JSON)
		name_w_termination += std::string(".json");

	Scene* scene_to_save = nullptr;
	string meta_name = scene_name + string(".json.meta");
	string meta_path = App->file_system->GetScenesPath();

	//Check if the meta path exists, if not we don't load the scene
	if (!App->file_system->IsFileInDirectory(meta_path, meta_name.c_str()))
	{
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
		string path = App->file_system->GetLibraryPath() + "\\Scenes\\" + to_string(obj_id) + ".json";
		std::ifstream stream;
		stream.open(path.c_str(), std::fstream::in);

		JSON_Value* root = json_parse_file(path.c_str());
		JSON_Object* root_obj = json_value_get_object(root);

		int obj_ammount = json_object_dotget_number(root_obj, "Scene.obj_num");
		UID main_cam_uid = json_object_dotget_number(root_obj, "Scene.main_camera_uid");

		std::multimap<UID, GameObject*> parenting_candidates = std::multimap<UID, GameObject*>();

		int i = 0;
		while (i < obj_ammount)
		{
			string item_to_get = "GameObject_" + to_string(i);

			bool isui = json_object_dotget_boolean(root_obj, std::string(item_to_get + ".IsUI").c_str());			
			GameObject* new_go = new GameObject("", isui);

			if (new_go->Load(root_obj, i, parenting_candidates))
				AddGameObjectToScene(new_go);

			new_go->selected = false;
			i++;
		}


		if (main_cam_uid != 0)
			App->camera->SetGameCamera(App->scene->GetGameObjectByID(main_cam_uid));

		for (auto it = parenting_candidates.begin(); it != parenting_candidates.end(); it++)
		{
			(*it).second->SetParent(App->scene->GetGameObjectByID((*it).first));

			if ((*it).second->GetIsUI() && (*it).second->GetComponent(CMP_CHECKBOX))
			{
				ComponentCheckBox* cmp_ch = (ComponentCheckBox*)(*it).second->GetComponent(CMP_CHECKBOX); 
				GameObject* o1 = App->scene->GetGameObjectByID(cmp_ch->GetCheckBox()->child_button_uid);
				cmp_ch->GetCheckBox()->SetChildButton(o1);
				cmp_ch->GetCheckBox()->SetToggleImage(App->scene->GetGameObjectByID(cmp_ch->GetCheckBox()->img_to_toggle_uid));
			}
		}
				
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
	case EventType::PLAY:
		SaveScene("temp_scene");
		break;
	case EventType::STOP:
		CleanAndLoadScene("temp_scene");
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
	//switch (App->GetGameState())
	//{
	//case RUNNING:
	//	CleanAndLoadScene("temp_scene");
	//	break;

	//case STOPPED:
	//	SaveScene("temp_scene");
	//	break;
	//case PAUSED:

	//	break;

	//default:
	//	break;
	//}
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
	if (load_when_interpolation_ends)
	{
		if (App->user_interface->HasInterpolationEnded())
		{
			CleanScene();
			LoadScene(scene_to_load);
			load_when_interpolation_ends = false;
		}		
	}

	current_scene->Update(dt);
	
	return UPDATE_CONTINUE;
}



