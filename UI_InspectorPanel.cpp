#include "UI_InspectorPanel.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

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
		//If there is not a current GO quit
		if (gameobject == nullptr)
		{
			ImGui::Text("No GameObject is Selected");
			ImGui::EndDock();			
			return false;
		}

		//Print common GO info  --------------------------------------
		char* name_buf = (char*)gameobject->name.c_str(); 

		bool is_out = true; 
		bool is_active = gameobject->IsActive(); 

		ImGui::Spacing();

		ImGui::Image((ImTextureID)App->resources->texture_importer->GetTexture("GameObjectIcon.png")->GetTextureID(), ImVec2(45, 38));  ImGui::SameLine(); 

		ImGui::InputText("Name", name_buf, 50); 

		ImGui::Spacing();

		if (ImGui::Checkbox("Active", &is_active))
			gameobject->SetActive(is_active); 

		ImGui::SameLine();

		if (ImGui::Checkbox("Static", &is_active))
			gameobject->SetActive(is_active);

		ImGui::Spacing(); 

		ImGui::Separator(); ImGui::Separator(); 

		ImGui::Spacing(); 

		if (ImGui::IsInputTextFocused())
		{
			App->camera->LockCamera();
			is_out = false;
		}

		if (is_out && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->camera->UnlockCamera();
		}

		// ------------------------------------------------------------------------------------
			
		; 
		if (gameobject->HasComponents())
		{
			for (std::list<Component*>::iterator it = gameobject->component_list.begin(); it != gameobject->component_list.end(); it++)
			{
				PrintProperties((*it)->GetType());
			}

			ImGui::Spacing();

			ImGui::Separator(); ImGui::Separator();

			ImGui::Spacing();
		}

		if (ImGui::Button("AddComponent", ImVec2(280, 25)))
		{
		
		}
	}

	ImGui::EndDock();

	return true;
}

void UI_InspectorPanel::SetGameObject(GameObject * new_go)
{
	//Delete prev selected objects 
	if (gameobject != nullptr)
		gameobject->SetSelectedRecursive(false); 

	gameobject = new_go; 
	new_go->SetSelectedRecursive(true); 
}

GameObject * UI_InspectorPanel::GetGameObject()
{
	return gameobject;
}

void UI_InspectorPanel::PrintProperties(CompType type)
{
	switch (type)
	{
	case CMP_TRANSFORM:
		//To Do
		break;

	case CMP_RENDERER:
		PrintMeshProperties();
		break;

	case CMP_MATERIAL:
		PrintMaterialProperties();
		break;
	}
}


void UI_InspectorPanel::PrintMeshProperties()
{
	if (ImGui::CollapsingHeader("Component Mesh Renderer"))
	{
		ComponentMesh* mesh_cmp = (ComponentMesh*)GetGameObject()->GetComponent(CMP_RENDERER);
		mesh_cmp->PrintRenderSettings(); 
	}
}

void UI_InspectorPanel::PrintMaterialProperties()
{
	if (ImGui::CollapsingHeader("Component Material"))
	{
		ComponentMaterial* mat_cmp = (ComponentMaterial*)GetGameObject()->GetComponent(CMP_MATERIAL);

		ImGui::Spacing();
		ImGui::Text("Diffuse: "); 
		
		ImGui::Image((ImTextureID)mat_cmp->GetDiffuseTexture()->GetTextureID(), ImVec2(150,150), ImVec2(0,1), ImVec2(1,0));

		ImGui::Spacing(); 

		ImGui::Text("Path: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1,1,0,1), "%s", mat_cmp->GetDiffuseTexture()->GetPath());

		ImGui::Text("Texture ID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetTextureID());
	
		ImGui::Text("Width:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetWidth());

		ImGui::Text("Height:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetHeight());

	}
		
}


