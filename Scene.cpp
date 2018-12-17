#include "Scene.h"

#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"
#include "UI_TagPanel.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_TextInput.h"
#include "Primitive.h"
#include "OpenGL.h"
#include "Octree.h"
#include "DebugDraw.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentButton.h"
#include "ComponentText.h"
#include "ComponentImage.h"
#include "ComponentCanvas.h"
#include "ComponentTextInput.h"
#include "ComponentRectTransform.h"
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

	//Draw normal GameObjects
	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if((*it)->GetIsUI() == false)
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

	App->user_interface->CleanCanvasList();

}

void Scene::AddGameObjectToDeleteList(GameObject * to_del)
{
	go_to_delete.push_back(to_del);
}

void Scene::AddGameObjectToTransparencyList(GameObject * to_add)
{
	transparent_gameobjects.push_back(to_add); 
}

std::map<float, GameObject*> Scene::GetSortedGOList(GameObject * new_go)
{
	std::map<float, GameObject*> list_to_ret; 

	return list_to_ret;
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
	GameObject* root_go = new GameObject(prf_name, false);

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

GameObject * Scene::CreateUIElement(UI_Widgget_Type widdget, GameObject* force_parent)
{
	// Find a parent for the new UI Element
	GameObject* UI_parent = nullptr;
	UI_Canvas* canvas_container = nullptr; 
	ComponentCanvas* cmp_canvas = nullptr; 
	ComponentRectTransform* canvas_rtransform = nullptr; 

	if (force_parent != nullptr && force_parent->GetIsUI())
	{
		UI_parent = force_parent;
		cmp_canvas = (ComponentCanvas*)UI_parent->GetFirstParentWith(CMP_CANVAS)->GetComponent(CMP_CANVAS); 
		canvas_container = cmp_canvas->GetCanvas();
	}
		
	else
	{
		UI_parent = App->user_interface->GetLastCanvas();

		if (UI_parent)
		{
			cmp_canvas = (ComponentCanvas*)UI_parent->GetComponent(CMP_CANVAS);
			canvas_container = cmp_canvas->GetCanvas();
		}
		
	}
		
	if (UI_parent == nullptr)
	{
		// If there is no canvas in the scene we create a default one 
		GameObject* parent_canvas = new GameObject("Canvas", true);
		parent_canvas->AddComponent(CMP_CANVAS);

		App->user_interface->AddaptCanvasToScreen();
		App->scene->AddGameObjectToScene(parent_canvas);

		UI_parent = parent_canvas; 

		cmp_canvas = (ComponentCanvas*)UI_parent->GetComponent(CMP_CANVAS);
		canvas_container = cmp_canvas->GetCanvas();
	}

	canvas_rtransform = (ComponentRectTransform*)cmp_canvas->GetGameObject()->GetComponent(CMP_RECTTRANSFORM); 
	
	// Create the UI Element
	const char* name = "";
	GameObject* new_ui_go = new GameObject("PlaceHold", true);
	new_ui_go->SetParent(UI_parent);

	// Set a RectTransform more likely for text
	ComponentRectTransform* rtransform = (ComponentRectTransform*)new_ui_go->GetComponent(CMP_RECTTRANSFORM);

	switch (widdget)
	{
	case UI_Widgget_Type::UI_IMAGE:
	{
		new_ui_go->SetName("Image");
		ComponentImage* img = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img->GetImage()->SetCanvas(canvas_container);

		float2 size = canvas_rtransform->GetSizeFromPercentage(img->GetImage()->GetPercentage(), UI_IMAGE);
		rtransform->Resize(size); 

		break;
	}
	
	case UI_Widgget_Type::UI_BUTTON:
	
	{
		// Create the text of the button as child
		GameObject* button_text = nullptr;
		new_ui_go->SetName("Button");

		ComponentImage* img = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img->GetImage()->SetCanvas(canvas_container);

		ComponentButton* button_cmp = (ComponentButton*)new_ui_go->AddComponent(CMP_BUTTON);
		button_cmp->GetButton()->SetCanvas(canvas_container);

		float2 size = canvas_rtransform->GetSizeFromPercentage(img->GetImage()->GetPercentage(), UI_IMAGE);
		rtransform->Resize(size);

		button_text = CreateUIElement(UI_LABEL, new_ui_go);
		ComponentText* text_cmp = (ComponentText*)button_text->AddComponent(CMP_TEXT);
		button_cmp->GetButton()->SetCanvas(canvas_container);

		break;
	}
		
	
		
	case UI_Widgget_Type::UI_LABEL:
	{
		new_ui_go->SetName("Text");
		ComponentText* text_cmp = (ComponentText*)new_ui_go->AddComponent(CMP_TEXT);
		text_cmp->GetLabel()->SetCanvas(canvas_container);

		float2 size = canvas_rtransform->GetSizeFromPercentage(text_cmp->GetLabel()->GetPercentage(), UI_LABEL);
		rtransform->Resize(size);

		text_cmp->SetClipping(CLIP_TOPLEFT);

		break;
	}


	case UI_Widgget_Type::UI_INPUTFIELD:

	{
		new_ui_go->SetName("Input Field");

		ComponentImage* img_cmp = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img_cmp->GetImage()->SetCanvas(canvas_container);

		ComponentTextInput* input_cmp = (ComponentTextInput*)new_ui_go->AddComponent(CMP_TEXTINPUT);
		input_cmp->GetInputField()->SetCanvas(canvas_container);

		img_cmp->GetImage()->SetPercentage(0.20f); 

		float2 size = canvas_rtransform->GetSizeFromPercentage(img_cmp->GetImage()->GetPercentage(), UI_INPUTFIELD);
		rtransform->Resize(size);

		//Create 2 text childs 
		GameObject* placeholder_text_go = CreateUIElement(UI_LABEL, new_ui_go); 
		placeholder_text_go->SetName("");

		ComponentText* text_cmp = nullptr; 
		text_cmp = (ComponentText*)placeholder_text_go->GetComponent(CMP_TEXT);
		text_cmp->SetClipping(CLIP_MIDDLELEFT); 
		text_cmp->GetLabel()->SetText("Text here");
		text_cmp->TranslateEnclosedPlane(float3(7,0,0));  //TODO: it should move a certain percentage

		GameObject* text_go = CreateUIElement(UI_LABEL, new_ui_go);

		text_cmp = (ComponentText*)text_go->GetComponent(CMP_TEXT);
		text_cmp->SetClipping(CLIP_MIDDLELEFT);
		text_cmp->GetLabel()->SetText("");

		text_cmp->TranslateEnclosedPlane(float3(7, 0, 0));  //TODO: it should move a certain percentage

		
		
		break;
	}
	
	}

	cmp_canvas->AddElement(new_ui_go);

	new_ui_go->Start();
	AddGameObjectToScene(new_ui_go); 

	return new_ui_go;
}

void Scene::TestLineAgainstGOs(LineSegment line)
{
	list<GameObject*> intersected_list;

	auto go_iterator = scene_gameobjects.begin();

	while (go_iterator != scene_gameobjects.end()) {
		GameObject* go = (*go_iterator);


		ComponentRectTransform* rect_trans = (ComponentRectTransform*)go->GetComponent(CMP_RECTTRANSFORM);
		if (rect_trans)
		{
			intersected_list.push_back(go);
			go_iterator++;
			continue;
		}

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

void Scene::TestLineAgainstUIGOsForGame(LineSegment line)
{
	list<GameObject*> intersected_list;

	auto go_iterator = scene_gameobjects.begin();

	while (go_iterator != scene_gameobjects.end()) {
		GameObject* go = (*go_iterator);


		ComponentRectTransform* rect_trans = (ComponentRectTransform*)go->GetComponent(CMP_RECTTRANSFORM);
		if (rect_trans)
		{
			intersected_list.push_back(go);
		}
		go_iterator++;
	}

	GameObject* closestGo = GetClosestUIGOinGame(line, intersected_list);

	if (!closestGo)
		return;

	ComponentButton* button = (ComponentButton*)closestGo->GetComponent(CMP_BUTTON);
	if (button)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
			button->GetButton()->SetState(UI_ElementState::ELM_PRESSED);
		}
		else {
			button->GetButton()->SetState(UI_ElementState::ELM_HOVERED);
		}
	}

}

void Scene::DrawGuizmo()
{
	if (selected_go == nullptr)
		return; 

	ImVec2 scene_pos = App->imgui->scene_panel->GetPos();
	ImVec2 scene_size = App->imgui->scene_panel->GetSize();
	ImGuizmo::SetRect(scene_pos.x, scene_pos.y, scene_size.x, scene_size.y);

	ComponentTransform* trans = nullptr;
	if (selected_go->GetIsUI() == false) //Draw guizmos
	{
		trans = (ComponentTransform*)selected_go->GetComponent(CMP_TRANSFORM);
	}
	else
	{
		ComponentRectTransform* rtransform = (ComponentRectTransform*)selected_go->GetComponent(CMP_RECTTRANSFORM);
		
		trans = rtransform->GetTransform(); 
		if (trans == nullptr)
		{
			CONSOLE_ERROR("Transform in RectTransform is NULLPTR"); 
			return;
		}
	}

	float4x4 vmat = App->camera->GetEditorCamera()->GetRawViewMatrix();

	float4x4 object_matrix = trans->GetGlobalViewMatrix();

	object_matrix.Transpose();

	if (selected_go->GetIsStatic())
		ImGuizmo::Enable(false);
	else
		ImGuizmo::Enable(true);

	ImGuizmo::Manipulate(&vmat[0][0], App->camera->GetEditorCamera()->camera->GetProjectionMatrix(), (ImGuizmo::OPERATION)guizmo_mode, ImGuizmo::LOCAL, (float*)&object_matrix);

	object_matrix.Transpose();

	ComponentRectTransform* rect_trans = (ComponentRectTransform*)selected_go->GetComponent(CMP_RECTTRANSFORM);
	
	if (!ImGuizmo::IsUsing())
		return;

	if (rect_trans)
	{
		float3 translate;
		float3 modified_translate;
		float3x3 rot;
		float3 scal;

		object_matrix.Decompose(modified_translate, rot, scal);

		trans->GetGlobalViewMatrix().Decompose(translate, rot, scal);

		float3 trans_diff = modified_translate - translate;

		float2 distance = rect_trans->GetRelativePos();

		distance += {trans_diff.x, trans_diff.y};

		//CONSOLE_LOG("translate%f", translate.x);
		//CONSOLE_LOG("transform%f", trans->transform.position.x);
		//CONSOLE_LOG("diff%f", trans_diff.x);

		rect_trans->SetRelativePos(distance);
	}
	else {
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

		ComponentRectTransform* rect_trans = (ComponentRectTransform*)go->GetComponent(CMP_RECTTRANSFORM);
		if (rect_trans)
		{
			float3 point = { 0,0,0 };
			if (rect_trans->GetClosestIntersectionPoint(line, point, distance))
			{
				something_intersected = true;
				if (distance < closest_distance || go->GetComponent(CMP_CANVAS) == nullptr)
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

GameObject * Scene::GetClosestUIGOinGame(LineSegment line, std::list<GameObject*> go_list)
{
	float3 closest_point;
	float closest_distance = 100000;
	GameObject* closest_go = nullptr;
	float distance;
	bool something_intersected = false;

	auto go_iterator = scene_gameobjects.begin();

	while (go_iterator != scene_gameobjects.end()) {
		GameObject* go = (*go_iterator);

		ComponentRectTransform* rect_trans = (ComponentRectTransform*)go->GetComponent(CMP_RECTTRANSFORM);
		if (rect_trans)
		{
			float3 point = { 0,0,0 };
			if (rect_trans->GetClosestIntersectionPointForGame(line, point, distance))
			{
				something_intersected = true;
				if (distance < closest_distance || go->GetComponent(CMP_CANVAS) == nullptr)
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

	// Try to create a canvas GO
	GameObject* parent_canvas = new GameObject("Canvas", true); 
	parent_canvas->AddComponent(CMP_CANVAS);
	AddGameObjectToScene(parent_canvas); 
}

void Scene::SaveScene(const char* scene_name)
{
	
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
			GameObject* new_go = new GameObject("", false);

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
