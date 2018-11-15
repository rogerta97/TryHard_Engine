#include "Prefab.h"
#include "JSON\parson.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include <fstream>


Prefab::Prefab()
{
	root = nullptr; 
}


Prefab::~Prefab()
{
}


void Prefab::SaveAsBinary()
{
	//In this case we don't generate buffers, but we need to save a JSON with all GameObject info
	if (root == nullptr)
		return; 

	string directory = App->file_system->DeleteLastPathItem(path);
	string meta_name = name + ".meta"; 

	//If the meta for the file don't exist we generate it 
	if (!App->file_system->IsFileInDirectory(directory, meta_name.c_str()))
	{
		string item_meta_path = directory + "\\" + meta_name;

		//Creating Meta File
		std::ofstream stream;
		stream.open(item_meta_path, std::fstream::out);

		JSON_Value* scene_v = json_value_init_object();
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		//Save Meta Info
		json_object_dotset_number(scene_obj, "MetaInfo.UID", GetUID());

		json_serialize_to_file(scene_v, item_meta_path.c_str());

		SetFileAttributes(item_meta_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

		stream.close();

		//Create Binary 
		string dest_str = App->file_system->GetLibraryPath() + string("\\Prefabs\\") + to_string(GetUID()) + ".jprefab";

		stream.open(dest_str, std::fstream::out);

		scene_v = json_value_init_object();
		scene_obj = json_value_get_object(scene_v);

		json_object_dotset_number(scene_obj, "Info.uid", GetUID());

		// Saving GameObject Info
		int index = 0;
		root->SaveRecursive(scene_obj, index);

		json_object_dotset_number(scene_obj, "Info.obj_num", index);

		json_serialize_to_file(scene_v, dest_str.c_str());

		stream.close();
	}

	//Load The Binary
	//LoadFromBinary(); 
	
}

void Prefab::LoadFromBinary()
{
	//If there is not ".Meta" we assume there is any binary, so we don't load 
	string directory = App->file_system->DeleteLastPathItem(path);
	string meta_name = name + ".meta"; 

	if (App->file_system->IsFileInDirectory(App->file_system->DeleteLastPathItem(path), meta_name.c_str()))
	{
		string meta_file_path = path + ".meta";
		//Get the ID to know which binary is attached
		std::ifstream stream;
		stream.open(meta_file_path, std::fstream::out);

		JSON_Value* scene_v = json_parse_file(meta_file_path.c_str());
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		UID lib_id = json_object_dotget_number(scene_obj, "MetaInfo.UID");
		stream.close();

		//Load the binary into root 
		string resource_path = App->file_system->GetLibraryPath() + "\\Prefabs\\" + to_string(lib_id) + ".jprefab";

		std::ifstream res_stream;
		res_stream.open(resource_path, std::fstream::out);

		JSON_Value* scene_v_res = json_parse_file(resource_path.c_str());
		JSON_Object* scene_obj_res = json_value_get_object(scene_v_res);

		LoadPrefabData(scene_obj_res);

		res_stream.close();
	}
}

void Prefab::LoadPrefabData(JSON_Object* gameobject_obj)
{
	int obj_ammount = json_object_dotget_number(gameobject_obj, "Info.obj_num");
	UID prefab_uid = json_object_dotget_number(gameobject_obj, "Info.uid");

	list<GameObject*> obj_list;

	for (int i = 0; i < obj_ammount; i++)
	{
		GameObject* new_go = new GameObject();
		new_go->Load(gameobject_obj, i, prefab_uid);
		App->scene->AddGameObjectToScene(new_go);
		obj_list.push_back(new_go);
	}

	if (!obj_list.empty())
	{
		auto it = obj_list.begin();
		root = (*it);
	}

}

GameObject* Prefab::GetRootGameObject()
{
	return root;
}

void Prefab::SetRootGameObject(GameObject* root)
{
	this->root = root; 
}

void Prefab::CreateContainingMeshResources()
{
	//If there is not ".Meta" we assume there is any binary, so we don't load 
	string directory = App->file_system->DeleteLastPathItem(path);
	string meta_name = name + ".meta";

	if (App->file_system->IsFileInDirectory(App->file_system->DeleteLastPathItem(path), meta_name.c_str()))
	{
		string meta_file_path = path + ".meta";
		//Get the ID to know which binary is attached
		std::ifstream stream;
		stream.open(meta_file_path, std::fstream::out);

		JSON_Value* scene_v = json_parse_file(meta_file_path.c_str());
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		UID lib_id = json_object_dotget_number(scene_obj, "MetaInfo.UID");
		stream.close();

		//Load the binary into root 
		string resource_path = App->file_system->GetLibraryPath() + "\\Prefabs\\" + to_string(lib_id) + ".jprefab";

		std::ifstream res_stream;
		res_stream.open(resource_path, std::fstream::out);

		JSON_Value* scene_v_res = json_parse_file(resource_path.c_str());
		JSON_Object* scene_obj_res = json_value_get_object(scene_v_res);

		LoadPrefabData(scene_obj_res);

		res_stream.close();
	}
}
