#include "UI_InspectorPanel.h"
#include "UI_TagPanel.h"
#include "UI_Label.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentRectTransform.h"
#include "ComponentText.h"
#include "ComponentImage.h"
#include "ComponentCamera.h"

#include "MaterialImporter.h"
#include "MeshImporter.h"
#include "ModuleScene.h"

#include "ImGuizmo/ImGuizmo.h"

UI_InspectorPanel::UI_InspectorPanel()
{
	name = "Inspector";

	show = true;
	gameobject = nullptr;
	show_addcmp_ui = false;

	show_tex_explorer = false;
	show_mesh_explorer = false;
}


UI_InspectorPanel::~UI_InspectorPanel()
{
}

bool UI_InspectorPanel::Start()
{


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

		Material* go_image = (Material*)App->resources->Get(RES_MATERIAL, "GameObjectIcon");
	
		if(go_image != nullptr)
			ImGui::Image((ImTextureID)go_image->GetDiffuseTexture()->GetTextureID(), ImVec2(22, 25), ImVec2(0, 1), ImVec2(1, 0));  ImGui::SameLine();

		ImGui::SameLine(); 

		ImGui::InputText("Name", name_buf, 50); 

		ImGui::SameLine(); 

		if(ImGui::SmallButton("X"))
		{
			gameobject->DeleteRecursive(); 
			ImGui::End();
			return false; 
		}

		if (ImGui::Checkbox("Active", &is_active))
			gameobject->SetActive(is_active);

		ImGui::SameLine();

		bool tmp_is_static = gameobject->GetIsStatic();
		if (ImGui::Checkbox("Static", &tmp_is_static)) 
		{
			gameobject->SetStatic(tmp_is_static);
			App->scene->current_scene->octree->Recalculate();
		}

		ImGui::SameLine();

		string but_tex = gameobject->GetTag().c_str();
		ImGui::Button(but_tex.c_str()); ImGui::SameLine();

		if (ImGui::Button("+##TagButton"))
		{
			App->imgui->tag_panel->show = true; 
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
					ComponentMesh* cmp_mesh = (ComponentMesh*)gameobject->AddComponent(CMP_MESH);					
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

	if(gameobject)
		new_go->SetSelectedRecursive(true); 
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

	case CMP_MESH:
		PrintMeshProperties();
		break;

	case CMP_MATERIAL:
		PrintMaterialProperties();
		break;

	case CMP_CAMERA:
		if (ImGui::CollapsingHeader("Camera"))
		{			
			PrintCameraProperties((ComponentCamera*)GetGameObject()->GetComponent(CMP_CAMERA));
		}	
		break;

	case CMP_RECTTRANSFORM:
		PrintRectTransformProperties();
		break;

	case CMP_CANVAS:
		PrintCanvasProperties();
		break;

	case CMP_IMAGE:
		PrintImageProperties();
		break;

	case CMP_BUTTON:
		PrintButtonProperties();
		break;

	case CMP_TEXT:
		PrintTextProperties();
		break;

	}
}

void UI_InspectorPanel::PrintTransformProperties()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ComponentTransform* trans_cmp = (ComponentTransform*)GetGameObject()->GetComponent(CMP_TRANSFORM);

		if (trans_cmp)
		{
			ImGui::Spacing();

			ImGui::SmallButton("Local"); ImGui::SameLine(); ImGui::SmallButton("Global"); ImGui::SameLine(); 

			if (ImGui::SmallButton("Reset"))
			{
				trans_cmp->ResetTransform();
			}

			SEPARATE_WITH_SPACE

			if (ImGui::RadioButton("Translate", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::TRANSLATE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::TRANSLATE);
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::ROTATE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::ROTATE);
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::SCALE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::SCALE);

			SEPARATE_WITH_SPACE

			float show_pos[3] = { trans_cmp->GetPosition().x, trans_cmp->GetPosition().y, trans_cmp->GetPosition().z };
			float show_rot[3] = { trans_cmp->GetRotationEuler().x, trans_cmp->GetRotationEuler().y, trans_cmp->GetRotationEuler().z };
			float show_scale[3] = { trans_cmp->GetScale().x, trans_cmp->GetScale().y, trans_cmp->GetScale().z };

			if (gameobject->GetIsStatic() == true)
			{
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Transforms are disabled while GameObject is Static."); 
			}
		
			if (ImGui::DragFloat3("Position", show_pos, 0.2f) && gameobject->GetIsStatic() == false)		
				trans_cmp->SetPosition({ show_pos[0], show_pos[1], show_pos[2] });
						
			if (ImGui::DragFloat3("Rotation", show_rot, 0.2f, -180.0f, 180.0f) && gameobject->GetIsStatic() == false)
			{
				if(trans_cmp->GetRotationEuler().x != show_rot[0])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

				if (trans_cmp->GetRotationEuler().y != show_rot[1])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

				if (trans_cmp->GetRotationEuler().z != show_rot[2])
					trans_cmp->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });
			}
				

			if(ImGui::DragFloat3("Scale", show_scale, 0.2f) && gameobject->GetIsStatic() == false)
				trans_cmp->SetScale({ show_scale[0], show_scale[1], show_scale[2] });

			ImGui::Spacing(); 
		}
	}
}

