#include "UI_HierarchyPanel.h"

#include "imgui_dock.h"
#include "Application.h"

UI_HierarchyPanel::UI_HierarchyPanel()
{
}


UI_HierarchyPanel::~UI_HierarchyPanel()
{

}

bool UI_HierarchyPanel::Start()
{
	show = true; 
	show_click_menu = false; 


	return true;
}

bool UI_HierarchyPanel::Update()
{
	if (ImGui::Begin("Hierarchy", &show, NULL))
	{
		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		static int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 2); // Increase spacing to differentiate leaves from expanded contents.

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

	if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
		App->imgui->hierarchy_panel->show_click_menu = true;


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
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End(); 

	return true;
}

bool UI_HierarchyPanel::CleanUp()
{
	return true;
}
