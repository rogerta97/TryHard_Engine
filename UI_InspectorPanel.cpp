#include "UI_InspectorPanel.h"
#include "UI_TagPanel.h"

#include "UI_Label.h"
#include "UI_Button.h"
#include "UI_CheckBox.h"
#include "UI_TextInput.h"
#include "UI_Canvas.h"

#include "Application.h"
#include "imgui_dock.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentRectTransform.h"
#include "ComponentText.h"
#include "ComponentButton.h"
#include "ComponentCanvas.h"
#include "ComponentCheckbox.h"
#include "ComponentTextInput.h"
#include "ComponentImage.h"
#include "ComponentCamera.h"
#include "ComponentCanvasScaler.h"

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

	case CMP_TEXTINPUT:
		PrintInputFieldProperties();
		break;

	case CMP_CANVASSCALER:
		PrintCanvasScalerProperties();
		break;

	case CMP_CHECKBOX:
		PrintCheckBoxProperties();
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
		float show_scale[3] = { rtransform->scale_to_show.x, rtransform->scale_to_show.y, rtransform->scale_to_show.z };
		bool move_container = false; 

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
		{
			rtransform->GetTransform()->SetPosition(float3(show_pos[0], show_pos[1], show_pos[2]));
			move_container = true; 
		}
				

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

		if (ImGui::DragFloat("Width", &rtransform->width))
		{
			rtransform->Resize({ rtransform->width, rtransform->height });
			rtransform->edited = true;
		}
			

		if (ImGui::DragFloat("Height", &rtransform->height))
		{
			rtransform->Resize({ rtransform->width, rtransform->height });
			rtransform->edited = true;
		}


		ImGui::Separator();
		ImGui::Spacing();

		float show_rel_size[2] = { rtransform->rel_size.x, rtransform->rel_size.y };

		if (ImGui::DragFloat2("Relative Size", show_rel_size,0.1f))
		{
			rtransform->rel_size.x = show_rel_size[0];
			rtransform->rel_size.y = show_rel_size[1];
			move_container = true;
		}
			

		ImGui::Spacing();
		ImGui::Text("Relative pos:");
		ImGui::Separator();
		ImGui::Spacing();

		float show_rel_pos[2] = { rtransform->GetRelativePos().x, rtransform->GetRelativePos().y };

		if (ImGui::DragFloat2("Pos", show_rel_pos))
		{
			rtransform->SetRelativePos(float2(show_rel_pos[0], show_rel_pos[1]));
			move_container = true; 
		}
			

		ImGui::Text("Anchors:");
		ImGui::Separator();
		ImGui::Spacing();

		//ANCHOR

		float show_anchor[4];
		
		show_anchor[0] = rtransform->GetAnchorPoint().min_x;
		show_anchor[1] = rtransform->GetAnchorPoint().min_y;
		show_anchor[2] = rtransform->GetAnchorPoint().max_x;
		show_anchor[3] = rtransform->GetAnchorPoint().max_y;
		

		ImGui::Columns(3, "", false);

		ImGui::Text("Min");

		ImGui::NextColumn();

		if (ImGui::DragFloat("X", &show_anchor[0], 0.01f, 0.0f, 1.0f))
			rtransform->SetAnchorPoint(show_anchor[0], show_anchor[1], show_anchor[2], show_anchor[3]);

		ImGui::NextColumn();

		if (ImGui::DragFloat("Y", &show_anchor[1], 0.01f, 0.0f, 1.0f))
			rtransform->SetAnchorPoint(show_anchor[0], show_anchor[1], show_anchor[2], show_anchor[3]);

		ImGui::NextColumn();

		ImGui::Text("Max");

		ImGui::NextColumn();

		if (ImGui::DragFloat("X", &show_anchor[2], 0.01f,0.0f,1.0f))
			rtransform->SetAnchorPoint(show_anchor[0], show_anchor[1], show_anchor[2], show_anchor[3]);

		ImGui::NextColumn();

		if (ImGui::DragFloat("Y", &show_anchor[3], 0.01f, 0.0f, 1.0f))
			rtransform->SetAnchorPoint(show_anchor[0], show_anchor[1], show_anchor[2], show_anchor[3]);

		ImGui::Columns(1);

		if (rtransform->edited || move_container)
		{
			
			ComponentText* cmp_text = (ComponentText*)gameobject->GetComponent(CMP_TEXT);

			if (cmp_text != nullptr)
			{
				if (rtransform->edited)
				{
					cmp_text->SetClipping(cmp_text->GetClipping());
					rtransform->edited = false;
				}
					
				else if (move_container)
				{				
					cmp_text->GetLabel()->SetText(cmp_text->GetLabel()->GetText().c_str());
					move_container = false; 
				}
					
			}
		}

		ImGui::Spacing();
	}
}

