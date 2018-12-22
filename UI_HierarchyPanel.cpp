#include "UI_HierarchyPanel.h"
#include "ComponentMesh.h"
#include "UI_Canvas.h"

#include "MeshImporter.h"

#include "ComponentRectTransform.h"

#include "imgui_dock.h"
#include "Application.h"
#include "GameObject.h"


#include "mmgr\mmgr.h"


UI_HierarchyPanel::UI_HierarchyPanel()
{
	name = "Hierarchy";
}


UI_HierarchyPanel::~UI_HierarchyPanel()
{

}

bool UI_HierarchyPanel::Start()
{
	show = true;
	show_click_menu = false;
	show_create_menu = false; 
	want_to_drag = false;

	source_in_drag = nullptr;
	dst_in_drag = nullptr;

	return true;
}

bool UI_HierarchyPanel::Update()
{
	if (ImGui::Begin("Hierarchy", &show, NULL))
	{
		ImGui::Text(App->scene->GetSceneName()); ImGui::SameLine(); 
		
		if (ImGui::SmallButton("Clean"))
		{
			App->scene->CleanScene(); 
		}

		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		static int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 2); // Increase spacing to differentiate leaves from expanded contents.

		ImGui::Separator();
		ImGui::BeginChild("");

		int id = -1;
		for (auto it = App->scene->current_scene->scene_gameobjects.begin(); it != App->scene->current_scene->scene_gameobjects.end(); it++)
		{
			if ((*it)->parent != nullptr)
				continue;

			else
			{
				if ((*it)->PrintHierarchyRecursive(selection_mask, node_clicked, id))
				{
					if (App->scene->current_scene->scene_gameobjects.empty())
					{
						ImGui::PopStyleVar();
						ImGui::End();
						return true;
					}

				}
			}

			if (node_clicked != -1)
				selection_mask = (1 << node_clicked);
		}
		ImGui::PopStyleVar();

		ImGui::EndChild();
	}
	ImGui::End();



	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_UP)
	{
		App->imgui->hierarchy_panel->show_click_menu = false;
		GameObject* selected = App->scene->GetSelectedGameObject();

		if (selected)
			selected->DeleteRecursive();

		App->scene->SetSelectedGameObject(nullptr);
		delete App->scene->GetSelectedGameObject();
	}

	if (App->imgui->hierarchy_panel->show_click_menu)
	{
		ImGui::OpenPopup("Options");
		if (ImGui::BeginPopup("Options"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				App->imgui->hierarchy_panel->show_click_menu = false;
				App->scene->GetSelectedGameObject()->SetStatic(false); 
				App->scene->current_scene->octree->Recalculate();
				App->scene->GetSelectedGameObject()->DeleteRecursive();
				App->scene->SetSelectedGameObject(nullptr);
				delete App->scene->GetSelectedGameObject();
			}

			if (ImGui::MenuItem("Create Empty Child"))
			{
				//TODO: Create a function that duplicates a GameObject, remember to add it to the scene.
				GameObject* child_go = new GameObject("Empty", false);	
				child_go->SetParent(App->scene->GetSelectedGameObject());
				App->imgui->hierarchy_panel->show_click_menu = false;
			}

			if (ImGui::MenuItem("Create Prefab"))
			{
				App->imgui->hierarchy_panel->show_click_menu = false;
				App->scene->GetSelectedGameObject()->SaveAsPrefab(); 				
			}

			if (App->scene->GetSelectedGameObject() != nullptr && App->scene->GetSelectedGameObject()->GetIsUI() == true)
			{
				if (ImGui::BeginMenu("Create Child UI"))
				{
					ImGui::MenuItem("Image");

					if (ImGui::IsItemClicked())
					{
						App->scene->CreateUIElement(UI_IMAGE, App->scene->GetSelectedGameObject());
					}


					ImGui::MenuItem("Button");

					if (ImGui::IsItemClicked())
					{
						App->scene->CreateUIElement(UI_BUTTON, App->scene->GetSelectedGameObject());
					}


					ImGui::MenuItem("Text");

					if (ImGui::IsItemClicked())
					{
						App->scene->CreateUIElement(UI_LABEL, App->scene->GetSelectedGameObject());
					}

					ImGui::MenuItem("Input Text");

					if (ImGui::IsItemClicked())
					{
						App->scene->CreateUIElement(UI_INPUTFIELD, App->scene->GetSelectedGameObject());
					}

					ImGui::MenuItem("CheckBox");

					if (ImGui::IsItemClicked())
					{
						App->scene->CreateUIElement(UI_CHECKBOX, App->scene->GetSelectedGameObject());
					}

					ImGui::EndMenu();
				}
			
			}

			if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered())
				App->imgui->hierarchy_panel->show_click_menu = false;

			ImGui::EndPopup();
		}

	}


	if (App->imgui->hierarchy_panel->show_create_menu)
	{
		ImGui::OpenPopup("CreateOptions");
		if (ImGui::BeginPopup("CreateOptions"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				App->imgui->hierarchy_panel->show_create_menu = false;

				GameObject* new_go = App->scene->CreateGameObject();
				new_go->name = "Empty";
				App->scene->SetSelectedGameObject(new_go);
			}

			if (ImGui::BeginMenu("UI"))
			{
		
				ImGui::MenuItem("Canvas");

				if (ImGui::IsItemClicked())
				{
					GameObject* parent_canvas = new GameObject("Canvas", true);
					parent_canvas->AddComponent(CMP_CANVAS);

					App->user_interface->AddaptCanvasToScreen(); 
					App->scene->AddGameObjectToScene(parent_canvas);
				}

				ImGui::MenuItem("Image");				
				if (ImGui::IsItemClicked())				
					App->scene->CreateUIElement(UI_IMAGE);  //Create the element specified to the last canvas 
													
				ImGui::MenuItem("Button");
				if (ImGui::IsItemClicked())			
					App->scene->CreateUIElement(UI_BUTTON);  //Create the element specified to the last canvas //Force scale 
				
				ImGui::MenuItem("Text");
				if (ImGui::IsItemClicked())
					App->scene->CreateUIElement(UI_LABEL);

				ImGui::MenuItem("Input Field");
				if (ImGui::IsItemClicked())
					App->scene->CreateUIElement(UI_INPUTFIELD);

				ImGui::MenuItem("CheckBox");
				if (ImGui::IsItemClicked())
					App->scene->CreateUIElement(UI_CHECKBOX);
				
				ImGui::EndMenu();
			}

			if(ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered())
				App->imgui->hierarchy_panel->show_create_menu = false;

			ImGui::EndPopup();
		}
	}



	return true;
}

