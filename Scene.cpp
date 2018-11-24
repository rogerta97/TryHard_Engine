#include "Scene.h"

#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"
#include "UI_TagPanel.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "Octree.h"
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


Scene::Scene()
{
	scene_name = "Untitled";
}

Scene::~Scene()
{

}

// Load assets
bool Scene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	selected_go = nullptr;

	App->renderer3D->OnResize(1000, 1000);
	octree = new Octree();
	octree->draw = false;

	App->imgui->tag_panel->AddTag("Untagged");
	SetDefaultScene();
	scene_name = "Untitled";

	App->camera->SetGameCamera(App->scene->GetGameObject("Main Camera"));

	guizmo_mode = TRANSLATE;

	return ret;
}

// Load assets
bool Scene::CleanUp()
{
	LOG("Unloading Intro scene");

	for (std::list<GameObject*>::iterator item = scene_gameobjects.begin(); item != scene_gameobjects.end(); item++)
	{
		delete (*item);
	}

	delete octree;

	return true;
}

void Scene::DrawSceneGameObjects(GameObject* camera)
{
	bool editor_cam = false;

	if (App->camera->skybox)
	{
		App->camera->skybox->AttachTo(camera);
		App->camera->skybox->Draw();
	}

	ComponentCamera* cam = (ComponentCamera*)camera->GetComponent(CMP_CAMERA);

	if (cam == App->camera->GetEditorCamera())
	{
		pPlane p(0, 1, 0, 500);
		p.axis = true;
		p.color = { 0.0f, 0.0f, 0.0f };

		App->renderer3D->UseDebugRenderSettings();
		p.Render();
		App->renderer3D->GetDefaultRenderSettings();
		editor_cam = true;
	}

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		(*it)->Draw(editor_cam);
	}

	if (editor_cam) {
		cam->Draw(editor_cam);
		App->camera->DrawMouseRay();
	}
}

void Scene::SetGuizmoMode(OPERATION new_mode)
{
	guizmo_mode = new_mode;
}

OPERATION Scene::GetGuizmoMode()
{
	return guizmo_mode;
}

void Scene::DeleteGameObjectsNow()
{
	for (auto it = go_to_delete.begin(); it != go_to_delete.end();)
	{
		if ((*it)->GetIsStatic())
		{
			(*it)->SetStatic(false);
		}
		(*it)->DeleteAllComponents();

		if ((*it)->parent != nullptr)
			(*it)->parent->DeleteChildFromList((*it));

		(*it)->parent = nullptr;

		if ((*it)->GetIsStatic())
			DeleteGOFromStaticList((*it));

		DeleteGameObjectFromList((*it));

		if ((*it) != App->camera->GetGameCameraObject())
		{
			delete (*it);
			(*it) = nullptr;
		}

		it = go_to_delete.erase(it);
	}
}

void Scene::CleanScene()
{
	if (!scene_gameobjects.empty())
	{
		for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
		{
			if ((*it)->GetParent() == nullptr)
				(*it)->DeleteRecursive();
		}

		App->scene->DeleteGameObjectsNow();
	}

}

void Scene::AddGameObjectToDeleteList(GameObject * to_del)
{
	go_to_delete.push_back(to_del);
}

void Scene::AddGOToStaticList(GameObject * go)
{
	static_gameobjects.push_back(go);
}

void Scene::DeleteGOFromStaticList(GameObject * go)
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

GameObject * Scene::LoadPrefab(const char * prf_name)
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
		GameObject* new_go = new GameObject();
		new_go->Load(scene_obj, i);
		AddGameObjectToScene(new_go);
		obj_list.push_back(new_go);
	}

	auto it = obj_list.begin();
	root_go = (*it);

	stream.close();

	return root_go;
}

GameObject * Scene::CreateGameObject()
{
	GameObject* new_go = new GameObject("");

	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go;
}

GameObject * Scene::GetGameObjectByID(UID uid)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->unique_id == uid)
			return (*it);
	}

	return nullptr;
}

GameObject * Scene::GetGameObject(const char * name)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if (string((*it)->GetName()) == string(name))
			return (*it);
	}

	return nullptr;
}



std::list<GameObject*> Scene::GetAllGameObjectsWith(CompType type)
{
	std::list<GameObject*> to_ret;

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetComponent(type) != nullptr)
			to_ret.push_back((*it));
	}

	return to_ret;
}

GameObject * Scene::CreateGameObject(const char* name)
{
	GameObject* new_go = new GameObject(name);

	//Here we will add the component transform, every GO should have it.

	AddGameObjectToScene(new_go);


	return new_go;
}

GameObject * Scene::CreateGameObject(std::list<GameObject*> list_childs, const char* name = "")
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

int Scene::GetGameObjectsAmmount()
{
	return scene_gameobjects.size();
}

void Scene::DeleteGameObjectFromList(GameObject* go)
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

void Scene::AddGameObjectToScene(GameObject* go)
{
	scene_gameobjects.push_back(go);
	go->Start();
}

bool Scene::IsTextureUsed(int id, GameObject* skip)
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

void Scene::TestLineAgainstGOs(LineSegment line)
{
	list<GameObject*> intersected_list;

	auto go_iterator = scene_gameobjects.begin();

	while (go_iterator != scene_gameobjects.end()) {
		GameObject* go = (*go_iterator);

		if (!go->bounding_box)
		{
			go_iterator++;
			continue;
		}

		bool hit = line.Intersects(*go->bounding_box);

		if (hit)
		{
			intersected_list.push_back(go);
		}

		go_iterator++;
	}

	GameObject* closestGo = GetClosestGO(line, intersected_list);

	SetSelectedGameObject(closestGo);

}