void UI_InspectorPanel::PrintRectTransformProperties()
{
	if (ImGui::CollapsingHeader("Rect Transform"))
	{
		
		ComponentRectTransform* rtransform = (ComponentRectTransform*)GetGameObject()->GetComponent(CMP_RECTTRANSFORM); 

		float show_pos[3] = { rtransform->GetTransform()->transform.position.x, rtransform->GetTransform()->transform.position.y, rtransform->GetTransform()->transform.position.z };
		float show_rot[3] = { rtransform->GetTransform()->transform.euler_angles.x, rtransform->GetTransform()->transform.euler_angles.y, rtransform->GetTransform()->transform.euler_angles.z };
		float show_scale[3] = { rtransform->scale_to_show.x,rtransform->scale_to_show.y, rtransform->scale_to_show.z };

		ImGui::Spacing();
		ImGui::Text("Transform:");

		SEPARATE_WITH_SPACE

			if (ImGui::RadioButton("Translate", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::TRANSLATE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::TRANSLATE);

			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::ROTATE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::ROTATE);

			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", App->scene->current_scene->GetGuizmoMode() == ImGuizmo::SCALE))
				App->scene->current_scene->SetGuizmoMode((OPERATION)ImGuizmo::SCALE);

		SEPARATE_WITH_SPACE

		if (ImGui::DragFloat3("Position", show_pos, 0.2f) && gameobject->GetIsStatic() == false)
				rtransform->GetTransform()->SetPosition(float3(show_pos[0], show_pos[1], show_pos[2]));

		if (ImGui::DragFloat3("Rotation", show_rot, 0.2f, -180.0f, 180.0f) && gameobject->GetIsStatic() == false)
		{
			if (rtransform->GetTransform()->GetRotationEuler().x != show_rot[0])
				rtransform->GetTransform()->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

			if (rtransform->GetTransform()->GetRotationEuler().y != show_rot[1])
				rtransform->GetTransform()->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });

			if (rtransform->GetTransform()->GetRotationEuler().z != show_rot[2])
				rtransform->GetTransform()->SetRotationEuler({ show_rot[0], show_rot[1], show_rot[2] });
		}


		if (ImGui::DragFloat3("Scale", show_scale, 0.2f) && gameobject->GetIsStatic() == false)
			rtransform->GetTransform()->SetScale({ show_scale[0], show_scale[1], show_scale[2] });

		ImGui::Separator(); 

		ImGui::DragFloat("Width", &rtransform->width); 
		ImGui::DragFloat("Height", &rtransform->height);

		ImGui::Spacing();
		ImGui::Text("Relative pos:");
		ImGui::Separator();
		ImGui::Spacing();

		//float show_point[2] = { rtransform->GetRelativePos().x, rtransform->GetRelativePos().y };

		//if (ImGui::DragFloat2("Anchor", show_point))
		//	rtransform->SetAnchorPoint(float2(show_point[0], show_point[1]));
	}
}

void UI_InspectorPanel::PrintCanvasProperties()
{
	if (ImGui::CollapsingHeader("Canvas"))
	{

	}
}

void UI_InspectorPanel::PrintTextProperties()
{
	if (ImGui::CollapsingHeader("Text (UI)"))
	{
		ComponentText* cmp_text = (ComponentText*)gameobject->GetComponent(CMP_TEXT); 

		ImGui::Spacing();

		ImGui::InputText("Text", (char*)cmp_text->GetLabel()->GetText().c_str(), 256);

		ImGui::Spacing();

		static float col[3]; 
		ImGui::ColorPicker3("Color", col); 

	}
}

