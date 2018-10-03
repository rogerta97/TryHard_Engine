#include "UI_InspectorPanel.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentBoundingBox.h"

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
	show_addcmp_ui = false; 

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

		Texture* go_image = App->resources->texture_importer->GetTexture("GameObjectIcon"); 

		if(go_image != nullptr)
			ImGui::Image((ImTextureID)go_image->GetTextureID(), ImVec2(45, 38));  ImGui::SameLine();

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
			show_addcmp_ui = true; 
		}

		if (show_addcmp_ui)
		{
			SEPARATE_WITH_SPACE

			static int curr_selection = 0; 
			if (ImGui::Combo("CMP TYPE", &curr_selection, "Select Component\0Component Mesh\0Component Material\0Component Bounding Box\0"))
			{

				switch (curr_selection)
				{
				case 0:
					break; 

				case 1: 
				{
					ComponentMesh* cmp_mesh = (ComponentMesh*)gameobject->CreateComponent(CMP_RENDERER);
					gameobject->AddComponent((ComponentMesh*)cmp_mesh);
					break;
				}
					

				case 2:
				{
					ComponentMaterial * cmp_mat = (ComponentMaterial*)gameobject->CreateComponent(CMP_MATERIAL);
					gameobject->AddComponent((ComponentMaterial*)cmp_mat);
					break;
				}
					

				case 3:
				{
					ComponentBoundingBox * cmp_aabb = (ComponentBoundingBox*)gameobject->CreateComponent(CMP_BOUNDINGBOX);
					gameobject->AddComponent((ComponentBoundingBox*)cmp_aabb);
					break;
				}
					
					
				}
				show_addcmp_ui = false; 
			}

			SEPARATE_WITH_SPACE
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
		PrintTransformProperties();
		break;

	case CMP_RENDERER:
		PrintMeshProperties();
		break;

	case CMP_MATERIAL:
		PrintMaterialProperties();
		break;

	case CMP_BOUNDINGBOX:
		PrintBoundingBoxProperties();
		break;
	}
}

void UI_InspectorPanel::PrintTransformProperties()
{
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		ComponentTransform* trans_cmp = (ComponentTransform*)GetGameObject()->GetComponent(CMP_TRANSFORM);

		if (trans_cmp)
		{
			ImGui::Spacing();

			ImGui::SmallButton("Local"); ImGui::SameLine(); ImGui::SmallButton("Global");

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float show_pos[3] = { trans_cmp->GetPosition().x, trans_cmp->GetPosition().y, trans_cmp->GetPosition().z };
			float3 tmp_rot = trans_cmp->GetRotation().identity.ToEulerXYZ(); 
			float show_rot[3] = { tmp_rot.x, tmp_rot.y,tmp_rot.z };
			float show_scale[3] = { trans_cmp->GetScale().x, trans_cmp->GetScale().y, trans_cmp->GetScale().z };
		
			ImGui::InputFloat3("Position", show_pos, 2);
			ImGui::InputFloat3("Rotation", show_rot, 2);
			ImGui::InputFloat3("Scale", show_scale, 2);
		}
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

		ImGui::Text("Diffuse:"); ImGui::SameLine(); 

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", mat_cmp->GetDiffuseTexture()->GetName()); ImGui::SameLine(); 

		static bool show_tex_explorer = false;
		if (ImGui::SmallButton("Explore..."))
		{
			show_tex_explorer = true; 
		}

		if (show_tex_explorer)
		{		
			Texture* selected_tex = App->resources->texture_importer->DrawTextureList();

			if (selected_tex)
			{

			}

			
		}
		
		ImGui::Spacing();
		
		ImGui::Image((ImTextureID)mat_cmp->GetDiffuseTexture()->GetTextureID(), ImVec2(150,150), ImVec2(0,1), ImVec2(1,0));

		ImGui::Spacing(); 

		ImGui::TextWrapped("Path: "); ImGui::SameLine(); 
		ImGui::TextWrapped(/*ImVec4(1,1,0,1),*/ "%s", mat_cmp->GetDiffuseTexture()->GetPath());

		ImGui::Text("Texture ID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetTextureID());
	
		ImGui::Text("Width:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetWidth());

		ImGui::Text("Height:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetDiffuseTexture()->GetHeight());

	}
		
}

void UI_InspectorPanel::PrintBoundingBoxProperties()
{
	if (ImGui::CollapsingHeader("Component Bounding Box"))
	{
		ImGui::Text("Hi"); 
	}
}