void Scene::DrawGuizmo()
{
	if (selected_go) //Draw guizmos
	{
		ImVec2 scene_pos = App->imgui->scene_panel->GetPos();
		ImVec2 scene_size = App->imgui->scene_panel->GetSize();
		ImGuizmo::SetRect(scene_pos.x, scene_pos.y, scene_size.x, scene_size.y);

		ComponentTransform* trans = (ComponentTransform*)selected_go->GetComponent(CMP_TRANSFORM);

		float4x4 vmat = App->camera->GetEditorCamera()->GetRawViewMatrix();

		float4x4 object_matrix = trans->GetGlobalViewMatrix();

		object_matrix.Transpose();

		if (selected_go->GetIsStatic())
			ImGuizmo::Enable(false);
		else
			ImGuizmo::Enable(true);

		ImGuizmo::Manipulate(&vmat[0][0], App->camera->GetEditorCamera()->camera->GetProjectionMatrix(), (ImGuizmo::OPERATION)guizmo_mode, ImGuizmo::LOCAL, (float*)&object_matrix);

		object_matrix.Transpose();

		trans->SetGlobalViewMatrix(object_matrix);

	}
}

GameObject * Scene::GetClosestGO(LineSegment line, std::list<GameObject*> go_list)
{
	float3 closest_point;
	float closest_distance = 100000;
	GameObject* closest_go = nullptr;
	float distance;
	bool something_intersected = false;

	auto go_iterator = scene_gameobjects.begin();

	while (go_iterator != scene_gameobjects.end()) {
		GameObject* go = (*go_iterator);

		ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(CMP_MESH);

		if (mesh)
		{
			float3 point = { 0,0,0 };

			if (mesh->GetClosestIntersectionPoint(line, point, distance))
			{
				something_intersected = true;
				if (distance < closest_distance)
				{
					closest_distance = distance;
					closest_point = point;
					closest_go = go;
				}
			}
		}
		go_iterator++;
	}

	//if (something_intersected)
	//	CONSOLE_LOG("CLOSEST: x:%f, y:%f, z:%f distance:%f", closest_point.x, closest_point.y, closest_point.z, closest_distance);

	return closest_go;
}

void Scene::SetSelectedGameObject(GameObject * selected)
{
	if (App->imgui->inspector_panel == nullptr)
		return;

	App->imgui->inspector_panel->SetGameObject(selected);
	selected_go = selected;

}

GameObject* Scene::GetSelectedGameObject() const
{
	return selected_go;
}

void Scene::SetDefaultScene()
{
	//Create Empty GO with a camera
	GameObject* main_cam = CreateGameObject("Main Camera");

	ComponentCamera* cam = (ComponentCamera*)main_cam->AddComponent(CMP_CAMERA);
	cam->camera->frustum.farPlaneDistance = 1000;
	cam->is_editor = false;

}

void Scene::SaveScene(const char* scene_name)
{
	bool overwrite = false; 

	// Check if a meta with the same name exists
	Scene* scene_to_save = nullptr; 
	string meta_name = scene_name + string(".json.meta"); 
	string meta_path = App->file_system->GetScenesPath(); 

	if (!App->file_system->IsFileInDirectory(meta_path, meta_name.c_str()))
	{
		overwrite = true; 
		scene_to_save = (Scene*)App->resources->CreateNewResource(RES_MATERIAL); 

		scene_to_save->name = scene_name; 
		scene_to_save->path = App->file_system->GetLibraryPath() +  "\\Scenes" + scene_name + ".json"; 

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
		json_object_dotset_number(scene_obj, "Scene.obj_num", scene_gameobjects.size());
		json_object_dotset_number(scene_obj, "Scene.tags_num", 0);

		if (App->camera->GetGameCameraObject() != nullptr)
			json_object_dotset_number(scene_obj, "Scene.main_camera_uid", App->camera->GetGameCamera()->GetGameObject()->unique_id);
		else
			json_object_dotset_number(scene_obj, "Scene.main_camera_uid", 0);

		int index = 0;
		for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
		{
			scene_obj = json_value_get_object(scene_v);
			(*it)->Save(scene_obj, index++);
		}

		json_serialize_to_file(scene_v, new_scene_path.c_str());

		stream.close();
	}



}

void Scene::LoadScene(const char * scene_path, bool clean)
{
	string name_w_termination = scene_path;

	SetSceneName(name_w_termination.c_str());

	if (App->file_system->IsFileInDirectory(App->file_system->GetScenesPath().c_str(), name_w_termination.c_str()))
	{
		if (clean)
			CleanScene();

		string path = App->file_system->GetScenesPath() + std::string("\\") + name_w_termination;
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
			GameObject* new_go = new GameObject();

			if (new_go->Load(root_obj, i))
				AddGameObjectToScene(new_go);

			new_go->selected = false;
			i++;
		}

		if (main_cam_uid != 0)
			App->camera->SetGameCamera(App->scene->GetGameObjectByID(main_cam_uid));

		stream.close();
	}
}

const char * Scene::GetSceneName() const
{
	return scene_name.c_str();
}

void Scene::SetSceneName(const char * new_name)
{
	//If it has termination we delete it 
	string name(new_name);

	int pos = name.find_last_of('.');

	if (pos != 0)
	{
		int to_copy = name.length() - pos;
		name = name.substr(0, pos);
		scene_name = name;
		return;
	}

	scene_name = new_name;
}

// Update
update_status Scene::Update(float dt)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr || (*it)->IsActive() == false)
		{
			(*it)->Update();
		}
	}

	if (App->camera->frustum_culling) {

		std::list<UID> intersections_list_uid;

		if (octree->GetRoot() != nullptr && go_to_delete.empty())
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

	return UPDATE_CONTINUE;
}