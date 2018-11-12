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

	App->imgui->tag_panel->AddTag("Untagged");
	SetDefaultScene(); 
	scene_name = "Untitled"; 

	App->camera->SetGameCamera(App->scene->GetGameObject("Main Camera"));

	guizmo_mode = TRANSLATE;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleScene::DrawSceneGameObjects(GameObject* camera)
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

void ModuleScene::DeleteGameObjectsNow()
{
	for (auto it = go_to_delete.begin(); it != go_to_delete.end();)
	{
		(*it)->DeleteAllComponents();
		
		if ((*it)->parent != nullptr)
			(*it)->parent->DeleteChildFromList((*it));
				
		(*it)->parent = nullptr;
		
		DeleteGameObjectFromList((*it)); 

		if ((*it) != App->camera->GetGameCameraObject())
		{
			delete (*it);
			(*it) = nullptr;
		}
	
		it = go_to_delete.erase(it);			
	}
}

void ModuleScene::CleanScene()
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
		GameObject* new_go = new GameObject();
		new_go->Load(scene_obj, i);
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

	AddGameObjectToScene(new_go);


	return new_go; 
}

GameObject * ModuleScene::GetGameObjectByID(UID uid)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->unique_id == uid)
			return (*it);
	}

	return nullptr; 
}

GameObject * ModuleScene::GetGameObject(const char * name)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if (string((*it)->GetName()) == string(name))
			return (*it);
	}

	return nullptr;
}



std::list<GameObject*> ModuleScene::GetAllGameObjectsWith(CompType type)
{
	std::list<GameObject*> to_ret;

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetComponent(type) != nullptr)
			to_ret.push_back((*it)); 
	}

	return to_ret; 
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
	go->Start();
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

void ModuleScene::TestLineAgainstGOs(LineSegment line)
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

void ModuleScene::DrawGuizmo()
{
	if (selected_go) //Draw guizmos
	{
		ImVec2 scene_pos = App->imgui->scene_panel->GetPos();
		ImVec2 scene_size = App->imgui->scene_panel->GetSize();
		ImGuizmo::SetRect(scene_pos.x, scene_pos.y, scene_size.x, scene_size.y);

		ComponentTransform* trans = (ComponentTransform*)selected_go->GetComponent(CMP_TRANSFORM);
		float3 object_pos = trans->GetPosition();
		float3 object_scale = trans->GetScale();
		float3 object_rot_rad = trans->GetRotationEuler();
		float objectMatrix[16] =
		{ 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f };
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];

		matrixTranslation[0] = object_pos.x;
		matrixTranslation[1] = object_pos.y;
		matrixTranslation[2] = object_pos.z;

		matrixRotation[0] = object_rot_rad.x;
		matrixRotation[1] = object_rot_rad.y;
		matrixRotation[2] = object_rot_rad.z;

		matrixScale[0] = object_scale.x;
		matrixScale[1] = object_scale.y;
		matrixScale[2] = object_scale.z;

		float4x4 vmat = App->camera->GetEditorCamera()->GetRawViewMatrix();

		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, objectMatrix);
		ImGuizmo::Manipulate(&vmat[0][0], App->camera->GetEditorCamera()->camera->GetProjectionMatrix(), (ImGuizmo::OPERATION)guizmo_mode, ImGuizmo::WORLD, objectMatrix);

		ImGuizmo::DecomposeMatrixToComponents(objectMatrix, matrixTranslation, matrixRotation, matrixScale);

		switch (guizmo_mode)
		{
		case TRANSLATE:
			trans->SetPosition({ matrixTranslation[0],matrixTranslation[1],matrixTranslation[2] });
			break;
		case ROTATE:
			trans->SetRotationEuler({ matrixRotation[0],matrixRotation[1],matrixRotation[2] });
			break;
		case SCALE:
			trans->SetScale({ matrixScale[0],matrixScale[1],matrixScale[2] });
			break;
		case BOUNDS:
			break;
		default:
			break;
		}

	}
}

GameObject * ModuleScene::GetClosestGO(LineSegment line, std::list<GameObject*> go_list)
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
	//CONSOLE_LOG("CLOSEST: x:%f, y:%f, z:%f distance:%f", closest_point.x, closest_point.y, closest_point.z, closest_distance);
	return closest_go;
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

	ComponentCamera* cam = (ComponentCamera*)main_cam->AddComponent(CMP_CAMERA);
	cam->is_editor = false; 

}

void ModuleScene::SaveScene(const char* scene_name)
{
	//Create the path were the scene is going to be saved
	string new_scene_path = App->file_system->GetScenesPath() + std::string("\\") + std::string(scene_name) + std::string(".json");

	if (App->file_system->IsFileInDirectory(App->file_system->GetScenesPath().c_str(), scene_name))
	{
		CONSOLE_DEBUG("Scene '%s' already exist. Overwritting..."); 
	}
	else
	{
		//Create the new json file 
		std::ofstream stream;
		stream.open(new_scene_path, std::fstream::out);

		JSON_Value* scene_v = json_value_init_object();
		JSON_Object* scene_obj = json_value_get_object(scene_v);

		//Save Scene Info
		json_object_dotset_number(scene_obj, "Scene.obj_num", scene_gameobjects.size()); 
		json_object_dotset_number(scene_obj, "Scene.tags_num", 0);

		if(App->camera->GetGameCameraObject() != nullptr)
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

void ModuleScene::LoadScene(const char * scene_path, bool clean)
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
		while(i < obj_ammount)
		{
			string item_to_get = "GameObject_" + to_string(i); 
			GameObject* new_go = new GameObject(); 

			if (new_go->Load(root_obj, i))
				AddGameObjectToScene(new_go);

			new_go->selected = false;
			i++;
		}

		if(main_cam_uid != 0)
			App->camera->SetGameCamera(App->scene->GetGameObjectByID(main_cam_uid));

		stream.close(); 
	}
}

const char * ModuleScene::GetSceneName() const
{
	return scene_name.c_str();
}

void ModuleScene::SetSceneName(const char * new_name)
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
		App->scene->LoadScene("temp_scene.json");
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

// Update
update_status ModuleScene::Update(float dt)
{
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetParent() == nullptr || (*it)->IsActive() == false)
		{
			(*it)->Update(); 
		}
	}

	std::list<GameObject*> intersections_list;



	//if (octree->GetRoot() != nullptr)
	//	octree->Draw(); 


	if (octree->GetRoot() != nullptr)
	{

		octree->GetIntersections(intersections_list, *App->camera->GetGameCamera()->GetFrustum());

		for (auto it = intersections_list.begin(); it != intersections_list.end(); it++)
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