void UI_InspectorPanel::PrintImageProperties()
{
	if (ImGui::CollapsingHeader("Image (UI)"))
	{
		ComponentImage* img_cmp = (ComponentImage*)GetGameObject()->GetComponent(CMP_IMAGE);

		ImGui::Spacing();

		bool ray = img_cmp->GetImage()->GetRaycast();
		if (ImGui::Checkbox("Raycast detection", &ray))
			img_cmp->GetImage()->SetRaycast(ray);

		ImGui::Separator();
		ImGui::Spacing(); 

		ImGui::BeginChild("c1", ImVec2(90, 90));
		ImGui::Image((ImTextureID)img_cmp->GetImage()->GetMaterial()->GetDiffuseTexture()->GetTextureID(), ImVec2(75, 75), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("c2", ImVec2(0, 90));
		ImGui::Text("Material:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", img_cmp->GetImage()->GetMaterial()->name.c_str());

		ImGui::SameLine(); 
		if (ImGui::Button("+"))
		{
			ImGui::OpenPopup("select_texture");
		}

		App->resources->material_importer->DrawTextureList(true);

		ImGui::EndChild();
	}
}

void UI_InspectorPanel::PrintButtonProperties()
{
	if (ImGui::CollapsingHeader("Button (UI)"))
	{

	}
}


void UI_InspectorPanel::PrintMeshProperties()
{
	if (ImGui::CollapsingHeader("Mesh Renderer"))
	{
		ComponentMesh* mesh_cmp = (ComponentMesh*)GetGameObject()->GetComponent(CMP_MESH);

		ImGui::Spacing();
		ImGui::Checkbox("Active##1", &mesh_cmp->active);

		if (!mesh_cmp->active)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "INACTIVE");
		}

		ImGui::Separator(); 

		ImGui::Spacing();

		ImGui::Text("Current Mesh:"); 

		if (mesh_cmp->GetMesh() != nullptr)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", mesh_cmp->GetMesh()->name.c_str());

			ImGui::SameLine(); 
			if (ImGui::SmallButton("+"))
			{
				ImGui::OpenPopup("select_mesh");
			}

			App->resources->mesh_importer->DrawMeshList(); 

			if (ImGui::TreeNode("UID"))
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "'%d'", mesh_cmp->GetMesh()->GetUID());
				ImGui::TreePop();
			}
			
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();

			mesh_cmp->PrintRenderSettings();

			ImGui::SameLine();

			ImGui::Checkbox("Draw BB", &mesh_cmp->draw_bounding_box);

			ImGui::Spacing();
		}			
		else
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "*NONE*");

			ImGui::SameLine();
			if (ImGui::SmallButton("+"))
			{
				ImGui::OpenPopup("select_mesh");
			}

			App->resources->mesh_importer->DrawMeshList();
		}	
	}
}

void UI_InspectorPanel::PrintMaterialProperties()
{
	if (ImGui::CollapsingHeader("Material"))
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

		if(mat_cmp->GetMaterial() != nullptr && mat_cmp->GetMaterial()->GetDiffuseTexture() != nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", mat_cmp->GetMaterial()->name.c_str()); ImGui::SameLine();

		if (ImGui::SmallButton("+"))
		{
			ImGui::OpenPopup("select_texture");
		}

		App->resources->material_importer->DrawTextureList();

		ImGui::SameLine(); 

		ImGui::ImageButton((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(15, 15));

		if (ImGui::IsItemClicked())
		{
			Texture* check_tex = App->resources->material_importer->GetCheckerTexture();
			mat_cmp->GetMaterial()->SetDiffuseTexture(check_tex);
		}

		ImGui::Spacing();
		
		if ((mat_cmp->GetMaterial() != nullptr && mat_cmp->GetMaterial()->GetDiffuseTexture() != nullptr))
		{
			ImGui::Image((ImTextureID)mat_cmp->GetMaterial()->GetDiffuseTexture()->GetTextureID(), ImVec2(150, 150), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::SameLine();

			ImGui::Spacing();

			ImGui::TextWrapped("Path: "); ImGui::SameLine();
			ImGui::TextWrapped(/*ImVec4(1,1,0,1),*/ "%s", mat_cmp->GetMaterial()->GetDiffuseTexture()->GetPath());

			ImGui::Text("Texture ID: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetMaterial()->GetDiffuseTexture()->GetTextureID());

			ImGui::Text("Width:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetMaterial()->GetDiffuseTexture()->GetWidth());

			ImGui::Text("Height:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mat_cmp->GetMaterial()->GetDiffuseTexture()->GetHeight());
		}

		ImGui::Spacing();
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

	float temp_v_fov_in_degrees = camera_cmp->camera->frustum.verticalFov * RADTODEG;

	if (ImGui::SliderFloat("Field Of View", &temp_v_fov_in_degrees, 1.0f, 180.0f))
	{
		camera_cmp->camera->SetFOV(temp_v_fov_in_degrees * DEGTORAD);
	}

	
	if (ImGui::SliderFloat("Aspect Ratio modifier", &camera_cmp->camera->aspect_ratio, 0.1, 10.0f))
	{
		camera_cmp->camera->SetAspectRatio(camera_cmp->camera->aspect_ratio); 
	}


	float focal_lenght = camera_cmp->camera->frustum.farPlaneDistance - camera_cmp->camera->frustum.nearPlaneDistance;

	if (ImGui::InputFloat("Focal Lenght", &focal_lenght, 0.1f))
	{
	}

	ImGui::Checkbox("Draw Frustum", &camera_cmp->draw_frustum);
	
}


void UI_InspectorPanel::DeleteElement(CompType type)
{

}


