#include "UI_InspectorPanel.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentMesh.h"

UI_InspectorPanel::UI_InspectorPanel()
{
}


UI_InspectorPanel::~UI_InspectorPanel()
{
}

bool UI_InspectorPanel::Start()
{
	show = true;
	gameobject = nullptr; 

	return true;
}

bool UI_InspectorPanel::Update()
{
	if (show == false)
		return false; 

	if (ImGui::BeginDock("Inspector", &show, NULL))
	{
		if (gameobject == nullptr)
		{
			ImGui::Text("No GameObject is Selected");
			ImGui::EndDock();			
			return false;
		}
			
		if (gameobject->HasComponents())
		{
			for (std::list<Component*>::iterator it = gameobject->component_list.begin(); it != gameobject->component_list.end(); it++)
			{
				PrintProperties((*it)->GetType());
			}
		}	
	}

	ImGui::EndDock();

	return true;
}

void UI_InspectorPanel::SetGameObject(GameObject * new_go)
{
	gameobject = new_go; 
	new_go->SelectGameObjectRecursive(); 
}

GameObject * UI_InspectorPanel::GetGameObject()
{
	return gameobject;
}



void UI_InspectorPanel::PrintProperties(CompType type)
{
	ImGui::Text("GameObject Info:");
	
	ImGui::Spacing(); 
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();



	ImGui::Spacing();
	ImGui::Separator();

	switch (type)
	{
	case CMP_TRANSFORM:
		//To Do
		break;

	case CMP_RENDERER:
		PrintMeshProperties();
		break;
	}
}


void UI_InspectorPanel::PrintMeshProperties()
{
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		ComponentMesh* mesh_cmp = (ComponentMesh*)GetGameObject()->GetComponent(CMP_RENDERER);
		mesh_cmp->PrintRenderSettings(); 
	}
}


