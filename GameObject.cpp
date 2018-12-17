#include "GameObject.h"
#include "imgui_dock.h"
#include "imgui.h"
#include "Application.h"
#include "JSON\parson.h"

#include "UI_InspectorPanel.h"
#include "UI_HierarchyPanel.h"
#include "UI_TagPanel.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentRectTransform.h"
#include "ComponentText.h"
#include "ComponentButton.h"
#include "ComponentCanvas.h"
#include "ComponentTextInput.h"
#include "ComponentImage.h"
#include "ComponentMesh.h"

#include "ComponentMaterial.h"

#include "UI_GamePanel.h"
#include "DebugDraw.h"

#include <fstream>


#include "mmgr\mmgr.h"



GameObject::GameObject(const char * name, bool _is_ui)
{
	this->name = name;
	SetIsUI(_is_ui);
	unique_id = App->file_system->GenerateUID();

	is_static = false;
	bounding_box = nullptr;
	parent = nullptr; 
	selected = false; 
	
	if (!GetIsUI())
	{
		Component* new_cmp = new ComponentTransform(this);
		component_list.push_back(new_cmp);
		transform = (ComponentTransform*)new_cmp;
	}
	else
	{
		ComponentRectTransform* new_cmp = (ComponentRectTransform*)AddComponent(CMP_RECTTRANSFORM);
		rect_transform = (ComponentRectTransform*)new_cmp;
		tag = "UI"; 
	}

}


GameObject::~GameObject()
{

}

void GameObject::Start()
{
	tag = App->imgui->tag_panel->GetTagByIndex(0);
}

void GameObject::Update()
{
 
	if (!active)
		return; 

	for (auto it = component_list.begin(); it != component_list.end(); it++)
	{
		if((*it)->GetType() != CMP_MATERIAL&& (*it)->active)
			(*it)->Update(); 
	}


	for (auto it = child_list.begin(); it != child_list.end(); it++)
	{
		(*it)->Update();
	}
}

void GameObject::Draw(bool is_editor)
{
	if (!active)
		return; 

	for (auto it = component_list.begin(); it != component_list.end(); it++)
	{
		(*it)->Draw(is_editor);
	}

}

GameObject * GameObject::GetFirstParentWith(CompType type)
{
	GameObject* to_ret = this; 

	while (to_ret->GetComponent(CMP_CANVAS) == nullptr)
		to_ret = to_ret->GetParent();

	return to_ret;
}

