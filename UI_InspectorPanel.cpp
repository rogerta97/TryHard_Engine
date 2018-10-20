#include "UI_InspectorPanel.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

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

	if (ImGui::Begin("Inspector", &show, NULL))
	{
		//If there is not a current GO quit
		if (gameobject == nullptr)
		{
			ImGui::Text("No GameObject is Selected");
			ImGui::End();			
			return false;
		}

		//Print common GO info  --------------------------------------
		char* name_buf = (char*)gameobject->name.c_str(); 

		bool is_out = true; 
		bool is_active = gameobject->IsActive(); 

		ImGui::Spacing();

		Texture* go_image = App->resources->texture_importer->GetTexture("GameObjectIcon"); 

		if(go_image != nullptr)
			ImGui::Image((ImTextureID)go_image->GetTextureID(), ImVec2(22, 25));  ImGui::SameLine();

		if (ImGui::Checkbox(" ", &is_active))
			gameobject->SetActive(is_active);

		ImGui::SameLine(); 

		ImGui::InputText("Name", name_buf, 50); 

		ImGui::SameLine(); 

		if(ImGui::SmallButton("X"))
		{
			gameobject->DeleteRecursive(); 
			ImGui::End();
			return false; 
		}

		ImGui::Spacing(); 

		ImGui::Separator(); ImGui::Separator(); 

		ImGui::Spacing(); 

		if (is_out && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			App->camera->GetEditorCamera()->UnlockCamera();
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

		if (show_addcmp_ui)
		{
				static int curr_selection = 0;
			if (ImGui::Combo("CMP TYPE", &curr_selection, "Select Component\0Component Mesh\0Component Material\0Component Camera"))
			{

				switch (curr_selection)
				{
				case 0:
					break;

				case 1:
				{
					ComponentMesh* cmp_mesh = (ComponentMesh*)gameobject->AddComponent(CMP_RENDERER);					
					break;
				}


				case 2:
				{
					ComponentMaterial * cmp_mat = (ComponentMaterial*)gameobject->AddComponent(CMP_MATERIAL);			
					break;
				}

				case 3:
				{
					ComponentCamera * cmp_mat = (ComponentCamera*)gameobject->AddComponent(CMP_CAMERA);
					break;
				}


				}
				show_addcmp_ui = false;
			}
		}
		else
		{
			if (ImGui::Button("AddComponent", ImVec2(280, 25)))
			{
				show_addcmp_ui = true;
			}
		}		
	}

	ImGui::End();

	return true;
}

void UI_InspectorPanel::SetGameObject(GameObject * new_go)
{
	//Delete prev selected objects 
	if (gameobject != nullptr)
		gameobject->SetSelectedRecursive(false); 

	gameobject = new_go; 
	if(gameobject) new_go->SetSelectedRecursive(true); 
}

GameObject * UI_InspectorPanel::GetGameObject() const
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

	case CMP_CAMERA:
		if (ImGui::CollapsingHeader("Component Camera"))
		{			
			PrintCameraProperties((ComponentCamera*)GetGameObject()->GetComponent(CMP_CAMERA));
		}	
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

			ImGui::SmallButton("Local"); ImGui::SameLine(); ImGui::SmallButton("Global"); ImGui::SameLine(); 
			
			if(ImGui::SmallButton("Reset"))
			{
				trans_cmp->ResetTransform(); 
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float show_pos[3] = { trans_cmp->GetPosition().x, trans_cmp->GetPosition().y, trans_cmp->GetPosition().z };
			float show_rot[3] = { trans_cmp->GetRotationEuler().x, trans_cmp->GetRotationEuler().y, trans_cmp->GetRotationEuler().z };
			float show_scale[3] = { trans_cmp->GetScale().x, trans_cmp->GetScale().y, trans_cmp->GetScale().z };
		
			if (ImGui::DragFloat3("Position", show_pos, 0.2f))		
				trans_cmp->SetPosition({ show_pos[0], show_pos[1], show_pos[2] });
						
			if (ImGui::DragFloat3("Rotation", show_rot, 0.2f, -180.0f, 180.0f))
			{
				if(trans_cmp->GetRotationEuler().x != show_rot[0])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] }, "X");

				if (trans_cmp->GetRotationEuler().y != show_rot[1])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] }, "Y");

				if (trans_cmp->GetRotationEuler().z != show_rot[2])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] }, "Z");
			}
				

			if(ImGui::DragFloat3("Scale", show_scale, 0.2f))
				trans_cmp->SetScale({ show_scale[0], show_scale[1], show_scale[2] });

			ImGui::Spacing(); 
		}
	}
}


