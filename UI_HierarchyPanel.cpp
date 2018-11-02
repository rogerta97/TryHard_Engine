#include "UI_HierarchyPanel.h"
#include "ComponentMesh.h"

#include "imgui_dock.h"
#include "Application.h"
#include "GameObject.h"

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
		ImGui::Text(App->scene->GetSceneName());

		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		static int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 2); // Increase spacing to differentiate leaves from expanded contents.

		ImGui::Separator();
		ImGui::BeginChild("");

		int id = -1;
		for (auto it = App->scene->scene_gameobjects.begin(); it != App->scene->scene_gameobjects.end(); it++)
		{
			if ((*it)->parent != nullptr)
				continue;

			else
			{
				if ((*it)->PrintHierarchyRecursive(selection_mask, node_clicked, id))
				{
					if (App->scene->scene_gameobjects.empty())
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
	}

	if (App->imgui->hierarchy_panel->show_click_menu)
	{
		ImGui::OpenPopup("Options");
		if (ImGui::BeginPopup("Options"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				App->imgui->hierarchy_panel->show_click_menu = false;
				App->scene->GetSelectedGameObject()->DeleteRecursive();
				App->scene->SetSelectedGameObject(nullptr);
				delete App->scene->GetSelectedGameObject();
			}

			ImGui::EndPopup();
		}
	}

	if (ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
	{
		App->imgui->hierarchy_panel->show_create_menu = true; 
	}

	if (App->imgui->hierarchy_panel->show_create_menu)
	{
		ImGui::OpenPopup("CreateOptions");
		if (ImGui::BeginPopup("CreateOptions"))
		{
			if (ImGui::MenuItem("Empty"))
			{
				App->imgui->hierarchy_panel->show_create_menu = false;

				GameObject* new_go = App->scene->CreateGameObject();
				new_go->name = "Empty";
				App->scene->SetSelectedGameObject(new_go);
			}

			if (ImGui::MenuItem("Cube"))
			{
				App->imgui->hierarchy_panel->show_create_menu = false;

				GameObject* new_go = App->scene->CreateGameObject();
				new_go->SetParent(nullptr);

				new_go->name = "Cube";

				//Add Mesh
				ComponentMesh* cmp = (ComponentMesh*)new_go->AddComponent(CMP_MESH);
				Mesh* new_mesh = App->resources->mesh_importer->GetMeshByType(MESH_CUBE);

				new_mesh->name = "CubeMesh";
				new_mesh->LoadToMemory();
				cmp->SetMesh(new_mesh);
				cmp->CreateEnclosedMeshAABB();

				App->scene->SetSelectedGameObject(new_go);
			}

			ImGui::EndPopup();
		}
	}

	ImGui::EndChild();
	ImGui::End();

	return true;
}

bool UI_HierarchyPanel::CleanUp()
{
	return true;
}

void UI_HierarchyPanel::ManageDragAndDrop(GameObject* current)
{
 
	if (App->imgui->hierarchy_panel->want_to_drag && ImGui::IsMouseReleased(0) && ImGui::IsMouseHoveringWindow())
	{
		if (ImGui::IsItemHoveredRect())
		{
			//Get the dst
			App->imgui->hierarchy_panel->want_to_drag = false;
			App->imgui->hierarchy_panel->dst_in_drag = current;

			if (App->imgui->hierarchy_panel->dst_in_drag == App->imgui->hierarchy_panel->source_in_drag)
				return;

			else if (App->imgui->hierarchy_panel->dst_in_drag->GetChild(App->imgui->hierarchy_panel->source_in_drag->GetName().c_str()))
				return;

			//Assign parenting
			App->imgui->hierarchy_panel->source_in_drag->SetParent(App->imgui->hierarchy_panel->dst_in_drag);
			CONSOLE_LOG("DEST: %s", App->imgui->hierarchy_panel->dst_in_drag->GetName().c_str());
		}
		else if (!ImGui::IsAnyItemHovered())
		{
			App->imgui->hierarchy_panel->source_in_drag->SetParent(nullptr);
		}
	}	
}