Component * GameObject::GetComponent(CompType cmp_type) const
{
	for (auto it = component_list.begin(); it != component_list.end(); it++)
	{
		if ((*it)->GetType() == cmp_type && (*it))
			return (*it); 
	}

	return nullptr; 
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

GameObject * GameObject::GetRootParent() 
{
	GameObject* to_ret = this; 

	while (to_ret->GetParent() != nullptr)
		to_ret = to_ret->GetParent(); 

	return to_ret; 
}

void GameObject::SetIsUI(bool newValue)
{
	is_ui = newValue;
}

bool GameObject::GetIsUI() const
{
	return is_ui;
}


string GameObject::GetTag() const
{
	return tag;
}

void GameObject::SetTag(string new_tag)
{
	tag = new_tag; 
}

void GameObject::SetParent(GameObject * new_parent)
{	
	if (parent == new_parent)
		return; 

	if (parent)
		parent->DeleteChildFromList(this);
	
	parent = new_parent;

	if (new_parent != nullptr)
		new_parent->child_list.push_back(this);
}

void GameObject::SetActive(bool activated)
{
	active = activated; 
}

void GameObject::SetStatic(bool set_static)
{
	if (set_static)
	{	if (!is_static)
		App->scene->AddGOToStaticList(this);
	}

	else
	{
		if (is_static)
		App->scene->DeleteGOFromStaticList(this);
	}
	
	is_static = set_static;

	auto child = GetChildList()->begin();
	while (child != GetChildList()->end())
	{
		(*child)->SetStatic(set_static);
		child++;
	}
}

bool GameObject::GetIsStatic()
{
	return is_static;
}

std::list<GameObject*>* GameObject::GetChildList()
{
	return &child_list;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const char* name)
{
	this->name = name; 
}

bool GameObject::IsActive() const
{
	return active;
}

bool GameObject::IsUsingTexture(int id, bool& used)
{

	ComponentMaterial* mat = (ComponentMaterial*)GetComponent(CMP_MATERIAL);

	if (mat != nullptr && mat->GetMaterial()->GetDiffuseTexture() != nullptr)
	{
		if (mat->GetMaterial()->GetDiffuseTexture()->GetTextureID() == id)
			return true;
		else
			return false;
	}

}

int GameObject::GetNumChilds()
{
	return (int)child_list.size();
}

Component* GameObject::AddComponent(CompType new_type)
{
	//First we should check if it exist
	Component* new_cmp = GetComponent(new_type);

	if (new_cmp != nullptr)
	{
		return GetComponent(new_type);
	}
	else
	{
		//First we create it 
		switch (new_type)
		{
			case CMP_TRANSFORM:
				new_cmp = new ComponentTransform(this);
				break;

			case CMP_MESH:
				new_cmp = new ComponentMesh(this);
				break;

			case CMP_MATERIAL:
				new_cmp = new ComponentMaterial(this);
				break;

			case CMP_CAMERA:
				new_cmp = new ComponentCamera(this);
				break;

			case CMP_CANVAS:
				new_cmp = new ComponentCanvas(this);
				break;

			case CMP_RECTTRANSFORM:
				new_cmp = new ComponentRectTransform(this);
				break;

			case CMP_IMAGE:
				new_cmp = new ComponentImage(this);
				break;

			case CMP_BUTTON:
				new_cmp = new ComponentButton(this);
				break;

			case CMP_TEXT:
				new_cmp = new ComponentText(this);
				break;

			case CMP_TEXTINPUT:
				new_cmp = new ComponentTextInput(this);
				break;
		}

		if (new_cmp->GetType() == CMP_MATERIAL)
		{
			ComponentMesh* mesh = (ComponentMesh*)GetComponent(CMP_MESH);

			if (new_cmp->GetType() == CMP_MATERIAL)
			{
				if (mesh == nullptr)
				{
					CONSOLE_ERROR("Component Material can not be added to a GameObject with no Mesh");
					return false;
				}
				else
				{
					mesh->AssignMaterial((ComponentMaterial*)new_cmp);
				}
			}
			else
			{
				if (mesh == nullptr)
				{
					CONSOLE_ERROR("Bounding Box can not be added to a GameObject with no Mesh");
					return false;
				}
			}
		}
	}

	component_list.push_back(new_cmp);
	new_cmp->Start(); 
	return new_cmp; 
}

void GameObject::AddComponentFromJSON(JSON_Object * cmp_obj, const char * cmp_name, UID prefab_uid)
{
	if (string(cmp_name) == string("ComponentTransform"))
	{
		ComponentTransform* new_transform = (ComponentTransform*)AddComponent(CMP_TRANSFORM);
		new_transform->Load(cmp_obj);		
		return;
	}

	if (string(cmp_name) == string("ComponentMesh"))
	{
		ComponentMesh* new_mesh = (ComponentMesh*)AddComponent(CMP_MESH);
		new_mesh->Load(cmp_obj, prefab_uid);
		new_mesh->UpdateBoundingBox();
		return;
	}

	if (string(cmp_name) == string("ComponentMaterial"))
	{
		ComponentMaterial* new_mat = (ComponentMaterial*)AddComponent(CMP_MATERIAL);
		new_mat->Load(cmp_obj);
		return;
	}

	if (string(cmp_name) == string("ComponentCamera"))
	{
		ComponentCamera* new_cam = (ComponentCamera*)AddComponent(CMP_CAMERA);
		new_cam->Load(cmp_obj);
		return;
	}

	if (string(cmp_name) == string("ComponentRectTransform"))
	{
		ComponentRectTransform* rtransform = (ComponentRectTransform*)AddComponent(CMP_RECTTRANSFORM);
		rtransform->Load(cmp_obj);
		return;
	}


	if (string(cmp_name) == string("ComponentCanvas"))
	{
		ComponentCanvas* cmp_canvas = (ComponentCanvas*)AddComponent(CMP_CANVAS);
		cmp_canvas->Load(cmp_obj);
		App->user_interface->AddCanvas(this); 
		return;
	}

	if (string(cmp_name) == string("ComponentImage"))
	{
		ComponentImage* cmp_img = (ComponentImage*)AddComponent(CMP_IMAGE);
		cmp_img->Load(cmp_obj);
		return;
	}

	if (string(cmp_name) == string("ComponentText"))
	{
		ComponentText* cmp_txt = (ComponentText*)AddComponent(CMP_TEXT);
		cmp_txt->Load(cmp_obj);
		return;
	}
}

bool GameObject::AddChild(GameObject * child)
{
	child_list.push_back(child); 

	return true;
}

void GameObject::DeleteGameObject()
{
	this->~GameObject();
}

void GameObject::DeleteRecursive()
{
	if (this == App->imgui->inspector_panel->GetGameObject())
	{
		App->scene->SetSelectedGameObject(nullptr);
	}

	if (HasChilds() > 0)
	{
		for (auto it = child_list.begin(); it != child_list.end(); it++)
		{		
			if ((*it)->GetIsStatic())
			{
				(*it)->SetStatic(false);

				if (App->scene->GetCurrentScene()->octree)
					App->scene->GetCurrentScene()->octree->Recalculate();
			}
			
			(*it)->DeleteRecursive();
		}
	}
 
	App->scene->AddGameObjectToDeleteList(this);

}

void GameObject::DeleteAllComponents()
{
	for (auto it = component_list.begin(); it != component_list.end();it++)
	{
		(*it)->CleanUp();
		delete (*it);	
	}

	component_list.clear(); 
}

bool GameObject::DeleteComponent(CompType cmp)
{
	for (auto it = component_list.begin(); it != component_list.end(); it++)
	{
		if ((*it)->GetType() == cmp)
		{
			(*it)->CleanUp();
			delete (*it);
			component_list.erase(it);
			return true;
		}
	}

	return false;
}

GameObject * GameObject::Duplicate()
{
	GameObject* to_ret = new GameObject(name.c_str(), false); 

	to_ret->selected = true; 
	to_ret->bounding_box = bounding_box; 
	to_ret->name = name; 
	to_ret->parent = parent; 
	to_ret->unique_id = unique_id; 

	

	return nullptr;
}

void GameObject::SetSelectedRecursive(bool selected)
{
	for (auto it = child_list.begin(); it != child_list.end(); it++)
	{
		(*it)->SetSelectedRecursive(selected);
	}

	this->selected = selected;
}


bool GameObject::PrintHierarchyRecursive(int mask, int& node_clicked, int& id)
{
	bool ret = false; 
	
	id++;

	string node_name = name + "##" + to_string(unique_id);

	if (HasChilds())
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
	
		bool opened = ImGui::TreeNodeEx(node_name.c_str(), node_flags);

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
		{
			node_clicked = id;
			App->scene->SetSelectedGameObject(this); 
			App->imgui->hierarchy_panel->want_to_drag = true;
			App->imgui->hierarchy_panel->source_in_drag = this;
		}

		App->imgui->hierarchy_panel->ManageDragAndDrop(this);

		if (ImGui::IsItemClicked(1))
			App->imgui->hierarchy_panel->show_click_menu = true;

		if (ImGui::IsMouseClicked(1))
			if(!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
				App->imgui->hierarchy_panel->show_create_menu = true; 

		if(opened)
		{
			for (auto it = child_list.begin(); it != child_list.end(); it++)
			{
				(*it)->PrintHierarchyRecursive(mask, node_clicked, id);
			}

			ImGui::TreePop(); 		
		}	
	}
	
	else
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ((mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0); // ImGuiTreeNodeFlags_Bullet;

		ImGui::TreeNodeEx(node_name.c_str(), node_flags);
		
		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
		{
			node_clicked = id;
			App->scene->SetSelectedGameObject(this);
			App->imgui->hierarchy_panel->want_to_drag = true; 
			App->imgui->hierarchy_panel->source_in_drag = this; 
		}

		if (ImGui::IsItemClicked(1))
			App->imgui->hierarchy_panel->show_click_menu = true;

		if (ImGui::IsMouseClicked(1))
			if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
				App->imgui->hierarchy_panel->show_create_menu = true;

		App->imgui->hierarchy_panel->ManageDragAndDrop(this);
			
	}

	

	return ret; 
}

void GameObject::Save(JSON_Object* scene_obj, int index)
{
	string node_name = "GameObject_" + to_string(index);
	string item_name = ""; 

	item_name = node_name + ".Name"; 
	json_object_dotset_string(scene_obj, item_name.c_str(), name.c_str());

	item_name = node_name + ".UID";
	json_object_dotset_number(scene_obj, item_name.c_str(), unique_id);

	item_name = node_name + ".IsUI";
	json_object_dotset_boolean(scene_obj, item_name.c_str(), GetIsUI());

	item_name = node_name + ".Parent";

	if(GetParent() == nullptr)
		json_object_dotset_number(scene_obj, item_name.c_str(), 0);
	else
		json_object_dotset_number(scene_obj, item_name.c_str(), GetParent()->unique_id);

	node_name += ".Components"; 

	for(auto it = component_list.begin(); it != component_list.end(); it++)
		(*it)->Save(scene_obj, node_name.c_str());	
}

void GameObject::SaveRecursive(JSON_Object* scene_obj, int& index)
{
	string node_name = "GameObject_" + to_string(index);
	string item_name = "";

	item_name = node_name + ".Name";
	json_object_dotset_string(scene_obj, item_name.c_str(), name.c_str());

	item_name = node_name + ".UID";
	json_object_dotset_number(scene_obj, item_name.c_str(), unique_id);

	item_name = node_name + ".IsUI";
	json_object_dotset_boolean(scene_obj, item_name.c_str(), GetIsUI());

	item_name = node_name + ".Parent";

	if (GetParent() == nullptr)
		json_object_dotset_number(scene_obj, item_name.c_str(), 0);
	else
		json_object_dotset_number(scene_obj, item_name.c_str(), GetParent()->unique_id);

	node_name += ".Components";

	for (auto it = component_list.begin(); it != component_list.end(); it++)
		(*it)->Save(scene_obj, node_name.c_str());
	
	index++; 
	for (auto it = child_list.begin(); it != child_list.end(); it++)
		(*it)->SaveRecursive(scene_obj, index);
	
}

bool GameObject::Load(JSON_Object* scene_obj, int index, UID prefab_uid)
{
	//Load basic GO info
	string node_name = "GameObject_" + to_string(index);

	//Check if the GO exist (to know if we end up loading)
	if (json_object_get_value(scene_obj, node_name.c_str()) == nullptr) 
		return false;
		
	scene_obj = json_object_get_object(scene_obj, node_name.c_str()); 
	
	//If it exist load it's base properties
	name = json_object_dotget_string(scene_obj, "Name");
	unique_id = json_object_dotget_number(scene_obj, "UID");
	is_ui = json_object_dotget_boolean(scene_obj, "IsUI"); 

	UID parent_id = json_object_dotget_number(scene_obj, "Parent");

	if (parent_id != 0)	
		SetParent(App->scene->GetGameObjectByID(parent_id)); 

	//Load Components, if it has any
	if (json_object_get_value(scene_obj, "Components") == nullptr) 
		return true;
			
	scene_obj = json_object_get_object(scene_obj, "Components");
	for (int i = 0; i < json_object_get_count(scene_obj); i++)
	{
		string cmp_name = json_object_get_name(scene_obj, i);
		JSON_Object* curr_cmp_obj = json_object_get_object(scene_obj, cmp_name.c_str());
		AddComponentFromJSON(curr_cmp_obj, cmp_name.c_str(), prefab_uid);
	}
	
	return true; 
}

void GameObject::SaveAsPrefab()
{
	string dest_str = App->file_system->GetPrefabPath() + string("\\") + name.c_str() + ".jprefab"; 

	std::ofstream stream;
	stream.open(dest_str, std::fstream::out);

	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	int index = 0;
	SaveRecursive(scene_obj, index); 

	json_object_dotset_number(scene_obj, "Info.obj_num", index);

	json_serialize_to_file(scene_v, dest_str.c_str());

	stream.close(); 
}

void GameObject::LoadPrefab(const char* prefab_name)
{
	
}

void GameObject::ModifyIDSet()
{
	unique_id = App->resources->GenerateUID();

	if (!child_list.empty())
		for (auto it = child_list.begin(); it != child_list.end(); it++)				
			(*it)->ModifyIDSet();
			
}

bool GameObject::HasComponents()  
{
	return  !component_list.empty();
}

bool GameObject::HasChilds() const
{
	return !child_list.empty();
}

GameObject * GameObject::GetChild(int index) const
{
	int i = 0; 
	for (auto it = child_list.begin(); it != child_list.end(); it++, i++)
	{
		if (i == index)
			return (*it);
	}

	return nullptr; 
}

GameObject * GameObject::GetChild(const char * name) const
{
	int i = 0;
	for (auto it = child_list.begin(); it != child_list.end(); it++, i++)
	{
		if (string(name) == string((*it)->GetName()))
			return (*it);
	}

	return nullptr;
}

GameObject * GameObject::GetChild(UID id) const
{
	int i = 0;
	for (auto it = child_list.begin(); it != child_list.end(); it++, i++)
	{
		if ((*it)->unique_id == id)
			return (*it);
	}

	return nullptr;
}

void GameObject::DeleteChildFromList(GameObject * child_to_delete)
{
	if (child_list.empty() == false)
	{
		for (auto it = child_list.begin(); it != child_list.end(); it++)
		{
			if ((*it) == child_to_delete)
			{
				child_list.erase(it);
				return;
			}
		}
	}

}

void GameObject::GetEnclosedAABB(float3& min, float3& max)
{
	if (HasChilds())
	{
		for (auto it = child_list.begin(); it != child_list.end(); it++)
		{
			(*it)->GetEnclosedAABB(min, max);
		}
	}

	ComponentMesh* curr_mesh = (ComponentMesh*)GetComponent(CMP_MESH);

	//if the object don't has mesh and it's ui, i should have a rect transform. 
	if (curr_mesh == nullptr)
	{
		if (GetIsUI())
		{
			ComponentRectTransform* rtrans = (ComponentRectTransform*)GetComponent(CMP_RECTTRANSFORM);
			curr_mesh = rtrans->GetRectQuadComponent();
		}			
	}

	if (curr_mesh)
	{
		curr_mesh->CheckAABBPoints(min, max);
	}
	else
	{
		ComponentTransform* trans = (ComponentTransform*)GetComponent(CMP_TRANSFORM);

		if (trans->GetPosition().Distance({ 0,0,0 }) >= max.Distance({ 0,0,0 }))
			max = trans->GetPosition(); 

		else if (trans->GetPosition().Distance({ 0,0,0 }) >= min.Distance({ 0,0,0 }))
			min = trans->GetPosition();
	}

}


void GameObject::SetCenterCamDataRecursive(float3 & position_amm, float & distance_amm)
{
	ComponentMesh* cmp_mesh = (ComponentMesh*)GetComponent(CMP_MESH);

	if (cmp_mesh != nullptr)
	{
		position_amm += cmp_mesh->bounding_box.CenterPoint();
		distance_amm += cmp_mesh->bounding_box.Diagonal().Length();
	}

	if (HasChilds())
	{
		for (auto it = child_list.begin(); it != child_list.end(); it++)
		{
			(*it)->SetCenterCamDataRecursive(position_amm, distance_amm);
		}			
	}
}

void GameObject::GetGOAmount(int& counter)
{
	counter++;

	for (auto it = child_list.begin(); it != child_list.end(); it++)
	{
		(*it)->GetGOAmount(counter);
	}
}
