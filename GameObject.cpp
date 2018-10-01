#include "GameObject.h"
#include "imgui_dock.h"
#include "imgui.h"
#include "Application.h"

#include "Component.h"
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
		if((*it)->GetType() != CMP_MATERIAL)
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
		if ((*it)->GetType() == cmp_type)
			return (*it); 
	}

	return nullptr; 
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetActive(bool activated)
{
	active = activated; 
}

bool GameObject::IsActive() const
{
	return active;
}

int GameObject::GetNumChilds()
{
	return (int)child_list.size();
}

bool GameObject::AddComponent(Component * new_cmp)
{
	if (new_cmp->GetType() == CMP_MATERIAL) 
	{
		ComponentMesh* mesh = (ComponentMesh*)GetComponent(CMP_RENDERER);

		if (mesh == nullptr)
		{
			CONSOLE_ERROR("Component Material can't be added to a GameObject with no Mesh");
			return false;
		}
		else
		{
			mesh->AssignMaterial((ComponentMaterial*)new_cmp); 
		}
	
	}

	component_list.push_back(new_cmp);
	return true; 
}

bool GameObject::AddChild(GameObject * child)
{
	child_list.push_back(child); 

	return true;
}

Component * GameObject::CreateComponent(CompType cmp_type)
{
	Component* new_cmp = nullptr; 

	switch (cmp_type)
	{
		case CMP_TRANSFORM:
			break; 

		case CMP_RENDERER:
			new_cmp = new ComponentMesh(); 
			new_cmp->SetGameObject(this); 
			break;

		case CMP_MATERIAL:
			new_cmp = new ComponentMaterial();
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


void GameObject::PrintHierarchyRecursive(int mask, int& node_clicked, int& id)
{
	ImGuiTreeNodeFlags node_flags; 
	id++;

	if (HasChilds())
	{
		node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx(name.c_str(), node_flags); 

		if (ImGui::IsItemClicked())
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
		node_flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ((mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0); // ImGuiTreeNodeFlags_Bullet;

		ImGui::TreeNodeEx(name.c_str(), node_flags);
		
		if (ImGui::IsItemClicked())
		{
			node_clicked = id;
			App->scene->SetSelectedGameObject(this);
			
		}
	}



}

bool GameObject::HasComponents()
{
	return !component_list.empty();
}

bool GameObject::HasChilds()
{
	return !child_list.empty();;
}