bool UI_HierarchyPanel::CleanUp()
{
	return true;
}

void UI_HierarchyPanel::ManageDragAndDrop(GameObject* current)
{

	//if (App->imgui->hierarchy_panel->show_click_menu || App->imgui->hierarchy_panel->show_create_menu)
	//	return; 

	//if (App->imgui->hierarchy_panel->want_to_drag && ImGui::IsMouseReleased(0) && ImGui::IsMouseHoveringWindow())
	//{
	//	if (ImGui::IsItemHoveredRect())
	//	{
	//		//Get the dst
	//		App->imgui->hierarchy_panel->want_to_drag = false;
	//		App->imgui->hierarchy_panel->dst_in_drag = current;

	//		if (App->imgui->hierarchy_panel->dst_in_drag == App->imgui->hierarchy_panel->source_in_drag)
	//			return;
	//		
	//		if (App->imgui->hierarchy_panel->dst_in_drag->GetChild(App->imgui->hierarchy_panel->source_in_drag->unique_id))
	//			return;

	//		//Assign parenting
	//		if (source_in_drag != nullptr && dst_in_drag != nullptr)
	//			App->imgui->hierarchy_panel->source_in_drag->SetParent(App->imgui->hierarchy_panel->dst_in_drag);

	//		App->imgui->hierarchy_panel->source_in_drag = nullptr; 
	//		App->imgui->hierarchy_panel->dst_in_drag = nullptr; 
	//		
	//	}
	//	else if (!ImGui::IsAnyItemHovered())
	//	{
	//		if(source_in_drag != nullptr && dst_in_drag != nullptr)
	//			App->imgui->hierarchy_panel->source_in_drag->SetParent(nullptr);
	//	}
	//}	
}