void UI_InspectorPanel::PrintCanvasProperties()
{
	if (ImGui::CollapsingHeader("Canvas"))
	{
		ComponentCanvas* canvas_cmp = (ComponentCanvas*)gameobject->GetComponent(CMP_CANVAS); 

		ImGui::Text("UI Elements in Canvas: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(), "%d", canvas_cmp->GetCanvas()->elements_in_canvas.size()); 

		for (auto it = canvas_cmp->GetCanvas()->elements_in_canvas.begin(); it != canvas_cmp->GetCanvas()->elements_in_canvas.end(); it++)
		{
			ImGui::TextColored(ImVec4(1,1,0,1), "%s", (*it)->GetName().c_str()); 
		}
	}
}

void UI_InspectorPanel::PrintCheckBoxProperties()
{
	if (ImGui::CollapsingHeader("CheckBox (UI)"))
	{
		ImGui::Spacing(); 

		ComponentCheckBox* cmp_check = (ComponentCheckBox*)gameobject->GetComponent(CMP_CHECKBOX);

		//static int combo_value = 0; 
		//if (ImGui::Combo("Runtime Behaviour", &combo_value, "Toggle Boolean\0Custom"))
		//{
		//	cmp_check->GetCheckBox()->SetType((CheckBoxType)combo_value);
		//}

		//ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		switch (cmp_check->GetCheckBox()->GetType())
		{
		case CheckBoxType::CHT_TOGGLE_BOOL:	
			cmp_check->callback_system->PrintSystemUI();
			break;

		case CheckBoxType::CHT_CUSTOM:
			break;

		}

		SEPARATE_WITH_SPACE

		bool is_on = cmp_check->GetCheckBox()->GetIsOn();
		if (ImGui::Checkbox("Is On", &is_on))
			cmp_check->GetCheckBox()->SetIsOn(is_on); 				
	}
}

void UI_InspectorPanel::PrintTextProperties()
{
	if (ImGui::CollapsingHeader("Text (UI)"))
	{
		ComponentText* cmp_text = (ComponentText*)gameobject->GetComponent(CMP_TEXT);

		ImGui::Spacing();

		if (ImGui::InputText("Text", cmp_text->GetLabel()->inspector_text, 256))
			cmp_text->GetLabel()->SetText(cmp_text->GetLabel()->inspector_text);

		
		if (ImGui::InputInt("Size", &cmp_text->GetLabel()->text_size))
		{
			if (cmp_text->GetLabel()->text_size < 1) cmp_text->GetLabel()->text_size = 1; 
			cmp_text->GetLabel()->ResizeFont(); 
		}

		float tmp_col[3] = { cmp_text->GetLabel()->color.x, cmp_text->GetLabel()->color.y, cmp_text->GetLabel()->color.z };
		if (ImGui::ColorEdit3("Color", tmp_col))
			cmp_text->GetLabel()->color = { tmp_col[0], tmp_col[1] , tmp_col[2] }; 

		//Clipping
		int curr_type = cmp_text->GetClipping(); 

		if (ImGui::Combo("Clipping", &curr_type, "Top Left\0Bottom Left\0Top Right\0Bottom Right\0Center\0Middle Left"))
		{
			cmp_text->SetClipping((ClipTextType)curr_type); 
		}

		curr_type = cmp_text->GetHorizontalOverflow();

		if (ImGui::Combo("Horizontal Overflow", &curr_type, "Overflow\0Wrap\0"))
		{
			cmp_text->SetHorizontalOverflow((horizontalTextOverflow)curr_type);
		}

		curr_type = cmp_text->GetVerticalOverflow();

		if (ImGui::Combo("Vertical Overflow", &curr_type, "Overflow\0Truncate\0"))
		{
			cmp_text->SetVerticalOverflow((verticalTextOverflow)curr_type);
		}

		ImGui::InputInt("Line Spacing", &cmp_text->line_spacing);

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


		SEPARATE_WITH_SPACE
		
		float tmp_col[3] = { img_cmp->GetImage()->image_color.x, img_cmp->GetImage()->image_color.y, img_cmp->GetImage()->image_color.z };
		if (ImGui::ColorPicker3("Color", tmp_col))
			img_cmp->GetImage()->image_color = { tmp_col[0], tmp_col[1] , tmp_col[2] };

		SEPARATE_WITH_SPACE

		ImGui::BeginChild("c1", {75,75});

		if (img_cmp->GetImage()->GetMaterial())
		{
			ImGui::Image((ImTextureID)img_cmp->GetImage()->GetMaterial()->GetDiffuseTexture()->GetTextureID(), ImVec2(75, 75), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
		
		}
		else
		{
			ImGui::Image((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(75, 75), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
		}

		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("c2", {500, 100});
			
		if (img_cmp->GetImage()->GetMaterial())
		{
			ImGui::Text("Material:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", img_cmp->GetImage()->GetMaterial()->name.c_str());
			ImGui::Text("Path:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", img_cmp->GetImage()->GetMaterial()->path.c_str());
		}
		else
		{			
			ImGui::Text("Material:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "NONE");
			ImGui::Text("Path:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "NONE");
		}

		ImGui::Button("+");

		if(ImGui::IsItemClicked(0))
		{
			ImGui::OpenPopup("select_texture");
		}

		App->resources->material_importer->DrawTextureList(true);

		ImGui::SameLine();
		ImGui::Button("X");

		if (ImGui::IsItemClicked(0))
		{
			img_cmp->GetImage()->SetMaterial(nullptr);
		}

		ImGui::EndChild();
		
		ImGui::Spacing();
	}
}

void UI_InspectorPanel::PrintInputFieldProperties()
{
	if (ImGui::CollapsingHeader("Input Field (UI)"))
	{
		ComponentTextInput* input_cmp = (ComponentTextInput*)GetGameObject()->GetComponent(CMP_TEXTINPUT);

		GameObject* place_holder_go = input_cmp->GetInputField()->GetPlaceHolderText();
		ImGui::Text("PlaceHolder: "); ImGui::SameLine();

		if (place_holder_go != nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", place_holder_go->GetName().c_str()); 
		else
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty");

		GameObject* text_go = input_cmp->GetInputField()->GetShowText();
		ImGui::Text("Text: "); ImGui::SameLine();

		if (text_go != nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", text_go->GetName().c_str());
		else
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty");

	}
}

void UI_InspectorPanel::PrintButtonProperties()
{
	if (ImGui::CollapsingHeader("Button (UI)"))
	{
		ComponentButton* button_cmp = (ComponentButton*)GetGameObject()->GetComponent(CMP_BUTTON);

		static int trans_type = 0; 
		if (ImGui::Combo("Transition Style", &trans_type, "Tint\0Swap Images"))
		{
			button_cmp->GetButton()->SetTransition((Button_Transition)trans_type); 
		}

		SEPARATE_WITH_SPACE

		switch (trans_type)
		{
		case Button_Transition::TRANSITION_COLOR:
		{
			float hover_c[3] = { button_cmp->GetHoverColor().x, button_cmp->GetHoverColor().y, button_cmp->GetHoverColor().z };
			float pressed_c[3] = { button_cmp->GetPressedColor().x, button_cmp->GetPressedColor().y, button_cmp->GetPressedColor().z };

			if (ImGui::ColorEdit3("Hover Tint", hover_c))
				button_cmp->SetHoverColor(float3(hover_c[0], hover_c[1], hover_c[2]));

			if (ImGui::ColorEdit3("Click Tint", pressed_c))
				button_cmp->SetPressedColor(float3(pressed_c[0], pressed_c[1], pressed_c[2]));

			break;
		}

		case Button_Transition::TRANSITION_IMG_SWAP:

			break;

		}

		SEPARATE_WITH_SPACE

		button_cmp->callback_system->PrintSystemUI(); 
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

void UI_InspectorPanel::PrintCanvasScalerProperties()
{
	ComponentCanvasScaler* canvas_scaler = (ComponentCanvasScaler*)GetGameObject()->GetComponent(CMP_CANVASSCALER);

	int selected_scaler_type = canvas_scaler->GetScaleType();
	std::string label = "Scale Type##";

	if (ImGui::CollapsingHeader("Canvas Scaler"))
	{
		if (ImGui::Combo(label.c_str(), &selected_scaler_type, "Constant\0Screen size"))
		{
			switch (selected_scaler_type)
			{
			case 0:
				canvas_scaler->SetScaleType(ST_CONSTANT);
				break;

			case 1:
				canvas_scaler->SetScaleType(ST_SCREEN_SIZE);
				break;
			}

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


