#include "Scene.h"

#include "Globals.h"
#include "Material.h"
#include "Application.h"
#include "Scene.h"
#include "UI_InspectorPanel.h"
#include "UI_ScenePanel.h"
#include "UI_TagPanel.h"
#include "UI_Button.h"
#include "UI_GamePanel.h"
#include "UI_Label.h"
#include "UI_CheckBox.h"
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
#include "ComponentCheckbox.h"
#include "ComponentTextInput.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvasScaler.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ModuleImGui.h"

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
		if ((*it)->GetIsUI() == false)
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

std::list<GameObject*> Scene::GetAllObjectsWithTag(const std::string & tag_to_search)
{
	std::list<GameObject*> to_ret_list = std::list<GameObject*>(); 

	for (auto it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->GetTag() == tag_to_search)
			to_ret_list.push_back((*it)); 
	}

	return to_ret_list; 
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

GameObject * Scene::CreateUIElement(UI_Widgget_Type widdget, GameObject* force_parent, bool add_to_scene)
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

	// Get the own recttransform & canvas
	ComponentRectTransform* rtransform = (ComponentRectTransform*)new_ui_go->GetComponent(CMP_RECTTRANSFORM);
	ComponentCanvasScaler* canvas_scaler = (ComponentCanvasScaler*)cmp_canvas->GetGameObject()->GetComponent(CMP_CANVASSCALER);

	//For the creating the element with the right size

	switch (widdget)
	{
	case UI_Widgget_Type::UI_IMAGE:
	{
		new_ui_go->SetName("Image");
		ComponentImage* img = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img->GetImage()->SetCanvas(canvas_container);

		float2 size;

		size = img->default_size;


		rtransform->Resize(size);
		rtransform->rel_size = float2(2.5, 2.5);

		if (add_to_scene)
		{
			cmp_canvas->AddElement(new_ui_go);
			AddGameObjectToScene(new_ui_go);
		}

		break;
	}

	case UI_Widgget_Type::UI_BUTTON:

	{
		// Create the text of the button as child
		GameObject* button_text = nullptr;
		new_ui_go->SetName("Button");

		// Img background ------
		ComponentImage* img = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img->GetImage()->SetCanvas(canvas_container);

		// Button -------------- 
		ComponentButton* button_cmp = (ComponentButton*)new_ui_go->AddComponent(CMP_BUTTON);
		button_cmp->GetButton()->SetCanvas(canvas_container);

		img->GetImage()->SetPercentage(0.30f);
		float2 size = button_cmp->default_size;
		rtransform->Resize(size);

		rtransform->rel_size = float2(8.0, 2.0);
		
		// Child Text -----------

		button_text = CreateUIElement(UI_LABEL, new_ui_go, false);
		ComponentText* text_cmp = (ComponentText*)button_text->AddComponent(CMP_TEXT);
		button_cmp->GetButton()->SetCanvas(canvas_container);
		text_cmp->GetLabel()->SetText("Button");
		text_cmp->SetClipping(CLIP_CENTER);

		if (add_to_scene)
		{
			cmp_canvas->AddElement(new_ui_go);
			cmp_canvas->AddElement(button_text);
			AddGameObjectToScene(new_ui_go);
			AddGameObjectToScene(button_text);
		}

		break;
	}



	case UI_Widgget_Type::UI_LABEL:
	{
		new_ui_go->SetName("Text");
		ComponentText* text_cmp = (ComponentText*)new_ui_go->AddComponent(CMP_TEXT);
		text_cmp->GetLabel()->SetCanvas(canvas_container);

		float2 size = text_cmp->default_size;
		rtransform->Resize(size);
		rtransform->rel_size = float2(6, 1);

		text_cmp->SetClipping(CLIP_TOPLEFT);

		if (add_to_scene)
		{
			cmp_canvas->AddElement(new_ui_go);
			AddGameObjectToScene(new_ui_go);
		}


		break;
	}


	case UI_Widgget_Type::UI_INPUTFIELD:
	{
		new_ui_go->SetName("Input Field");

		// Background image -----------------------------
		ComponentButton* but_cmp = (ComponentButton*)new_ui_go->AddComponent(CMP_BUTTON);
		but_cmp->GetButton()->SetCanvas(canvas_container);
		but_cmp->GetButton()->SetPercentage(0.30f);

		ComponentImage* img_cmp = (ComponentImage*)new_ui_go->AddComponent(CMP_IMAGE);
		img_cmp->GetImage()->SetCanvas(canvas_container);
		img_cmp->GetImage()->SetPercentage(0.30f);

		float2 size = but_cmp->default_size;
		rtransform->Resize(size);

		rtransform->rel_size = float2(7.5, 2.0);

		ComponentTextInput* input_cmp = (ComponentTextInput*)new_ui_go->AddComponent(CMP_TEXTINPUT);
		input_cmp->GetInputField()->SetCanvas(canvas_container);

		// Create 2 text childs ---------------------------
		// 1 ---

		GameObject* placeholder_text_go = CreateUIElement(UI_LABEL, new_ui_go, false);
		placeholder_text_go->SetName("PlaceHolder"); 

		ComponentText* text_cmp = (ComponentText*)placeholder_text_go->AddComponent(CMP_TEXT);
		text_cmp->GetLabel()->SetCanvas(canvas_container);
		text_cmp->GetLabel()->SetText("Text Here...");
		text_cmp->GetLabel()->section = { 0, 12 }; 
		text_cmp->SetClipping(CLIP_MIDDLELEFT);
		text_cmp->GetLabel()->color = { 0.3f, 0.3f, 0.3f };

		ComponentRectTransform* text_r_trans = (ComponentRectTransform*)placeholder_text_go->GetComponent(CMP_RECTTRANSFORM);
		text_r_trans->rel_size = float2(2.5, 0.3);

		// 2 ---
		GameObject* text_go = CreateUIElement(UI_LABEL, new_ui_go, false);
		ComponentText* text_show_cmp = (ComponentText*)text_go->AddComponent(CMP_TEXT);
		text_show_cmp->GetLabel()->SetCanvas(canvas_container);
		text_show_cmp->GetLabel()->SetText("");
		text_show_cmp->GetLabel()->section = { 0, 12 };
		text_show_cmp->SetClipping(CLIP_MIDDLELEFT);

		input_cmp->GetInputField()->SetPlaceHolderText(placeholder_text_go);
		input_cmp->GetInputField()->SetShowText(text_go);

		if (add_to_scene)
		{
			AddGameObjectToScene(new_ui_go);
			cmp_canvas->AddElement(new_ui_go);

			AddGameObjectToScene(placeholder_text_go);
			cmp_canvas->AddElement(placeholder_text_go);

			AddGameObjectToScene(text_go);
			cmp_canvas->AddElement(text_go);
		}

		text_go->SetActive(false);

		break;
	}

	case UI_Widgget_Type::UI_CHECKBOX:
	{
		new_ui_go->SetName("CheckBox");

		ComponentCheckBox* check_cmp = (ComponentCheckBox*)new_ui_go->AddComponent(CMP_CHECKBOX);
		check_cmp->GetCheckBox()->SetCanvas(canvas_container);

		float2 size = check_cmp->default_size;
		rtransform->Resize(size);

		// Create the child with the button and the image 

		// Button ---------------
		GameObject* background_go = new GameObject("Background", true);
		background_go->SetParent(new_ui_go);

		ComponentImage* img_cmp = (ComponentImage*)background_go->AddComponent(CMP_IMAGE);
		img_cmp->GetImage()->SetCanvas(canvas_container);

		ComponentButton* button_cmp = (ComponentButton*)background_go->AddComponent(CMP_BUTTON);
		button_cmp->GetButton()->SetCanvas(canvas_container);

		ComponentRectTransform* background_rtransform = (ComponentRectTransform*)background_go->GetComponent(CMP_RECTTRANSFORM); 
		background_rtransform->Resize({ size.y - 5, size.y  - 5});
		background_rtransform->rel_size = float2(2, 2);

		// Checkmark -------------
		GameObject* checkmark_go = new GameObject("Checkmark", true);
		checkmark_go->SetParent(background_go);

		ComponentImage* img_check_cmp = (ComponentImage*)checkmark_go->AddComponent(CMP_IMAGE);
		img_check_cmp->GetImage()->SetCanvas(canvas_container);

		ComponentRectTransform* chekmark_rt = (ComponentRectTransform*)checkmark_go->GetComponent(CMP_RECTTRANSFORM);
		chekmark_rt->rel_size = float2(2, 2);

		// Set CheckMark Image
		Material* checkmark_material = (Material*)App->resources->Get(RES_MATERIAL, "Check_Mark");
	
		if (checkmark_material->reference_counting == 0)
			checkmark_material->LoadToMemory();

		checkmark_material->reference_counting++; 
		img_check_cmp->GetImage()->SetMaterial(checkmark_material);

		// Make image smaller
		ComponentRectTransform* check_rtransform = (ComponentRectTransform*)checkmark_go->GetComponent(CMP_RECTTRANSFORM);
		check_rtransform->Resize({ size.y - 15, size.y - 15});

		// Assign checkbox data 
		check_cmp->GetCheckBox()->SetChildButton(button_cmp->GetGameObject());
		check_cmp->GetCheckBox()->SetToggleImage(img_check_cmp->GetGameObject());

		float3 draw_point = rtransform->GetPointFromPercentage(check_cmp->GetBackgroundDistancePercentage());
		float real_x_position = (-rtransform->width / 2.0f) + draw_point.x;
		background_rtransform->SetRelativePos(float2(real_x_position, 0));
		
		// Label ----------------
		GameObject* label_go = new GameObject("Label", true);
		label_go->SetParent(new_ui_go);

		ComponentText* cmp_txt = (ComponentText*)label_go->AddComponent(CMP_TEXT);
		cmp_txt->GetLabel()->SetCanvas(canvas_container);

		cmp_txt->GetLabel()->SetText("CheckBox"); 

		ComponentRectTransform* labbel_rtransform = (ComponentRectTransform*)label_go->GetComponent(CMP_RECTTRANSFORM);
		labbel_rtransform->Resize({ size.x * 0.66f , size.y * 0.75f});

		labbel_rtransform->rel_size = float2(6.1, 0.3);

		cmp_txt->SetClipping(CLIP_MIDDLELEFT);

		draw_point = rtransform->GetPointFromPercentage(check_cmp->GetLabelPercentage());
		real_x_position = (-rtransform->width / 2.0f) + draw_point.x - cmp_txt->GetLabel()->GetOrigin().x;
		labbel_rtransform->SetRelativePos(float2(real_x_position, 0));
	
		 //Create the child with the text
		if (add_to_scene)
		{
			cmp_canvas->AddElement(new_ui_go);
			AddGameObjectToScene(new_ui_go);

			cmp_canvas->AddElement(background_go);
			AddGameObjectToScene(background_go);

			cmp_canvas->AddElement(label_go);
			AddGameObjectToScene(label_go);

			cmp_canvas->AddElement(checkmark_go);
			AddGameObjectToScene(checkmark_go);
		}


		break;
	}

	}
	new_ui_go->Start();

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
	ImVec2 mouse_pos = App->imgui->game_panel->GetMousePosInDockNormalized();


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
		list<GameObject*> intersected_list;

		auto go_iterator = scene_gameobjects.begin();

		while (go_iterator != scene_gameobjects.end())
		{
			GameObject* go = (*go_iterator);
			ComponentRectTransform* rect_trans = (ComponentRectTransform*)go->GetComponent(CMP_RECTTRANSFORM);

			if (rect_trans)
			{
				intersected_list.push_back(go);
			}

			go_iterator++;
		}

		GameObject* closestGo = GetClosestUIGOinGame(line, intersected_list);

		auto go_intersected_iterator = intersected_list.begin();
		while (go_intersected_iterator != intersected_list.end()) {
			GameObject* go = (*go_intersected_iterator);

				CONSOLE_LOG("%s", go->name.c_str());
			
			go_intersected_iterator++;
		}
	}
	//if (closestGo != nullptr)
	//{
	//	CONSOLE_LOG("%s", closestGo->name.c_str());
	//}

	//if (!closestGo)
	//	return;

	//ComponentButton* button = (ComponentButton*)closestGo->GetComponent(CMP_BUTTON);
	//if (button)
	//{
	//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) 
	//	{
	//		button->GetButton()->SetState(UI_ElementState::ELM_PRESSED);
	//		CONSOLE_LOG("PRESSED");
	//	}
	//	else 
	//	{
	//		button->GetButton()->SetState(UI_ElementState::ELM_HOVERED);
	//		CONSOLE_LOG("HOVER");
	//	}
	//}


	//ComponentTextInput* inputfield_cmp = (ComponentTextInput*)closestGo->GetComponent(CMP_TEXTINPUT);
	//if (inputfield_cmp)
	//{
	//	ComponentButton* inputtext_button = inputfield_cmp->GetButtonField();

	//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
	//		inputtext_button->GetButton()->SetState(UI_ElementState::ELM_PRESSED);
	//	}
	//	else {
	//		inputtext_button->GetButton()->SetState(UI_ElementState::ELM_HOVERED);
	//	}
	//}

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
	main_cam->SetTag("Main Camera"); 

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

void Scene::LoadScene(const char* scene_path, bool clean)
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

		std::multimap<UID, GameObject*> obj_to_redo_parent = std::multimap<UID, GameObject*>();

		int i = 0;
		while (i < obj_ammount)
		{
			string item_to_get = "GameObject_" + to_string(i);
			GameObject* new_go = new GameObject("", false);

			if (new_go->Load(root_obj, i, obj_to_redo_parent))
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
		if ((*it)->GetParent() == nullptr && ((*it)->GetIsUI() == false) || (*it)->GetComponent(CMP_CANVAS) == nullptr)
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