void UI_InspectorPanel::PrintMeshProperties()
{
	if (ImGui::CollapsingHeader("Component Mesh Renderer"))
	{
		ComponentMesh* mesh_cmp = (ComponentMesh*)GetGameObject()->GetComponent(CMP_RENDERER);

		ImGui::Spacing();
		ImGui::Checkbox("Active##1", &mesh_cmp->active);

		if (!mesh_cmp->active)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "INACTIVE");
		}

		ImGui::Separator(); 

		ImGui::Spacing();

		ImGui::Text("Current Mesh:"); ImGui::SameLine();

		if (mesh_cmp->GetMesh() != nullptr)
		{
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", mesh_cmp->GetMesh()->name.c_str());

			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();

			mesh_cmp->PrintRenderSettings();

			ImGui::SameLine();

			ImGui::Checkbox("Draw BB", &mesh_cmp->draw_bounding_box);

			ImGui::Spacing();
		}			
		else
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "*NONE*");


	/*	ImGui::SameLine(); 
		static bool show_mesh_explorer = false;
		if (ImGui::SmallButton("+##MeshResourceList"))
		{
			ImGui::OpenPopup("select_mesh");
			show_mesh_explorer = true; 
		}
	
		if (show_mesh_explorer)
		{
			App->resources->mesh_importer->DrawMeshList();
		}*/

			
	}
}

void UI_InspectorPanel::PrintMaterialProperties()
{
	if (ImGui::CollapsingHeader("Component Material"))
	{
		ComponentMaterial* mat_cmp = (ComponentMaterial*)GetGameObject()->GetComponent(CMP_MATERIAL);

		ImGui::Spacing();
		ImGui::Checkbox("Active##2", &mat_cmp->active);


		if (!mat_cmp->active)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "INACTIVE");
		}

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Diffuse Texture:"); ImGui::SameLine();

		if(mat_cmp->diffuse != nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", mat_cmp->diffuse->GetName().c_str()); 

		static bool show_tex_explorer = false;
		if (ImGui::SmallButton("Explore..."))
		{
			ImGui::OpenPopup("select_texture");
		}

		App->resources->texture_importer->DrawTextureList();

		ImGui::SameLine(); 

		ImGui::ImageButton((ImTextureID)App->resources->texture_importer->GetCheckerTexture()->GetTextureID(), ImVec2(15, 15), ImVec2(0, 1), ImVec2(1, 0), 1);

		if (ImGui::IsItemClicked())
		{
			Texture* check_tex = App->resources->texture_importer->GetCheckerTexture();
			mat_cmp->diffuse = check_tex;
		}

		
		ImGui::Spacing();
		
		if (mat_cmp->diffuse != nullptr)
		{
			ImGui::Image((ImTextureID)mat_cmp->diffuse->GetTextureID(), ImVec2(150, 150), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::SameLine();

			ImGui::Spacing();

			ImGui::TextWrapped("Path: "); ImGui::SameLine();
			ImGui::TextWrapped(/*ImVec4(1,1,0,1),*/ "%s", mat_cmp->diffuse->GetPath());

			ImGui::Text("Texture ID: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->diffuse->GetTextureID());

			ImGui::Text("Width:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->diffuse->GetWidth());

			ImGui::Text("Height:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->diffuse->GetHeight());
		}
	}		
}

void UI_InspectorPanel::PrintCameraProperties(ComponentCamera* camera_cmp)
{
	
	int selected_proj = camera_cmp->camera->frustum.type;
	std::string label = "Projection##" + camera_cmp->GetGameObject()->GetName();

	if (ImGui::Combo(label.c_str(), &selected_proj, "Perspective\0Orthogonal"))
	{
		switch (selected_proj)
		{
		case 0:
			camera_cmp->camera->frustum.type = PerspectiveFrustum;
			break; 

		case 1:
			camera_cmp->camera->frustum.type = OrthographicFrustum;
			break; 
		}
		
	}

	if (ImGui::InputFloat("Near Plane", &camera_cmp->camera->frustum.nearPlaneDistance, 0.1f))
	{

	}

	if (ImGui::InputFloat("Far Plane", &camera_cmp->camera->frustum.farPlaneDistance, 0.1f))
	{

	}

	if (ImGui::Checkbox("Frustum Culling", &camera_cmp->camera->frustum_culling))
	{

	}
}


void UI_InspectorPanel::DeleteElement(CompType type)
{

}


