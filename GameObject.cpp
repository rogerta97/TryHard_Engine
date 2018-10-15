#include "GameObject.h"
#include "imgui_dock.h"
#include "imgui.h"
#include "Application.h"

#include "UI_InspectorPanel.h"
#include "UI_HierarchyPanel.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

#include "ComponentMaterial.h"

GameObject::GameObject()
{
	name = ""; 
	parent = nullptr; 
}

GameObject::GameObject(const char * name)
{
	this->name = name;
	parent = nullptr; 
	selected = false; 
}


GameObject::~GameObject()
{

}

void GameObject::Start()
{
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

void GameObject::SetParent(GameObject * new_parent)
{
	parent = new_parent; 

	if(new_parent != nullptr)
		new_parent->child_list.push_back(this); 
}

void GameObject::SetActive(bool activated)
{
	active = activated; 
}

bool GameObject::IsActive() const
{
	return active;
}



bool GameObject::IsUsingTexture(int id, bool& used)
{

	ComponentMaterial* mat = (ComponentMaterial*)GetComponent(CMP_MATERIAL);

	if (mat != nullptr && mat->diffuse != nullptr)
	{
		if (mat->diffuse->GetTextureID() == id)
			return true;
		else
			return false;
	}

}

int GameObject::GetNumChilds()
{
	return (int)child_list.size();
}

bool GameObject::AddComponent(Component * new_cmp)
{
	//First we should check if it exist
	Component* tmp_cmp = GetComponent(new_cmp->GetType());

	if (tmp_cmp != nullptr)
	{
		CONSOLE_ERROR("GameObject '%s' already has this component assigned", name.c_str()); 
	}
	else
	{
		if (new_cmp->GetType() == CMP_MATERIAL)
		{
			ComponentMesh* mesh = (ComponentMesh*)GetComponent(CMP_RENDERER);

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
	return true; 
}

bool GameObject::AddChild(GameObject * child)
{
	child_list.push_back(child); 

	return true;
}

void GameObject::DeleteGameObject()
{

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
			(*it)->DeleteRecursive();
		}

	}
 
	App->scene->AddGameObjectToDeleteList(this);

}

void GameObject::DeleteAllComponents()
{
	for (auto it = component_list.begin(); it != component_list.end();)
	{
		(*it)->CleanUp();
		delete (*it);
		it = component_list.erase(it);
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


Component * GameObject::CreateComponent(CompType cmp_type)
{
	Component* new_cmp = nullptr; 

	switch (cmp_type)
	{
		case CMP_TRANSFORM:
			new_cmp = new ComponentTransform();
			new_cmp->SetGameObject(this);
			break; 

		case CMP_RENDERER:
			new_cmp = new ComponentMesh(); 
			new_cmp->SetGameObject(this); 
			break;

		case CMP_MATERIAL:
			new_cmp = new ComponentMaterial();
			new_cmp->SetGameObject(this);
			break;

		case CMP_CAMERA:
			new_cmp = new ComponentCamera();
			new_cmp->SetGameObject(this);
			break;
	}

	return new_cmp; 
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

	if (HasChilds())
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx(name.c_str(), node_flags); 

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
		{
			node_clicked = id;
			App->scene->SetSelectedGameObject(this); 			
		}

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

		ImGui::TreeNodeEx(name.c_str(), node_flags);
		
		
		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
		{
			node_clicked = id;
			App->scene->SetSelectedGameObject(this);		
		}
	
	}
	return ret; 
}

bool GameObject::HasComponents()  
{
	return  !component_list.empty();
}

bool GameObject::HasChilds() const
{
	return !child_list.empty();;
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

void GameObject::DeleteChildFromList(GameObject * child_to_delete)
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

void GameObject::SetCenterCamDataRecursive(float3 & position_amm, float & distance_amm)
{
	ComponentMesh* cmp_mesh = (ComponentMesh*)GetComponent(CMP_RENDERER);

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
